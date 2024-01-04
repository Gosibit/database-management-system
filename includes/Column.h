#pragma once

#include <string>

class Column {
    std::string name;
    std::string type;
    Column* relatedColumn;
    bool isPrimaryKey;
    bool isForeignKey;
    bool isUnique;
    bool isNotNull;
    bool isAutoIncrement;

public:
    Column(std::string nameArg, std::string typeArg);
    void println();
    std::string getName();
    std::string getType();
};