#pragma once
#include "./Table.h"
#include <iostream>
#include <fmt/core.h>
#include <fmt/ranges.h>

std::map<std::string, Table*> Table::tables = std::map<std::string, Table*>();

Table::Table(std::string nameArg, std::vector<std::string> columnsArg) {
    name = nameArg;
    columns = columnsArg;

    Table::tables.insert(std::make_pair(name, this));

};

void Table::println() {
    fmt::print("Table: {}, ", name);
    fmt::print("Columns: {}, ", fmt::join(columns, ", "));
    fmt::print("Rows: {}", rows);
}

void Table::printTables() {
    for (auto& table : Table::tables) {
        table.second->println();
    }
}
