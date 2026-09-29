#include "game.hpp"
#include "handlers/draw_handler.hpp"
#include "handlers/gamemode_manager.hpp"
#include "handlers/resource_manager.hpp"
#include "handlers/words_database.hpp"

#ifdef VIDEO_RECORDING
#include "special/video_encoder.hpp"
#endif

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

bool cursor_not_in_frame = true;
bool cursor_updated      = false;

void draw_bg()
{
    static Shader& bg_shader = ResourceManager::shaders.at("bg");

    bg_shader.use();
    DrawHandler::draw(QUAD);
}

void draw_cursor()
{
    static Shader&  sprite_shader  = ResourceManager::shaders.at("sprite");
    static Texture& cursor_texture = ResourceManager::textures.at("cursor");

    static glm::mat4 cursor_model(
        glm::scale(
            glm::translate(
                glm::mat4(1.0f),
                glm::vec3(ResourceManager::cursor_x, ResourceManager::cursor_x, 0.0f)
            ),
            glm::vec3(12.0f, 20.0f, 1.0f) // Not adding a scaling fix yet to see whether cursor should shrink or not
        )
    );

    if (cursor_not_in_frame) {
        return;
    }

    if (cursor_updated) {
        cursor_model = glm::scale(
            glm::translate(
                glm::mat4(1.0f),
                glm::vec3(ResourceManager::cursor_x + 9.0f, ResourceManager::cursor_y + 15.0f, 0.0f)
            ),
            glm::vec3(12.0f, 20.0f, 1.0f)
        );

        cursor_updated = false;
    }

    sprite_shader.use();
    sprite_shader.uniform("brightness", 1.0f);
    sprite_shader.uniform("model", cursor_model);

    cursor_texture.bind();
    DrawHandler::draw(QUAD);
}

