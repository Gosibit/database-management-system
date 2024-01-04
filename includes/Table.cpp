#pragma once
#include "./Table.h"
#include <iostream>
#include <fmt/core.h>
#include <fmt/ranges.h>

std::map<std::string, Table*> Table::tables = std::map<std::string, Table*>();

Table::Table(std::string nameArg) {
    name = nameArg;


    Table::tables.insert(std::make_pair(name, this));

};

Table* Table::getTable(std::string name) {
    return Table::tables[name];
};

void Table::println() {
    fmt::print("Table: {}, ", name);
    printColumns();
    fmt::print("Rows: {}", rows);
}

void Table::printTables() {
    for (auto& table : Table::tables) {
        table.second->println();
    }
}

void Table::printColumns() {
    for (auto& column : columns) {
        column.second->println();
    }
}

void Table::renameTo(std::string newName) {
    name = newName;
}

void Table::addColumn(std::string columnName, std::string columnType) {
    auto* column = new Column(columnName, columnType);
    fmt::println("Adding column: {}", columnName);
    columns.insert(std::make_pair(columnName, column));
}
