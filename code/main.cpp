#include "game.hpp"

int main()
{
    Game::setup(1280, 720, "me_program");
    Game::run();
    Game::finish();

    return 0;
}
