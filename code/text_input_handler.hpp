#pragma once
#include <string>
#include <vector>

class TextInputHandler {
public:
    TextInputHandler() = delete;

    static bool        draw_textbox(bool is_KR_or_EN, const std::string& word, const std::string& meaning);

private:
    static void        add_char(bool is_KR_or_EN, char ch);
    static void        delete_char();
    static std::string get_text();

    // Data format
    // 1st bit        = is Korean if 1 and EN if 0
    // 2nd to 7th bit = actual character value
    static std::vector<unsigned char> input_buffer;
};
