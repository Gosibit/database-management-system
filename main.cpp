#include "./includes/Keyword.h"
#include "./includes/queryProcessor.h"
#include "./includes/stringUtilities.h"
#include "./includes/tokenizer.h"
#include <fmt/core.h>
#include <iostream>

//ZASADY
//1. Wszystkie nazwy tabel i kolumn sa case sensitive
//2. Wszystkie nazwy tabel i kolumn zawartych w tabelach sa unikalne
//3. Zawartosc tekstowa podajemy w pojedynczych apostrofach
//4. Przy porownywaniu tekstów bierzemy pod uwagę wielkosc liter
//5. Przy porownywaniu wartosci musimy rozdzielic nazwę kolumny, operator porownania i wartosc spacjami (np. col1 = 10)


//PRZYKLADY
//DDL
// CREATE_TABLE tab1 (id int PRIMARY_KEY NOT_NULL, col1 varchar NOT_NULL, col2 varchar, col3 int UNIQUE);
// DESC tab1;
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
// SELECT * FROM tab1 WHERE col3 IS_NULL OR col2 > 10;
// SELECT col1, col3 FROM tab1;
// SELECT * FROM tab1 WHERE col1 = 'val1' AND col2 IS_NOT_NULL;

//BACKUP
//DUMP_CREATE > dumps/dump.txt
//DUMP_RESTORE < dumps/dump.txt

int main() {
  while (true) {
    auto input = std::string();
    std::getline(std::cin, input);

    if(input == "exit") break;

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
