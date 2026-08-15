#pragma once
#include "word_data.hpp"
#include <imgui.h>
#include <vector>

// GAMEMODE TYPES AND DIFFICULTY settings
// Match The Word:
// - EASY
//    1 word in KR/EN with 4 options in the other language and match the right one.
// - EASY+
//    4 words in KR/EN with 4 options in the other language and match everyone with their corresponding answer.
//    + Conjugations
// - MEDIUM
//    1 word in KR/EN with textbox input to type the correct answer itself.
// - MEDIUM+
//    2–4 word phrases in KR/EN with textbox input.
// - HARD
//    MANY words in KR and EN to be matched to pre-made set of options and type in the missing ones.
//    + Maybe with gravity.
//
// ARRANGE_THE_SENTENCE:
// - EASY
//    T.B.A.
// - EASY+
//    T.B.A.
// - MEDIUM
//    T.B.A.
// - MEDIUM+
//    T.B.A.
// - HARD
//    T.B.A.
enum GamemodeType {
    MATCH_THE_WORD,
    ARRANGE_THE_SENTENCE
};

enum DifficultyLevel {
    EASY = 0,
    EASY_PLUS,
    MEDIUM,
    MEDIUM_PLUS,
    HARD
};

class GamemodeManager {
public:
    static void add_font(const char* file_path, bool is_KR_or_EN);
    static void draw_gui();
    static void shuffle_choices(const char* group_name);

    static bool         is_inbetween_rounds;
    static bool         is_KR_or_EN;
    static bool         is_typing;
    static GamemodeType gamemode;

    // Categories:
    // [0] -> is_KR_or_EN toggle
    // [1] -> global difficulty toggle
    // [2] -> group words
    struct GamemodeSettings {
        static unsigned int             cur_cat;      // Current selected settings category
        static unsigned int             sel_ind[3];   // Each categories' selected option's index
        static std::vector<const char*> selection[3]; // List of options per category
    };

private:
    static const char* draw_settings_selector();
    static void        draw_mtw_mode(const char* group_name);
    static void        draw_ats_mode(const char* group_name);

    static ImFont* font_sizes_EN[10];
    static ImFont* font_sizes_KR[10];

    struct MTW {
        static const WordData* choices[4];
        static const WordData* correct;
    };

    struct ATS {
        static std::vector<std::string>     bodies;
        static std::vector<const WordData*> blanks;
    };
};
