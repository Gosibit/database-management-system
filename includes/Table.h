#pragma once

#include <iostream>
#include <map>
#include "aliases.h"
#include "Column.h"


class Table {
  std::string name;
  std::map<std::string, Column *> columns;               // name, type
  std::map<std::string, std::map<Column *, valueField>> // id[column, value]
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
  void select(std::vector<std::string> columnNames);
  void insertInto(
      std::vector<std::pair<std::string, std::string>> columnNamesAndValues);
  void printColumns();
  void printRows();

  Column *getColumn(std::string columnName);
};




