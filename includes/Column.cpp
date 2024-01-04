#pragma once

#include <fmt/core.h>
#include "Column.h"

Column::Column(std::string nameArg, std::string typeArg) {
    name = nameArg;
    type = typeArg;
    relatedColumn = nullptr;
    isPrimaryKey = false;
    isForeignKey = false;
    isUnique = false;
    isNotNull = false;
    isAutoIncrement = false;
}

void Column::println() {
    fmt::print("Column: {}, Type: {}", name, type);
}

std::string Column::getName() {
    return name;
}

std::string Column::getType() {
    return type;
}