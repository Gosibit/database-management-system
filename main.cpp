#include <iostream>
#include <fmt/core.h>
#include "./includes/Table.h"
#include "./includes/Table.h"
#include "./includes/tokenizer.h"
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <fmt/ranges.h>
#include "./includes/Keyword.h"
#include "./includes/stringUtilities.h"

int main() {
    auto tables = std::map<std::string, Table*>();

    while (true) {
        auto query = std::string();
        std::getline(std::cin, query);

        auto& keywords = Keyword::keywords;

        auto queryWords = tokenize(query, " ");

        for (auto& word : queryWords) {
            auto find = keywords.find(word);

            if (find != keywords.end()) {
                fmt::println("Found keyword: {}", word);
                auto keyword = find->second;
                keyword->prepare(query);
                keyword->process();
            }
        }
    }



    return 0;
}
