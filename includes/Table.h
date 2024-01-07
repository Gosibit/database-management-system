#pragma once

#include "ArgumentsForComparing.h"
#include "Column.h"
#include "aliases.h"
#include <iostream>
#include <map>

class Table {
  std::string name;
  std::map<std::string, Column *> columns;                  // name, type
  std::map<std::string, std::map<Column *, fieldValueType>> // id[column, value]
      rows;
  int lastId;

public:
  static std::map<std::string, Table *> tables;
  static void printTables();
  static Table *getTable(std::string name);
  Table(std::string nameArg);
  void println();
  void renameTo(std::string newName);
  void addColumn(std::string columnName, std::string columnType);
  void dropColumn(std::string columnName);

  void select(std::vector<std::string> columnNames,
              std::vector<ArgumentsForComparing> argumentsForComparing);

  void insertInto(
      std::vector<std::pair<std::string, std::string>> columnNamesAndValues);

  void printColumns();
  void drawTable(std::vector<std::string> columnNames,
                 std::vector<std::string> rowIdsToSelect);
  std::vector<std::string> getIdRowsMatchingConditions(
      std::vector<ArgumentsForComparing> argumentsForComparing);
  Column *getColumn(std::string columnName);
};
