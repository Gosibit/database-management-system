#include "compare.h"
#include <fmt/core.h>

bool compareWithLogicalOperator(const fieldValueType &arg1,
                                const std::string &operatorArg,
                                const fieldValueType &arg2,
                                const std::string &logicalOperator,
                                const bool &actualState) {

  auto comparationResult = compare(arg1, operatorArg, arg2);
  if (logicalOperator == "AND") {
    return actualState && comparationResult;

  } else if (logicalOperator == "OR") {
    return actualState || comparationResult;
  } else {
    return comparationResult;
  }
}

bool compare(const fieldValueType &arg1, const std::string &operatorArg,
             const fieldValueType &arg2) {

  if (operatorArg == "IS_NULL") {
    return std::holds_alternative<nullptr_t>(arg1);
  } else if (operatorArg == "IS_NOT_NULL") {
    return !std::holds_alternative<nullptr_t>(arg1);
  }

  if (std::holds_alternative<int>(arg1) && std::holds_alternative<int>(arg2)) {
    if (operatorArg == "=") {
      return std::get<int>(arg1) == std::get<int>(arg2);
    } else if (operatorArg == ">") {
      return std::get<int>(arg1) > std::get<int>(arg2);
    } else if (operatorArg == "<") {
      return std::get<int>(arg1) < std::get<int>(arg2);
    } else if (operatorArg == ">=") {
      return std::get<int>(arg1) >= std::get<int>(arg2);
    } else if (operatorArg == "<=") {
      return std::get<int>(arg1) <= std::get<int>(arg2);
    } else if (operatorArg == "!=") {
      return std::get<int>(arg1) != std::get<int>(arg2);
    }
  }
  if (std::holds_alternative<std::string>(arg1) &&
      std::holds_alternative<std::string>(arg2)) {
    if (operatorArg == "=") {
      return std::get<std::string>(arg1) == std::get<std::string>(arg2);
    } else if (operatorArg == ">") {
      return std::get<std::string>(arg1) > std::get<std::string>(arg2);
    } else if (operatorArg == "<") {
      return std::get<std::string>(arg1) < std::get<std::string>(arg2);
    } else if (operatorArg == ">=") {
      return std::get<std::string>(arg1) >= std::get<std::string>(arg2);
    } else if (operatorArg == "<=") {
      return std::get<std::string>(arg1) <= std::get<std::string>(arg2);
    } else if (operatorArg == "!=") {
      return std::get<std::string>(arg1) != std::get<std::string>(arg2);
    }
  }

  if (std::holds_alternative<double>(arg1) &&
      std::holds_alternative<double>(arg2)) {
    if (operatorArg == "=") {
      return std::get<double>(arg1) == std::get<double>(arg2);
    } else if (operatorArg == ">") {
      fmt::println("arg1: {}, arg2: {}", std::get<double>(arg1),
                   std::get<double>(arg2));
      return std::get<double>(arg1) > std::get<double>(arg2);
    } else if (operatorArg == "<") {
      return std::get<double>(arg1) < std::get<double>(arg2);
    } else if (operatorArg == ">=") {
      return std::get<double>(arg1) >= std::get<double>(arg2);
    } else if (operatorArg == "<=") {
      return std::get<double>(arg1) <= std::get<double>(arg2);
    } else if (operatorArg == "!=") {
      return std::get<double>(arg1) != std::get<double>(arg2);
    }
  }

  if (std::holds_alternative<float>(arg1) &&
      std::holds_alternative<float>(arg2)) {
    if (operatorArg == "=") {
      return std::get<float>(arg1) == std::get<float>(arg2);
    } else if (operatorArg == ">") {
      return std::get<float>(arg1) > std::get<float>(arg2);
    } else if (operatorArg == "<") {
      return std::get<float>(arg1) < std::get<float>(arg2);
    } else if (operatorArg == ">=") {
      return std::get<float>(arg1) >= std::get<float>(arg2);
    } else if (operatorArg == "<=") {
      return std::get<float>(arg1) <= std::get<float>(arg2);
    } else if (operatorArg == "!=") {
      return std::get<float>(arg1) != std::get<float>(arg2);
    }
  }

  if (std::holds_alternative<bool>(arg1) &&
      std::holds_alternative<bool>(arg2)) {
    if (operatorArg == "=") {
      return std::get<bool>(arg1) == std::get<bool>(arg2);
    }
  }
}
