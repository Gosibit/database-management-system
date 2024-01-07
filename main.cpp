#include "./includes/Keyword.h"
#include "./includes/stringUtilities.h"
#include "./includes/tokenizer.h"
#include <fmt/core.h>
#include <iostream>

int main() {
  //  while (true) {
  auto input = std::string();

  input = "CREATE_TABLE tab1 (col12345678 int, col2 varchar, col3 int, col4 "
          "boolean);"
          "INSERT_INTO tab1 (col12345678, col2, col4) VALUES (1, 'stringval', "
          "true);"
          "INSERT_INTO tab1 (col12345678, col2, col4) VALUES (12232332, "
          "'stringvalLLLLLL', false);"
          "INSERT_INTO tab1 (col12345678, col2, col3, col4) VALUES (12232332, "
          "'stringvalLLLLLL', 2, true);"
          "SELECT col12345678, col2, col3, col4 FROM tab1 WHERE col12345678 = "
          "12232332 OR col4 = true;"
          "SELECT col12345678, col2, col3, col4 FROM tab1;";

  //    std::getline(std::cin, input);
  auto multipleQueriesSplitted = splitByQueries(input);

  auto &keywords = Keyword::keywords;

  for (auto &query : multipleQueriesSplitted) {
    auto queryWords = tokenize(query, " ");
    auto find = keywords.find(queryWords[0]);

    if (find != keywords.end()) {
      auto keyword = find->second;
      keyword->prepare(query);
      try {
        keyword->process();
      } catch (std::exception &e) {
        fmt::println("Error while executing query: {}", e.what());
        Keyword::resetKeywordsData();
        break;
      }
    }

    Keyword::resetKeywordsData();
    //    }
  }

  return 0;
}
