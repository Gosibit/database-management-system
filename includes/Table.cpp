#pragma once
#include "./Table.h"

#include <fmt/core.h>
#include <fmt/ranges.h>
#include "stringUtilities.h"
#include <iostream>

std::map<std::string, Table *> Table::tables = std::map<std::string, Table *>();

Table::Table(std::string nameArg) {
  name = nameArg;

  Table::tables.insert(std::make_pair(name, this));
};

Table *Table::getTable(std::string name) {
  if (Table::tables.find(name) == Table::tables.end())
    throw std::runtime_error("Table " + name + " not found");
  return Table::tables[name];
};

void Table::println() {
  fmt::print("Table: {}, ", name);
  printColumns();
  printRows();
  fmt::println("");
}

void Table::printTables() {
  fmt::println("Printing tables...");
  if (Table::tables.empty()) {
    fmt::println("No tables left");
  }
  for (auto &table : Table::tables) {
    table.second->println();
  }
}

void Table::printColumns() {
  for (auto &column : columns) {
    column.second->println();
  }
}

Column *Table::getColumn(std::string columnName) {
  if (columns.find(columnName) == columns.end())
    throw std::runtime_error("Column " + columnName + " not found");

  return columns[columnName];
}

void Table::renameTo(std::string newName) { name = newName; }

void Table::addColumn(std::string columnName, std::string columnType) {

  auto *type = Type::getType(columnType);

  auto *column = new Column(columnName, type);
  columns.insert(std::make_pair(columnName, column));
}

void Table::dropColumn(std::string columnName) {
  delete columns.at(columnName);
  columns.erase(columnName);
}

void Table::printRows() {
  fmt::println("");
  for(auto &row : rows) {
    fmt::print("id: {}, ", row.first);
    for(auto &column : row.second) {
      auto typeName = column.first->getType()->getName();
        auto value = column.second;
        fmt::print("column: {}, value: {}, ", column.first->getName(), fieldToString(value));
    }
  }
}

void Table::insertInto(
    std::vector<std::pair<std::string, std::string>> columnNamesAndValues) {
  lastId++;

  for (auto &pair : columnNamesAndValues) {
    auto columnName = pair.first;
    auto value = pair.second;

    auto *column = getColumn(columnName);
    auto *type = column->getType();

    if (!type->isValueValid(value)) {
      lastId--;
      throw std::runtime_error("Value " + value + " is not valid for type " + type->getName());
    }

    rows[std::to_string(lastId)][column] = type->parseValue(value);
  }
}


void Table::select(std::vector<std::string> columnNames) {
  fmt::println("Selecting columns: {}", fmt::join(columnNames, ", "));
  for (auto &row : rows) {
    for (auto &columnValuePair : row.second) {
      auto *column = columnValuePair.first;
      auto value = columnValuePair.second;

      if (std::find(columnNames.begin(), columnNames.end(), column->getName()) != columnNames.end()) {
        fmt::println("Column name: {}, value: {}",column->getName(), fieldToString(value));
      }
    }
    fmt::println("");
  }
}
