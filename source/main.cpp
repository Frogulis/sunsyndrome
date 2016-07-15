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

int main(int argc, char** argv)
{
    if (!al_init())
    {
        std::cout << "Failed to start Allegro.";
        return -1;
    }

    if (!al_init_image_addon())
    {
        std::cout << "Failed to start Allegro Image Addon.\n";
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
    al_register_event_source(eq, al_get_timer_event_source(fps_timer));

    if (!al_install_keyboard())
    {
        std::cout << "Failed to install keyboard.";
        return -1;
    }
    al_register_event_source(eq, al_get_keyboard_event_source());

    if (!al_install_mouse())
    {
        std::cout << "Failed to install mouse.";
        return -1;
    }
    al_register_event_source(eq, al_get_mouse_event_source());

    Game base;

    bool ready_to_draw = false;
    al_start_timer(fps_timer);

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
