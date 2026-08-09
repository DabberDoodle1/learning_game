#include "gamemode_manager.hpp"
#include "game.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "words_database.hpp"
#include <random>

ImFont*      GamemodeManager::font_sizes_EN[8];
ImFont*      GamemodeManager::font_sizes_KR[8];
bool         GamemodeManager::is_inbetween_rounds = false;
bool         GamemodeManager::is_KR_or_EN         = false;
GamemodeType GamemodeManager::gamemode            = MULTI_CHOICE_QUIZ;
unsigned int GamemodeManager::cur_cat             = 0;
unsigned int GamemodeManager::sel_ind[2]          = { 0, 0 };

const std::unique_ptr<const char*[]> GamemodeManager::selection[] = {
    std::unique_ptr<const char*[]>(new const char*[2]{ "KR -> EN", "EN -> KR"}),
    std::unique_ptr<const char*[]>(new const char*[2]{ "Academics", "Question Words"})
};

const WordData* GamemodeManager::MCQ::choices[4]{};
const WordData* GamemodeManager::MCQ::correct;

std::vector<std::string> GamemodeManager::FITB::bodies;
std::vector<const WordData*>   GamemodeManager::FITB::blanks;

void GamemodeManager::add_font(const char* file_path, float size_unit, bool is_KR_or_EN)
{
    ImGuiIO& IO = ImGui::GetIO();

    if (is_KR_or_EN) {
        font_sizes_KR[0] = IO.Fonts->AddFontFromFileTTF(file_path, 0.8f * size_unit);
        font_sizes_KR[1] = IO.Fonts->AddFontFromFileTTF(file_path, 1.0f * size_unit);
        font_sizes_KR[2] = IO.Fonts->AddFontFromFileTTF(file_path, 1.2f * size_unit);
        font_sizes_KR[3] = IO.Fonts->AddFontFromFileTTF(file_path, 2.0f * size_unit);
        font_sizes_KR[4] = IO.Fonts->AddFontFromFileTTF(file_path, 3.0f * size_unit);
        font_sizes_KR[5] = IO.Fonts->AddFontFromFileTTF(file_path, 5.0f * size_unit);
        font_sizes_KR[6] = IO.Fonts->AddFontFromFileTTF(file_path, 8.0f * size_unit);
        font_sizes_KR[7] = IO.Fonts->AddFontFromFileTTF(file_path, 10.0f * size_unit);
    } else {
        font_sizes_EN[0] = IO.Fonts->AddFontFromFileTTF(file_path, 0.8f * size_unit);
        font_sizes_EN[1] = IO.Fonts->AddFontFromFileTTF(file_path, 1.0f * size_unit);
        font_sizes_EN[2] = IO.Fonts->AddFontFromFileTTF(file_path, 1.2f * size_unit);
        font_sizes_EN[3] = IO.Fonts->AddFontFromFileTTF(file_path, 2.0f * size_unit);
        font_sizes_EN[4] = IO.Fonts->AddFontFromFileTTF(file_path, 3.0f * size_unit);
        font_sizes_EN[5] = IO.Fonts->AddFontFromFileTTF(file_path, 5.0f * size_unit);
        font_sizes_EN[6] = IO.Fonts->AddFontFromFileTTF(file_path, 8.0f * size_unit);
        font_sizes_EN[7] = IO.Fonts->AddFontFromFileTTF(file_path, 10.0f * size_unit);
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
        case MULTI_CHOICE_QUIZ:
            draw_mcq_mode(selected_word_group, EASY);
            break;
        case FILL_IN_THE_BLANK:
            draw_fitb_mode(selected_word_group, EASY);
            break;
    }

    ImGui::End();
    ImGui::Render();
}

const char* GamemodeManager::draw_settings_selector()
{
    const char* categories[] = {
        "Source and Target",
        "Word Group"
    };

    const char* category_label      = categories[cur_cat]; 
    const char* selected_word_group = selection[1][sel_ind[1]];
    const ImVec2 display_text       = ImGui::CalcTextSize(selection[cur_cat][sel_ind[cur_cat]]);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25f, 0.5f, 0.5f, 1.0f));
    ImGui::SetCursorPos(ImVec2(20.0f, 20.0f));
    ImGui::PushFont(font_sizes_EN[3]);
    ImGui::Text(category_label);

    static const ImVec2 lss = ImGui::CalcTextSize("A"); // Left  select size
    static const ImVec2 rss = ImGui::CalcTextSize("D"); // Right select size

    ImGui::SetCursorPos(ImVec2(20.0f, 42.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2((20.0f - lss.x) * 0.5f, (20.0f - lss.y) * 0.5f));
    if (ImGui::Button("A##toggle", ImVec2(20.0f, 20.0f))) {
        switch (cur_cat) {
            case 0:
                is_KR_or_EN = !is_KR_or_EN;
                sel_ind[0]  = static_cast<unsigned int>(is_KR_or_EN);

                break;
            case 1:
                if (++sel_ind[1] > 1) {
                    sel_ind[1] = 0;
                }

                selected_word_group = selection[1][sel_ind[1]];
                shuffle_choices(selected_word_group);
                if (is_inbetween_rounds) {
                    is_inbetween_rounds = false;
                }

                break;
        }
    }
    ImGui::PopStyleVar();

    ImGui::SetCursorPos(ImVec2(165.0f, 42.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2((20.0f - rss.x) * 0.5f, (20.0f - rss.y) * 0.5f));
    if (ImGui::Button("D##toggle", ImVec2(20.0f, 20.0f))) {
        switch (cur_cat) {
            case 0:
                is_KR_or_EN = !is_KR_or_EN;
                sel_ind[0]  = static_cast<unsigned int>(is_KR_or_EN);

                break;
            case 1:
                if (--sel_ind[1] == 0xFFFFFFFF) {
                    sel_ind[1] = 1;
                }

                selected_word_group = selection[1][sel_ind[1]];
                shuffle_choices(selected_word_group);
                if (is_inbetween_rounds) {
                    is_inbetween_rounds = false;
                }

                break;
        }
    }
    ImGui::PopStyleVar();
    
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(40.0f, 42.5f), ImVec2(165.0f, 62.5f), IM_COL32(77, 77, 77, 255));
    ImGui::SetCursorPos(ImVec2(45.0f, 42.5f));
    ImGui::Text(selection[cur_cat][sel_ind[cur_cat]]);

    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    return selected_word_group;
}

