#pragma once
#include "Type.h"
#include <string>

std::map<std::string, Type *> Type::types = std::map<std::string, Type *>();

Type::Type(std::string nameArg) { name = nameArg; };

bool Type::validate(std::string value) { return true; }

std::string Type::getName() { return name; }

Type *Type::getType(std::string name) {
  if (types.find(name) == types.end())
    throw std::runtime_error("Type " + name + " not found");
  return types[name];
}