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
  SEGMENTATION_FAULT,
  INVALID_ARGUMENT,
  DIVISION_BY_ZERO,
  UNKNOWN
} ErrorCode;

struct Error {
  ErrorCode ErrCode;
  std::string Message;

  Error(ErrorCode error_code, std::string msg)
      : ErrCode{error_code}, Message{msg} {};
};

}  // namespace dlvm