void Game::setup(unsigned int width, unsigned int height, const char* title)
{
    ResourceManager::game_width     = width;
    ResourceManager::game_height    = height;
    ResourceManager::scaling_factor = width / 1280.0f;

    // Setting up libraries
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    ResourceManager::game_window = glfwCreateWindow(ResourceManager::game_width, ResourceManager::game_height, title, nullptr, nullptr);

    glfwMakeContextCurrent(ResourceManager::game_window);
    glfwSwapInterval(1);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetKeyCallback(ResourceManager::game_window, key_callback);
    glfwSetCursorPosCallback(ResourceManager::game_window, cursor_pos_callback);
    glfwSetCursorEnterCallback(ResourceManager::game_window, cursor_enter_callback);
    glfwSetInputMode(ResourceManager::game_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glViewport(0, 0, ResourceManager::game_width, ResourceManager::game_height);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui::GetIO().IniFilename       = nullptr; // Disable Dear ImGui's data saving
    ImGui::GetIO().ConfigFlags      |= ImGuiConfigFlags_NoMouseCursorChange;
    ImGui::GetStyle().DisabledAlpha  = 1.0f;

    ImGui_ImplGlfw_InitForOpenGL(ResourceManager::game_window, true);
    ImGui_ImplOpenGL3_Init("#version 450 core");

    // Loading resources
    const float       SQUARES_PER_WIDTH = 32.0f;           // How many squares to draw across background width
    const std::string FONT_DIR_PATH     = "res/fonts/";    // Font files location
    const std::string SHADER_DIR_PATH   = "res/shaders/";  // Shader files location
    const std::string TEXTURE_DIR_PATH  = "res/textures/";  // Sprite files location

    ResourceManager::projection = glm::ortho(
        0.0f,
        static_cast<float>(ResourceManager::game_width),
        static_cast<float>(ResourceManager::game_height),
        0.0f,
        -1.0f,
        1.0f
    );

    // Words
    WordDatabase::init();

    // Fonts
    GamemodeManager::init(
        (FONT_DIR_PATH + "BebasNeue-Regular.ttf").c_str(),
        (FONT_DIR_PATH + "NotoSansKR-Regular.ttf").c_str()
    );

    // Shaders
    Shader& bg_shader = ResourceManager::shaders.try_emplace(
        "bg",
        (SHADER_DIR_PATH + "bg_vert.glsl").c_str(),
        (SHADER_DIR_PATH + "bg_frag.glsl").c_str()
    ).first->second;
    Shader& sprite_shader = ResourceManager::shaders.try_emplace(
        "sprite",
        (SHADER_DIR_PATH + "sprite_vert.glsl").c_str(),
        (SHADER_DIR_PATH + "sprite_frag.glsl").c_str()
    ).first->second;
    Shader& mono_color_shader = ResourceManager::shaders.try_emplace(
        "mono_color",
        (SHADER_DIR_PATH + "mono_color_vert.glsl").c_str(),
        (SHADER_DIR_PATH + "mono_color_frag.glsl").c_str()
    ).first->second;

    bg_shader.use();
    bg_shader.uniform("pattern_size", ResourceManager::game_width / SQUARES_PER_WIDTH);

    sprite_shader.use();
    sprite_shader.uniform("texture_unit", 0);
    sprite_shader.uniform("projection", ResourceManager::projection);

    mono_color_shader.use();
    mono_color_shader.uniform("projection", ResourceManager::projection);

    // Textures
    // Only using texture unit 0 as far as progress has gone
    glActiveTexture(GL_TEXTURE0);
    ResourceManager::textures.try_emplace("settings", (TEXTURE_DIR_PATH + "Settings.png").c_str());
    ResourceManager::textures.try_emplace("border",   (TEXTURE_DIR_PATH + "Border.png").c_str());
    ResourceManager::textures.try_emplace("cursor",   (TEXTURE_DIR_PATH + "Cursor.png").c_str());

    // Drawables
    DrawHandler::init_VAOs();
    ResourceManager::drawables.try_emplace(
        "settings",
        ResourceManager::game_width * 31.0f / 32.0f, ResourceManager::game_width / 32.0f,
        ResourceManager::game_width / 32.0f,         ResourceManager::game_width / 32.0f
    );
}

void Game::run()
{
#ifdef VIDEO_RECORDING
    VideoEncoder encoder("/home/DesiresDeepDown/Videos/output.mp4", ResourceManager::game_width, ResourceManager::game_height, 60);
#endif

    while (!glfwWindowShouldClose(ResourceManager::game_window)) {
        glfwPollEvents();

        draw_bg();
        GamemodeManager::draw_gui();
        draw_cursor();

#ifdef VIDEO_RECORDING
        encoder.add_frame();
#endif

        glfwSwapBuffers(ResourceManager::game_window);
    }
}

void Game::finish()
{
    ResourceManager::clear();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(ResourceManager::game_window);
    glfwTerminate();
}

void Game::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Abbreviating aliases
    using game_man = GamemodeManager;
    using settings = GamemodeManager::GamemodeSettings;

    if (action == GLFW_PRESS) {
        const unsigned int num_cat         = 3;                             // Number of categories
        const unsigned int difficulty_size = settings::selection[1].size(); // Number of difficulty settings
        const unsigned int word_group_size = settings::selection[2].size(); // Number of word groups

        // Break early whenever focused on typing
        if (GamemodeManager::is_typing) {
            return;
        }

        // Handle pressed key
        // Escape = close game
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, true);

                break;
            case GLFW_KEY_W:
            case GLFW_KEY_UP:
                if (++settings::cur_cat > num_cat - 1) {
                    settings::cur_cat = 0;
                }

                break;
            case GLFW_KEY_S:
            case GLFW_KEY_DOWN:
                if (--settings::cur_cat == 0xFFFFFFFF) {
                    settings::cur_cat = num_cat - 1;
                }

                break;
            case GLFW_KEY_A:
            case GLFW_KEY_LEFT:
                switch (settings::cur_cat) {
                    case 0:
                        game_man::is_KR_or_EN = false;
                        settings::sel_ind[0]  = static_cast<unsigned int>(game_man::is_KR_or_EN);

                        break;
                    case 1:
                        if (--settings::sel_ind[1] == 0xFFFFFFFF) {
                            settings::sel_ind[1] = 0;
                        }

                        break;
                    case 2:
                        if (--settings::sel_ind[2] == 0xFFFFFFFF) {
                            settings::sel_ind[2] = 0;
                            break;
                        }

                        game_man::should_shuffle_choices = true;
                        if (game_man::is_inbetween_rounds) {
                            game_man::is_inbetween_rounds = false;
                        }

                        break;
                }

                break;
            case GLFW_KEY_D:
            case GLFW_KEY_RIGHT:
                switch (settings::cur_cat) {
                    case 0:
                        game_man::is_KR_or_EN = true;
                        settings::sel_ind[0]  = static_cast<unsigned int>(game_man::is_KR_or_EN);

                        break;
                    case 1:
                        if (++settings::sel_ind[1] > difficulty_size - 1) {
                            settings::sel_ind[1] = difficulty_size - 1;
                        }

                        break;
                    case 2:
                        if (++settings::sel_ind[2] > word_group_size - 1) {
                            settings::sel_ind[2] = word_group_size - 1;
                            break;
                        }

                        game_man::should_shuffle_choices = true;
                        if (game_man::is_inbetween_rounds) {
                            game_man::is_inbetween_rounds = false;
                        }

                        break;
                }

                break;
        }
    }
}

void Game::cursor_enter_callback(GLFWwindow* window, int entered)
{
    if (entered) {
        cursor_not_in_frame = false;
    } else {
        cursor_not_in_frame = true;
    }
}

void Game::cursor_pos_callback(GLFWwindow* window, double _x, double _y)
{
    ResourceManager::cursor_x = _x;
    ResourceManager::cursor_y = _y;

    cursor_updated = true;
}
