#pragma once

#include <string>

struct WordData {
    WordData(const char* _word, const char* _meaning): word(_word), meaning(_meaning) {}
    std::string word;
    std::string meaning;
};
