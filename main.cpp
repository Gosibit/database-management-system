#include "./includes/Keyword.h"
#include "./includes/queryProcessor.h"
#include "./includes/stringUtilities.h"
#include "./includes/tokenizer.h"
#include <fmt/core.h>
#include <iostream>

int main() {
  //  while (true) {
  auto input = std::string();

  input = "CREATE_TABLE tab1 (col1 varchar NOT_NULL UNIQUE, col2 varchar, col3 "
          "int);"
          "INSERT_INTO tab1 (col1, col2) VALUES ('val1', 'test;1;');"
          "INSERT_INTO tab1 (col1, col2) VALUES ('val2', 'test;2;');"
          "INSERT_INTO tab1 (col1, col2, col3) VALUES ('val3', 'tes t;3;', 3);"
          "UPDATE tab1 SET col1 = 'val4', col2 = 'test;4;', col3 = 4 WHERE "
          "col1 = 'val2';"
          "SELECT * FROM tab1 WHERE col3 IS_NULL;"
          "SELECT col1, col3 FROM tab1;"
          "SELECT * FROM tab1 WHERE col1 = 'val1';";

  input = replaceValuesWithPlaceholders(input);
  input = removeAllOccurencies(input, '\'');

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
