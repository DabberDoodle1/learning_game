#include "gamemode_manager.hpp"
#include "game.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"
#include "words_database.hpp"
#include <cstring>
#include <iostream>
#include <random>

ImFont*      GamemodeManager::font_sizes_EN[10];
ImFont*      GamemodeManager::font_sizes_KR[10];
bool         GamemodeManager::is_inbetween_rounds = false;
bool         GamemodeManager::is_KR_or_EN         = false;
bool         GamemodeManager::is_typing           = false;
GamemodeType GamemodeManager::gamemode            = MATCH_THE_WORD;

unsigned int             GamemodeManager::GamemodeSettings::cur_cat    = 0;
unsigned int             GamemodeManager::GamemodeSettings::sel_ind[3] = { 0, 0, 0 };
std::vector<const char*> GamemodeManager::GamemodeSettings::selection[3];

const WordData* GamemodeManager::MTW::choices[4]{};
const WordData* GamemodeManager::MTW::correct;

std::vector<std::string>     GamemodeManager::ATS::bodies;
std::vector<const WordData*> GamemodeManager::ATS::blanks;


void GamemodeManager::add_font(const char* file_path, bool is_KR_or_EN)
{
    static const float size_unit = Game::m_width * 0.0085f;
    ImGuiIO& IO = ImGui::GetIO();

    if (is_KR_or_EN) {
        font_sizes_KR[0] = IO.Fonts->AddFontFromFileTTF(file_path, 0.8f * size_unit);
        font_sizes_KR[1] = IO.Fonts->AddFontFromFileTTF(file_path, 1.0f * size_unit);
        font_sizes_KR[2] = IO.Fonts->AddFontFromFileTTF(file_path, 1.2f * size_unit);
        font_sizes_KR[3] = IO.Fonts->AddFontFromFileTTF(file_path, 2.4f * size_unit);
        font_sizes_KR[4] = IO.Fonts->AddFontFromFileTTF(file_path, 3.0f * size_unit);
        font_sizes_KR[5] = IO.Fonts->AddFontFromFileTTF(file_path, 5.0f * size_unit);
        font_sizes_KR[6] = IO.Fonts->AddFontFromFileTTF(file_path, 8.0f * size_unit);
        font_sizes_KR[7] = IO.Fonts->AddFontFromFileTTF(file_path, 10.0f * size_unit);
        font_sizes_KR[8] = IO.Fonts->AddFontFromFileTTF(file_path, 18.0f * size_unit);
        font_sizes_KR[9] = IO.Fonts->AddFontFromFileTTF(file_path, 22.5f * size_unit);
    } else {
        font_sizes_EN[0] = IO.Fonts->AddFontFromFileTTF(file_path, 0.8f * size_unit);
        font_sizes_EN[1] = IO.Fonts->AddFontFromFileTTF(file_path, 1.0f * size_unit);
        font_sizes_EN[2] = IO.Fonts->AddFontFromFileTTF(file_path, 1.2f * size_unit);
        font_sizes_EN[3] = IO.Fonts->AddFontFromFileTTF(file_path, 2.0f * size_unit);
        font_sizes_EN[4] = IO.Fonts->AddFontFromFileTTF(file_path, 3.0f * size_unit);
        font_sizes_EN[5] = IO.Fonts->AddFontFromFileTTF(file_path, 5.0f * size_unit);
        font_sizes_EN[6] = IO.Fonts->AddFontFromFileTTF(file_path, 8.0f * size_unit);
        font_sizes_EN[7] = IO.Fonts->AddFontFromFileTTF(file_path, 10.0f * size_unit);
        font_sizes_EN[8] = IO.Fonts->AddFontFromFileTTF(file_path, 18.0f * size_unit);
        font_sizes_EN[9] = IO.Fonts->AddFontFromFileTTF(file_path, 22.5f * size_unit);
    }

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
    const ImVec2 display_text        = ImGui::CalcTextSize(st::selection[st::cur_cat][st::sel_ind[st::cur_cat]]);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.5f, 0.5f, 1.0f));
    ImGui::SetCursorPos(ImVec2(20.0f, 40.0f));
    ImGui::PushFont(font_sizes_EN[3]);

    ImGui::letter_spacing = 1.0f;
    ImGui::Text(category_label);
    ImGui::letter_spacing = 0.0f;

    static const ImVec2 lss = ImGui::CalcTextSize("A"); // Left  select size
    static const ImVec2 rss = ImGui::CalcTextSize("D"); // Right select size

    // Draw A and D buttons (disabled)
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
    ImGui::SetCursorPos(ImVec2(45.0f, 20.0f));
    ImGui::Text(st::selection[st::cur_cat][st::sel_ind[st::cur_cat]]);

    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    return selected_word_group;
}

