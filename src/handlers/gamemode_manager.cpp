#include "handlers/gamemode_manager.hpp"
#include "handlers/resource_manager.hpp"
#include "handlers/text_input_handler.hpp"
#include "handlers/words_database.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <algorithm>
#include <cstring>
#include <random>

#define SCALING_FACTOR_FIX   ResourceManager::game_width * 0.0085f / 10.88f

// Gamemode
bool                         GamemodeManager::is_inbetween_rounds    = false;
bool                         GamemodeManager::is_KR_or_EN            = false;
bool                         GamemodeManager::is_typing              = false;
bool                         GamemodeManager::should_shuffle_choices = true;
GamemodeType                 GamemodeManager::gamemode               = MATCH_THE_WORD;

// Settings
unsigned int                 GamemodeManager::GamemodeSettings::cur_cat    = 0;
unsigned int                 GamemodeManager::GamemodeSettings::sel_ind[3] = { 0, 0, 0 };
std::vector<const char*>     GamemodeManager::GamemodeSettings::selection[3];

// TBA
std::vector<std::string>     GamemodeManager::ATS::bodies;
std::vector<const WordData*> GamemodeManager::ATS::blanks;

void GamemodeManager::init(const char* EN_file_path, const char* KR_file_path)
{
    ImGuiIO& IO = ImGui::GetIO();

    // Loading fonts
    ResourceManager::font_EN = IO.Fonts->AddFontFromFileTTF(EN_file_path);
    ResourceManager::font_KR = IO.Fonts->AddFontFromFileTTF(KR_file_path);

    // Loading size of fonts in px
    const float scaling_unit = ResourceManager::game_width * 0.01f;

    ResourceManager::font_sizes[0] = 0.8f  * scaling_unit;
    ResourceManager::font_sizes[1] = 1.2f  * scaling_unit;
    ResourceManager::font_sizes[2] = 1.8f  * scaling_unit;
    ResourceManager::font_sizes[3] = 2.4f  * scaling_unit;
    ResourceManager::font_sizes[4] = 3.0f  * scaling_unit;
    ResourceManager::font_sizes[5] = 5.0f  * scaling_unit;
    ResourceManager::font_sizes[6] = 8.0f  * scaling_unit;
    ResourceManager::font_sizes[7] = 10.0f * scaling_unit;
    ResourceManager::font_sizes[8] = 18.0f * scaling_unit;
    ResourceManager::font_sizes[9] = 22.5f * scaling_unit;

    IO.Fonts->Build();
}

void GamemodeManager::draw_gui()
{
    // Create frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Draw window
    ImGui::SetNextWindowSize(ImVec2(ResourceManager::game_width, ResourceManager::game_height));
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Hello", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        ImGui::End();
        return;
    }

    // Settings
    draw_settings();

    switch (gamemode) {
        case MATCH_THE_WORD:
            draw_mtw_mode(GamemodeSettings::selection[2][GamemodeSettings::sel_ind[2]]);
            break;
        case ARRANGE_THE_SENTENCE:
            // draw_ats_mode(selected_word_group);
            break;
    }

    // Finish rendering
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void GamemodeManager::draw_settings()
{
    /*
    // Alias
    using settings = GamemodeSettings;

    const char* categories[]   = {
        "Source and target",
        "Difficulty level",
        "Word group"
    };

    const char*  category_label = categories[settings::cur_cat]; 
    const ImVec2 display_size   = ImGui::CalcTextSize(settings::selection[settings::cur_cat][settings::sel_ind[settings::cur_cat]]);

    ImGui::SetCursorPos(ImVec2(20.0f, 40.0f));
    ImGui::PushFont(
        ResourceManager::font_EN,
        ResourceManager::font_sizes[2]
    );

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
    ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(40.0f, 20.0f), ImVec2(165.0f, 40.0f), IM_COL32(102, 102, 102, 255));
    ImGui::SetCursorPos(ImVec2(45.0f, 30.0f - display_size.y * 0.5f));
    ImGui::Text(settings::selection[settings::cur_cat][settings::sel_ind[settings::cur_cat]]);

    ImGui::PopFont();
    ImGui::PopStyleColor(3);
    */

    // Draw settings menu page slider button thing
    static const ImVec2 button_size(
        ResourceManager::game_width / 32.0f,
        ResourceManager::game_width / 32.0f
    );
    static const ImVec2 button_pos(
        button_size.x * 0.5f,
        button_size.x * 0.5f
    );

    ImGui::SetCursorPos(button_pos);
    if (ImGui::InvisibleButton("##Settings", button_size)) {
        ;
    }

    static const Shader&   sprite_shader    = ResourceManager::shaders.at("sprite");
    static const Texture&  settings_texture = ResourceManager::textures.at("settings");
    static const Texture&  settings_border  = ResourceManager::textures.at("border");
    static const Drawable& settings         = ResourceManager::drawables.at("settings");

    sprite_shader.use();
    sprite_shader.uniform("model", settings.get_model());

    settings_texture.bind();
    settings.draw();

    settings_border.bind();
    
}

