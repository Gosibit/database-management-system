#include "Type.h"
#include "aliases.h"
#include <string>
std::map<std::string, Type *> Type::types = std::map<std::string, Type *>();

Type::Type(const std::string &nameArg) : name(nameArg) {}

bool Type::isValueValid(const std::string &value) { return true; }

std::string Type::getName() { return name; }

Type *Type::getType(const std::string &name) {
  if (types.find(name) == types.end())
    throw std::runtime_error("Type " + name + " not found");
  return types[name];
}

fieldValueType Type::parseValue(const std::string &value) {
  if (value == "NULL")
    return nullptr;

  if (name == "int") {
    return std::stoi(value);
  } else if (name == "float") {
    return std::stof(value);
  } else if (name == "double") {
    return std::stod(value);
  } else if (name == "varchar") {
    return value;
  } else if (name == "boolean") {
    if (value == "true")
      return true;
    else if (value == "false")
      return false;
    else
      return std::stoi(value);
  } else {
    throw std::runtime_error("Type " + name + " not found");
  }
}
