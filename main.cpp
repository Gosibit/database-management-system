#include "./includes/Keyword.h"
#include "./includes/queryProcessor.h"
#include "./includes/stringUtilities.h"
#include "./includes/tokenizer.h"
#include <fmt/core.h>
#include <iostream>

int main() {
  //  while (true) {
  auto input = std::string();

  input = "CREATE_TABLE tab1 (col1 varchar, col2 varchar);"
          "INSERT_INTO tab1 (col1, col2) VALUES ('val1', 'test;2;');"
          "INSERT_INTO tab1 (col1, col2) VALUES ('val2', 'test;2;');"
          "SELECT * FROM tab1 WHERE col1 = 'val2';"
          "SELECT * FROM tab1;";

  input = replaceValuesWithPlaceholders(input);
  input = removeAllOccurencies(input, '\'');
  fmt::println("input: {}", input);
  auto multipleQueriesSplitted = splitByQueries(input);

  for (auto &query : multipleQueriesSplitted) {
    try {
      processQuery(query);
    } catch (std::exception &e) {
      fmt::println("Error while executing query: {}", e.what());
      Keyword::resetKeywordsData();
      break;
    }
    Keyword::resetKeywordsData();
    //    }
  }

  return 0;
}
