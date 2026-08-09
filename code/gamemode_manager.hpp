#pragma once

#include "word_data.hpp"
#include <imgui.h>
#include <memory>
#include <vector>

enum GamemodeType {
    MULTI_CHOICE_QUIZ,
    FILL_IN_THE_BLANK
};

enum DifficultyLevel {
    EASY,
    MEDIUM,
    HIGH
};

class GamemodeManager {
public:
    static void add_font(const char* file_path, float size_unit, bool is_KR_or_EN);
    static void draw_gui();
    static void shuffle_choices(const char* group_name);

    static bool         is_inbetween_rounds;
    static bool         is_KR_or_EN;
    static GamemodeType gamemode;

    static const std::unique_ptr<const char*[]> selection[];
    static unsigned int                         cur_cat;
    static unsigned int                         sel_ind[2];
private:
    static const char* draw_settings_selector();
    static void        draw_mcq_mode(const char* group_name, DifficultyLevel difficulty);
    static void        draw_fitb_mode(const char* group_name, DifficultyLevel difficulty);

    static ImFont* font_sizes_EN[8];
    static ImFont* font_sizes_KR[8];

    struct MCQ {
        static const WordData* choices[4];
        static const WordData* correct;
    };

    struct FITB {
        static std::vector<std::string>     bodies;
        static std::vector<const WordData*> blanks;
    };
};
