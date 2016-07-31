#include <iostream>
#include <string>
#include <functional>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include "Game.h"
#include "ImageList.h"
#include "IDisplay.h"
#include "IsometricDisplay.h"
#include "Space.h"
#include "Actor.h"
#include "HashTable.h"
#include "DialogueBox.h"
#include "StringUtils.h"
#include "AStar.h"

void safe_exit();

int main(int argc, char** argv)
{
    /*bool** ar = new bool*[10];
    for (int i = 0; i < 10; i++)
    {
        ar[i] = new bool[10];
        for (int j = 0; j < 10; j++)
        {
            if (i == 5 && j > 3)
            {
                ar[i][j] = false;
            }
            else if (j == 3 && i > 0 && i < 7)
            {
                ar[i][j] = false;
            }
            else
            {
                ar[i][j] = true;
            }
        }
    }

    JH::AStar pf(ar, 10, 10);
    pf.generatePath(std::pair<int,int>(2,7), std::pair<int,int>(8,7));

    if (!pf.getPath().size())
    {
        std::cout << "no solution.";
    }

    for (int c = 0; c < pf.getPath().size(); c++)
    {
        for (int y = 0; y < 10; y++)
        {
            for (int x = 0; x < 10; x++)
            {
                if (pf.getPath()[c].first == x && pf.getPath()[c].second == y)
                {
                    std::cout << 'O';
                }
                else if (!ar[x][y])
                {
                    std::cout << char(178);
                }
                else
                {
                    std::cout << char(176);
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    for (int y = 0; y < 10; y++)
    {
        for (int x = 0; x < 10; x++)
        {
            bool next = false;
            for (int c = 0; c < pf.getPath().size(); c++)
            {
                if (pf.getPath()[c].first == x && pf.getPath()[c].second == y)
                {
                    std::cout << 'O';
                    next = true;
                }
            }
            if (!next)
            {
                if (!ar[x][y])
                {
                    std::cout << char(178);
                }
                else
                {
                    std::cout << char(176);
                }
            }
        }
        std::cout << "\n";
    }

    return 0;
    */

    if (!al_init())
    {
        std::cout << "Failed to start Allegro.";
        return -1;
    }

    if (!al_init_image_addon())
    {
        std::cout << "Failed to start Allegro Imagea person who gives information to the police or to some other authority about the bad behavior or criminal activity of someone else Addon.\n";
        return -1;
    }

    if (!al_init_primitives_addon())
    {
        std::cout << "Failed to start Allegro Primitives Addon.\n";
        return -1;
    }

    if (!al_init_font_addon())
    {
        std::cout << "Failed to start Allegro Font Addon.\n";
        return -1;
    }

    if (!al_init_ttf_addon())
    {
        std::cout << "Failed to start Allegro TTF Addon.\n";
        return -1;
    }

    ALLEGRO_DISPLAY* main_window = al_create_display(800, 600);
    if (!main_window)
    {
        std::cout << "Failed to create display.";
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* eq = al_create_event_queue();
    if (!eq)
    {
        std::cout << "Failed to create event queue.";
        return -1;
    }

    ALLEGRO_TIMER* fps_timer = al_create_timer(1.0 / 60);
    if (!fps_timer)
    {
        std::cout << "Failed to create timer.";
    }


    if (!al_install_keyboard())
    {
        std::cout << "Failed to install keyboard.";
        return -1;
    }


    if (!al_install_mouse() || !al_set_mouse_cursor(main_window, al_create_mouse_cursor(al_load_bitmap("resources/sprites/UI/cursor/clicker.png"), 16, 31)))
    {
        std::cout << "Failed to install mouse.";
        return -1;
    }


    al_register_event_source(eq, al_get_timer_event_source(fps_timer));
    al_register_event_source(eq, al_get_keyboard_event_source());
    al_register_event_source(eq, al_get_mouse_event_source());
    al_register_event_source(eq, al_get_display_event_source(main_window));

    Game base;
    if (!base.init())
    {
        std::cout << "Failed to initialise game! Quitting...\n";
        return -1;
    }

    bool ready_to_draw = false;
    al_start_timer(fps_timer);

    Actor a;
    a.loadByName("shadowman");

    while (true)
    {
        while (!al_event_queue_is_empty(eq))
        {
            ALLEGRO_EVENT ev;
            al_get_next_event(eq, &ev);
            if (ev.type == ALLEGRO_EVENT_TIMER)
            {
                ready_to_draw = true;
            }
            else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            {
                std::cout << "Safely quitting.";
                al_destroy_display(main_window);
                al_destroy_event_queue(eq);
                al_destroy_timer(fps_timer);
                return 0;
            }
            else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) //current keyboard input section
                {
                    std::cout << "Safely quitting.";
                    al_destroy_display(main_window);
                    al_destroy_event_queue(eq);
                    al_destroy_timer(fps_timer);
                    return 0;
                }
            }
            base.runEvents(ev);
        }
        if (ready_to_draw)
        {
            base.runLogic();
            base.runDisplay();
            al_flip_display();
            ready_to_draw = false;
        }
    }
}

