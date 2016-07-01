#include "DialogueBox.h"

DialogueBox::DialogueBox()
{
    this->tile_size = 10;
}


void DialogueBox::setDimensions(float x, float y, float tile_size)
{
    this->x = x;
    this->y = y;
    this->tile_size = tile_size;
}

bool DialogueBox::load(std::string boxname)
{
    ALLEGRO_BITMAP* full = al_load_bitmap(("resources/sprites/boxes/" + boxname + ".png").c_str());
    if (!full)
    {
        std::cout << "Failed to load dialogue box: " << boxname << "\n";
        return false;
    }

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            this->tiles[i*3 + j] = al_clone_bitmap(al_create_sub_bitmap(full, j*this->tile_size, i*this->tile_size, this->tile_size, this->tile_size));
        }
    }
    al_destroy_bitmap(full);
    return true;
}

ALLEGRO_BITMAP* DialogueBox::getBoxBitmap()
{
    ALLEGRO_BITMAP* result = al_create_bitmap(this->x + this->tile_size, this->y + this->tile_size);
    al_set_target_bitmap(result);

    for (int w = 10; w < this->x - this->tile_size/2; w += 10)
    {
        al_draw_bitmap(this->tiles[1], w, 0, 0);
        al_draw_bitmap(this->tiles[7], w, this->y, 0);
    }
    for (int h = 10; h < this->y - this->tile_size/2; h += 10)
    {
        al_draw_bitmap(this->tiles[3], 0, h, 0);
        al_draw_bitmap(this->tiles[5], this->x, h, 0);
    }
    al_draw_bitmap(this->tiles[0], 0, 0, 0);
    al_draw_bitmap(this->tiles[2], this->x, 0, 0);
    al_draw_bitmap(this->tiles[6], 0, this->y, 0);
    al_draw_bitmap(this->tiles[8], this->x, this->y, 0);


    return result;
}

