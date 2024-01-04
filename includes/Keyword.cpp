#pragma once

#include <vector>
#include <fmt/core.h>
#include <fmt/ranges.h>
#include "Keyword.h"
#include <string>
#include "tokenizer.h"

std::vector<std::string> Keyword::supportedKeywords = {
   "ALTER_TABLE",
   "CREATE_TABLE",
   "INSERT",
   "SELECT",
   "FROM",
   "WHERE"
};

std::map<std::string, Keyword*> Keyword::keywords = std::map<std::string, Keyword*>();

Keyword::Keyword(std::string nameArg, std::vector<std::string> possibleSuccessorsArg) {
    name = nameArg;
    possibleSuccessors = possibleSuccessorsArg;
}

void Keyword::process() {
    fmt::println("Keyword::process() triggered but no action defined");
}

std::string Keyword::getName() {
    return name;
}

void Keyword::prepare(std::string queryArg) {
    query = queryArg;
    foundInteractions = std::vector<std::string>();

    auto queryWords = tokenize(query, " ");

    for (auto& word : queryWords) {

        auto find = std::find(possibleSuccessors.begin(), possibleSuccessors.end(), word);

        if (find != possibleSuccessors.end()) {
            foundInteractions.push_back(word);
        }
    }
}

void Keyword::println() {
    fmt::print("Keyword: {}, ", name);
    fmt::print("Possible successors: {}, ", fmt::join(possibleSuccessors, ", "));
    fmt::print("Found interactions: {}", fmt::join(foundInteractions, ", "));
}

std::vector<std::string> Keyword::getPossibleSuccessors() {
    return possibleSuccessors;
}

std::vector<std::string> Keyword::getFoundInteractions() {
    return foundInteractions;
}

std::string Keyword::getQuery() {
    return query;
}











