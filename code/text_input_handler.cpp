#include "text_input_handler.hpp"
#include "gamemode_manager.hpp"
#include "imgui.h"
#include <iostream>
#include <map>

std::vector<unsigned char> TextInputHandler::input_buffer;

// EN characters to KR characters conversion(matches KR key layout on qwerty keyboard)
// Consonants (19)  Vowels (21)  (No direct key equivalent for compound vowels and double batchim)
//   q -> ㅂ         y -> ㅛ           ( -> ㅟ (proxy key)         ` -> ㄳ (proxy key)
//   Q -> ㅃ         u -> ㅕ           ) -> ㅝ (proxy key)         1 -> ㄵ (proxy key)
//   w -> ㅈ         i -> ㅑ           [ -> ㅞ (proxy key)         2 -> ㄶ (proxy key)
//   W -> ㅉ         o -> ㅐ           ] -> ㅚ (proxy key)         3 -> ㄺ (proxy key)
//   e -> ㄷ         O -> ㅒ           { -> ㅘ (proxy key)         4 -> ㄻ (proxy key)
//   E -> ㄸ         p -> ㅔ           } -> ㅙ (proxy key)         5 -> ㄼ (proxy key)
//   r -> ㄱ         P -> ㅖ           + -> ㅢ (proxy key)         6 -> ㄽ (proxy key)
//   R -> ㄲ         h -> ㅗ                                       7 -> ㄾ (proxy key)
//   t -> ㅅ         j -> ㅓ                                       8 -> ㄿ (proxy key)
//   T -> ㅆ         k -> ㅏ                                       9 -> ㅀ (proxy key)
//   a -> ㅁ         l -> ㅣ                                       0 -> ㅄ (proxy key)
//   s -> ㄴ         b -> ㅠ
//   d -> ㅇ         n -> ㅜ
//   f -> ㄹ         m -> ㅡ
//   g -> ㅎ         
//   z -> ㅋ         
//   x -> ㅌ         
//   c -> ㅊ         
//   v -> ㅍ         
const std::map<char, unsigned int> display_offset_mapping = []
{
    std::map<char, unsigned int> mapping;

    mapping.emplace('r', 0);
    mapping.emplace('R', 1);
    mapping.emplace('`', 2);
    mapping.emplace('s', 3);
    mapping.emplace('1', 4);
    mapping.emplace('2', 5);
    mapping.emplace('e', 6);
    mapping.emplace('E', 7);
    mapping.emplace('f', 8);
    mapping.emplace('3', 9);
    mapping.emplace('4', 10);
    mapping.emplace('5', 11);
    mapping.emplace('6', 12);
    mapping.emplace('7', 13);
    mapping.emplace('8', 14);
    mapping.emplace('9', 15);
    mapping.emplace('a', 16);
    mapping.emplace('q', 17);
    mapping.emplace('Q', 18);
    mapping.emplace('0', 19);
    mapping.emplace('t', 20);
    mapping.emplace('T', 21);
    mapping.emplace('d', 22);
    mapping.emplace('w', 23);
    mapping.emplace('W', 24);
    mapping.emplace('c', 25);
    mapping.emplace('z', 26);
    mapping.emplace('x', 27);
    mapping.emplace('v', 28);
    mapping.emplace('g', 29);
    mapping.emplace('k', 30);
    mapping.emplace('o', 31);
    mapping.emplace('i', 32);
    mapping.emplace('O', 33);
    mapping.emplace('j', 34);
    mapping.emplace('p', 35);
    mapping.emplace('u', 36);
    mapping.emplace('P', 37);
    mapping.emplace('h', 38);
    mapping.emplace('{', 39);
    mapping.emplace('}', 40);
    mapping.emplace(']', 41);
    mapping.emplace('y', 42);
    mapping.emplace('n', 43);
    mapping.emplace(')', 44);
    mapping.emplace('[', 45);
    mapping.emplace('(', 46);
    mapping.emplace('b', 47);
    mapping.emplace('m', 48);
    mapping.emplace('+', 49);
    mapping.emplace('l', 50);

    return mapping;
}();

const std::map<char, unsigned int> consonants_offset_mapping = []
{
    std::map<char, unsigned int> mapping;

    mapping.emplace('r', 0);
    mapping.emplace('R', 1);
    mapping.emplace('s', 2);
    mapping.emplace('e', 3);
    mapping.emplace('E', 4);
    mapping.emplace('f', 5);
    mapping.emplace('a', 6);
    mapping.emplace('q', 7);
    mapping.emplace('Q', 8);
    mapping.emplace('t', 9);
    mapping.emplace('T', 10);
    mapping.emplace('d', 11);
    mapping.emplace('w', 12);
    mapping.emplace('W', 13);
    mapping.emplace('c', 14);
    mapping.emplace('z', 15);
    mapping.emplace('x', 16);
    mapping.emplace('v', 17);
    mapping.emplace('g', 18);

    return mapping;
}();

