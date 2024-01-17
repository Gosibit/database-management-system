#pragma once

#include "Type.h"
#include <string>

class Int : public Type {
public:
  Int();
  bool isValueValid(std::string value) override;
};

class Varchar : public Type {
public:
  Varchar();
  bool isValueValid(std::string value) override;
};

class Float : public Type {
public:
  Float();
  bool isValueValid(std::string value) override;
};

class Double : public Type {
public:
  Double();
  bool isValueValid(std::string value) override;
};

class boolean : public Type {
public:
  boolean();
  bool isValueValid(std::string value) override;
};
