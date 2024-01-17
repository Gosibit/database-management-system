#pragma once

#include "Type.h"
#include <string>

class Int : public Type {
public:
  Int();
  bool isValueValid(const std::string &value) override;
};

class Varchar : public Type {
public:
  Varchar();
  bool isValueValid(const std::string &value) override;
};

class Float : public Type {
public:
  Float();
  bool isValueValid(const std::string &value) override;
};

class Double : public Type {
public:
  Double();
  bool isValueValid(const std::string &value) override;
};

class boolean : public Type {
public:
  boolean();
  bool isValueValid(const std::string &value) override;
};
