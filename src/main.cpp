#include "game.hpp"

#define GAME_WINDOW_WIDTH  1280
#define GAME_WINDOW_HEIGHT 720

int main()
{
    Game::setup(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT, "me_program");
    Game::run();
    Game::finish();

    return 0;
}
