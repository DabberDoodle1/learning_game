#include "gamemode_manager.hpp"
#include "game.hpp"
#include "text_input_handler.hpp"
#include "words_database.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <random>

#define DELTA_SPACE_WIDTH Game::m_width * 0.0085f / 10.88f

ImFont*                      GamemodeManager::font_EN;
ImFont*                      GamemodeManager::font_KR;
float                        GamemodeManager::font_sizes[10];

bool                         GamemodeManager::is_inbetween_rounds    = false;
bool                         GamemodeManager::is_KR_or_EN            = false;
bool                         GamemodeManager::is_typing              = false;
bool                         GamemodeManager::is_typing_KR           = true;
bool                         GamemodeManager::should_shuffle_choices = true;
GamemodeType                 GamemodeManager::gamemode               = MATCH_THE_WORD;

unsigned int                 GamemodeManager::GamemodeSettings::cur_cat    = 0;
unsigned int                 GamemodeManager::GamemodeSettings::sel_ind[3] = { 0, 0, 0 };
std::vector<const char*>     GamemodeManager::GamemodeSettings::selection[3];

std::vector<std::string>     GamemodeManager::ATS::bodies;
std::vector<const WordData*> GamemodeManager::ATS::blanks;

void GamemodeManager::init(const char* EN_file_path, const char* KR_file_path, float scaling_unit)
{
    ImGuiIO& IO = ImGui::GetIO();

    font_EN = IO.Fonts->AddFontFromFileTTF(EN_file_path);
    font_KR = IO.Fonts->AddFontFromFileTTF(KR_file_path);

    font_sizes[0] = 0.8f  * Game::m_width * 0.01f;
    font_sizes[1] = 1.2f  * Game::m_width * 0.01f;
    font_sizes[2] = 1.8f  * Game::m_width * 0.01f;
    font_sizes[3] = 2.4f  * Game::m_width * 0.01f;
    font_sizes[4] = 3.0f  * Game::m_width * 0.01f;
    font_sizes[5] = 5.0f  * Game::m_width * 0.01f;
    font_sizes[6] = 8.0f  * Game::m_width * 0.01f;
    font_sizes[7] = 10.0f * Game::m_width * 0.01f;
    font_sizes[8] = 18.0f * Game::m_width * 0.01f;
    font_sizes[9] = 22.5f * Game::m_width * 0.01f;

    IO.Fonts->Build();
}

void GamemodeManager::draw_gui()
{
    // Create frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Draw window
    ImGui::SetNextWindowSize(ImVec2(Game::m_width, Game::m_height));
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Hello", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
        ImGui::End();
        return;
    }

    // KR-EN toggle button
    const char* selected_word_group = draw_settings_selector();

    switch (gamemode) {
        case MATCH_THE_WORD:
            draw_mtw_mode(selected_word_group);
            break;
        case ARRANGE_THE_SENTENCE:
            draw_ats_mode(selected_word_group);
            break;
    }

    // Finish rendering
    ImGui::End();
    ImGui::Render();
}

