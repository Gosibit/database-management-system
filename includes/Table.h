#pragma once

#include <iostream>
#include <map>

class Table {
    std::string name;
    std::vector<std::string> columns;
    std::map<std::string, std::map<std::string, std::string>> rows; //id, column , val
    int lastId;
public:
    static std::map<std::string, Table*> tables;
    static void printTables();
    Table(std::string nameArg, std::vector<std::string> columnsArg);
    void println();

};