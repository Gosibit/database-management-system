#pragma once
#include "stringUtilities.h"

#include <string>

#include "tokenizer.h"

std::string getPartAfterDelimiter(std::string str, std::string delimiter) {
    return str.substr(str.find(delimiter) + delimiter.length(), str.length());
}
std::string getPartBeforeDelimiter(std::string str, std::string delimiter) {
    return str.substr(0, str.find(delimiter));
}
std::string getPartBetweenDelimiters(std::string str,
                                     std::string begginingDelimiter,
                                     std::string endingDelimiter) {
    auto strAfterBegginingDelimiter =
        getPartAfterDelimiter(str, begginingDelimiter);
    auto strBetweenDelimiters =
        getPartBeforeDelimiter(strAfterBegginingDelimiter, endingDelimiter);
    return strBetweenDelimiters;
}

std::string removeAllOccurencies(std::string str, char toRemove) {
    str.erase(std::remove(str.begin(), str.end(), toRemove), str.end());
    return str;
}

std::string removeAllOccurencies(std::string str, std::vector<char> toRemove) {
    for (auto& c : toRemove) {
        str.erase(std::remove(str.begin(), str.end(), c), str.end());
    }
    return str;
}

std::string removeBrackets(std::string str) {
    auto brackets = std::vector<char>{'(', ')'};
    return removeAllOccurencies(str, brackets);
}

std::vector<std::string> splitByComma(std::string str) {
    auto splitted = std::vector<std::string>();
    auto splittedByComma = tokenize(str, ",");
    return splittedByComma;
}

std::vector<std::string> splitBySpace(std::string str) {
    auto splitted = std::vector<std::string>();
    auto splittedBySpace = tokenize(str, " ");

    return splittedBySpace;
}

std::string trim(std::string str) {
    auto trimmed = std::string();
    for (auto& c : str) {
        if (c != ' ')
            trimmed += c;
    }
    return trimmed;
}

std::vector<std::string> splitByQueries(std::string str) {
    auto splitted = std::vector<std::string>();
    auto splittedByQueries = tokenize(str, ";");

    for (auto& s : splittedByQueries) {
        s += ";";
    }

    return splittedByQueries;
}

std::string removeSemicolons(std::string str) {
    return removeAllOccurencies(str, ';');
}
