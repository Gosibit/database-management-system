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

fieldValueType ArgumentsForComparing::getValueArg() { return valueArg; }
std::string ArgumentsForComparing::getOperatorArg() { return operatorArg; }
std::string ArgumentsForComparing::getColumnName() { return columnName; }
std::string ArgumentsForComparing::getLogicalOperator() {
  return logicalOperator;
}
