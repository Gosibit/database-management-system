#pragma once

#include <vector>
#include <map>

class Keyword
{
    std::string name;
    std::vector<std::string> possibleSuccessors;
    std::vector<std::string> foundInteractions;
    std::string query;

public:
    static std::vector<std::string> supportedKeywords;
    static std::map<std::string, Keyword*> keywords;
    Keyword(std::string nameArg, std::vector<std::string> possibleSuccessorsArg);
    void println();
    std::string getName();
    void prepare(std::string queryArg);
    virtual void process();
    std::vector<std::string> getPossibleSuccessors();
    std::vector<std::string> getFoundInteractions();
    std::string getQuery();
};
