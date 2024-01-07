#pragma once

#include "TypeDerivaties.h"
#include <fmt/core.h>
#include <iostream>
#include <sstream>
#include <string>

Int::Int() : Type("int") { Type::types.insert(std::make_pair("int", this)); }
// check
bool Int::isValueValid(std::string value) {
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

bool Varchar::isValueValid(std::string value) { return true; }

Float::Float() : Type("float") {
  Type::types.insert(std::make_pair("float", this));
}

bool Float::isValueValid(std::string value) {
  try {
    auto parsedValue = std::stof(value);
    //    fmt::println("parsedValue: {}, value: {}",
    //    std::to_string(parsedValue), value); if (std::to_string(parsedValue)
    //    != value) return false;
  } catch (std::exception &e) {
    return false;
  }
  return true;
}

Double::Double() : Type("double") {
  Type::types.insert(std::make_pair("double", this));
}

bool Double::isValueValid(std::string value) {
  try {
    auto parsedValue = std::stod(value);
    if (std::to_string(parsedValue) != value)
      return false;
  } catch (std::exception &e) {
    return false;
  }
  return true;
}

boolean::boolean() : Type("boolean") {
  Type::types.insert(std::make_pair("boolean", this));
}

bool boolean::isValueValid(std::string value) {
  if (value == "true" || value == "false")
    return true;
  return false;
}
