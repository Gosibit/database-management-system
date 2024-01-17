#pragma once
#include "ArgumentsForComparing.h"
#include "aliases.h"

bool compareWithLogicalOperator(const fieldValueType &arg1,
                                const std::string &operatorArg,
                                const fieldValueType &arg2,
                                const std::string &logicalOperator,
                                const bool &actualState);
bool compare(const fieldValueType &value, const std::string &operatorArg,
             const fieldValueType &valueArg);
