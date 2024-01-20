#pragma once
#include <string>

#include "Keyword.h"

class AlterTable : public Keyword {
public:
  AlterTable();
  void process() override;
};

class CreateTable : public Keyword {
public:
  CreateTable();
  void process() override;
};

class DropTable : public Keyword {
public:
  DropTable();
  void process() override;
};

class Desc : public Keyword {
public:
  Desc();
  void process() override;
};

class InsertInto : public Keyword {
public:
  InsertInto();
  void process() override;
};

class Select : public Keyword {
public:
  Select();
  void process() override;
};

class Update : public Keyword {
public:
  Update();
  void process() override;
};

class DeleteFrom : public Keyword {
public:
  DeleteFrom();
  void process() override;
};

class DumpRestore : public Keyword {
public:
  DumpRestore();
  void process() override;
};

class DumpCreate : public Keyword {
public:
  DumpCreate();
  void process() override;
};