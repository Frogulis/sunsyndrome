#include <iostream>
#include <string>

#include <allegro5/allegro.h>

int main(int argc, char** argv)
{
    std::string s = "This is a good ; thing.";

    std::cout << s;
    return 0;

    if (!al_init())
    {
        std::cout << "Failed to start Allegro.";
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

    if (!al_install_keyboard())
    {
        std::cout << "Failed to install keyboard.";
        return -1;
    }
    al_register_event_source(eq, al_get_keyboard_event_source());

    while (true)
    {
        while (!al_event_queue_is_empty(eq))
        {
            ALLEGRO_EVENT ev;
            al_get_next_event(eq, &ev);
            if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    std::cout << "Safely quitting.";
                    return 0;
                }
            }
        }
        al_clear_to_color(al_map_rgb(255,0,255));
        al_flip_display();
    }
}
