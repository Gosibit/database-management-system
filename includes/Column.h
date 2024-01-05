#pragma once

#include "Type.h"
#include <string>

class Column {
  std::string name;
  Type *type;
  Column *relatedColumn;
  bool isPrimaryKey;
  bool isForeignKey;
  bool isUnique;
  bool isNotNull;
  bool isAutoIncrement;

public:
  Column(std::string nameArg, Type *typeArg);
  void println();
  std::string getName();
  Type *getType();
};