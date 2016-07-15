#ifndef __SPACEH_GUARD
#define __SPACEH_GUARD

class ImageList;

#include <string>

#include <allegro5/allegro.h>

#include "ImageList.h"

class Space
{
public:
    struct Tile
    {
        Tile()
        {
            id = 999;
            walkable = false;
        }
        int id;
        bool walkable;
    };
    Space();

    bool loadFromFile(std::string filename);

    void setImageList(ImageList* il);
    int getWidth();
    int getDepth();
    int getHeight();

    ALLEGRO_BITMAP* getImageFromLocation(int w, int d, int h);

private:
    Tile*** id_array;
    int w;
    int d;
    int h;
    ImageList image_list;
};

#endif // __SPACEH_GUARD
