#include "Game.h"

Game::Game()
{
    this->loadIsoDisplay("practice");
    for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
    {
        this->keys[i] = false;
    }
    this->mouse_button_state = 0;
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



void Game::loadIsoDisplay(std::string area_name)
{
    if (!this->cur_isometric_display)
    {
        this->cur_isometric_display.reset(new IsometricDisplay);
        Space* s = new Space;
        s->loadFromFile(area_name);
        this->cur_isometric_display->setSpace(s);
    }
    this->drawer = this->cur_isometric_display;
    this->mode = 1;
}

void Game::runEvents(ALLEGRO_EVENT &ev)
{
    if (ev.type == ALLEGRO_EVENT_KEY_UP)
    {
        this->keys[ev.keyboard.keycode] = false;
    }
    else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        this->keys[ev.keyboard.keycode] = true;
    }
    else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
        this->mouse_button_state |= 1 << (ev.mouse.button - 1);

    }
    else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
    {
        this->mouse_button_state &= ~(1 << (ev.mouse.button - 1));
    }
}

void Game::runLogic()
{
    if (mode == 0)
    {

    }
    else if (mode == 1)
    {
        if (this->keys[ALLEGRO_KEY_UP])
        {
            this->drawer->changeOffset(0, -5);
        }
        if (this->keys[ALLEGRO_KEY_DOWN])
        {
            this->drawer->changeOffset(0, 5);
        }
        if (this->keys[ALLEGRO_KEY_LEFT])
        {
            this->drawer->changeOffset(-5, 0);
        }
        if (this->keys[ALLEGRO_KEY_RIGHT])
        {
            this->drawer->changeOffset(5, 0);
        }
    }
}

void Game::runDisplay()
{
    al_clear_to_color(al_map_rgb(0,0,0));
    this->drawer->draw();
}

