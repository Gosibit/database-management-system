#pragma once
#include <map>
#include <string>
#include "aliases.h"
class Type  {
  std::string name;

public:
  static std::map<std::string, Type *> types;
  Type(std::string name);
  static Type *getType(std::string name);
  virtual bool isValueValid(std::string value);
  std::string getName();
  valueField parseValue(std::string value);
};
