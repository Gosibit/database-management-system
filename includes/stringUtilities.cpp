#include "stringUtilities.h"

#include "tokenizer.h"
#include <string>
#include <variant>

std::string getPartAfterDelimiter(const std::string &str,
                                  const std::string &delimiter) {
  return str.substr(str.find(delimiter) + delimiter.length(), str.length());
}
std::string getPartBeforeDelimiter(const std::string &str,
                                   const std::string &delimiter) {
  return str.substr(0, str.find(delimiter));
}
std::string getPartBetweenDelimiters(const std::string &str,
                                     const std::string &begginingDelimiter,
                                     const std::string &endingDelimiter) {
  auto strAfterBegginingDelimiter =
      getPartAfterDelimiter(str, begginingDelimiter);
  auto strBetweenDelimiters =
      getPartBeforeDelimiter(strAfterBegginingDelimiter, endingDelimiter);
  return strBetweenDelimiters;
}

std::string removeAllOccurencies(std::string str, char toRemove) {
  str.erase(std::remove(str.begin(), str.end(), toRemove), str.end());
  return str;
}

std::string removeAllOccurencies(std::string str, std::vector<char> toRemove) {
  for (auto &c : toRemove) {
    str.erase(std::remove(str.begin(), str.end(), c), str.end());
  }
  return str;
}

std::string removeBrackets(const std::string &str) {
  auto brackets = std::vector<char>{'(', ')'};
  return removeAllOccurencies(str, brackets);
}

std::vector<std::string> splitByComma(const std::string &str) {
  auto splitted = std::vector<std::string>();
  auto splittedByComma = tokenize(str, ",");
  return splittedByComma;
}

std::vector<std::string> splitBySpace(const std::string &str) {
  auto splitted = std::vector<std::string>();
  auto splittedBySpace = tokenize(str, " ");

  return splittedBySpace;
}

std::string trim(const std::string &str) {
  auto trimmed = std::string();
  for (auto &c : str) {
    if (c != ' ')
      trimmed += c;
  }
  return trimmed;
}

std::vector<std::string> splitByQueries(const std::string &str) {
  auto splitted = std::vector<std::string>();
  auto splittedByQueries = tokenize(str, ";");

  for (auto &s : splittedByQueries) {
    s += ";";
  }

  return splittedByQueries;
}

std::string removeSemicolons(std::string str) {
  return removeAllOccurencies(str, ';');
}

std::string fieldToString(const fieldValueType &field) {
  if (std::holds_alternative<int>(field)) {
    return std::to_string(std::get<int>(field));
  } else if (std::holds_alternative<double>(field)) {
    return std::to_string(std::get<double>(field));
  } else if (std::holds_alternative<float>(field)) {
    return std::to_string(std::get<float>(field));
  } else if (std::holds_alternative<bool>(field)) {
    return std::to_string(std::get<bool>(field));
  } else if (std::holds_alternative<std::nullptr_t>(field)) {
    return "NULL";
  } else if (std::holds_alternative<std::string>(field)) {
    return std::get<std::string>(field);
  }
  throw std::runtime_error("Unknown type, couldn't parse to string");
}