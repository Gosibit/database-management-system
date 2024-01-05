#pragma once

#include "Column.h"
#include "Type.h"
#include <fmt/core.h>

Column::Column(std::string nameArg, Type *typeArg) {
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
  fmt::print("Column: {}, Type: {}", name, type->getName());
}

std::string Column::getName() { return name; }

Type *Column::getType() { return type; }