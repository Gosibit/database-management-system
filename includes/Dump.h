#pragma once

#include <string>

class Dump {
public:
  static void dump(std::string path);
  static void restore(std::string path);
};