void GamemodeManager::draw_mtw_mode(const char* group_name)
{
    //ImGui::SetCursorPos(ImVec2((Game::m_width - text_size.x) * 0.5f, Game::m_height * 0.5f - text_size.y));

    if (is_inbetween_rounds) {
        ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
        if (ImGui::InvisibleButton("##prompt_continue", ImVec2(Game::m_width, Game::m_height))) {
            shuffle_choices(group_name);
            is_inbetween_rounds = false;
        }
    }

    switch (static_cast<DifficultyLevel>(GamemodeSettings::GamemodeSettings::sel_ind[1])) {
        case EASY:
            {
                // Draw "correct" display text
                // Draw question text
                static const char* text;
                static ImVec2 text_size;

                if (is_KR_or_EN) {
                    text = MTW::correct->word.data();
                    ImGui::PushFont(font_sizes_KR[8]);
                } else {
                    text = MTW::correct->meaning.data();
                    ImGui::PushFont(font_sizes_EN[8]);
                }
                text_size = ImGui::CalcTextSize(text);

                ImGui::SetCursorPos(ImVec2((Game::m_width - text_size.x) * 0.5f, Game::m_height * 0.25f - text_size.y * 0.5f));
                ImGui::Text(text);

                ImGui::PopFont();
                ImGui::PushFont(is_KR_or_EN ? font_sizes_EN[5] : font_sizes_KR[5]);

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
                    const char* button_text;

                    if (is_KR_or_EN) {
                        button_text = MTW::choices[i]->meaning.c_str();
                    } else {
                        button_text = MTW::choices[i]->word.c_str();
                    }

                    ImGui::SetCursorPos(pos[i]);
                    if (is_inbetween_rounds) {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_Button, MTW::choices[i] == MTW::correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonActive, MTW::choices[i] == MTW::correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
                        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, MTW::choices[i] == MTW::correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
                        ImGui::BeginDisabled();
                        ImGui::PushID(i);
                        ImGui::Button(button_text, button_size);
                        ImGui::PopID();

                        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                            ImGui::PushFont(is_KR_or_EN ? font_sizes_KR[3] : font_sizes_EN[3]);

                            std::string text = is_KR_or_EN ? MTW::choices[i]->word : MTW::choices[i]->meaning;
                            ImVec2 text_size = ImGui::CalcTextSize(text.c_str());

                            ImGui::SetCursorPos(ImVec2(pos[i].x + (button_size.x - text_size.x) * 0.5f, pos[i].y + button_size.y * 0.75f - text_size.y * 0.5f));
                            if (!is_KR_or_EN) {
                                ImGui::letter_spacing = 1.5f;
                                ImGui::Text(text.c_str());
                                ImGui::letter_spacing = 0.0f;
                            } else {
                                ImGui::Text(text.c_str());
                            }

                            ImGui::PopFont();
                        }

                        ImGui::EndDisabled();
                        ImGui::PopStyleColor(4);
                    } else {
                        // if (false) {
                        //     ImGui::InvisibleButton("##button", button_size);
                        //
                        //     bool is_hovered = ImGui::IsItemHovered();
                        //
                        //     ImDrawList* draw_list = ImGui::GetWindowDrawList();
                        //
                        //     draw_list->AddRectFilled(
                        //             ImVec2(pos[0].x - 8.0f, pos[0].y - 8.0f),
                        //             ImVec2(pos[0].x + button_size.x + 8.0f, pos[0].y + button_size.y + 8.0f),
                        //             is_hovered ? IM_COL32(255, 87, 87, 255) : IM_COL32(239, 239, 239, 255)
                        //             );
                        //
                        //     draw_list->AddRectFilled(
                        //             pos[0],
                        //             ImVec2(pos[0].x + button_size.x, pos[0].y + button_size.y),
                        //             is_hovered ? IM_COL32(239, 239, 239, 255) : IM_COL32(255, 87, 87, 255)
                        //             );
                        //
                        //     draw_list->AddRectFilled(
                        //             ImVec2(pos[0].x + 8.0f, pos[0].y + 8.0f),
                        //             ImVec2(pos[0].x + button_size.x - 8.0f, pos[0].y + button_size.y - 8.0f),
                        //             is_hovered ? IM_COL32(255, 87, 87, 255) : IM_COL32(239, 239, 239, 255)
                        //             );
                        // } else {
                        //     ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
                        //     if (ImGui::Button(button_text, button_size)) {
                        //         is_inbetween_rounds = true;
                        //     }
                        //     ImGui::PopStyleColor();
                        // }

                        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
                        if (ImGui::Button(button_text, button_size)) {
                            is_inbetween_rounds = true;
                        }
                        ImGui::PopStyleColor();
                    }
                }

                ImGui::PopFont();
                break;
            }
        case EASY_PLUS:
            {
                ;

                break;
            }
        case MEDIUM:
            {
                // Draw "correct" display text
                // Draw question text
                const char* text;
                ImVec2 text_size;

                if (is_KR_or_EN) {
                    text = MTW::correct->word.data();
                    ImGui::PushFont(font_sizes_KR[9]);
                } else {
                    text = MTW::correct->meaning.data();
                    ImGui::PushFont(font_sizes_EN[9]);
                }
                text_size = ImGui::CalcTextSize(text);

                ImGui::SetCursorPos(ImVec2((Game::m_width - text_size.x) * 0.5f, Game::m_height * 0.5f - text_size.y));
                ImGui::Text(text);

                ImGui::PopFont();
                ImGui::PushFont(is_KR_or_EN ? font_sizes_EN[7] : font_sizes_KR[7]);

                static const float size_unit      = Game::m_width * 0.0085f;
                static const float textbox_height = ImGui::GetFrameHeight();

                ImGui::SetCursorPos(ImVec2(Game::m_width * 0.2f, Game::m_height * 0.6875f - textbox_height * 0.5f));
                ImGui::SetNextItemWidth(Game::m_width * 0.6f);
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(1.5f * size_unit, 0.5f * size_unit));

                char buffer[512]{};
                if (ImGui::InputText("##input_text", buffer, 512, ImGuiInputTextFlags_EnterReturnsTrue)) {
                    const std::string& correct_answer = is_KR_or_EN ? MTW::correct->meaning : MTW::correct->word;
                    if (buffer == correct_answer) {
                        is_inbetween_rounds = true;
                        std::memset(buffer, 0, 512);
                    }
                }
                ImGui::PopStyleVar();
                ImGui::PopFont();

                if (ImGui::IsItemActive()) {
                    is_typing = true;
                }

                if (ImGui::IsItemDeactivated()) {
                    is_typing = false;
                }

                break;
            }
        case MEDIUM_PLUS:
            break;
        case HARD:
            break;
    }

    // Finish rendering
}

