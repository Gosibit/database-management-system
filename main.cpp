#include "./includes/Keyword.h"
#include "./includes/queryProcessor.h"
#include "./includes/stringUtilities.h"
#include "./includes/tokenizer.h"
#include <fmt/core.h>
#include <iostream>

//PRZYKLADY
//DDL
// CREATE_TABLE tab1 (id int PRIMARY_KEY NOT_NULL, col1 varchar NOT_NULL, col2 varchar PRIMARY_KEY NOT_NULL, col3 int UNIQUE);
// ALTER_TABLE tab1 ADD_COLUMN col4 varchar;
// ALTER_TABLE tab1 DROP_COLUMN col4;
// ALTER_TABLE tab1 RENAME_TO tab2;
// ALTER_TABLE tab1 ALTER_COLUMN col1 varchar;
// DROP_TABLE tab1;

//DML
// INSERT_INTO tab1 (col1, col2) VALUES ('val1', 'test;1 ;');
// UPDATE tab1 SET col1 = 'val2', col2 = 'test;2 ;' WHERE col1 = 'val1';
// DELETE_FROM tab1 WHERE col1 = 'val1';

//DQL
// SELECT * FROM tab1 WHERE col3 IS_NULL;
// SELECT col1, col3 FROM tab1;

//BACKUP






int main() {
  while (true) {
    auto input = std::string();
    std::getline(std::cin, input);

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
    }
    emptyHiddenStrings();
  }

  return 0;
}
