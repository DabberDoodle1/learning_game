#include "scenes/starting_menu.hpp"

#include "handlers/resource_manager.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

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

    // Title
    ImGui::PushFont(ResourceManager::font_EN, 216);

    static const ImVec2 title_size = ImGui::CalcTextSize("LARPIPILAND");

    ImGui::SetCursorPos(ImVec2(ResourceManager::game_width * 0.5f - title_size.x * 0.5f, 304.0f - title_size.y));
    ImGui::Text("LARPIPILAND");

    ImGui::PopFont();

    // Button list
    static const char*  labels[4] = {
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
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(175, 130, 100, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(180, 140, 110, 255));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(225, 170, 130, 255));
    for (unsigned int i = 0; i < 4; ++i) {
        ImGui::SetCursorPos(pos[i]);
        if (ImGui::Button(labels[i], button_size)) {
            std::cout << i << '\n';
        }
    }
    ImGui::PopStyleColor(3);
    ImGui::PopFont();

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
