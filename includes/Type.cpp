#pragma once
#include "Type.h"
#include "aliases.h"
#include <string>
std::map<std::string, Type *> Type::types = std::map<std::string, Type *>();

Type::Type(std::string nameArg) { name = nameArg; };

bool Type::isValueValid(std::string value) { return true; }

std::string Type::getName() { return name; }

Type *Type::getType(std::string name) {
  if (types.find(name) == types.end())
    throw std::runtime_error("Type " + name + " not found");
  return types[name];
}

fieldValueType Type::parseValue(std::string value) {
  if (name == "int") {
    return std::stoi(value);
  } else if (name == "float") {
    return std::stof(value);
  } else if (name == "double") {
    return std::stod(value);
  } else if (name == "boolean") {
    return value == "true";
  } else if (name == "varchar") {
    return value;
  } else {
    throw std::runtime_error("Type " + name + " not found");
  }
}