void GamemodeManager::draw_mtw_mode(const char* group_name)
{
    static const WordData* easy_choices[4];
    static const WordData* right_choices[4];
    static const WordData* correct;
    static bool            is_first_call = true;
    static std::mt19937    gen(std::random_device{}());

    auto repick_correct = []() {
        std::uniform_int_distribution<int> four(0, 3);

        const WordData* temp = easy_choices[four(gen)];

        while (temp == correct) {
            temp = easy_choices[four(gen)];
        }

        correct = temp;
    };

    auto shuffle = [group_name, &repick_correct](bool& reset) {
        MTW::shuffle_choices(group_name, easy_choices);
        std::memcpy(right_choices, easy_choices, sizeof(easy_choices));
        std::shuffle(std::begin(right_choices), std::end(right_choices), gen);
        repick_correct();

        reset = false;
    };

    if (is_first_call) {
        shuffle(is_first_call);
    }

    // For when word groups are changed and choices and such have to be reset externally
    if (should_shuffle_choices) {
        shuffle(should_shuffle_choices);
    }

    switch (static_cast<DifficultyLevel>(GamemodeSettings::GamemodeSettings::sel_ind[1])) {
        case EASY:
            if (is_inbetween_rounds) {
                ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
                if (ImGui::InvisibleButton("##prompt_continue", ImVec2(ResourceManager::game_width, ResourceManager::game_height))) {
                    shuffle(is_inbetween_rounds);
                }
            }
            MTW::draw_easy(easy_choices, correct);

            break;
        case EASY_PLUS:
            MTW::draw_easy_plus(easy_choices, right_choices);

            break;
        case MEDIUM:
            MTW::draw_medium(group_name, correct);

            break;
        case MEDIUM_PLUS:
            MTW::draw_medium_plus(); // NOT DONE
            break;
        case HARD:
            MTW::draw_hard();        // NOT DONE
            break;
    }
}

// void GamemodeManager::draw_ats_mode(const char* group_name)
// {
//     // anime (Not yet added)
// }

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

