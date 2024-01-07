#pragma once

#include "Column.h"
#include "Type.h"
#include <fmt/core.h>

Column::Column(std::string nameArg, Type *typeArg) {
  name = nameArg;
  type = typeArg;
  relatedColumn = nullptr;
  primaryKey = false;
  foreignKey = false;
  unique = false;
  nullable = true;
  autoIncrement = false;
}

void Column::println() {
  fmt::print("Column: {}, Type: {}", name, type->getName());
}

std::string Column::getName() { return name; }

Type *Column::getType() { return type; }

bool Column::isNullable() { return nullable; }