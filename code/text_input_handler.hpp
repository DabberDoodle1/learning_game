#pragma once
#include "imgui.h"
#include "word_data.hpp"
#include <string>
#include <vector>

class TextInputHandler {
public:
    TextInputHandler() = delete;

    static bool         draw_medium_textbox(const WordData* correct, float width, ImVec2 pos);
    static bool         draw_medium_plus_textbox(const WordData* correct);

    static unsigned int cursor_pos;

private:
    static void         add_char(bool is_KR_or_EN, char ch);
    static void         delete_char();
    static std::string  get_text();

    // Data format
    // 1st bit        = is Korean if 1 and EN if 0
    // 2nd to 7th bit = actual character value
    static std::vector<unsigned char> input_buffer;
};
