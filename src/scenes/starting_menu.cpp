#include "scenes/starting_menu.hpp"

#include "handlers/draw_handler.hpp"
#include "handlers/resource_manager.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glad/glad.h>

void StartingMenu::run()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::SetNextWindowSize(ImVec2(ResourceManager::game_width, ResourceManager::game_height));
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("chungus", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse)) {
        ImGui::End();
        return;
    }

    switch (location) {
        case STARTING_MENU:
            starting_menu_ui();
            break;

        case GAMEMODE_SELECTION:
            gamemode_selection_ui();
            break;
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void StartingMenu::starting_menu_ui()
{
    // Title
    ImGui::PushFont(ResourceManager::font_EN, 216);

    static const ImVec2 title_size = ImGui::CalcTextSize("LARPIPILAND");

    ImGui::SetCursorPos(ImVec2(ResourceManager::game_width * 0.5f - title_size.x * 0.5f, 304.0f - title_size.y));
    ImGui::Text("LARPIPILAND");

    ImGui::PopFont();

    // Button list
    static const char* labels[4] = {
        "PEEPEE",
        "POOPOO",
        "POMPOM",
        "PUNGUS"
    };
    static const ImVec2 pos[4] = {
        { 625.0f, 325.0f },
        { 625.0f, 450.0f },
        { 625.0f, 575.0f },
        { 625.0f, 700.0f }
    };
    static const ImVec2 button_size(350.0f, 75.0f);

    ImGui::PushFont(ResourceManager::font_EN, 57.4);
    ImGui::PushStyleColor(ImGuiCol_Button,        IM_COL32(175, 130, 100, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(180, 140, 110, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,  IM_COL32(225, 170, 130, 255));

    ImGui::SetCursorPos(pos[0]);
    if (ImGui::Button(labels[0], button_size)) {
        location = GAMEMODE_SELECTION;
    }

    ImGui::SetCursorPos(pos[1]);
    ImGui::Button(labels[1], button_size);
    ImGui::SetCursorPos(pos[2]);
    ImGui::Button(labels[2], button_size);
    ImGui::SetCursorPos(pos[3]);
    ImGui::Button(labels[3], button_size);

    ImGui::PopStyleColor(3);
    ImGui::PopFont();
}

void StartingMenu::gamemode_selection_ui()
{
    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    static Shader&   sprite_sh   = ResourceManager::shaders.at("sprite");
    static Drawable& brochaco_dw = ResourceManager::drawables.at("brochaco");
    static Texture&  brochaco_tx = ResourceManager::textures.at("brochaco");

    draw_list->AddCallback([](const ImDrawList* parent, const ImDrawCmd* cmd) {
        int im_sampler = 0;
        glGetIntegeri_v(GL_SAMPLER_BINDING, 0, &im_sampler);

        glBindSampler(0, 0);

        sprite_sh.use();
        sprite_sh.uniform("model", brochaco_dw.get_model());
        brochaco_tx.bind();
        DrawHandler::draw(QUAD);

        glBindSampler(0, im_sampler);
    });
    draw_list->AddCallback(ImDrawCallback_ResetRenderState);
}
