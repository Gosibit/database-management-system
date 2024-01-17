#pragma once

#include "aliases.h"
#include <string>

class ArgumentsForComparing {
  fieldValueType valueArg;
  std::string operatorArg;
  std::string columnName;
  std::string logicalOperator;

public:
  ArgumentsForComparing(fieldValueType valueArg, std::string operatorArg,
                        std::string columnName, std::string logicalOperator);
  fieldValueType getValueArg() const;
  std::string getOperatorArg() const;
  std::string getColumnName() const;
  std::string getLogicalOperator() const;
};
