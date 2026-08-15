#include "game.hpp"
#include "gamemode_manager.hpp"
#include "words_database.hpp"
#include <GLFW/glfw3.h>

#ifdef VIDEO_RECORDING
#include "special/video_encoder.hpp"
#endif

#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

GLFWwindow*  Game::window;
unsigned int Game::m_width;
unsigned int Game::m_height;
float        progress = 0.0f;
bool         is_inbetween_rounds = false;

void Game::setup(unsigned int width, unsigned int height, const char* title)
{
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
    ImGui::GetIO().IniFilename = nullptr; // Disable Dear ImGui's data saving

    ImGui::GetStyle().DisabledAlpha = 1.0f;
    glViewport(0, 0, m_width, m_height);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

    GamemodeManager::add_font("res/NotoSansKR-Regular.ttf", true);
    GamemodeManager::add_font("res/BebasNeue-Regular.ttf", false);
    WordDatabase::init();
}

void Game::run()
{
    GamemodeManager::shuffle_choices("Academics");

#ifdef VIDEO_RECORDING
    VideoEncoder encoder("/home/DesiresDeepDown/Videos/output.mp4", m_width, m_height, 60);
#endif

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        GamemodeManager::draw_gui();
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

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Abbreviating aliases
    using gm = GamemodeManager;
    using st = GamemodeManager::GamemodeSettings;

    if (action == GLFW_PRESS) {
        const unsigned int num_cat = 3;
        const unsigned int df_size = st::selection[1].size();
        const unsigned int wg_size = st::selection[2].size();

        // Break early whenever focused on typing
        if (GamemodeManager::is_typing) {
            return;
        }

        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);

                break;
            case GLFW_KEY_W:
                if (++st::cur_cat > num_cat - 1) {
                    st::cur_cat = 0;
                }

                break;
            case GLFW_KEY_S:
                if (--st::cur_cat == 0xFFFFFFFF) {
                    st::cur_cat = num_cat - 1;
                }

                break;
            case GLFW_KEY_A:
                switch (st::cur_cat) {
                    case 0:
                        gm::is_KR_or_EN = false;
                        st::sel_ind[0]  = static_cast<unsigned int>(gm::is_KR_or_EN);

                        break;
                    case 1:
                        if (--st::sel_ind[1] == 0xFFFFFFFF) {
                            st::sel_ind[1] = 0;
                        }

                        break;
                    case 2:
                        if (--st::sel_ind[2] == 0xFFFFFFFF) {
                            st::sel_ind[2] = 0;
                            break;
                        }

                        gm::shuffle_choices(st::selection[2][st::sel_ind[2]]);
                        if (gm::is_inbetween_rounds) {
                            gm::is_inbetween_rounds = false;
                        }

                        break;
                }
                break;
            case GLFW_KEY_D:
                switch (st::cur_cat) {
                    case 0:
                        gm::is_KR_or_EN = true;
                        st::sel_ind[0]  = static_cast<unsigned int>(gm::is_KR_or_EN);

                        break;
                    case 1:
                        if (++st::sel_ind[1] > df_size - 1) {
                            st::sel_ind[1] = df_size - 1;
                        }

                        break;
                    case 2:
                        if (++st::sel_ind[2] > wg_size - 1) {
                            st::sel_ind[2] = wg_size - 1;
                            break;
                        }

                        gm::shuffle_choices(st::selection[2][st::sel_ind[2]]);
                        if (gm::is_inbetween_rounds) {
                            gm::is_inbetween_rounds = false;
                        }

                        break;
                }
                break;
            // case GLFW_KEY_R:
            //     gm::shuffle_choices(const char *group_name);
        }
    }
}
