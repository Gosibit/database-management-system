#pragma once

#include "Keyword.h"
#include <string>

std::string replacePlaceholdersWithValues(const std::string &str);
std::string replaceValuesWithPlaceholders(std::string str);

void processQuery(const std::string &query);