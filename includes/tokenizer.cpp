#pragma once
#include "tokenizer.h"
#include <string>

std::vector<std::string> tokenize(std::string str, std::string delimiter)
{
    char strCopy[str.size() + 1];
    strcpy(strCopy, str.c_str());

    char delimiterCopy[delimiter.size() + 1];
    strcpy(delimiterCopy, delimiter.c_str());

    char* token = strtok(strCopy, delimiterCopy);

    auto words = std::vector<std::string>();

    while (token != NULL)
    {
        words.push_back(token);
        token = strtok(NULL, delimiterCopy);
    };

    return words;
}