const char* GamemodeManager::draw_settings_selector()
{
    // Alias
    using st = GamemodeSettings;

    const char* categories[]   = {
        "Source and target",
        "Difficulty level",
        "Word group"
    };
    const unsigned int df_size = st::selection[1].size();
    const unsigned int wg_size = st::selection[2].size();

    const char*  category_label      = categories[st::cur_cat]; 
    const char*  selected_word_group = st::selection[2][st::sel_ind[2]];
    const ImVec2 display_size        = ImGui::CalcTextSize(st::selection[st::cur_cat][st::sel_ind[st::cur_cat]]);

    ImGui::SetCursorPos(ImVec2(20.0f, 40.0f));
    ImGui::PushFont(font_EN, font_sizes[2]);

    ImGui::letter_spacing = 1.0f;
    ImGui::Text(category_label);
    ImGui::letter_spacing = 0.0f;

    static const ImVec2 lss = ImGui::CalcTextSize("A"); // Left  select size
    static const ImVec2 rss = ImGui::CalcTextSize("D"); // Right select size

    // Draw A and D buttons (disabled)
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.5f, 0.5f, 1.0f));

    ImGui::BeginDisabled();
    ImGui::SetCursorPos(ImVec2(20.0f, 20.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2((20.0f - lss.x) * 0.5f, (20.0f - lss.y) * 0.5f));
    ImGui::Button("A##toggle", ImVec2(20.0f, 20.0f));
    ImGui::PopStyleVar();

    ImGui::SetCursorPos(ImVec2(165.0f, 20.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2((20.0f - rss.x) * 0.5f, (20.0f - rss.y) * 0.5f));
    ImGui::Button("D##toggle", ImVec2(20.0f, 20.0f));
    ImGui::PopStyleVar();
    ImGui::EndDisabled();

    // Draw the main display of current category and selected category value
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(40.0f, 20.0f), ImVec2(165.0f, 40.0f), IM_COL32(77, 77, 77, 255));
    ImGui::SetCursorPos(ImVec2(45.0f, 30.0f - display_size.y * 0.5f));
    ImGui::Text(st::selection[st::cur_cat][st::sel_ind[st::cur_cat]]);

    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    return selected_word_group;
}

void GamemodeManager::draw_mtw_mode(const char* group_name)
{
    static const WordData* easy_choices[4];
    static const WordData* correct;
    static bool            is_first_call = true;

    auto repick_correct = []() {
        std::uniform_int_distribution<int> four(0, 3);
        std::mt19937                       gen(std::random_device{}());

        correct = easy_choices[four(gen)];
    };

    if (is_first_call) {
        MTW::shuffle_choices(group_name, easy_choices);
        repick_correct();

        is_first_call = false;
    }

    if (should_shuffle_choices) {
        MTW::shuffle_choices(GamemodeSettings::selection[2][GamemodeSettings::sel_ind[2]], easy_choices);
        repick_correct();

        should_shuffle_choices = false;
    }

    DifficultyLevel difficulty = static_cast<DifficultyLevel>(GamemodeSettings::GamemodeSettings::sel_ind[1]);

    switch (difficulty) {
        case EASY:
        case EASY_PLUS:
            if (is_inbetween_rounds) {
                ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
                if (ImGui::InvisibleButton("##prompt_continue", ImVec2(Game::m_width, Game::m_height))) {
                    MTW::shuffle_choices(group_name, easy_choices);
                    repick_correct();

                    is_inbetween_rounds = false;
                }
            }

            if (difficulty == EASY) {
                MTW::draw_easy(easy_choices, correct);
            } else {
                //MTW::draw_easy_plus(easy_choices);
            }

            break;
        case MEDIUM:
            MTW::draw_medium(group_name, correct);

            break;
        case MEDIUM_PLUS:
            MTW::draw_medium_plus();      // NOT DONE
            break;
        case HARD:
            MTW::draw_hard();             // NOT DONE
            break;
    }
}

void GamemodeManager::draw_ats_mode(const char* group_name)
{
    // anime
}

void GamemodeManager::MTW::shuffle_choices(const char* group_name, const WordData* (&choices)[4])
{
    const std::vector<const WordData*>* word_group = WordDatabase::get_word_group(group_name);
    std::uniform_int_distribution       picker     = std::uniform_int_distribution<int>(0, static_cast<int>(word_group->size() - 1));
    std::mt19937                        gen        = std::mt19937(std::random_device{}());

    // Populating MTW::choices with random words and ensuring no dupes
    for (unsigned int i = 0; i < 4; ++i) {
        const WordData* item;
        bool            is_dupe;

        while (true) {
            item    = (*word_group)[picker(gen)];
            is_dupe = false;

            for (unsigned int j = 0; j < i; ++j) {
                if (choices[j] == item) {
                    is_dupe = true;
                }
            }

            if (!is_dupe) {
                break;
            }
        }

        choices[i] = item;
    }
}

