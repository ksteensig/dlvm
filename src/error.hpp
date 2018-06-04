#pragma once

#include <cstdint>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <vector>

namespace dlvm {

typedef enum ErrorCode {
  OUT_OF_MEMORY,
  OUT_OF_BOUNDS,
  STACK_OVERFLOW,
  TYPE_ERROR,
  NULL_REFERENCE,
  INVALID_ARGUMENT,
  UNKNOWN
} ErrorCode;

struct Error {
  ErrorCode ErrCode;
  std::string Message;

  Error(ErrorCode error_code, std::string msg)
      : ErrCode{error_code}, Message{msg} {};
};

}  // namespace dlvm
