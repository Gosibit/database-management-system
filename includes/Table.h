#pragma once

#include "ArgumentsForComparing.h"
#include "Column.h"
#include "aliases.h"
#include <iostream>
#include <map>

struct PrintInstruction {
  std::vector<std::string> columnNames;
  std::vector<std::vector<std::string>> rows;

  PrintInstruction(std::vector<std::string> columnNames,
                   std::vector<std::vector<std::string>> rows)
      : columnNames(columnNames), rows(rows) {}
};

class Table {
  std::string name;
  std::map<std::string, Column *> columns;                  // name, type
  std::map<std::string, std::map<Column *, fieldValueType>> // id[column, value]
      rows;
  int lastId;

public:
  Table(const std::string &nameArg);

  static std::map<std::string, Table *> tables;
  static Table *getTable(std::string name);

  std::map<std::string, std::map<Column *, fieldValueType>> getRows();

  std::map<std::string, Column *> getColumns();
  void renameTo(std::string newName);

  void addColumn(std::string columnName, std::string columnType, bool nullable,
                 bool primaryKey, bool unique);

  void alterColumn(std::string columnName, std::string columnType,
                   bool nullable, bool primaryKey, bool unique);

  void dropColumn(std::string columnName);

  void describe();

  std::vector<std::string> getColumnNames();

  void select(const std::vector<std::string> &columnNames,
              const std::vector<ArgumentsForComparing> &argumentsForComparing);

  void insertInto(
      std::vector<std::pair<std::string, std::string>> &columnNamesAndValues);

  void deleteFrom(std::vector<ArgumentsForComparing> &argumentsForComparing);

  void validateNewValue(const std::string &columnName,
                        const std::string &value);

  void
  update(std::vector<std::pair<std::string, std::string>> &columnNamesAndValues,
         std::vector<ArgumentsForComparing> &argumentsForComparing);

  void setValues(
      std::vector<std::pair<std::string, std::string>> &columnNamesAndValues,
      std::string rowId);

  void printColumns();

  void drawTable(PrintInstruction &printInstruction);

  std::vector<std::string> getIdRowsMatchingConditions(
      const std::vector<ArgumentsForComparing> &argumentsForComparing);

  PrintInstruction preparePrintInstructionForSelect(
      const std::vector<std::string> &columnNames,
      const std::vector<std::string> &rowIdsToSelect);

  PrintInstruction preparePrintInstructionForDescribe();

  Column *getColumn(std::string columnName);
};
