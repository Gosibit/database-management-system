#include "./includes/Keyword.h"
#include "./includes/queryProcessor.h"
#include "./includes/stringUtilities.h"
#include "./includes/tokenizer.h"
#include <fmt/core.h>
#include <iostream>

int main() {
  //  while (true) {
  auto input = std::string();

  input = "DUMP_RESTORE DATABASE < dumps/dump.txt;"
          "SELECT * FROM tab1";

  //    std::getline(std::cin, input);
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
