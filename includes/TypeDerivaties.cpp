#include "TypeDerivaties.h"
#include <string>

Int::Int() : Type("int") { Type::types.insert(std::make_pair("int", this)); }

bool Int::isValueValid(const std::string &value) {
  try {
    auto parsedValue = std::stoi(value);
    if (std::to_string(parsedValue) != value)
      return false;
  } catch (std::exception &e) {
    return false;
  }

  return true;
}

Varchar::Varchar() : Type("varchar") {
  Type::types.insert(std::make_pair("varchar", this));
}

bool Varchar::isValueValid(const std::string &value) { return true; }

Float::Float() : Type("float") {
  Type::types.insert(std::make_pair("float", this));
}

bool Float::isValueValid(const std::string &value) {
  try {
    std::stof(value);
  } catch (std::exception &e) {
    return false;
  }
  return true;
}

Double::Double() : Type("double") {
  Type::types.insert(std::make_pair("double", this));
}

bool Double::isValueValid(const std::string &value) {
  try {
    auto parsedValue = std::stod(value);
  } catch (std::exception &e) {
    return false;
  }
  return true;
}

boolean::boolean() : Type("boolean") {
  Type::types.insert(std::make_pair("boolean", this));
}

bool boolean::isValueValid(const std::string &value) {
  if (value == "true" || value == "false" || value == "0" || value == "1")
    return true;
  else
    return false;
}
