#include <iostream>
#include <string>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "ImageList.h"
#include "IDisplay.h"
#include "IsometricDisplay.h"
#include "Space.h"

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

    ImageList tiles;

    if (!tiles.loadImagesFromIndexFile("resources/levels/practice"))
    {
        std::cout << "Failed to load images.\n";
        return -1;
    }

    IDisplay* my_drawer = new IsometricDisplay;

    Space level;

    level.setImageList(&tiles);

    if (!level.loadFromFile("resources/levels/practice"))
    {
        std::cout << "Failed to load level;";
        return -1;
    }

    my_drawer->setSpace(&level);

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
                if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                {
                    std::cout << "Safely quitting.";
                    al_destroy_display(main_window);
                    al_destroy_event_queue(eq);
                    al_destroy_timer(fps_timer);
                    delete my_drawer;
                    return 0;
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_UP)
                {
                    my_drawer->changeOffset(0, -20);
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN)
                {
                    my_drawer->changeOffset(0, 20);
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT)
                {
                    my_drawer->changeOffset(-20, 0);
                }
                else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT)
                {
                    my_drawer->changeOffset(20, 0);
                }
            }
        }
        if (ready_to_draw)
        {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            my_drawer->draw();
            al_flip_display();
            ready_to_draw = false;
        }
    }
}
