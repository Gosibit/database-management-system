#pragma once

#include "Type.h"
#include <string>

class Column {
  std::string name;
  Type *type;
  Column *relatedColumn;
  bool primaryKey;
  bool foreignKey;
  bool unique;
  bool nullable;

public:
  Column(const std::string &nameArg, Type *typeArg, const bool &nullableArg,
         const bool &primaryKeyArg, const bool &uniqueArg);
  
  void println();
  std::string getName();
  Type *getType();
  bool isNullable();
  bool isPrimaryKey();
  bool isUnique();
};