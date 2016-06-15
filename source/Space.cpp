#include "Space.h"

Space::Space()
{
    this->image_list = nullptr;
    this->id_array = nullptr;
    this->w = 0;
    this->d = 0;
    this->h = 0;
}

bool Space::loadFromFile(std::string filename)
{
    this->id_array = new int**[4];
    for (int i = 0; i < 4; i++)
    {
        this->id_array[i] = new int*[4];
        for (int j = 0; j < 4; j++)
        {
            this->id_array[i][j] = new int[4];
            this->id_array[i][j][0] = 0;
            this->id_array[i][j][1] = 1;
            this->id_array[i][j][2] = 0;
            this->id_array[i][j][3] = 1;
        }
    }
    this->w = 4;
    this->d = 4;
    this->h = 4;
}

void Space::setImageList(ImageList* il)
{
    this->image_list = il;
}

ALLEGRO_BITMAP* Space::getImageFromLocation(int w, int d, int h)
{
    return this->image_list->getImageFromID(this->id_array[w][d][h]);
}

int Space::getWidth()
{
    return this->w;
}

int Space::getDepth()
{
    return this->d;
}

int Space::getHeight()
{
    return this->h;
}
