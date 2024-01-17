#pragma once

#include "aliases.h"
#include <string>

class ArgumentsForComparing {
  fieldValueType valueArg;
  std::string operatorArg;
  std::string columnName;
  std::string logicalOperator;

public:
  ArgumentsForComparing(const fieldValueType &valueArg,
                        const std::string &operatorArg,
                        const std::string &columnName,
                        const std::string &logicalOperator);
  fieldValueType getValueArg() const;
  std::string getOperatorArg() const;
  std::string getColumnName() const;
  std::string getLogicalOperator() const;
};
