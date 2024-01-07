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
  bool autoIncrement;


public:
  Column(std::string nameArg, Type *typeArg);
  void println();
  std::string getName();
  Type *getType();
  bool isNullable();
};