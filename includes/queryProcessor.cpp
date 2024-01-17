#pragma once
#include "queryProcessor.h"
#include "Keyword.h"
#include "tokenizer.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <ranges>
#include <regex>
#include <stack>

auto hiddenStrings = std::map<int, std::string>();

std::string replacePlaceholdersWithValues(const std::string &str) {
  std::regex regex("<(\\d+)>");
  std::smatch match;
  std::string result = str;

  while (std::regex_search(result, match, regex)) {
    int index = std::stoi(match[1].str());
    auto it = hiddenStrings.find(index);

    if (it != hiddenStrings.end()) {
      result = std::regex_replace(result, regex, it->second,
                                  std::regex_constants::format_first_only);
    }
  }

  return result;
}

std::string replaceValuesWithPlaceholders(std::string str) {
  std::regex regex("'([^']*)'");
  std::smatch match;
  int count = 1;

  while (std::regex_search(str, match, regex)) {
    std::string value = match[1].str();
    str = std::regex_replace(str, regex, "<" + std::to_string(count) + ">",
                             std::regex_constants::format_first_only);
    hiddenStrings.insert(std::make_pair(count++, value));
  }

  return str;
}

void processQuery(const std::string &query) {
  auto &keywords = Keyword::keywords;

  auto queryWords = tokenize(query, " ");

  auto find = keywords.find(queryWords[0]);

  if (find != keywords.end()) {
    auto keyword = find->second;
    keyword->prepare(query);
    keyword->process();
  }
}
