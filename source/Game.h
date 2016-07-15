#ifndef __GAMEH_GUARD
#define __GAMEH_GUARD


#include <iostream>
#include <memory>
#include <string>

#include <allegro5/allegro.h>

#include "Space.h"
#include "IDisplay.h"
#include "IsometricDisplay.h"
#include "MenuDisplay.h"

class Game
{
    /***
    Handles logic and everything else.
    Basically a hub that manages the game behind the screen.
    ***/
public:
    Game();
    bool init();
    void runEvents(ALLEGRO_EVENT &ev);
    void runLogic();
    void runDisplay();
private:
    int mode; // -1 = nothing placeholder?, 0 = menu, 1 = combat,
    bool keys[ALLEGRO_KEY_MAX];
    int mouse_button_state;
    void loadIsoDisplay(std::string area_name);
    void loadMenuDisplay(std::string menu_name);
    std::shared_ptr<IDisplay> drawer;
    std::shared_ptr<IsometricDisplay> cur_isometric_display;
    std::shared_ptr<Space> cur_map;
    std::shared_ptr<MenuDisplay> cur_menu_display;
};

#endif // __GAMEH_GUARD