void GamemodeManager::MTW::draw_easy(const WordData* choices[4], const WordData* correct)
{
    // Draw "correct" display text
    ImGui::PushFont(
        is_KR_or_EN ? ResourceManager::font_KR : ResourceManager::font_EN,
        ResourceManager::font_sizes[8]
    );

    const char* display_text = is_KR_or_EN ? correct->KR[0].c_str() : correct->EN[0].c_str();
    ImVec2      display_size = ImGui::CalcTextSize(display_text);

    ImGui::SetCursorPos(ImVec2((ResourceManager::game_width - display_size.x) * 0.5f, ResourceManager::game_height * 0.25f - display_size.y * 0.5f));
    ImGui::Text(display_text);

    ImGui::PopFont();
    ImGui::PushFont(
        is_KR_or_EN ? ResourceManager::font_EN : ResourceManager::font_KR,
        ResourceManager::font_sizes[5]
    );

    // Draw the options
    // Button settings
    static const float  gap = 50.0f;
    static const ImVec2 button_size(ResourceManager::game_width * 0.3f, (ResourceManager::game_height * 0.5f - gap * 2) * 0.5f);
    static const ImVec2 pos[4] = {
        ImVec2(ResourceManager::game_width * 0.5f - button_size.x - gap * 0.5f,
                ResourceManager::game_height * 0.5f - button_size.y * 0.2f),
        ImVec2(ResourceManager::game_width * 0.5f + gap * 0.5f,
                ResourceManager::game_height * 0.5f - button_size.y * 0.2f),
        ImVec2(ResourceManager::game_width * 0.5f - button_size.x - gap * 0.5f,
                ResourceManager::game_height * 0.5f + button_size.y * 0.8f + gap),
        ImVec2(ResourceManager::game_width * 0.5f + gap * 0.5f,
                ResourceManager::game_height * 0.5f + button_size.y * 0.8f + gap)
    };

    // The actual buttons
    for (unsigned int i = 0; i < 4; ++i) {
        const char* button_text = is_KR_or_EN ? choices[i]->EN[0].c_str() : choices[i]->KR[0].c_str();

        ImGui::SetCursorPos(pos[i]);
        if (is_inbetween_rounds) {
            bool is_correct = (choices[i] == correct);

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Button, is_correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, is_correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, is_correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));

            ImGui::BeginDisabled();
            ImGui::PushID(i);
            ImGui::Button(button_text, button_size);
            ImGui::PopID();

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                ImGui::PushFont(
                    is_KR_or_EN ? ResourceManager::font_KR       : ResourceManager::font_EN,
                    is_KR_or_EN ? ResourceManager::font_sizes[3] : ResourceManager::font_sizes[2]
                );

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

void GamemodeManager::MTW::draw_easy_plus(const WordData* left_choices[4], const WordData* right_choices[4])
{
    // Left and right side widget variables
    static const float  gap         = 25.0f * SCALING_FACTOR_FIX;
    static const ImVec2 widget_size = ImVec2(ResourceManager::game_width * 0.25f, (ResourceManager::game_height * 0.75f - 3 * gap) * 0.25f);
    static const ImVec2 left_pos[4] = {
        ImVec2(ResourceManager::game_width * 0.15f, ResourceManager::game_height * 0.125f),
        ImVec2(ResourceManager::game_width * 0.15f, ResourceManager::game_height * 0.125f + (widget_size.y + gap)),
        ImVec2(ResourceManager::game_width * 0.15f, ResourceManager::game_height * 0.125f + (widget_size.y + gap) * 2.0f),
        ImVec2(ResourceManager::game_width * 0.15f, ResourceManager::game_height * 0.125f + (widget_size.y + gap) * 3.0f)
    };
    static const ImVec2 right_pos[4] = {
        ImVec2(ResourceManager::game_width * 0.85f - widget_size.x, ResourceManager::game_height * 0.125f),
        ImVec2(ResourceManager::game_width * 0.85f - widget_size.x, ResourceManager::game_height * 0.125f + (widget_size.y + gap)),
        ImVec2(ResourceManager::game_width * 0.85f - widget_size.x, ResourceManager::game_height * 0.125f + (widget_size.y + gap) * 2.0f),
        ImVec2(ResourceManager::game_width * 0.85f - widget_size.x, ResourceManager::game_height * 0.125f + (widget_size.y + gap) * 3.0f),
    };

    static int curr_selected    = -1;
    static int left_partners[4] = {
        -1,
        -1,
        -1,
        -1
    };
    static const ImVec4 colors[4][3] = {
        ImVec4(0.75f, 0.0f,  0.0f,  1.0f), ImVec4(0.8f, 0.0f, 0.0f, 1.0f), ImVec4(0.9f, 0.0f, 0.0f, 1.0f),
        ImVec4(0.0f,  0.75f, 0.0f,  1.0f), ImVec4(0.0f, 0.8f, 0.0f, 1.0f), ImVec4(0.0f, 0.9f, 0.0f, 1.0f),
        ImVec4(0.0f,  0.0f,  0.75f, 1.0f), ImVec4(0.0f, 0.0f, 0.8f, 1.0f), ImVec4(0.0f, 0.0f, 0.9f, 1.0f),
        ImVec4(0.75f, 0.75f, 0.0f,  1.0f), ImVec4(0.8f, 0.8f, 0.0f, 1.0f), ImVec4(0.9f, 0.9f, 0.0f, 1.0f),
    };

    // Draw left side
    ImGui::PushFont(
        is_KR_or_EN ? ResourceManager::font_EN : ResourceManager::font_KR,
        ResourceManager::font_sizes[5]
    );

    for (unsigned int i = 0; i < 4; ++i) {
        ImGui::SetCursorPos(left_pos[i]);

        bool   is_highlighted = left_partners[i] != -1 || curr_selected == i;
        ImVec4 curr_color(0.0f, 0.5f, 0.5f, 1.0f);

        ImGui::PushStyleColor(ImGuiCol_Button,        is_highlighted ? colors[i][0] : curr_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, is_highlighted ? colors[i][1] : curr_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  is_highlighted ? colors[i][2] : curr_color);
        if (ImGui::Button(is_KR_or_EN ? left_choices[i]->EN[0].c_str() : left_choices[i]->KR[0].c_str(), widget_size)) {
            if (curr_selected == i) {
                curr_selected = -1;
            } else {
                curr_selected = i;
            }
        }
        ImGui::PopStyleColor(3);
    }
    ImGui::PopFont();

    // Draw right side
    ImGui::PushFont(
        is_KR_or_EN ? ResourceManager::font_KR : ResourceManager::font_EN,
        ResourceManager::font_sizes[5]
    );

    for (unsigned int i = 0; i < 4; ++i) {
        ImGui::SetCursorPos(right_pos[i]);

        bool   is_highlighted    = false;
        int    highlighted_index = -1;
        ImVec4 curr_color(0.0f, 0.5f, 0.5f, 1.0f);

        for (unsigned int j = 0; j < 4; ++j) {
            if (left_partners[j] == i) {
                is_highlighted    = true;
                highlighted_index = j;
            }
        }

        ImGui::PushStyleColor(ImGuiCol_Button,        is_highlighted ? colors[highlighted_index][0] : curr_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, is_highlighted ? colors[highlighted_index][1] : curr_color);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  is_highlighted ? colors[highlighted_index][2] : curr_color);
        if (ImGui::Button(is_KR_or_EN ? right_choices[i]->KR[0].c_str() : right_choices[i]->EN[0].c_str(), widget_size)) {
            if (curr_selected != -1) {
                left_partners[curr_selected] = (left_partners[curr_selected] == i) ? -1 : i;

                for (unsigned int j = 0; j < 4; ++j) {
                    if (curr_selected == j) {
                        continue;
                    }

                    if (left_partners[j] == left_partners[curr_selected]) {
                        left_partners[j] = -1;
                    }
                }

                curr_selected = -1;
            }

            bool is_fully_chosen = true;
            for (unsigned int j = 0; j < 4; ++j) {
                if (left_partners[j] == -1) {
                    is_fully_chosen = false;
                    break;
                }
            }

            if (is_fully_chosen) {
                bool is_all_correct = true;

                for (unsigned int j = 0; j < 4; ++j) {
                    if (left_choices[j] != right_choices[left_partners[j]]) {
                        is_all_correct = false;
                        break;
                    }
                }

                if (is_all_correct) {
                    std::fill(std::begin(left_partners), std::end(left_partners), -1);

                    should_shuffle_choices = true;
                }
            }
        }
        ImGui::PopStyleColor(3);
    }
    ImGui::PopFont();
}

