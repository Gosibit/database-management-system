#pragma once
#include "./Table.h"

#include <fmt/core.h>
#include <fmt/ranges.h>

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
  fmt::print("Rows: {}", rows);
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
