#include "./Table.h"

#include "compare.h"
#include "queryProcessor.h"
#include "stringUtilities.h"
#include <fmt/core.h>
#include <fmt/ranges.h>
#include <iostream>
#include <ranges>
#include <variant>

bool doesRowWithSameValueExist(
    std::map<std::string, std::map<Column *, fieldValueType>> &rows,
    Column *column) {

  auto columnValues = std::vector<fieldValueType>();
  for (auto &row : rows) {
    if (!std::holds_alternative<nullptr_t>(row.second[column]) &&
        std::find(columnValues.begin(), columnValues.end(),
                  row.second[column]) != columnValues.end()) {
      return true;
    }
  }
  return false;
}

bool doesRowWithNullValueExist(
    std::map<std::string, std::map<Column *, fieldValueType>> &rows,
    Column *column) {

  for (auto &row : rows) {
    if (std::holds_alternative<nullptr_t>(row.second[column])) {
      return true;
    }
  }
  return false;
}

bool doesRowWithNotNullValueExist(
    std::map<std::string, std::map<Column *, fieldValueType>> &rows,
    Column *column) {

  for (auto &row : rows) {
    if (!std::holds_alternative<nullptr_t>(row.second[column])) {
      return true;
    }
  }
  return false;
}

std::map<std::string, Table *> Table::tables = std::map<std::string, Table *>();

