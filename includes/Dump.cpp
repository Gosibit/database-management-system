#include "Dump.h"
#include "Table.h"
#include "queryProcessor.h"
#include "stringUtilities.h"
#include <fstream>
#include <iostream>

void Dump::dump(const std::string &path) {
  auto tables = Table::tables;

  std::ofstream dumpFile;
  dumpFile.open("../" + path);

  for (auto &table : tables) {
    auto createTableQuery = "CREATE_TABLE " + table.first + " (";
    auto columns = table.second->getColumns();
    for (auto &column : columns) {
      auto nullable = column.second->isNullable() ? "" : " NOT_NULL";
      auto unique = column.second->isUnique() ? " UNIQUE" : "";
      auto primaryKey = column.second->isPrimaryKey() ? " PRIMARY_KEY" : "";
      createTableQuery += column.first + " " +
                          column.second->getType()->getName() + nullable +
                          unique + primaryKey + ", ";
    }

    createTableQuery = createTableQuery.substr(0, createTableQuery.size() - 2);
    createTableQuery += ");";
    dumpFile << createTableQuery + "\n";

    auto rows = table.second->getRows();

    for (auto &row : rows) {
      auto insertIntoQuery = "INSERT_INTO " + table.first + " (";
      for (auto &column : columns) {
        insertIntoQuery += column.first + ", ";
      }
      insertIntoQuery = insertIntoQuery.substr(0, insertIntoQuery.size() - 2);
      insertIntoQuery += ") VALUES (";

      for (auto &column : columns) {
        auto value = row.second[column.second];
        if (std::holds_alternative<std::string>(value))
          insertIntoQuery += "'" + std::get<std::string>(value) + "', ";
        else {
          insertIntoQuery += fieldToString(value) + ", ";
        }
      }
      insertIntoQuery = insertIntoQuery.substr(0, insertIntoQuery.size() - 2);
      insertIntoQuery += ");";
      dumpFile << insertIntoQuery + "\n";
    }
  }

  dumpFile.close();
}

void Dump::restore(const std::string &path) {
  for (auto &table : Table::tables) {
    delete table.second;
  }
  Table::tables.clear();

  std::ifstream dumpFile;
  dumpFile.open("../" + path);
  std::string line;
  while (std::getline(dumpFile, line)) {
    line = replaceValuesWithPlaceholders(line);
    line = removeAllOccurencies(line, '\'');
    processQuery(line);
    emptyHiddenStrings();
  }
  dumpFile.close();
}
