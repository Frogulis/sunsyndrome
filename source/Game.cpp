#include "Game.h"

Game::Game()
{
    this->mode = -1;
}

/*void Game::loadMenuDisplay(std::string menu_name)
{
    if (menu_name == "main")
    {
        MenuDisplay* result = new MenuDisplay;
        this->cur_menu_display = result;
    }
    else
    {
        std::cout << "Invalid menu name: " << menu_name << ".\n";
    }
}*/

bool Game::init()
{
    if (!this->loadIsoDisplay("practice"))
    {
        return false;
    }
    return true;
}

bool Game::loadArea(std::string area_name)
{
    if (!this->loadIsoDisplay(area_name))
    {
        return false;
    }
    return true;
}

bool Game::loadIsoDisplay(std::string area_name)
{
    if (!this->cur_isometric_display)
    {
        this->cur_isometric_display.reset(new IsometricDisplay);
    }
    this->drawer = this->cur_isometric_display;
    if (!this->drawer->load(area_name))
    {
        return false;
    }
    this->mode = 1;
    return true;
}

void Game::runEvents(ALLEGRO_EVENT &ev)
{
    this->drawer->runEvents(ev);
}

void Game::runLogic()
{
    this->drawer->runLogic();
}

void Game::runDisplay()
{
    al_clear_to_color(al_map_rgb(0,0,0));
    this->drawer->draw();
}


