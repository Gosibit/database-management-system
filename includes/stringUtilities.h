#pragma once
#include "aliases.h"
#include <string>
#include <vector>

std::string getPartAfterDelimiter(const std::string &str,
                                  const std::string &delimiter);
std::string getPartBeforeDelimiter(const std::string &str,
                                   const std::string &delimiter);
std::string getPartBetweenDelimiters(const std::string &str,
                                     const std::string &begginingDelimiter,
                                     const std::string &endingDelimiter);
std::string removeAllOccurencies(std::string str, char toRemove);
std::string removeAllOccurencies(std::string str, std::vector<char> toRemove);
std::string removeBrackets(const std::string &str);
std::vector<std::string> splitByComma(const std::string &str);
std::vector<std::string> splitBySpace(const std::string &str);
std::vector<std::string> splitByQueries(const std::string &str);
std::string removeSemicolons(std::string str);
std::string trim(const std::string &str);
std::string fieldToString(const fieldValueType &field);