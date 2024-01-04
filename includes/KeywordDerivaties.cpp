#pragma once

#include "KeywordDerivaties.h"
#include <fmt/core.h>
#include "Table.h"
#include "tokenizer.h"
#include <fmt/ranges.h>
#include "stringUtilities.h"

AlterTable::AlterTable() : Keyword("ALTER_TABLE", { "ADD_COLUMN", "DROP_COLUMN", "RENAME_TO" })
{
    Keyword::keywords.insert(std::make_pair("ALTER_TABLE", this));
};

void AlterTable::process()
{
    auto tableName = removeAllOccurencies(keywordArguments, ' ');
    auto table = Table::getTable(tableName);

    for (auto& interaction : foundInteractions) {
        auto interactionName = interaction.first;
        auto interactionContent = interaction.second;

        if (interactionName == "RENAME_TO") {
            interactionContent = removeAllOccurencies(interactionContent, ' ');
            table->renameTo(interactionContent);
        }
    }

    Table::printTables();
}

CreateTable::CreateTable() : Keyword("CREATE_TABLE", {})
{
    Keyword::keywords.insert(std::make_pair("CREATE_TABLE", this));
};

void CreateTable::process()
{
    fmt::println("CreateTable::process() triggered");



    auto args = tokenize(keywordArguments, " "); // {tab_name, (col1 int, col2 string, col3 int)}
    auto tableName = trim(args[0]);
    auto* table = new Table(tableName);

    auto columnsArgs = getPartBetweenDelimiters(this->getQuery(), "(", ")"); // col1 int, col2 string, col3 int
    auto splittedColumnArgs = splitByComma(columnsArgs); // {col1 int, col2 string, col3 int}

    for (auto& columnArgs : splittedColumnArgs) {

        auto columnArgTokenized = splitBySpace(columnArgs); // {col1, int}

        fmt::println("columnArgTokenized: {}", columnArgTokenized);

        auto columnName = trim(columnArgTokenized[0]); // col1
        auto columnType = trim(columnArgTokenized[1]); // int

        table->addColumn(columnName, columnType);
    }
    table->printColumns();
}