void GamemodeManager::MTW::shuffle_choices(const char* group_name, const WordData*& correct)
{
    const std::vector<const WordData*>* word_group = WordDatabase::get_word_group(group_name);
    std::uniform_int_distribution       picker     = std::uniform_int_distribution<int>(0, static_cast<int>(word_group->size() - 1));
    std::mt19937                        gen        = std::mt19937(std::random_device{}());

    const WordData* temp = (*word_group)[picker(gen)];
    while (temp == correct) {
        temp = (*word_group)[picker(gen)];
    }

    correct = temp;
}

void GamemodeManager::MTW::draw_easy(const WordData* (&choices)[4], const WordData* correct)
{
    // Draw "correct" display text
    ImGui::PushFont(is_KR_or_EN ? font_KR : font_EN, font_sizes[8]);
    const char* display_text = is_KR_or_EN ? correct->KR[0].c_str() : correct->EN[0].c_str();
    ImVec2      display_size = ImGui::CalcTextSize(display_text);

    ImGui::SetCursorPos(ImVec2((Game::m_width - display_size.x) * 0.5f, Game::m_height * 0.25f - display_size.y * 0.5f));
    ImGui::Text(display_text);

    ImGui::PopFont();
    ImGui::PushFont(is_KR_or_EN ? font_EN : font_KR , font_sizes[5]);

    // Draw the options
    // Button settings
    static const float  gap = 50.0f;
    static const ImVec2 button_size(Game::m_width * 0.3f, (Game::m_height * 0.5f - gap * 2) * 0.5f);
    static const ImVec2 pos[4] = {
        ImVec2(Game::m_width * 0.5f - button_size.x - gap * 0.5f,
                Game::m_height * 0.5f - button_size.y * 0.2f),
        ImVec2(Game::m_width * 0.5f + gap * 0.5f,
                Game::m_height * 0.5f - button_size.y * 0.2f),
        ImVec2(Game::m_width * 0.5f - button_size.x - gap * 0.5f,
                Game::m_height * 0.5f + button_size.y * 0.8f + gap),
        ImVec2(Game::m_width * 0.5f + gap * 0.5f,
                Game::m_height * 0.5f + button_size.y * 0.8f + gap)
    };

    // The actual buttons
    for (unsigned int i = 0; i < 4; ++i) {
        const char* button_text = is_KR_or_EN ? choices[i]->EN[0].c_str() : choices[i]->KR[0].c_str();

        ImGui::SetCursorPos(pos[i]);
        if (is_inbetween_rounds) {
            bool is_correct = choices[i] == correct;

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Button, is_correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, is_correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, is_correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));

            ImGui::BeginDisabled();
            ImGui::PushID(i);
            ImGui::Button(button_text, button_size);
            ImGui::PopID();

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                ImGui::PushFont(is_KR_or_EN ? font_KR : font_EN, is_KR_or_EN ? font_sizes[3] : font_sizes[2]);

                const char* hover_text = is_KR_or_EN ? choices[i]->KR[0].c_str() : choices[i]->EN[0].c_str();
                ImVec2      hover_size = ImGui::CalcTextSize(hover_text);

                ImGui::SetCursorPos(ImVec2(pos[i].x + (button_size.x - hover_size.x) * 0.5f, pos[i].y + button_size.y * 0.75f - hover_size.y * 0.5f));
                if (!is_KR_or_EN) {
                    ImGui::letter_spacing = 1.5f; // I added this property myself so it's not part of the official Dear ImGui repo
                    ImGui::Text(hover_text);
                    ImGui::letter_spacing = 0.0f;
                } else {
                    ImGui::Text(hover_text);
                }

                ImGui::PopFont();
            }

            ImGui::EndDisabled();
            ImGui::PopStyleColor(4);
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
            if (ImGui::Button(button_text, button_size)) {
                is_inbetween_rounds = true;
            }
            ImGui::PopStyleColor();
        }
    }

    ImGui::PopFont();
}

