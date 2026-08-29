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
    static void init(const char* KR_path, const char* EN_path, float scaling_unit);
    static void draw_gui();

    static bool         is_inbetween_rounds;
    static bool         is_KR_or_EN;
    static bool         is_typing;
    static bool         should_shuffle_choices;
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

    static ImFont* font_EN;
    static ImFont* font_KR;
    static float   font_sizes[10];

    struct MTW {
        static void draw_easy(const WordData* (&choices)[4], const WordData* correct);
        static void draw_easy_plus(const WordData *(&left_choices)[4], const WordData *(&right_choices)[4]);
        static void draw_medium(const char* group_name, const WordData*& correct);
        static void draw_medium_plus();
        static void draw_hard();

        static void shuffle_choices(const char* group_name, const WordData* (&choices)[4]);
        static void shuffle_choices(const char* group_name, const WordData*& correct);
    };

    struct ATS {
        static std::vector<std::string>     bodies;
        static std::vector<const WordData*> blanks;
    };
};
