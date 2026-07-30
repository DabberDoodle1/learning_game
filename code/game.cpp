#include "game.hpp"
#include "words_database.hpp"

#ifdef VIDEO_RECORDING
#include "special/video_encoder.hpp"
#endif

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <random>

GLFWwindow     *Game::window;
unsigned int    Game::m_width;
unsigned int    Game::m_height;
const WordData *correct;
ImFont         *font_sizes[10];
float           progress = 0.0f;
bool            is_inbetween_rounds = false;

void shuffle_choices(const WordData* choices_buffer[]) {
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution four(0, 3);
    static std::uniform_int_distribution picker(0, word_count);

    int indices[4];
    for (unsigned int i = 0; i < 4; ++i) {
        int index;
        bool is_dupe;

        while (true) {
            index = picker(gen);
            is_dupe = false;

            for (unsigned int j = 0; j < i; ++j) {
                if (indices[j] == index) {
                    is_dupe = true;
                }
            }

            if (!is_dupe) {
                break;
            }
        }

        indices[i] = index;
    }

    int correct_index = four(gen);
    for (unsigned int i = 0; i < 4; ++i) {
        choices_buffer[i] = &words[indices[i]];

        if (i == correct_index) {
            correct = &words[indices[i]];
        }
    }
}

void Game::draw_gui(const WordData* choices[]) {
    // Create frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Draw window
    ImGui::SetNextWindowSize(ImVec2(m_width, m_height));
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);

    if (!ImGui::Begin("Hello", nullptr, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar)) {
        ImGui::End();
        return;
    }

    ImGui::PushFont(font_sizes[9]);

    // Draw question text
    const char *text = correct->meaning.data();
    ImVec2 text_size = ImGui::CalcTextSize(text);
    ImGui::PushClipRect(ImVec2(0.0f, m_height * 0.125f), ImVec2(m_width, m_height * 0.375f), false);
    ImGui::SetCursorPos(ImVec2(m_width / 2.0f - text_size.x * 0.5, m_height / 4.0f - text_size.y * 0.5f));
    ImGui::Text(text);
    ImGui::PopClipRect();

    ImGui::PopFont();
    ImGui::PushFont(font_sizes[4]);

    // Draw choice buttons
    static const float  gap = 50.0f;
    static const ImVec2 button_size(m_width * 0.3f, (m_height * 0.5f - gap * 2) * 0.5f);

    ImVec2 pos[4] = {
        ImVec2(m_width * 0.5f - button_size.x - gap * 0.5f,
                m_height * 0.5f - button_size.y * 0.2f),
        ImVec2(m_width * 0.5f + gap * 0.5f,
                m_height * 0.5f - button_size.y * 0.2f),
        ImVec2(m_width * 0.5f - button_size.x - gap * 0.5f,
                m_height * 0.5f + button_size.y * 0.8f + gap),
        ImVec2(m_width * 0.5f + gap * 0.5f,
                m_height * 0.5f + button_size.y * 0.8f + gap)
    };

    if (is_inbetween_rounds) {
        ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));
        if (ImGui::InvisibleButton("##prompt_continue", ImVec2(m_width, m_height))) {
            shuffle_choices(choices);
            is_inbetween_rounds = false;
        }
    }

    for (unsigned int i = 0; i < 4; ++i) {
        ImGui::SetCursorPos(pos[i]);

        if (is_inbetween_rounds) {
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_Button, choices[i] == correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, choices[i] == correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, choices[i] == correct ? ImVec4(0.0f, 0.6f, 0.0f, 1.0f) : ImVec4(0.6f, 0.0f, 0.0f, 1.0f));
            ImGui::BeginDisabled();
            ImGui::PushID(i);
            ImGui::Button(choices[i]->word.c_str(), button_size);
            ImGui::PopID();

            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled)) {
                ImGui::PushFont(font_sizes[1]);

                std::string text = '(' + choices[i]->meaning + ')';
                ImVec2 text_size = ImGui::CalcTextSize(text.c_str());

                ImGui::SetCursorPos(ImVec2(pos[i].x + (button_size.x - text_size.x) * 0.5f, pos[i].y + button_size.y * 0.75f - text_size.y * 0.5f));
                ImGui::Text(text.c_str());

                ImGui::PopFont();
            }

            ImGui::EndDisabled();
            ImGui::PopStyleColor(4);
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.5f, 0.5f, 1.0f));
            if (ImGui::Button(choices[i]->word.c_str(), button_size)) {
                is_inbetween_rounds = true;
            }
            ImGui::PopStyleColor();
        }
    }

    // Finish rendering
    ImGui::PopFont();
    ImGui::End();
    ImGui::Render();
}

void Game::setup(unsigned int width, unsigned int height, const char *title) {
    m_width  = width;
    m_height = height;

    // Setting up libraries
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(m_width, m_height, title, nullptr, nullptr);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 450 core");

    ImGuiIO& IO   = ImGui::GetIO();
    IO.IniFilename = nullptr; // Disable Dear ImGui's data saving

    font_sizes[0] = IO.Fonts->AddFontFromFileTTF("res/NotoSansKR-Regular.ttf", 12.0f);
    font_sizes[1] = IO.Fonts->AddFontFromFileTTF("res/NotoSansKR-Regular.ttf", 24.0f);
    font_sizes[2] = IO.Fonts->AddFontFromFileTTF("res/NotoSansKR-Regular.ttf", 36.0f);
    font_sizes[3] = IO.Fonts->AddFontFromFileTTF("res/NotoSansKR-Regular.ttf", 48.0f);
    font_sizes[4] = IO.Fonts->AddFontFromFileTTF("res/NotoSansKR-Regular.ttf", 60.0f);
    font_sizes[5] = IO.Fonts->AddFontFromFileTTF("res/NotoSansKR-Regular.ttf", 72.0f);
    font_sizes[6] = IO.Fonts->AddFontFromFileTTF("res/NotoSansKR-Regular.ttf", 84.0f);
    font_sizes[7] = IO.Fonts->AddFontFromFileTTF("res/NotoSansKR-Regular.ttf", 96.0f);
    font_sizes[8] = IO.Fonts->AddFontFromFileTTF("res/NotoSansKR-Regular.ttf", 108.0f);
    font_sizes[9] = IO.Fonts->AddFontFromFileTTF("res/NotoSansKR-Regular.ttf", 120.0f);
    IO.Fonts->Build();

    ImGui::GetStyle().DisabledAlpha = 1.0f;
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
}

void Game::run() {
    const WordData* choices[4]{};
    shuffle_choices(choices);

#ifdef VIDEO_RECORDING
    VideoEncoder encoder("/home/DesiresDeepDown/Videos/output.mp4", m_width, m_height, 60);
#endif

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        draw_gui(choices);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

#ifdef VIDEO_RECORDING
        encoder.add_frame();
#endif

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
