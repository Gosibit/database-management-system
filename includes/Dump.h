#pragma once

#include <string>

class Dump {
public:
  static void dump(const std::string &path);
  static void restore(const std::string &path);
};