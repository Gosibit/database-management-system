#pragma once

#include "Keyword.h"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <string>
#include <vector>

#include "stringUtilities.h"
#include "tokenizer.h"

std::vector<std::string> Keyword::supportedKeywords = {
    "ALTER_TABLE", "CREATE_TABLE", "ADD_COLUMN", "DROP_COLUMN",
    "INSERT",      "SELECT",       "FROM",       "WHERE"};

std::map<std::string, Keyword *> Keyword::keywords =
    std::map<std::string, Keyword *>();

Keyword::Keyword(std::string nameArg,
                 std::vector<std::string> compatibleKeywordsArg) {
  name = nameArg;
  compatibleKeywords = compatibleKeywordsArg;
}

void Keyword::process() {
  fmt::println("Keyword::process() triggered but no action defined");
}

std::string Keyword::getName() { return name; }

void Keyword::assignInteractions(std::vector<std::string> foundInteractions) {
  for (auto iter = foundInteractions.begin(); iter != foundInteractions.end();
       ++iter) {
    auto endDelimiter = std::string();
    if (iter + 1 != foundInteractions.end()) {
      endDelimiter = *(iter + 1);
    } else {
      endDelimiter = ";";
    }

    auto interaction = std::make_pair(
        *iter, getPartBetweenDelimiters(query, *iter, endDelimiter));
    this->foundInteractions.insert(interaction);
  }
};

void Keyword::assignKeywordArguments() {
  auto keywordArgumentsDelimiter = std::string();
  if (!foundInteractions.empty())
    keywordArgumentsDelimiter = foundInteractions.begin()->first;
  else
    keywordArgumentsDelimiter = ";";

  keywordArguments =
      getPartBetweenDelimiters(query, name, keywordArgumentsDelimiter);
}

void Keyword::prepare(std::string queryArg) {
  query = queryArg;
  foundInteractions = std::map<std::string, std::string>();
  auto foundInteractions = std::vector<std::string>();
  auto queryWords = tokenize(query, " ");

  for (auto &word : queryWords) {
    if (isKeywordCompatible(word)) {
      foundInteractions.push_back(word);
    }
  }

  assignInteractions(foundInteractions);
  assignKeywordArguments();
}

void Keyword::resetKeywordsData() {
  for (auto &keyword : keywords) {
    keyword.second->foundInteractions = std::map<std::string, std::string>();
    keyword.second->keywordArguments = std::string();
    keyword.second->query = std::string();
  }
}

void Keyword::println() {
  fmt::print("Keyword: {}, ", name);
  fmt::print("Possible successors: {}, ", fmt::join(compatibleKeywords, ", "));
  fmt::print("Keyword arguments: {}, ", keywordArguments);
  fmt::print("Found interactions: {}", fmt::join(foundInteractions, ", "));
}

std::vector<std::string> Keyword::getCompatibleKeywords() {
  return compatibleKeywords;
}

std::string Keyword::getQuery() { return query; }

bool Keyword::isKeywordCompatible(std::string keyword) {
  auto find =
      std::find(compatibleKeywords.begin(), compatibleKeywords.end(), keyword);

  if (find != compatibleKeywords.end()) {
    return true;
  }

  return false;
}
