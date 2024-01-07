#pragma once
#include "./includes/KeywordDerivaties.h"
#include "./includes/TypeDerivaties.h"

auto createTable = CreateTable();
auto alterTable = AlterTable();
auto dropTable = DropTable();
auto insertInto = InsertInto();
auto select = Select();

auto intType = Int();
auto varcharType = Varchar();
auto floatType = Float();
auto doubleType = Double();
auto booleanType = boolean();