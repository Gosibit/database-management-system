#pragma once

#include <map>
#include <string>
#include <vector>

class Keyword {
    std::string name;
    std::vector<std::string> compatibleKeywords;
    std::string query;
    void assignKeywordArguments();
    void assignInteractions(std::vector<std::string> foundInteractionsArg);

protected:
    std::string keywordArguments;
    std::vector<std::pair<std::string, std::string>> foundInteractions;

public:
    static std::vector<std::string> supportedKeywords;
    static std::map<std::string, Keyword*> keywords;  // list of all keywords
    Keyword(std::string nameArg,
            std::vector<std::string> compatibleKeywordsArg);
    void println();
    bool isKeywordCompatible(std::string keyword);
    static void resetKeywordsData();
    std::string getName();
    void prepare(std::string queryArg);
    virtual void process();
    std::vector<std::string> getCompatibleKeywords();
    std::vector<std::pair<std::string, std::string>> getFoundInteractions();
    std::string getQuery();
};
