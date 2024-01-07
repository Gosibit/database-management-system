#pragma once
#include "ArgumentsForComparing.h"
#include "aliases.h"

bool compareWithLogicalOperator(fieldValueType arg1, std::string operatorArg,
                                fieldValueType arg2,
                                std::string logicalOperator, bool actualState);
bool compare(fieldValueType value, std::string operatorArg,
             fieldValueType valueArg);