Table::Table(const std::string &nameArg) {
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

void Table::renameTo(std::string newName) {
  if (Table::tables.find(newName) != Table::tables.end())
    throw std::runtime_error("Table " + newName + " already exists");
  Table::tables.erase(name);
  Table::tables.insert(std::make_pair(newName, this));
  name = newName;
}

void Table::addColumn(std::string columnName, std::string columnType,
                      bool nullable, bool primaryKey, bool unique) {

  if (columns.find(columnName) != columns.end()) {
    throw std::runtime_error("Column " + columnName + " already exists");
  }

  auto *type = Type::getType(columnType);

  if ((primaryKey || !nullable) && !rows.empty()) {
    throw std::runtime_error(
        "Can't add not nullable column to non empty table, "
        "consider truncating table first");
  }

  if (nullable && primaryKey) {
    throw std::runtime_error("Column " + columnName +
                             " can't be nullable and primary key, consider "
                             "adding NOT_NULL constraint");
  }

  auto *column = new Column(columnName, type, nullable, primaryKey, unique);
  columns.insert(std::make_pair(columnName, column));

  for (auto &row : rows) {
    row.second.insert(std::make_pair(column, nullptr));
  }
}

void Table::alterColumn(std::string columnName, std::string columnType,
                        bool nullable, bool primaryKey, bool unique) {

  if (columns.find(columnName) == columns.end()) {
    throw std::runtime_error("Column " + columnName + " not found");
  }

  auto *column = columns[columnName];

  if (nullable && primaryKey) {
    throw std::runtime_error("Column " + columnName +
                             " can't be nullable and primary key, consider "
                             "adding NOT_NULL constraint");
  }

  if (!rows.empty()) {
    if (!column->isPrimaryKey() && primaryKey) {
      if (doesRowWithNullValueExist(rows, column) ||
          doesRowWithSameValueExist(rows, column))
        throw std::runtime_error(
            "Cannot change PRIMARY_KEY constraint on column " + columnName +
            ", "
            "consider removing NULL values and duplicates");
    }
    if (!column->isUnique() && unique) {
      if (doesRowWithSameValueExist(rows, column))
        throw std::runtime_error(
            "Column " + columnName +
            " contains duplicate values, cannot add UNIQUE "
            "constraint, consider removing duplicates");
    }

    if (column->isNullable() && !nullable) {
      if (doesRowWithNullValueExist(rows, column))
        throw std::runtime_error("Column " + columnName +
                                 " contains NULL values");
    }
    if (columnType != column->getType()->getName() &&
        doesRowWithNotNullValueExist(rows, column)) {
      throw std::runtime_error("Cannot change type of column " + columnName +
                               "while it contains not null values");
    }
  }

  auto *type = Type::getType(columnType);

  column->setNullable(nullable);
  column->setPrimaryKey(primaryKey);
  column->setUnique(unique);
  column->setType(type);
}

void Table::dropColumn(std::string columnName) {

  for (auto &row : rows) {
    row.second.erase(getColumn(columnName));
  }

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
    std::vector<std::pair<std::string, std::string>> &columnNamesAndValues) {
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
    std::vector<ArgumentsForComparing> &argumentsForComparing) {
  auto meetingConditionsRowIds =
      getIdRowsMatchingConditions(argumentsForComparing);

  for (auto &rowId : meetingConditionsRowIds) {
    rows.erase(rowId);
  }
}

void Table::update(
    std::vector<std::pair<std::string, std::string>> &columnNamesAndValues,
    std::vector<ArgumentsForComparing> &argumentsForComparing) {
  auto meetingConditionsRowIds =
      getIdRowsMatchingConditions(argumentsForComparing);

  for (auto &rowId : meetingConditionsRowIds) {
    setValues(columnNamesAndValues, rowId);
  }
}

void Table::setValues(
    std::vector<std::pair<std::string, std::string>> &columnNamesAndValues,
    std::string rowId) {

  // validation for every field before setting to avoid partial changes
  for (auto &pair : columnNamesAndValues) {
    auto [columnName, value] = pair;
    validateNewValue(columnName, value);
  }

  for (auto &pair : columnNamesAndValues) {
    auto [columnName, value] = pair;

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

void Table::validateNewValue(const std::string &columnName,
                             const std::string &value) {

  auto *column = getColumn(columnName);
  auto *type = column->getType();

  auto replacedValue = replacePlaceholdersWithValues(value);

  if (column->isPrimaryKey() && value == "NULL") {
    throw std::runtime_error("Column " + columnName +
                             " is primary key, cannot set NULL value");
  }

  if (column->isUnique() || column->isPrimaryKey()) {
    for (auto &row : rows) {
      if (value != "NULL" &&
          fieldToString(row.second[column]) == replacedValue) {
        throw std::runtime_error("Value " + replacedValue + " in column " +
                                 columnName + " is not unique");
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

void Table::describe() {
  auto printInstruction = preparePrintInstructionForDescribe();
  drawTable(printInstruction);
}

void Table::select(
    const std::vector<std::string> &columnNames,
    const std::vector<ArgumentsForComparing> &argumentsForComparing) {

  auto meetingConditionsRowIds =
      getIdRowsMatchingConditions(argumentsForComparing);

  auto printInstruction =
      preparePrintInstructionForSelect(columnNames, meetingConditionsRowIds);
  drawTable(printInstruction);
}

PrintInstruction Table::preparePrintInstructionForSelect(
    const std::vector<std::string> &columnNames,
    const std::vector<std::string> &rowIdsToSelect) {

  auto printInstruction = std::vector<std::vector<std::string>>();

  for (auto &rowId : rowIdsToSelect) {
    if (rows.find(rowId) == rows.end()) {
      throw std::runtime_error("Row with id " + rowId + " not found");
    }
    auto row = rows[rowId];

    auto rowInstruction = std::vector<std::string>();
    for (auto &columnName : columnNames) {
      auto value = fieldToString(row[getColumn(columnName)]);
      rowInstruction.push_back(value);
    }
    printInstruction.push_back(rowInstruction);
  }

  return PrintInstruction(columnNames, printInstruction);
}

PrintInstruction Table::preparePrintInstructionForDescribe() {

  auto printInstruction = std::vector<std::vector<std::string>>();



  for (auto &column : columns) {
    auto rowInstruction = std::vector<std::string>();
    rowInstruction.push_back(column.first);
    rowInstruction.push_back(column.second->getType()->getName());
    rowInstruction.push_back(column.second->isNullable() ? "YES" : "NO");
    rowInstruction.push_back(column.second->isPrimaryKey() ? "YES" : "NO");
    rowInstruction.push_back(column.second->isUnique() ? "YES" : "NO");
    printInstruction.push_back(rowInstruction);
  }

  //move row instruction with id to the beggining
        auto idIndex = std::find_if(printInstruction.begin(), printInstruction.end(), [](auto &row) {
        return row[0] == "id";
        });
        if (idIndex != printInstruction.end()) {
            auto idRow = *idIndex;
            printInstruction.erase(idIndex);
            printInstruction.insert(printInstruction.begin(), idRow);
        }

  return PrintInstruction({"Field", "Type", "Null", "Primary Key", "Unique"},
                          printInstruction);
}

void Table::drawTable(PrintInstruction &printInstruction) {

  auto columnNames = printInstruction.columnNames;
  auto rows = printInstruction.rows;

  auto longestValueInColumn = std::map<std::string, int>();

  for (auto &columnName : columnNames) {
    longestValueInColumn[columnName] = columnName.size();
  }

  for (auto &row : rows) {
    for (int i = 0; i < row.size(); i++) {
      auto value = row[i];
      if (value.size() > longestValueInColumn[columnNames[i]]) {
        longestValueInColumn[columnNames[i]] = value.size();
      }
    }
  }
  auto longestValues = std::vector<int>();
  for (int i = 0; i < columnNames.size(); i++) {
    longestValues.push_back(longestValueInColumn[columnNames[i]]);
  }

  auto horizontalLine = std::string("*-");
  for (auto &longestValue : longestValues) {
    horizontalLine.append(longestValue, '-');
    horizontalLine += "-+-";
  }
  horizontalLine = horizontalLine.substr(0, horizontalLine.size() -
                                                1); // always one '-' too many
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

  for (auto &el : rows) {
    fmt::print("| ");

    for (int i = 0; i < el.size(); i++) {
      auto value = el[i];

      value.append(longestValues[i] - value.size(), ' ');
      value += " | ";
      fmt::print("{}", value);
    }

    fmt::println("");
  }
  if (rows.size() == 0) {
    auto supplyStr = std::string("| ");
    supplyStr.append(horizontalLine.size() - 4, ' ');
    supplyStr += " |";
    fmt::println("{}", supplyStr);
  }
  fmt::println("{}", horizontalLine);
}

std::vector<std::string> Table::getIdRowsMatchingConditions(
    const std::vector<ArgumentsForComparing> &argumentsForComparing) {

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
  return meetingConditionsRowIds;
}