void GamemodeManager::draw_mcq_mode(const char* group_name, DifficultyLevel difficulty)
{
    // Draw question text
    const char* text;

    if (is_KR_or_EN) {
        text = MCQ::correct->word.data();
        ImGui::PushFont(font_sizes_KR[7]);
    } else {
        text = MCQ::correct->meaning.data();
        ImGui::PushFont(font_sizes_EN[7]);
    }

    ImVec2 text_size = ImGui::CalcTextSize(text);
    ImGui::PushClipRect(ImVec2(0.0f, Game::m_height * 0.125f), ImVec2(Game::m_width, Game::m_height * 0.375f), false);
    ImGui::SetCursorPos(ImVec2(Game::m_width / 2.0f - text_size.x * 0.5, Game::m_height / 4.0f - text_size.y * 0.5f));
    ImGui::Text(text);
    ImGui::PopClipRect();

    ImGui::PopFont();
    ImGui::PushFont(is_KR_or_EN ? font_sizes_EN[5] : font_sizes_KR[5]);

    // Draw choice buttons
    static const float  gap = 50.0f;
    static const ImVec2 button_size(Game::m_width * 0.3f, (Game::m_height * 0.5f - gap * 2) * 0.5f);

    ImVec2 pos[4] = {
        ImVec2(Game::m_width * 0.5f - button_size.x - gap * 0.5f,
                Game::m_height * 0.5f - button_size.y * 0.2f),
        ImVec2(Game::m_width * 0.5f + gap * 0.5f,
                Game::m_height * 0.5f - button_size.y * 0.2f),
        ImVec2(Game::m_width * 0.5f - button_size.x - gap * 0.5f,
                Game::m_height * 0.5f + button_size.y * 0.8f + gap),
        ImVec2(Game::m_width * 0.5f + gap * 0.5f,
                Game::m_height * 0.5f + button_size.y * 0.8f + gap)
    };

    if (is_inbetween_rounds) {
        ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
        if (ImGui::InvisibleButton("##prompt_continue", ImVec2(Game::m_width, Game::m_height))) {
            shuffle_choices(group_name);
            is_inbetween_rounds = false;
        }
    }

    for (unsigned int i = 0; i < 4; ++i) {
        const char* button_text;

        if (is_KR_or_EN) {
            button_text = MCQ::choices[i]->meaning.c_str();
        } else {
            button_text = MCQ::choices[i]->word.c_str();
        }

        ImGui::SetCursorPos(pos[i]);
        if (is_inbetween_rounds) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Button, MCQ::choices[i] == MCQ::correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, MCQ::choices[i] == MCQ::correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, MCQ::choices[i] == MCQ::correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
            ImGui::BeginDisabled();
            ImGui::PushID(i);
            ImGui::Button(button_text, button_size);
            ImGui::PopID();

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                ImGui::PushFont(is_KR_or_EN ? font_sizes_EN[3] : font_sizes_KR[3]);

                std::string text = '(' + MCQ::choices[i]->meaning + ')';
                ImVec2 text_size = ImGui::CalcTextSize(text.c_str());

                ImGui::SetCursorPos(ImVec2(pos[i].x + (button_size.x - text_size.x) * 0.5f, pos[i].y + button_size.y * 0.75f - text_size.y * 0.5f));
                ImGui::Text(text.c_str());

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

    // Finish rendering
    ImGui::PopFont();
}

void GamemodeManager::draw_fitb_mode(const char* group_name, DifficultyLevel difficulty)
{
    ;
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
        case MULTI_CHOICE_QUIZ:
            {
                static std::uniform_int_distribution four(0, 3);
                static const WordData* selected_items[4];

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

                int correct_index = four(gen);

                while (MCQ::correct == selected_items[correct_index]) {
                    correct_index = four(gen);
                }

                for (unsigned int i = 0; i < 4; ++i) {
                    MCQ::choices[i] = selected_items[i];

                    if (i == correct_index) {
                        MCQ::correct = selected_items[i];
                    }
                }
            }
            break;
        case FILL_IN_THE_BLANK:
            break;
    }
}
