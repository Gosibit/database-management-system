#pragma once

#include "ArgumentsForComparing.h"
#include "Table.h"
#include <map>
#include <string>
#include <vector>

class Keyword {
  std::string name;
  std::vector<std::string> compatibleKeywords;
  std::string query;
  void assignKeywordArguments();
  void assignInteractions(const std::vector<std::string> &foundInteractionsArg);

protected:
  std::string keywordArguments;
  std::map<std::string, std::string> foundInteractions;

public:
  static std::vector<std::string> supportedKeywords;
  static std::map<std::string, Keyword *> keywords; // list of all keywords
  Keyword(std::string nameArg, std::vector<std::string> compatibleKeywordsArg);
  void println();
  bool isKeywordCompatible(const std::string &keyword);
  static void resetKeywordsData();

  std::vector<ArgumentsForComparing>
  parseWhereArguments(const std::string &whereArguments, Table *table);

  std::string getName();
  void prepare(const std::string &queryArg);
  virtual void process();
  std::vector<std::string> getCompatibleKeywords();
  std::map<std::string, std::string> getFoundInteractions();
  std::string getQuery();
};