void GamemodeManager::MTW::draw_medium(const char* group_name, const WordData*& correct)
{
    // Draw "correct" display text
    // Draw question text
    const char* text;
    ImVec2      text_size;

    if (is_KR_or_EN) {
        text = correct->KR[0].c_str();
        ImGui::PushFont(
            ResourceManager::font_KR,
            ResourceManager::font_sizes[9]
        );

    } else {
        text = correct->EN[0].c_str();
        ImGui::PushFont(
            ResourceManager::font_EN,
            ResourceManager::font_sizes[9]
        );
    }
    text_size = ImGui::CalcTextSize(text);

    ImGui::SetCursorPos(ImVec2((ResourceManager::game_width - text_size.x) * 0.5f, ResourceManager::game_height * 0.5f - text_size.y));
    ImGui::Text(text);

    ImGui::PopFont();
    ImGui::PushFont(
        is_KR_or_EN ? ResourceManager::font_EN : ResourceManager::font_KR,
        ResourceManager::font_sizes[7]
    );

    static const float  textbox_width = ResourceManager::game_width * 0.6f;
    static const ImVec2 pos           = ImVec2(ResourceManager::game_width * 0.2f, ResourceManager::game_height * 0.6875f - ImGui::GetFrameHeight() * 0.5f);
    static const ImVec2 padding       = ImVec2(1.5f * ResourceManager::game_width * 0.0085f, 0.5f * ResourceManager::game_width * 0.0085f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, padding);

    if (TextInputHandler::draw_medium_textbox(correct, textbox_width, pos)) {
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