const std::map<char, unsigned int> vowels_offset_mapping = []
{
    std::map<char, unsigned int> mapping;

    mapping.emplace('k', 0);  // ㅏ
    mapping.emplace('o', 1);  // ㅐ
    mapping.emplace('i', 2);  // ㅑ
    mapping.emplace('O', 3);  // ㅒ
    mapping.emplace('j', 4);  // ㅓ
    mapping.emplace('p', 5);  // ㅔ
    mapping.emplace('u', 6);  // ㅕ
    mapping.emplace('P', 7);  // ㅖ
    mapping.emplace('h', 8);  // ㅗ
    mapping.emplace('{', 9);  // ㅘ
    mapping.emplace('}', 10); // ㅙ
    mapping.emplace(']', 11); // ㅚ
    mapping.emplace('y', 12); // ㅛ
    mapping.emplace('n', 13); // ㅜ
    mapping.emplace(')', 14); // ㅝ
    mapping.emplace('[', 15); // ㅞ
    mapping.emplace('(', 16); // ㅟ
    mapping.emplace('b', 17); // ㅠ
    mapping.emplace('m', 18); // ㅡ
    mapping.emplace('+', 19); // ㅢ
    mapping.emplace('l', 20); // ㅣ

    return mapping;
}();

const std::map<char, unsigned int> batchim_offset_mapping = []
{
    std::map<char, unsigned int> mapping;

    mapping.emplace('r', 1);
    mapping.emplace('R', 2);
    mapping.emplace('`', 3);  // ㄳ
    mapping.emplace('s', 4);
    mapping.emplace('1', 5);  // ㄵ
    mapping.emplace('2', 6);  // ㄶ
    mapping.emplace('e', 7);
    mapping.emplace('f', 8);
    mapping.emplace('3', 9);  // ㄺ
    mapping.emplace('4', 10); // ㄻ
    mapping.emplace('5', 11); // ㄼ
    mapping.emplace('6', 12); // ㄽ
    mapping.emplace('7', 13); // ㄾ
    mapping.emplace('8', 14); // ㄿ
    mapping.emplace('9', 15); // ㅀ
    mapping.emplace('a', 16);
    mapping.emplace('q', 17);
    mapping.emplace('0', 18); // ㅄ
    mapping.emplace('t', 19);
    mapping.emplace('T', 20);
    mapping.emplace('d', 21);
    mapping.emplace('w', 22);
    mapping.emplace('c', 23);
    mapping.emplace('z', 24);
    mapping.emplace('x', 25);
    mapping.emplace('v', 26);
    mapping.emplace('g', 27);

    return mapping;
}();

std::string unicode_to_utf8(unsigned int code)
{
    std::string utf8_char;

    if (code <= 0x7f) {
        utf8_char += static_cast<char>(code);
    } else if (code <= 0x7ff) {
        utf8_char += static_cast<char>(0xc0 | (code >> 6));
        utf8_char += static_cast<char>(0x80 | (code & 0x3f));
    } else if (code <= 0xffff) {
        utf8_char += static_cast<char>(0xe0 | (code >> 12));
        utf8_char += static_cast<char>(0x80 | ((code >> 6) & 0x3f));
        utf8_char += static_cast<char>(0x80 | (code & 0x3f));
    } else if (code <= 0x10ffff) {
        utf8_char += static_cast<char>(0xf0 | (code >> 18));
        utf8_char += static_cast<char>(0x80 | ((code >> 12) & 0x3f));
        utf8_char += static_cast<char>(0x80 | ((code >> 6) & 0x3f));
        utf8_char += static_cast<char>(0x80 | (code & 0x3f));
    }

    return utf8_char;
}

const std::string get_sb(std::vector<char>& buffer)
{
    std::string target = "";

    if (buffer.size() == 1) {
        target += unicode_to_utf8(0x3131 + display_offset_mapping.at(buffer[0]));
        buffer.clear();

        return target;
    }

    unsigned int base = 0xac00;
    base += consonants_offset_mapping.at(buffer[0]) * 588;
    base += vowels_offset_mapping.at(buffer[1]) * 28;

    if (buffer.size() == 3) {
        base += batchim_offset_mapping.at(buffer[2]);
    }

    buffer.clear();
    target += unicode_to_utf8(base);

    return target;
}

