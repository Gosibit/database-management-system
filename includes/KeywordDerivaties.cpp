#pragma once

#include "KeywordDerivaties.h"

#include <fmt/core.h>
#include <fmt/ranges.h>

#include "ArgumentsForComparing.h"
#include "Dump.h"
#include "Table.h"
#include "queryProcessor.h"
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

    auto args = splitBySpace(interactionContent);
    auto unique =
        std::find(args.begin() + 1, args.end(), "UNIQUE") != args.end();
    auto nullable =
        std::find(args.begin() + 1, args.end(), "NOT_NULL") == args.end();
    auto primaryKey =
        std::find(args.begin() + 1, args.end(), "PRIMARY_KEY") != args.end();

    if (interactionName == "ADD_COLUMN") {
      auto args = splitBySpace(interactionContent); // {col1, int}
      auto columnName = trim(args[0]);
      auto columnType = trim(args[1]);

      table->addColumn(columnName, columnType, nullable, primaryKey, unique);
    }

    if (interactionName == "RENAME_TO") {
      interactionContent = removeAllOccurencies(interactionContent, ' ');
      table->renameTo(interactionContent);
    }

    if (interactionName == "DROP_COLUMN") {
      interactionContent = trim(interactionContent);
      table->dropColumn(interactionContent);
    }

    if (interactionName == "ALTER_COLUMN") {
      auto args = splitBySpace(interactionContent); // {col1, int}
      auto columnName = trim(args[0]);
      auto columnType = trim(args[1]);

      table->dropColumn(columnName);
      table->addColumn(columnName, columnType, nullable, primaryKey, unique);
    }
  }
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

    auto unique =
        std::find(columnArgTokenized.begin() + 1, columnArgTokenized.end(),
                  "UNIQUE") != columnArgTokenized.end();
    auto nullable =
        std::find(columnArgTokenized.begin() + 1, columnArgTokenized.end(),
                  "NOT_NULL") == columnArgTokenized.end();
    auto primaryKey =
        std::find(columnArgTokenized.begin() + 1, columnArgTokenized.end(),
                  "PRIMARY_KEY") != columnArgTokenized.end();

    table->addColumn(columnName, columnType, nullable, primaryKey, unique);
  }
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
}

InsertInto::InsertInto() : Keyword("INSERT_INTO", {"VALUES"}) {
  Keyword::keywords.insert(std::make_pair("INSERT_INTO", this));
};

void InsertInto::process() {
  auto trimmedKeywordArgs = trim(keywordArguments);
  auto tableName = getPartBeforeDelimiter(trimmedKeywordArgs, "(");

  auto columnNamesPart = getPartBetweenDelimiters(trimmedKeywordArgs, "(", ")");

  auto columnNamesVector = splitByComma(columnNamesPart);

  if (foundInteractions.find("VALUES") == foundInteractions.end()) {
    throw std::runtime_error("Missing `VALUES` statement");
  }

  auto valuesPart = foundInteractions["VALUES"];
  valuesPart = removeBrackets(trim(valuesPart));

  auto valuesVector = splitByComma(valuesPart);

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
}

Select::Select() : Keyword("SELECT", {"FROM", "WHERE"}) {
  Keyword::keywords.insert(std::make_pair("SELECT", this));
};

void Select::process() {

  if (foundInteractions.find("FROM") == foundInteractions.end()) {
    throw std::runtime_error("Missing `FROM` statement");
  }
  auto tableName = trim(foundInteractions["FROM"]);
  auto *table = Table::getTable(tableName);

  auto trimmedKeywordArgs = trim(keywordArguments);

  auto columnNames = std::vector<std::string>();
  if (std::find(trimmedKeywordArgs.begin(), trimmedKeywordArgs.end(), '*') !=
      trimmedKeywordArgs.end()) {
    columnNames = table->getColumnNames();
  } else
    columnNames = splitByComma(trimmedKeywordArgs);

  auto argumentsToCompare = std::vector<ArgumentsForComparing>();

  if (foundInteractions.find("WHERE") != foundInteractions.end()) {
    auto whereArguments = foundInteractions["WHERE"];
    argumentsToCompare = this->parseWhereArguments(whereArguments, table);
  }

  table->select(columnNames, argumentsToCompare);
}

Update::Update() : Keyword("UPDATE", {"SET", "WHERE"}) {
  Keyword::keywords.insert(std::make_pair("UPDATE", this));
};

void Update::process() {
  auto tableName = trim(keywordArguments);
  auto *table = Table::getTable(tableName);

  auto setArguments = foundInteractions["SET"];
  auto setArgumentsSplitted = splitByComma(setArguments);

  auto columnNameAndValues = std::vector<std::pair<std::string, std::string>>();

  for (auto &setArgument : setArgumentsSplitted) {
    auto setArgumentSplittedBySpace = splitBySpace(setArgument); // {col1, =, 1}
    auto columnName = setArgumentSplittedBySpace[0];
    auto valueArg = setArgumentSplittedBySpace[2];

    columnNameAndValues.push_back(std::make_pair(columnName, valueArg));
  }

  auto argumentsForComparing = std::vector<ArgumentsForComparing>();

  if (foundInteractions.find("WHERE") != foundInteractions.end()) {
    auto whereArguments = foundInteractions["WHERE"];
    argumentsForComparing = this->parseWhereArguments(whereArguments, table);
  }

  table->update(columnNameAndValues, argumentsForComparing);
}

DeleteFrom::DeleteFrom() : Keyword("DELETE_FROM", {"WHERE"}) {
  Keyword::keywords.insert(std::make_pair("DELETE_FROM", this));
};

void DeleteFrom::process() {
  auto tableName = trim(keywordArguments);
  auto *table = Table::getTable(tableName);

  auto argumentsForComparing = std::vector<ArgumentsForComparing>();

  if (foundInteractions.find("WHERE") != foundInteractions.end()) {
    auto whereArguments = foundInteractions["WHERE"];
    argumentsForComparing = this->parseWhereArguments(whereArguments, table);
  }

  table->deleteFrom(argumentsForComparing);
}

DumpRestore::DumpRestore() : Keyword("DUMP_RESTORE", {"<"}) {
  Keyword::keywords.insert(std::make_pair("DUMP_RESTORE", this));
};

void DumpRestore::process() {
  auto option = trim(keywordArguments);
  if (option == "DATABASE") {
    auto path = trim(foundInteractions["<"]);
    Dump::restore(path);
  } else {
    throw std::runtime_error("Invalid option");
  }
}

DumpCreate::DumpCreate() : Keyword("DUMP_CREATE", {">"}) {
  Keyword::keywords.insert(std::make_pair("DUMP_CREATE", this));
};

void DumpCreate::process() {
  auto option = trim(keywordArguments);
  if (option == "DATABASE") {
    auto path = trim(foundInteractions[">"]);
    Dump::dump(path);
  } else {
    throw std::runtime_error("Invalid option");
  }
}
