#pragma once
#include "./includes/TypeDerivaties.h"
#include "./includes/stringUtilities.h"
#include <fmt/core.h>
#include <iostream>

int main() {
  auto intType = Int();
  auto floatType = Float();
  auto doubleType = Double();
  auto varcharType = Varchar();

  fmt::println("int test for 123 {}", intType.validate("123"));
  fmt::println("int test for 123.11 {}", intType.validate("123.11"));

  fmt::println("float test for 123 {}", floatType.validate("123"));
  fmt::println("float test for 123.11 {}", floatType.validate("123.11"));
  fmt::println("float test for str {}", floatType.validate("str"));

  fmt::println("double test for 123 {}", doubleType.validate("123"));
  fmt::println("double test for 123.11 {}", doubleType.validate("123.11"));
  fmt::println("double test for str {}", doubleType.validate("str"));

  return 0;
}