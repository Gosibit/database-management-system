#pragma once
#include "ArgumentsForComparing.h"

ArgumentsForComparing::ArgumentsForComparing(fieldValueType valueArg,
                                             std::string operatorArg,
                                             std::string columnName,
                                             std::string logicalOperator) {
  this->valueArg = valueArg;
  this->operatorArg = operatorArg;
  this->columnName = columnName;
  this->logicalOperator = logicalOperator;
}

fieldValueType ArgumentsForComparing::getValueArg() const { return valueArg; }
std::string ArgumentsForComparing::getOperatorArg() const {
  return operatorArg;
}
std::string ArgumentsForComparing::getColumnName() const { return columnName; }
std::string ArgumentsForComparing::getLogicalOperator() const {
  return logicalOperator;
}
