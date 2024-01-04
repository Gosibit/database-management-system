#pragma once

#include "KeywordDerivaties.h"
#include <fmt/core.h>
#include "Table.h"
#include "tokenizer.h"
#include <fmt/ranges.h>

AlterTable::AlterTable() : Keyword("ALTER_TABLE", { "ADD", "DROP" })
{
    Keyword::keywords.insert(std::make_pair("ALTER_TABLE", this));
};

void AlterTable::process()
{
    fmt::println("AlterTable::process() triggered");
}

CreateTable::CreateTable() : Keyword("CREATE_TABLE", {})
{
    Keyword::keywords.insert(std::make_pair("CREATE_TABLE", this));
};

void CreateTable::process()
{
    fmt::println("CreateTable::process() triggered");

    auto args = tokenize(this->getQuery(), " ");
    auto name = args[1];
    auto columnsInfo = args[2];

    //todo: move to something like stringutils.cpp
    columnsInfo.erase(std::remove(columnsInfo.begin(), columnsInfo.end(), '('), columnsInfo.end());
    columnsInfo.erase(std::remove(columnsInfo.begin(), columnsInfo.end(), ')'), columnsInfo.end());
    columnsInfo.erase(std::remove(columnsInfo.begin(), columnsInfo.end(), '\''), columnsInfo.end());

    auto columnsTokenized = tokenize(columnsInfo, ",");

    fmt::println("Columns info: {}", columnsTokenized);

    auto table = new Table(name, columnsTokenized);

    Table::printTables();
}
