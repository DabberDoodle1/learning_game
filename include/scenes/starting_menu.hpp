#pragma once

#include "scene.hpp"

enum StartingMenuNavigation {
    STARTING_MENU = 0,
    GAMEMODE_SELECTION
};

class StartingMenu: public Scene {
public:
    StartingMenu(): location(STARTING_MENU) {}

    void run() override;

private:
    void starting_menu_ui();
    void gamemode_selection_ui();

    StartingMenuNavigation location;
};
