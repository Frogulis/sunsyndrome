#include <iostream>
#include <string>
#include <functional>

#include <cstdlib>
#include <cstdio>

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
    time_t t;

    srand((unsigned) time(&t));

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
        return -1;
    }

    ALLEGRO_TIMER* count_timer = al_create_timer(1.0);
    if (!count_timer)
    {
        std::cout << "Failed to create count timer.";
        return -1;
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
    al_register_event_source(eq, al_get_timer_event_source(count_timer));
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
    bool ready_to_draw_fps = false;
    al_start_timer(fps_timer);
    al_start_timer(count_timer);
    int fps_count = 0;

    ALLEGRO_FONT* fps_font = al_load_ttf_font("resources/fonts/MontereyFLF.ttf", 11, 0);
    if (!fps_font)
    {
        std::cout << "Failed to load font for fps counter!";
        return -1;
    }

    while (true)
    {
        while (!al_event_queue_is_empty(eq))
        {
            ALLEGRO_EVENT ev;
            al_get_next_event(eq, &ev);
            if (ev.type == ALLEGRO_EVENT_TIMER)
            {
                if (ev.timer.source == fps_timer)
                {
                    ready_to_draw = true;
                }
                else if (ev.timer.source == count_timer)
                {
                    ready_to_draw_fps = true;
                }
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
            fps_count++;
            base.runLogic();
            base.runDisplay();
            char buffer[25];
            if (ready_to_draw_fps)
            {
                sprintf(buffer, "%d", fps_count);
                fps_count = 0;
                ready_to_draw_fps = false;
            }
            al_draw_text(fps_font, al_map_rgb(0,255,0), 5, 5, 0, buffer);
            al_flip_display();
            ready_to_draw = false;
        }

    }
}

