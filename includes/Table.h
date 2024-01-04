#pragma once

#include <iostream>
#include <map>
#include "Column.h"

class Table {
    std::string name;
    std::map<std::string, Column*> columns; //name, type
    std::map<std::string, std::map<std::string, std::string>> rows; //id, column , val
    int lastId;
public:
    static std::map<std::string, Table*> tables;
    static void printTables();
    static Table* getTable(std::string name);
    Table(std::string nameArg);
    void println();
    void renameTo(std::string newName);
    void addColumn(std::string columnName, std::string columnType);
    void printColumns();

};