bool TextInputHandler::draw_textbox(bool is_KR_or_EN, const std::string& word, const std::string& meaning)
{
    bool should_shuffle = false;

    static std::string buffer{};
    if (ImGui::InputText("##input_text", const_cast<char*>(buffer.c_str()), buffer.size() + 1, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_ReadOnly)) {
        if (buffer == (is_KR_or_EN ? meaning : word)) {
            input_buffer.clear();
            buffer.clear();
            should_shuffle = true;
        }
    }

    if (ImGui::IsItemActive()) {
        GamemodeManager::is_typing = true;

        for (unsigned int i = ImGuiKey_A; i <= ImGuiKey_Z; ++i) {
            ImGuiKey key = static_cast<ImGuiKey>(i);

            if (ImGui::IsKeyPressed(key)) {
                char ch = 'a' + key - ImGuiKey_A;

                if (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) {
                    ch -= 'a' - 'A';
                }

                add_char(GamemodeManager::is_typing_KR, ch);
                buffer = get_text();
                break;
            }
        }

        if (ImGui::IsKeyPressed(ImGuiKey_Backspace)) {
            delete_char();
            buffer = get_text();
        }
    }

    if (ImGui::IsItemDeactivated()) {
        if (should_shuffle) {
            ImGui::SetKeyboardFocusHere(-1);
        } else {
            GamemodeManager::is_typing = false;
        }
    }

    return should_shuffle;
}

void TextInputHandler::delete_char()
{
    if (input_buffer.empty()) {
        return;
    }

    const char ch    = input_buffer.back() & 0b01111111;
    const bool is_KR = input_buffer.back() & 0b10000000;

    if (is_KR) {
        input_buffer.pop_back();

        switch (ch) {
            case '(':                                     // ㅟ (nl)
            case ')':                                     // ㅝ (nj)
            case '[':                                     // ㅞ (np)
                input_buffer.push_back('n' & 0b10000000);

                break;
            case ']':                                     // ㅚ (hl)
            case '{':                                     // ㅘ (hk)
            case '}':                                     // ㅙ (ho)
                input_buffer.push_back('h' & 0b10000000);

                break;
            case '+':                                     // ㅢ (ml)
                input_buffer.push_back('m' & 0b10000000);

                break;
            case '`':                                     // ㄳ (rt)
                input_buffer.push_back('r' & 0b10000000);

                break;
            case '1':                                     // ㄵ (sw)
            case '2':                                     // ㄶ (sg)
                input_buffer.push_back('s' & 0b10000000);

                break;
            case '3':                                     // ㄺ (fr)
            case '4':                                     // ㄻ (fa)
            case '5':                                     // ㄼ (fq)
            case '6':                                     // ㄽ (ft)
            case '7':                                     // ㄾ (fx)
            case '8':                                     // ㄿ (fv)
            case '9':                                     // ㅀ (fg)
                input_buffer.push_back('f' & 0b10000000);

                break;
            case '0':                                     // ㅄ (qt)
                input_buffer.push_back('q' & 0b10000000);

                break;
        }
    } else {
        input_buffer.pop_back();
    }
}

