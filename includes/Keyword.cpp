#include "Keyword.h"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include <string>
#include <vector>

#include "ArgumentsForComparing.h"
#include "Table.h"
#include "stringUtilities.h"
#include "tokenizer.h"

std::map<std::string, Keyword *> Keyword::keywords =
    std::map<std::string, Keyword *>();

Keyword::Keyword(const std::string &nameArg,
                 const std::vector<std::string> &compatibleKeywordsArg)
    : name(nameArg), compatibleKeywords(compatibleKeywordsArg) {}

void Keyword::process() {
  fmt::println("Keyword::process() triggered but no action defined");
}

std::string Keyword::getName() { return name; }

void Keyword::assignInteractions(
    const std::vector<std::string> &foundInteractions) {
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

void Keyword::prepare(const std::string &queryArg) {
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

bool Keyword::isKeywordCompatible(const std::string &keyword) {
  auto find =
      std::find(compatibleKeywords.begin(), compatibleKeywords.end(), keyword);

  if (find != compatibleKeywords.end()) {
    return true;
  }

  return false;
}

std::vector<ArgumentsForComparing>
Keyword::parseWhereArguments(const std::string &whereArguments, Table *table) {
  auto argumentsToCompare = std::vector<ArgumentsForComparing>();
  auto whereArgumentsSplitted =
      splitBySpace(whereArguments); // {col1, =, 1, AND, col2, =, 'a'}

  for (int i = 0; i < whereArgumentsSplitted.size(); i += 3) {
    auto columnName = whereArgumentsSplitted[i];
    auto operatorArg = whereArgumentsSplitted[i + 1];

    if (operatorArg == "IS_NULL" || operatorArg == "IS_NOT_NULL") {
      auto logicalOperator = whereArgumentsSplitted[i + 2];
      argumentsToCompare.push_back(
          ArgumentsForComparing("", operatorArg, columnName, logicalOperator));
      continue;
    }

    auto valueArg = whereArgumentsSplitted[i + 2];

    auto logicalOperator = std::string();
    if (whereArgumentsSplitted[i + 3] == "AND" ||
        whereArgumentsSplitted[i + 3] == "OR") {
      logicalOperator = whereArgumentsSplitted[i + 3];
      i++;
    } else {
      logicalOperator = "";
    }

    auto *column = table->getColumn(columnName);
    auto *type = column->getType();
    if (!type->isValueValid(valueArg)) {
      throw std::runtime_error("Value " + valueArg + " is not valid for type " +
                               type->getName());
    }

    auto parsedArgValue = column->getType()->parseValue(valueArg);

    argumentsToCompare.push_back(ArgumentsForComparing(
        parsedArgValue, operatorArg, columnName, logicalOperator));
  }
  return argumentsToCompare;
}
