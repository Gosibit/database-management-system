#include <fmt/core.h>
#include "./includes/Keyword.h"
#include "./includes/stringUtilities.h"
#include "./includes/tokenizer.h"

int main() {
  //    while (true) {
  auto input = std::string();
  //  input =
  //      "CREATE_TABLE tab1 (col1 int, col2 varchar, col3 int);ALTER_TABLE "
  //      "tab1 DROP_COLUMN col2; CREATE_TABLE tab2 (col1 varchar, col2 "
  //      "varchar); CREATE_TABLE tab3 (col1 varchar, col2 varchar); DROP_TABLE
  //      " "tab2;";

  input = "CREATE_TABLE tab1 (col1 int, col2 varchar, col3 int); INSERT_INTO "
          "tab1 (col1, col2, col3) VALUES (1, 'stringval', 123); SELECT col1, col3 FROM tab1;";

//  input = "CREATE_TABLE tab1 (col1 int, col2 varchar, col3 int); SELECT col1, col2 FROM tab1 WHERE col1 = 1 AND col2 = 'val2';";


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
  }

  fmt::println("");
  //    }

  return 0;
}
