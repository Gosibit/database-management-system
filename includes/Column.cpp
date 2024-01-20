#include "Column.h"
#include "Type.h"
#include <fmt/core.h>

Column::Column(const std::string &nameArg, Type *typeArg,
               const bool &nullableArg, const bool &primaryKeyArg,
               const bool &uniqueArg)
    : name(nameArg), type(typeArg), nullable(nullableArg),
      primaryKey(primaryKeyArg), unique(uniqueArg), foreignKey(false),
      relatedColumn(nullptr) {}

void Column::println() {
  fmt::print("Column: {}, Type: {}", name, type->getName());
}

std::string Column::getName() { return name; }

Type *Column::getType() { return type; }

bool Column::isNullable() { return nullable; }

bool Column::isPrimaryKey() { return primaryKey; }

bool Column::isUnique() { return unique; }

void Column::setPrimaryKey(bool &primaryKeyArg) { primaryKey = primaryKeyArg; }

void Column::setUnique(bool &uniqueArg) { unique = uniqueArg; }

void Column::setNullable(bool &nullableArg) { nullable = nullableArg; }

void Column::setType(Type *typeArg) { type = typeArg; }