void TextInputHandler::add_char(bool is_KR, char ch)
{
    unsigned char value = 0;

    value |= is_KR ? 0b10000000 : 0 ;
    value |= ch;

    if (is_KR) {
        static const char vowel_keys[] = { 'y', 'u', 'i', 'o', 'O', 'p', 'P', 'h', 'j', 'k', 'l', 'b', 'n', 'm' };
        
        bool is_a_vowel = false;
        for (char key : vowel_keys) {
            if (ch == key) {
                is_a_vowel = true;
            }
        }

        const char prev = (input_buffer.empty() ? '-' : input_buffer.back()) & 0b01111111 ;

        switch (prev) {
            case 'r':
                if (ch == 't') {
                    input_buffer.pop_back();
                    input_buffer.push_back('`' | 0b10000000);     // ㄳ
                } else {
                    input_buffer.push_back(value);
                }

                break;
            case 's':
                switch (ch) {
                    case 'w':
                        input_buffer.pop_back();
                        input_buffer.push_back('1' | 0b10000000); // ㄵ

                        break;
                    case 'g':
                        input_buffer.pop_back();
                        input_buffer.push_back('2' | 0b10000000); // ㄶ

                        break;
                    default:
                        input_buffer.push_back(value);

                        break;
                }

                break;
            case 'f':
                switch (ch) {
                    case 'r':
                        input_buffer.pop_back();
                        input_buffer.push_back('3' | 0b10000000); // ㄺ

                        break;
                    case 'a':
                        input_buffer.pop_back();
                        input_buffer.push_back('4' | 0b10000000); // ㄻ

                        break;
                    case 'q':
                        input_buffer.pop_back();
                        input_buffer.push_back('5' | 0b10000000); // ㄼ

                        break;
                    case 't':
                        input_buffer.pop_back();
                        input_buffer.push_back('6' | 0b10000000); // ㄽ

                        break;
                    case 'x':
                        input_buffer.pop_back();
                        input_buffer.push_back('7' | 0b10000000); // ㄾ

                        break;
                    case 'v':
                        input_buffer.pop_back();
                        input_buffer.push_back('8' | 0b10000000); // ㄿ

                        break;
                    case 'g':
                        input_buffer.pop_back();
                        input_buffer.push_back('9' | 0b10000000); // ㅀ

                        break;
                    default:
                        input_buffer.push_back(value);

                        break;
                }

                break;
            case 'q':
                if (ch == 't') {
                    input_buffer.pop_back();
                    input_buffer.push_back('0' | 0b10000000);     // ㅄ
                } else {
                    input_buffer.push_back(value);
                }

                break;
            case 'h':
                switch (ch) {
                    case 'k':
                        input_buffer.pop_back();
                        input_buffer.push_back('{' | 0b10000000); // ㅘ

                        break;
                    case 'o':
                        input_buffer.pop_back();
                        input_buffer.push_back('}' | 0b10000000); // ㅙ

                        break;
                    case 'l':
                        input_buffer.pop_back();
                        input_buffer.push_back(']' | 0b10000000); // ㅚ

                        break;
                    default:
                        input_buffer.push_back(value);

                        break;
                }

                break;
            case 'n':
                switch (ch) {
                    case 'j':
                        input_buffer.pop_back();
                        input_buffer.push_back(')' | 0b10000000); // ㅝ

                        break;
                    case 'p':
                        input_buffer.pop_back();
                        input_buffer.push_back('[' | 0b10000000); // ㅞ

                        break;
                    case 'l':
                        input_buffer.pop_back();
                        input_buffer.push_back('(' | 0b10000000); // ㅟ

                        break;
                    default:
                        input_buffer.push_back(value);

                        break;
                }

                break;
            case 'm':
                if (ch == 'l') {
                    input_buffer.pop_back();
                    input_buffer.push_back('+' | 0b10000000); // ㅢ
                } else {
                    input_buffer.push_back(value);
                }

                break;
            case '`':                            // ㄳ
                if (is_a_vowel) {
                    input_buffer.pop_back();
                    input_buffer.push_back('r' | 0b10000000);
                    input_buffer.push_back('t' | 0b10000000);
                }
                input_buffer.push_back(value);

                break;
            case '1':                            // ㄵ
                if (is_a_vowel) {
                    input_buffer.pop_back();
                    input_buffer.push_back('s' | 0b10000000);
                    input_buffer.push_back('w' | 0b10000000);
                }
                input_buffer.push_back(value);

                break;
            case '2':                            // ㄶ
                if (is_a_vowel) {
                    input_buffer.pop_back();
                    input_buffer.push_back('s' | 0b10000000);
                    input_buffer.push_back('g' | 0b10000000);
                }
                input_buffer.push_back(value);

                break;
            case '3':                            // ㄺ
                if (is_a_vowel) {
                    input_buffer.pop_back();
                    input_buffer.push_back('f' | 0b10000000);
                    input_buffer.push_back('r' | 0b10000000);
                }
                input_buffer.push_back(value);

                break;
            case '4':                            // ㄻ
                if (is_a_vowel) {
                    input_buffer.pop_back();
                    input_buffer.push_back('f' | 0b10000000);
                    input_buffer.push_back('a' | 0b10000000);
                }
                input_buffer.push_back(value);

                break;
            case '5':                            // ㄼ
                if (is_a_vowel) {
                    input_buffer.pop_back();
                    input_buffer.push_back('f' | 0b10000000);
                    input_buffer.push_back('q' | 0b10000000);
                }
                input_buffer.push_back(value);

                break;
            case '6':                            // ㄽ
                if (is_a_vowel) {
                    input_buffer.pop_back();
                    input_buffer.push_back('f' | 0b10000000);
                    input_buffer.push_back('t' | 0b10000000);
                }
                input_buffer.push_back(value);

                break;
            case '7':                            // ㄾ
                if (is_a_vowel) {
                    input_buffer.pop_back();
                    input_buffer.push_back('f' | 0b10000000);
                    input_buffer.push_back('x' | 0b10000000);
                }
                input_buffer.push_back(value);

                break;
            case '8':                            // ㄿ
                if (is_a_vowel) {
                    input_buffer.pop_back();
                    input_buffer.push_back('f' | 0b10000000);
                    input_buffer.push_back('v' | 0b10000000);
                }
                input_buffer.push_back(value);

                break;
            case '9':                            // ㅀ
                if (is_a_vowel) {
                    input_buffer.pop_back();
                    input_buffer.push_back('f' | 0b10000000);
                    input_buffer.push_back('g' | 0b10000000);
                }
                input_buffer.push_back(value);

                break;
            case '0':                            // ㅄ
                if (is_a_vowel) {
                    input_buffer.pop_back();
                    input_buffer.push_back('q' | 0b10000000);
                    input_buffer.push_back('t' | 0b10000000);
                }
                input_buffer.push_back(value);

                break;
            default:
                input_buffer.push_back(value);

                break;
        }

    } else {
        input_buffer.push_back(value);
    }
}

