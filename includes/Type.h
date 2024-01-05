#pragma once
#include <map>
#include <string>

class Type {
  std::string name;

public:
  static std::map<std::string, Type *> types;
  Type(std::string name);
  static Type *getType(std::string name);
  virtual bool validate(std::string value);
  std::string getName();
};