void GamemodeManager::MTW::draw_easy_plus(const WordData *(&choices)[4])
{
    // Left and right side widget variables
    static const float  gap         = 15.0f * DELTA_SPACE_WIDTH;
    static const ImVec2 widget_size = ImVec2(Game::m_width * 0.3f, (Game::m_height * 0.8f - 3 * gap) * 0.25f);
    static const ImVec2 left_pos[4] = {
        ImVec2(Game::m_width * 0.15f, Game::m_height * 0.1f),
        ImVec2(Game::m_width * 0.15f, Game::m_height * 0.1f + (widget_size.y + gap)),
        ImVec2(Game::m_width * 0.15f, Game::m_height * 0.1f + (widget_size.y + gap) * 2.0f),
        ImVec2(Game::m_width * 0.15f, Game::m_height * 0.1f + (widget_size.y + gap) * 3.0f)
    };
    static const ImVec2 right_pos[4] = {
        ImVec2(Game::m_width * 0.85f - widget_size.x, Game::m_height * 0.1f),
        ImVec2(Game::m_width * 0.85f - widget_size.x, Game::m_height * 0.1f + (widget_size.y + gap)),
        ImVec2(Game::m_width * 0.85f - widget_size.x, Game::m_height * 0.1f + (widget_size.y + gap) * 2.0f),
        ImVec2(Game::m_width * 0.85f - widget_size.x, Game::m_height * 0.1f + (widget_size.y + gap) * 3.0f),
    };

    const auto& EN_pos = is_KR_or_EN ? left_pos  : right_pos;
    const auto& KR_pos = is_KR_or_EN ? right_pos : left_pos;

    const auto& left_labels  = is_KR_or_EN ? choices[0]->EN : choices[0]->KR;
    const auto& right_labels = is_KR_or_EN ? choices[0]->KR : choices[0]->EN;

    // Draw EN side
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
    for (unsigned int i = 0; i < 4; ++i) {
        ImGui::SetCursorPos(left_pos[i]);
        if (ImGui::Button(left_labels[i].c_str(), widget_size)) {
            std::cout << "L " << i << '\n';
        }
    }

    // Draw KR side
    for (unsigned int i = 0; i < 4; ++i) {
        ImGui::SetCursorPos(left_pos[i]);
        if (ImGui::Button(right_labels[i].c_str(), widget_size)) {
            std::cout << "R " << i << '\n';
        }
    }
}

void GamemodeManager::MTW::draw_medium(const char* group_name, const WordData*& correct)
{
    // Draw "correct" display text
    // Draw question text
    const char* text;
    ImVec2      text_size;

    if (is_KR_or_EN) {
        text = correct->KR[0].c_str();
        ImGui::PushFont(font_KR, font_sizes[9]);
    } else {
        text = correct->EN[0].c_str();
        ImGui::PushFont(font_EN, font_sizes[9]);
    }
    text_size = ImGui::CalcTextSize(text);

    ImGui::SetCursorPos(ImVec2((Game::m_width - text_size.x) * 0.5f, Game::m_height * 0.5f - text_size.y));
    ImGui::Text(text);

    ImGui::PopFont();
    ImGui::PushFont(is_KR_or_EN ? font_EN : font_KR, font_sizes[7]);

    static const float size_unit      = Game::m_width * 0.0085f;
    static const float textbox_height = ImGui::GetFrameHeight();

    ImGui::SetCursorPos(ImVec2(Game::m_width * 0.2f, Game::m_height * 0.6875f - textbox_height * 0.5f));
    ImGui::SetNextItemWidth(Game::m_width * 0.6f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.5f * size_unit, 0.5f * size_unit));

    if (TextInputHandler::draw_medium_textbox(is_KR_or_EN, correct)) {
        shuffle_choices(group_name, correct);
    }

    ImGui::PopStyleVar();
    ImGui::PopFont();
}

void GamemodeManager::MTW::draw_medium_plus()
{
    ;
}

void GamemodeManager::MTW::draw_hard()
{
    ;
}