std::string TextInputHandler::get_text()
{
    std::string text = "";

    unsigned int      curr_sb_count = 0;
    std::vector<char> curr_sb_buffer;
    curr_sb_buffer.reserve(3);

    for (unsigned int i = 0; i < input_buffer.size(); ++i) {
        const unsigned char value = input_buffer[i];
        const char          ch    = value & 0b01111111;

        if (value & 0b10000000) {
            static const char keys[] = {
                'q', 'Q', 'w', 'W', 'e', 'E', 'r', 'R', 't', 'T', 'a', 's', 'd', 'f', 'g', 'z', 'x', 'c', 'v',           // Consonants
                '`', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',                                                   // Double Batchim
                'y', 'u', 'i', 'o', 'O', 'p', 'P', 'h', 'j', 'k', 'l', 'b', 'n', 'm', '(', ')', '[', ']', '{', '}', '+'  // Vowels
            };
            static const unsigned int size = sizeof(keys) / sizeof(keys[0]);

            bool is_consonant;
            bool is_batchim;
            for (unsigned int i = 0; i < size; ++i) {
                if (ch == keys[i]) {
                    if (i < 19) {
                        is_consonant = true;
                        is_batchim   = false;
                    } else if (i >= 19 && i <= 29) {
                        is_consonant = true;
                        is_batchim   = true;
                    } else {
                        is_consonant = false;
                        is_batchim   = false;
                    }
                }
            }

            switch (curr_sb_count) {
                case 0:
                    curr_sb_buffer.push_back(ch);

                    if (is_consonant && !is_batchim) {
                        ++curr_sb_count; // Continue
                    } else {
                        text += get_sb(curr_sb_buffer);
                    }

                    break;
                case 1:
                    if (is_consonant && !is_batchim) {
                        text += get_sb(curr_sb_buffer);

                        curr_sb_buffer.push_back(ch);
                        curr_sb_count = 1;

                        if (is_batchim) {
                            text += get_sb(curr_sb_buffer);
                            curr_sb_count = 0;
                        }
                    } else {
                        curr_sb_buffer.push_back(ch);
                        ++curr_sb_count;
                    }

                    break;
                case 2:
                    if (!is_consonant) {
                        text += get_sb(curr_sb_buffer);
                    }
                    curr_sb_buffer.push_back(ch);

                    if (i < input_buffer.size() - 1) {
                        const unsigned char next       = input_buffer[i + 1];
                        const char          next_ch    = next & 0b01111111;
                        const bool          next_is_KR = next & 0b10000000;

                        if (next_is_KR) {
                            bool is_next_a_vowel = false;
                            for (unsigned int i = 0; i < size; ++i) {
                                if (next_ch == keys[i]) {
                                    if (i > 29) {
                                        is_next_a_vowel = true;
                                    }
                                }
                            }

                            if (is_next_a_vowel) {
                                curr_sb_buffer.pop_back();
                                text         += get_sb(curr_sb_buffer);

                                curr_sb_buffer.push_back(ch);
                                curr_sb_count = 1;

                                break;
                            }
                        }
                    }

                    text         += get_sb(curr_sb_buffer);
                    curr_sb_count = 0;

                    break;
            }

            if (i == input_buffer.size() - 1 && curr_sb_count != 0) {
                text += get_sb(curr_sb_buffer);
                break;
            }

        } else {
            if (curr_sb_count != 0) {
                text += get_sb(curr_sb_buffer);
                curr_sb_count = 0;
            }

            text += ch;
        }
    }

    return text;
}
