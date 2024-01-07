#pragma once

#include "KeywordDerivaties.h"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "ArgumentsForComparing.h"
#include "Table.h"
#include "stringUtilities.h"
#include "tokenizer.h"

AlterTable::AlterTable()
    : Keyword("ALTER_TABLE", {"ADD_COLUMN", "DROP_COLUMN", "RENAME_TO"}) {
  Keyword::keywords.insert(std::make_pair("ALTER_TABLE", this));
};

void AlterTable::process() {
  auto tableName = removeAllOccurencies(keywordArguments, ' ');
  auto *table = Table::getTable(tableName);

  for (auto &interaction : foundInteractions) {
    auto interactionName = interaction.first;
    auto interactionContent = interaction.second;

    if (interactionName == "ADD_COLUMN") {
      auto args = splitBySpace(interactionContent); // {col1, int}
      auto columnName = trim(args[0]);
      auto columnType = trim(args[1]);

      table->addColumn(columnName, columnType);
    }

    if (interactionName == "RENAME_TO") {
      interactionContent = removeAllOccurencies(interactionContent, ' ');
      table->renameTo(interactionContent);
    }

    if (interactionName == "DROP_COLUMN") {
      interactionContent = trim(interactionContent);
      table->dropColumn(interactionContent);
    }
  }

  Table::printTables();
}

CreateTable::CreateTable() : Keyword("CREATE_TABLE", {}) {
  Keyword::keywords.insert(std::make_pair("CREATE_TABLE", this));
};

void CreateTable::process() {
  auto args = tokenize(keywordArguments,
                       " "); // {tab_name, (col1 int, col2 string, col3 int)}
  auto tableName = trim(args[0]);
  auto *table = new Table(tableName);

  auto columnsArgs = getPartBetweenDelimiters(
      this->getQuery(), "(", ")"); // col1 int, col2 string, col3 int
  auto splittedColumnArgs =
      splitByComma(columnsArgs); // {col1 int, col2 string, col3 int}

  for (auto &columnArgs : splittedColumnArgs) {
    auto columnArgTokenized = splitBySpace(columnArgs); // {col1, int}

    auto columnName = trim(columnArgTokenized[0]); // col1
    auto columnType = trim(columnArgTokenized[1]); // int

    table->addColumn(columnName, columnType);
  }

  Table::printTables();
}

DropTable::DropTable() : Keyword("DROP_TABLE", {}) {
  Keyword::keywords.insert(std::make_pair("DROP_TABLE", this));
};

void DropTable::process() {
  auto tableName = removeAllOccurencies(keywordArguments, ' ');
  tableName = removeSemicolons(tableName);
  auto *table = Table::getTable(tableName);
  delete table;
  Table::tables.erase(tableName);
  Table::printTables();
}

InsertInto::InsertInto() : Keyword("INSERT_INTO", {"VALUES"}) {
  Keyword::keywords.insert(std::make_pair("INSERT_INTO", this));
};

void InsertInto::process() {
  // INSERT_INTO tab1 (col1, col2, col3) VALUES (1, 'a', 2);
  auto trimmedKeywordArgs = trim(keywordArguments);
  auto tableName = getPartBeforeDelimiter(trimmedKeywordArgs, "(");

  auto columnNamesPart = getPartBetweenDelimiters(trimmedKeywordArgs, "(", ")");

  auto columnNamesVector = splitByComma(columnNamesPart);

  if (foundInteractions.find("VALUES") == foundInteractions.end()) {
    throw std::runtime_error("Missing `VALUES` statement");
  }

  auto valuesPart = foundInteractions["VALUES"];
  auto valuesPartReadyToSplit = removeBrackets(trim(valuesPart));

  auto valuesVector = splitByComma(valuesPartReadyToSplit);

  if (columnNamesVector.size() != valuesVector.size()) {
    throw std::runtime_error("Number of columns and values does not match");
  }

  auto *table = Table::getTable(tableName);

  auto insertArguments = std::vector<std::pair<std::string, std::string>>();

  for (int i = 0; i < columnNamesVector.size(); i++) {
    auto columnName = columnNamesVector[i];
    auto value = valuesVector[i];
    insertArguments.push_back(std::make_pair(columnName, value));
  }

  table->insertInto(insertArguments);
  Table::printTables();
}

Select::Select() : Keyword("SELECT", {"FROM", "WHERE"}) {
  Keyword::keywords.insert(std::make_pair("SELECT", this));
};
void Select::process() {

  if (foundInteractions.find("FROM") == foundInteractions.end()) {
    throw std::runtime_error("Missing `FROM` statement");
  }

  auto trimmedKeywordArgs = trim(keywordArguments);
  auto columnNames = splitByComma(trimmedKeywordArgs);

  auto tableName = trim(foundInteractions["FROM"]);
  auto *table = Table::getTable(tableName);

  auto argumentsToCompare = std::vector<ArgumentsForComparing>();

  if (foundInteractions.find("WHERE") != foundInteractions.end()) {
    auto whereArguments = foundInteractions["WHERE"]; // col1 = 1 AND col2 = 'a'
    auto whereArgumentsSplitted =
        splitBySpace(whereArguments); // {col1, =, 1, AND, col2, =, 'a'}

    for (int i = 0; i < whereArgumentsSplitted.size(); i += 3) {
      auto valueArg = whereArgumentsSplitted[i + 2];
      auto operatorArg = whereArgumentsSplitted[i + 1];
      auto columnName = whereArgumentsSplitted[i];
      auto logicalOperator = std::string();
      if (whereArgumentsSplitted[i + 3] == "AND" ||
          whereArgumentsSplitted[i + 3] == "OR") {
        logicalOperator = whereArgumentsSplitted[i + 3];
        i++;
      } else {
        logicalOperator = "AND";
      }

      auto *column = table->getColumn(columnName);
      auto *type = column->getType();
      fmt::println("valueArg: {}", valueArg);
      if (!type->isValueValid(valueArg)) {
        throw std::runtime_error("Value " + valueArg +
                                 " is not valid for type " + type->getName());
      }

      auto parsedArgValue = column->getType()->parseValue(valueArg);

      argumentsToCompare.push_back(ArgumentsForComparing(
          parsedArgValue, operatorArg, columnName, logicalOperator));
    }
  }

  table->select(columnNames, argumentsToCompare);
}