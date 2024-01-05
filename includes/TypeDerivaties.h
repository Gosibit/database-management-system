#pragma once

#include "Type.h"
#include <string>

class Int : public Type {
public:
  Int();
  bool validate(std::string value);
};

class Varchar : public Type {
public:
  Varchar();
  bool validate(std::string value);
};

class Float : public Type {
public:
  Float();
  bool validate(std::string value);
};

class Double : public Type {
public:
  Double();
  bool validate(std::string value);
};
