#pragma once
#include "compare.h"
#include "ArgumentsForComparing.h"

bool compareWithLogicalOperator(const fieldValueType &arg1,
                                const std::string &operatorArg,
                                const fieldValueType &arg2,
                                const std::string &logicalOperator,
                                const bool &actualState) {
  bool result;
  if (logicalOperator == "AND") {
    result = actualState && compare(arg1, operatorArg, arg2);
  } else if (logicalOperator == "OR") {
    result = actualState || compare(arg1, logicalOperator, arg2);
  } else {
    result = compare(arg1, operatorArg, arg2);
  }
  return result;
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
