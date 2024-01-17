#pragma once

#include "Column.h"
#include "Type.h"
#include <fmt/core.h>

Column::Column(const std::string &nameArg, Type *typeArg,
               const bool &nullableArg, const bool &primaryKeyArg,
               const bool &uniqueArg) {

  name = nameArg;
  type = typeArg;
  relatedColumn = nullptr;
  primaryKey = primaryKeyArg;
  foreignKey = false;
  unique = uniqueArg;
  nullable = nullableArg;
}

void Column::println() {
  fmt::print("Column: {}, Type: {}", name, type->getName());
}

std::string Column::getName() { return name; }

Type *Column::getType() { return type; }

bool Column::isNullable() { return nullable; }

bool Column::isPrimaryKey() { return primaryKey; }

bool Column::isUnique() { return unique; }