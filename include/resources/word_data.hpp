#pragma once

#include <string>
#include <vector>

struct WordData {
    WordData(const std::vector<std::string>& _KR, const std::vector<std::string>& _EN): KR(_KR), EN(_EN) {}

    const std::vector<std::string> KR;
    const std::vector<std::string> EN;
};
