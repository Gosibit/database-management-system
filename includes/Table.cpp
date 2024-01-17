#pragma once
#include "./Table.h"

#include "compare.h"
#include "queryProcessor.h"
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

std::map<std::string, Column *> Table::getColumns() { return columns; }

std::map<std::string, std::map<Column *, fieldValueType>> Table::getRows() {
  return rows;
}

Column *Table::getColumn(std::string columnName) {
  if (columns.find(columnName) == columns.end())
    throw std::runtime_error("Column " + columnName + " not found");

  return columns[columnName];
}

void Table::renameTo(std::string newName) { name = newName; }

void Table::addColumn(std::string columnName, std::string columnType,
                      bool nullable, bool primaryKey, bool unique) {

  auto *type = Type::getType(columnType);

  if (nullable && primaryKey) {
    throw std::runtime_error("Column " + columnName +
                             " can't be nullable and primary key, consider "
                             "adding NOT_NULL constraint");
  }

  auto *column = new Column(columnName, type, nullable, primaryKey, unique);
  columns.insert(std::make_pair(columnName, column));
}

void Table::dropColumn(std::string columnName) {
  delete columns.at(columnName);
  columns.erase(columnName);
}

std::vector<std::string> Table::getColumnNames() {
  auto columnNames = std::vector<std::string>();
  for (auto &column : columns) {
    columnNames.push_back(column.first);
  }
  return columnNames;
};

void Table::insertInto(
    std::vector<std::pair<std::string, std::string>> columnNamesAndValues) {
  lastId++;

  auto columnNames = std::vector<std::string>();
  for (auto &pair : columnNamesAndValues) {
    columnNames.push_back(pair.first);
  }

  // check if all non nullable columns have provided value in
  // columnNamesAndValues
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

  setValues(columnNamesAndValues, std::to_string(lastId));
}

void Table::deleteFrom(
    std::vector<ArgumentsForComparing> argumentsForComparing) {
  auto meetingConditionsRowIds =
      getIdRowsMatchingConditions(argumentsForComparing);

  for (auto &rowId : meetingConditionsRowIds) {
    rows.erase(rowId);
  }
}

void Table::update(
    std::vector<std::pair<std::string, std::string>> columnNamesAndValues,
    std::vector<ArgumentsForComparing> argumentsForComparing) {
  auto meetingConditionsRowIds =
      getIdRowsMatchingConditions(argumentsForComparing);

  for (auto &rowId : meetingConditionsRowIds) {
    setValues(columnNamesAndValues, rowId);
  }
}

void Table::setValues(
    std::vector<std::pair<std::string, std::string>> columnNamesAndValues,
    std::string rowId) {
  for (auto &pair : columnNamesAndValues) {
    auto columnName = pair.first;
    auto value = pair.second;

    validateNewValue(columnName, value);

    auto *column = getColumn(columnName);
    auto *type = column->getType();

    auto parsedValue = type->parseValue(value);
    if (std::holds_alternative<std::string>(parsedValue)) {
      parsedValue =
          replacePlaceholdersWithValues(std::get<std::string>(parsedValue));
    }
    rows[rowId][column] = parsedValue;
  }
}

void Table::validateNewValue(std::string columnName, std::string value) {

  auto *column = getColumn(columnName);
  auto *type = column->getType();

  auto replacedValue = replacePlaceholdersWithValues(value);

  if (column->isUnique() || column->isPrimaryKey()) {
    for (auto &row : rows) {
      if (value != "NULL" &&
          fieldToString(row.second[column]) == replacedValue) {
        throw std::runtime_error("Value " + replacedValue + " is not unique");
      }
    }
  }

  if (value == "NULL" && !column->isNullable()) {
    throw std::runtime_error("Column " + columnName + " is not nullable");
  } else if (value != "NULL" && !type->isValueValid(replacedValue)) {
    throw std::runtime_error("Value " + replacedValue +
                             " is not valid for type " + type->getName());
  }
}

void Table::select(std::vector<std::string> columnNames,
                   std::vector<ArgumentsForComparing> argumentsForComparing) {

  auto meetingConditionsRowIds =
      getIdRowsMatchingConditions(argumentsForComparing);

  drawTable(columnNames, meetingConditionsRowIds);
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
  if (rowIdsToSelect.size() == 0) {
    auto supplyStr = std::string("| ");
    supplyStr.append(horizontalLine.size() - 4, ' ');
    supplyStr += " |";
    fmt::println("{}", supplyStr);
  }
  fmt::println("{}", horizontalLine);
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

      if (std::holds_alternative<std::string>(valueArg))
        valueArg =
            replacePlaceholdersWithValues(std::get<std::string>(valueArg));

      actualState =
          compareWithLogicalOperator(valueFromColumn, operatorArg, valueArg,
                                     lastLogicalOperator, actualState);

      lastLogicalOperator = argument.getLogicalOperator();
    }

    if (actualState) {
      meetingConditionsRowIds.push_back(row.first);
    }
  }
  fmt::println("meetingConditionsRowIds: {}", meetingConditionsRowIds);
  return meetingConditionsRowIds;
}