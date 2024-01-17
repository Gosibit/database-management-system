#pragma once
#include "ArgumentsForComparing.h"

ArgumentsForComparing::ArgumentsForComparing(const fieldValueType &valueArg,
                                             const std::string &operatorArg,
                                             const std::string &columnName,
                                             const std::string &logicalOperator)
    : valueArg(valueArg), operatorArg(operatorArg), columnName(columnName),
      logicalOperator(logicalOperator) {}

fieldValueType ArgumentsForComparing::getValueArg() const { return valueArg; }
std::string ArgumentsForComparing::getOperatorArg() const {
  return operatorArg;
}
std::string ArgumentsForComparing::getColumnName() const { return columnName; }
std::string ArgumentsForComparing::getLogicalOperator() const {
  return logicalOperator;
}
