#pragma once
#include "queryProcessor.h"
#include "Keyword.h"
#include "tokenizer.h"
#include <fmt/core.h>

void processQuery(std::string query) {
  auto &keywords = Keyword::keywords;

  auto queryWords = tokenize(query, " ");
  auto find = keywords.find(queryWords[0]);

  if (find != keywords.end()) {
    auto keyword = find->second;
    keyword->prepare(query);
    keyword->process();
  }
}
