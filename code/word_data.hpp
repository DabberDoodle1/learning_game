#pragma once

#include <string>
#include <vector>

struct WordData {
    WordData(const std::vector<std::string>& _KR, const std::vector<std::string>& _EN): KR(_KR), EN(_EN) {}
    std::vector<std::string> KR;
    std::vector<std::string> EN;
};
