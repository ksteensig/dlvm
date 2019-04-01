#include "interpreter.hpp"

namespace dlvm {

void Interpreter::Execute() {
  while (true) {
    auto op = Next();
    switch (op) {
      case NOP:
        break;
      case POP:
        Pop().OnError();
        break;
      case PUSH_UINT:
        ReturnOk<>(ValueType{UINTEGER, (uint64_t)NextWord()})
            .MapOk(push)
            .OnError();
        break;
      case PUSH_INT:
        ReturnOk<>(ValueType{INTEGER, (int64_t)NextWord()})
            .MapOk(push)
            .OnError();
        break;
      case PUSH_FLOAT:
        ReturnOk<>(ValueType{FLOAT, (double)NextWord()}).MapOk(push).OnError();
        break;
      case PUSH_BOOL:
        ReturnOk<>(ValueType{BOOL, (bool)Next()}).MapOk(push).OnError();
        break;
      case ADD:
        Pop().AggregateOk(ArithmeticAdd, Pop()).MapOk(push);
        break;
      case SUB:
        Pop().AggregateOk(ArithmeticSub, Pop()).MapOk(push);
        break;
      case MUL:
        Pop().AggregateOk(ArithmeticMul, Pop()).MapOk(push);
        break;
      case DIV:
        Pop().AggregateOk(ArithmeticDiv, Pop()).MapOk(push);
        break;
      case AND:
        Pop().AggregateOk(BooleanAnd, Pop()).MapOk(push);
      case OR:
        Pop().AggregateOk(BooleanOr, Pop()).MapOk(push);
      case XOR:
        Pop().AggregateOk(BooleanXor, Pop()).MapOk(push);
      case NOT:
        Pop()
            .MapOk(static_cast<std::function<Result<ValueType>(ValueType)>>(
                BooleanNot))
            .MapOk(push);
      case LT:
        Pop().AggregateOk(LessThan, Pop()).MapOk(push);
      case LE:
        Pop().AggregateOk(LessThanEquals, Pop()).MapOk(push);
      case EQ:
        Pop().AggregateOk(Equals, Pop()).MapOk(push);
      case GE:
        Pop().AggregateOk(GreaterThanEquals, Pop()).MapOk(push);
      case GT:
        Pop().AggregateOk(GreaterThan, Pop()).MapOk(push);
      case CREATE_ARRAY: {
        EXCEPTION_HANDLE(size, Pop(), "");
        TYPE_CHECK(size, UINTEGER, "");
        size.Value = std::get<uint64_t>(size.Value)++;

        EXCEPTION_HANDLE(ptr, m_memory->Malloc(size), "");
        ptr.type = ARRAY;
        EXCEPTION_HANDLE(_, Push(ptr), "");

        m_memory->Insert(ptr, ValueType{UINTEGER, (uint64_t)0}, ptr);
      }; break;
      case ACCESS_ARRAY: {
        EXCEPTION_HANDLE(addr, Pop(), "");
        EXCEPTION_HANDLE(offset, Pop(), "");
        EXCEPTION_HANDLE(arr, m_memory->Access(addr, offset), "");

        TYPE_CHECK(
            arr, ARRAY,
            ReturnError<bool>(TYPE_ERROR,
                              "Type error: Tried to access non-existing array")
                .OnError());

        if (std::get<addr_t>(arr.Value) < std::get<uint64_t>(offset.Value)) {
          ReturnError<bool>(
              OUT_OF_BOUNDS,
              "Out of bounds: Tried to access outside of an array")
              .OnError();
        }

        offset.Value = std::get<uint64_t>(offset.Value)++;

        EXCEPTION_HANDLE(value, m_memory->Access(addr, offset), "");
        EXCEPTION_HANDLE(_, Push(value), "");
      } break;
      case INSERT_ARRAY: {
        EXCEPTION_HANDLE(addr, Pop(), "");
        EXCEPTION_HANDLE(offset, Pop(), "");
        EXCEPTION_HANDLE(arr, m_memory->Access(addr, offset), "");

        TYPE_CHECK(arr, ARRAY,
                   ReturnError<bool>(
                       TYPE_ERROR,
                       "Type error: Tried to insert into non-existing array")
                       .OnError());

        if (std::get<addr_t>(arr.Value) < std::get<uint64_t>(offset.Value)) {
          ReturnError<bool>(
              OUT_OF_BOUNDS,
              "Out of bounds: Tried to insert outside of an array")
              .OnError();
        }

        EXCEPTION_HANDLE(value, Pop(), "");
        offset.Value = std::get<uint64_t>(offset.Value)++;
        EXCEPTION_HANDLE(_, m_memory->Insert(addr, offset, value), "");
      } break;
      case JMPT: {
        EXCEPTION_HANDLE(value, Pop(), "");

        TYPE_CHECK(value, BOOL, "");

        EXCEPTION_HANDLE(new_addr_vt, Pop(), "");

        TYPE_CHECK(new_addr_vt, UINTEGER, "");

        pc = std::get<uint64_t>(new_addr_vt.Value);
      } break;
      case JMP:
        this->pc = NextQuad();
        break;
      case RET: {
        EXCEPTION_HANDLE(res, Pop(), "");

        m_memory->stack_ptr = frame_ptr;

        EXCEPTION_HANDLE(pc_old, Pop(), "");
        TYPE_CHECK(pc_old, UINTEGER, "");
        pc = std::get<uint64_t>(pc_old.Value);

        EXCEPTION_HANDLE(frame_ptr_old, Pop(), "");
        TYPE_CHECK(frame_ptr_old, UINTEGER, "");
        frame_ptr = std::get<uint64_t>(frame_ptr_old.Value);

        EXCEPTION_HANDLE(argc, Pop(), "");
        TYPE_CHECK(argc, UINTEGER, "");
        m_memory->stack_ptr -= std::get<uint64_t>(argc.Value);

        EXCEPTION_HANDLE(success, Push(res), "");
      }; break;
      case CALL_MANAGED: {
        EXCEPTION_HANDLE(function_ref_id, Pop(), "");
        EXCEPTION_HANDLE(function_ref, constant_pool->getEntry(function_ref_id),
                         "");

        TYPE_CHECK(function_ref, ConstantPoolEntryTypeTag::MANAGED_FUNCTION_REF,
                   "");

        auto [module_id, function_id] =
            std::get<std::pair<uint32_t, uint32_t>>(function_ref.data);

        EXCEPTION_HANDLE(module_name, constant_pool->getEntry(module_id), "");
        TYPE_CHECK(module_name, ConstantPoolEntryTypeTag::STRING, "");

        EXCEPTION_HANDLE(function_name, constant_pool->getEntry(function_id),
                         "");
        TYPE_CHECK(function_name, ConstantPoolEntryTypeTag::STRING, "");

        auto [addr, argc] = module_table->getFunction(
            std::get<std::string>(module_name.data),
            std::get<std::string>(function_name.data));

        EXCEPTION_HANDLE(argc_err, Push(ValueType{UINTEGER, argc}), "");
        EXCEPTION_HANDLE(fp_err, Push(ValueType{UINTEGER, frame_ptr}), "");
        EXCEPTION_HANDLE(pc_err, Push(ValueType{UINTEGER, pc}), "");

        frame_ptr = m_memory->stack_ptr;
        pc = addr;
      }; break;
      case CALL_NATIVE: {
        EXCEPTION_HANDLE(function_ref_id, Pop(), "");
        EXCEPTION_HANDLE(function_ref, constant_pool->getEntry(function_ref_id),
                         "");

        TYPE_CHECK(function_ref, ConstantPoolEntryTypeTag::MANAGED_FUNCTION_REF,
                   "");

        auto [so_name_id, handle_id] =
            std::get<std::pair<uint32_t, uint32_t>>(function_ref.data);

        EXCEPTION_HANDLE(so_name, constant_pool->getEntry(so_name_id), "");
        TYPE_CHECK(so_name, ConstantPoolEntryTypeTag::STRING, "");

        EXCEPTION_HANDLE(handle, constant_pool->getEntry(handle_id), "");
        TYPE_CHECK(handle, ConstantPoolEntryTypeTag::STRING, "");

        EXCEPTION_HANDLE(
            fun,
            m_native_table.Call(std::get<string>(so_name.data),
                                std::get<string>(handle.data), env),
            "");
      }; break;
      case HALT:
      default:
        return;
    }
  }
}

inline uint8_t Interpreter::Next() { return m_program[pc++]; }

inline uint32_t Interpreter::NextQuad() {
  return (Next() << 0x18) | (Next() << 0x10) | (Next() << 0x08) | Next();
}

inline uint64_t Interpreter::NextWord() {
  return (static_cast<uint64_t>(NextQuad()) << 0x20) | NextQuad();
}

}  // namespace dlvm
