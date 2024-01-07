#pragma once
#include <string>
#include <vector>
#include "aliases.h"

std::string getPartAfterDelimiter(std::string str, std::string delimiter);
std::string getPartBeforeDelimiter(std::string str, std::string delimiter);
std::string getPartBetweenDelimiters(std::string str,
                                     std::string begginingDelimiter,
                                     std::string endingDelimiter);
std::string removeAllOccurencies(std::string str, char toRemove);
std::string removeAllOccurencies(std::string str, std::vector<char> toRemove);
std::string removeBrackets(std::string str);
std::vector<std::string> splitByComma(std::string str);
std::vector<std::string> splitBySpace(std::string str);
std::vector<std::string> splitByQueries(std::string str);
std::string removeSemicolons(std::string str);
std::string trim(std::string str);
std::string fieldToString(fieldValueType field);