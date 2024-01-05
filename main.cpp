#include "./includes/TypeDerivaties.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <stdio.h>
#include <string.h>

#include <iostream>
#include <sstream>

#include "./includes/Keyword.h"
#include "./includes/Table.h"
#include "./includes/stringUtilities.h"
#include "./includes/tokenizer.h"

int main() {
  //    while (true) {
  auto input = std::string();
  input =
      "CREATE_TABLE tab1 (col1 int, col2 varchar, col3 int);ALTER_TABLE "
      "tab1 DROP_COLUMN col2; CREATE_TABLE tab2 (col1 varchar, col2 "
      "varchar); CREATE_TABLE tab3 (col1 varchar, col2 varchar); DROP_TABLE "
      "tab2; ALTER_TABLE andrzej RENAME_TO andrzej2;";

  //    std::getline(std::cin, input);
  auto multipleQuriesSplitted = splitByQueries(input);

  auto &keywords = Keyword::keywords;

  for (auto &query : multipleQuriesSplitted) {
    auto queryWords = tokenize(query, " ");
    auto find = keywords.find(queryWords[0]);

    if (find != keywords.end()) {
      auto keyword = find->second;
      keyword->prepare(query);
      try {
        keyword->process();
      } catch (std::exception &e) {
        fmt::println("Error while executing query: {}", e.what());
      }
    }

    Keyword::resetKeywordsData();
  }

  fmt::println("");
  //    }

  return 0;
}
