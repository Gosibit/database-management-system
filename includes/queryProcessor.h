#pragma once

#include "Keyword.h"
#include <string>

std::string replacePlaceholdersWithValues(std::string str);
std::string replaceValuesWithPlaceholders(std::string str);

void processQuery(std::string query);