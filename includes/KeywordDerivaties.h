#pragma once
#include <string>

#include "Keyword.h"

class AlterTable : public Keyword {
public:
  AlterTable();
  void process();
};

class CreateTable : public Keyword {
public:
  CreateTable();
  void process();
};

class DropTable : public Keyword {
public:
  DropTable();
  void process();
};

class InsertInto : public Keyword {
public:
  InsertInto();
  void process();
};