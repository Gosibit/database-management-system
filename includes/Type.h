#pragma once
#include "aliases.h"
#include <map>
#include <string>
class Type {
  std::string name;

public:
  Type(const std::string &name);

  static std::map<std::string, Type *> types;
  static Type *getType(const std::string &name);

  virtual bool isValueValid(const std::string &value);
  
  std::string getName();
  fieldValueType parseValue(const std::string &value);
};
