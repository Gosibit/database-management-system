#pragma once

#include "TypeDerivaties.h"
#include <fmt/core.h>
#include <iostream>
#include <sstream>
#include <string>

Int::Int() : Type("int") { Type::types.insert(std::make_pair("int", this)); }
// check
bool Int::validate(std::string value) {
  try {
    std::stoi(value);
    return true;
  } catch (std::exception &e) {
    return false;
  }
}

Varchar::Varchar() : Type("varchar") {
  Type::types.insert(std::make_pair("varchar", this));
}

bool Varchar::validate(std::string value) { return true; }

Float::Float() : Type("float") {
  Type::types.insert(std::make_pair("float", this));
}

bool Float::validate(std::string value) {
  try {
    std::stof(value);
    return true;
  } catch (std::exception &e) {
    return false;
  }
}

Double::Double() : Type("double") {
  Type::types.insert(std::make_pair("double", this));
}

bool Double::validate(std::string value) {
  try {
    std::stod(value);
    return true;
  } catch (std::exception &e) {
    return false;
  }
}
