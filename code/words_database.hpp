#pragma once

#include "word_data.hpp"
#include <map>
#include <vector>

class WordDatabase {
public:
    WordDatabase() = delete;
    WordDatabase(const WordDatabase& other) = delete;
    WordDatabase& operator=(const WordDatabase& other) = delete;

    static void init();

    static const WordData* get_word(const char* KR_word);
    static const std::vector<const WordData*>* get_word_group(const char* group_name);

private:
    static std::vector<WordData> words;
    static std::map<const std::string, std::vector<const WordData*>> word_groups;
};