void GamemodeManager::draw_ats_mode(const char* group_name)
{
    // anime
}

void GamemodeManager::shuffle_choices(const char* group_name)
{
    static const char*                         current_group = group_name;
    static const std::vector<const WordData*>* word_group    = WordDatabase::get_word_group(current_group);

    static std::mt19937                        gen(std::random_device{}());
    static std::uniform_int_distribution       picker(0, static_cast<int>(word_group->size() - 1));

    if (current_group != group_name) {
        current_group = group_name;
        word_group    = WordDatabase::get_word_group(current_group);
        picker        = std::uniform_int_distribution(0, static_cast<int>(word_group->size() - 1));
    }

    switch (gamemode) {
        case MATCH_THE_WORD:
            switch (static_cast<DifficultyLevel>(GamemodeSettings::GamemodeSettings::sel_ind[1])) {
                case EASY:
                case EASY_PLUS:
                    {
                        static std::uniform_int_distribution four(0, 3);
                        static const WordData* selected_items[4];

                        // Populating MTW::choices with random words and ensuring no dupes
                        for (unsigned int i = 0; i < 4; ++i) {
                            const WordData* item;
                            bool            is_dupe;

                            while (true) {
                                item    = (*word_group)[picker(gen)];
                                is_dupe = false;

                                for (unsigned int j = 0; j < i; ++j) {
                                    if (selected_items[j] == item) {
                                        is_dupe = true;
                                    }
                                }

                                if (!is_dupe) {
                                    break;
                                }
                            }

                            selected_items[i] = item;
                        }

                        // Making sure the new "correct" doesn't match the old one even if the old "correct" appears in the next choices
                        int correct_index = four(gen);
                        while (MTW::correct == selected_items[correct_index]) {
                            correct_index = four(gen);
                        }

                        // Assigning the new values to their respective buffers
                        for (unsigned int i = 0; i < 4; ++i) {
                            MTW::choices[i] = selected_items[i];

                            if (i == correct_index) {
                                MTW::correct = selected_items[i];
                            }
                        }
                    }
                    break;
                case MEDIUM:
                case MEDIUM_PLUS:
                    {
                        const WordData* new_correct = (*word_group)[picker(gen)];
                        while (MTW::correct == new_correct) {
                            new_correct = (*word_group)[picker(gen)];;
                        }

                        MTW::correct = new_correct;
                    }
                    break;
                case HARD:
                    break;
            }
            break;
        case ARRANGE_THE_SENTENCE:
            break;
    }
}
