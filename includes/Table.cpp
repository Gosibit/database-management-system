#pragma once
#include "./Table.h"

#include "compare.h"
#include "stringUtilities.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <iostream>
#include <ranges>
#include <variant>

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
  fmt::println("");
}

void Table::printTables() {
  //  fmt::println("Printing tables...");
  //  if (Table::tables.empty()) {
  //    fmt::println("No tables left");
  //  }
  //  for (auto &table : Table::tables) {
  //    table.second->println();
  //  }
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

void Table::drawTable(std::vector<std::string> columnNames,
                      std::vector<std::string> rowIdsToSelect) {
  auto longestValueInColumn = std::map<std::string, int>();

  for (auto &columnName : columnNames) {
    longestValueInColumn[columnName] = columnName.size();
  }

  for (auto &rowId : rowIdsToSelect) {
    if (rows.find(rowId) == rows.end()) {
      throw std::runtime_error("Row with id " + rowId + " not found");
    }
    auto row = rows[rowId];
    for (auto &columnName : columnNames) {
      auto value = fieldToString(row[getColumn(columnName)]);
      if (longestValueInColumn.find(columnName) == longestValueInColumn.end()) {
        longestValueInColumn[columnName] = value.size();
      } else {
        if (value.size() > longestValueInColumn[columnName]) {
          longestValueInColumn[columnName] = value.size();
        }
      }
    }
  }

  auto horizontalLine = std::string("*-");
  for (auto &longestValue : longestValueInColumn) {
    horizontalLine.append(longestValue.second, '-');
    horizontalLine += "-+-";
  }
  horizontalLine = horizontalLine.substr(0, horizontalLine.size() -
                                                1); // always one - too much
  fmt::println("{}", horizontalLine);
  fmt::print("| ");
  for (auto &columnName : columnNames) {
    auto value = columnName;
    value.append(longestValueInColumn[columnName] - value.size(), ' ');
    value += " | ";
    fmt::print("{}", value);
  }
  fmt::println("");
  fmt::println("{}", horizontalLine);

  for (auto &rowId : rowIdsToSelect) {
    fmt::print("| ");
    if (rows.find(rowId) == rows.end()) {
      throw std::runtime_error("Row with id " + rowId + " not found");
    }
    auto row = rows[rowId];
    for (auto &promptedColumnName : columnNames) {
      auto value = fieldToString(row[getColumn(promptedColumnName)]);

      value.append(longestValueInColumn[promptedColumnName] - value.size(),
                   ' ');
      value += " | ";
      fmt::print("{}", value);
    }

    fmt::println("");
  }
  fmt::println("{}", horizontalLine);
}
void Table::insertInto(
    std::vector<std::pair<std::string, std::string>> columnNamesAndValues) {
  lastId++;

  auto columnNames = std::vector<std::string>();
  for (auto &pair : columnNamesAndValues) {
    columnNames.push_back(pair.first);
  }

  for (auto &column : columns) {
    if (std::find(columnNames.begin(), columnNames.end(), column.first) ==
        columnNames.end()) {
      if (!column.second->isNullable()) {
        lastId--;
        throw std::runtime_error("Column " + column.first + " is not nullable");
      } else {
        rows[std::to_string(lastId)][column.second] = nullptr;
      }
    }
  }

  for (auto &pair : columnNamesAndValues) {
    auto columnName = pair.first;
    auto value = pair.second;

    auto *column = getColumn(columnName);
    auto *type = column->getType();

    if (!type->isValueValid(value)) {
      lastId--;
      throw std::runtime_error("Value " + value + " is not valid for type " +
                               type->getName());
    }

    rows[std::to_string(lastId)][column] = type->parseValue(value);
  }
}

std::vector<std::string> Table::getIdRowsMatchingConditions(
    std::vector<ArgumentsForComparing> argumentsForComparing) {

  auto meetingConditionsRowIds = std::vector<std::string>();

  if (argumentsForComparing.size() == 0) {
    auto kv = std::views::keys(rows);
    return {kv.begin(), kv.end()}; // if no conditions then every row is valid
    // https://stackoverflow.com/questions/110157/how-to-retrieve-all-keys-or-values-from-a-stdmap-and-put-them-into-a-vector
  }
  for (auto &row : rows) {
    auto actualState = true;
    auto lastLogicalOperator = std::string();
    for (auto &argument : argumentsForComparing) {
      auto *column = getColumn(argument.getColumnName());
      auto valueFromColumn = row.second[column];
      auto operatorArg = argument.getOperatorArg();
      auto valueArg = argument.getValueArg();

      actualState =
          compareWithLogicalOperator(valueFromColumn, operatorArg, valueArg,
                                     lastLogicalOperator, actualState);

      lastLogicalOperator = argument.getLogicalOperator();

      //      std::visit(
      //          [operatorArg](auto &&arg1, auto &&arg2) {
      //            fmt::println("{}{}{} | {}", arg1, operatorArg, arg2,
      //                         compare(arg1, operatorArg, arg2));
      //          },
      //          valueFromColumn, valueArg);
    }

    if (actualState) {
      meetingConditionsRowIds.push_back(row.first);
    }
  }
  fmt::println("meetingConditionsRowIds: {}", meetingConditionsRowIds);
  return meetingConditionsRowIds;
}

void Table::select(std::vector<std::string> columnNames,
                   std::vector<ArgumentsForComparing> argumentsForComparing) {

  auto meetingConditionsRowIds =
      getIdRowsMatchingConditions(argumentsForComparing);

  drawTable(columnNames, meetingConditionsRowIds);
}
