#ifndef __SPACEH_GUARD
#define __SPACEH_GUARD

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
    bool getWalkableFromLocation(int w, int d, int h);
private:
    bool within_array_bounds(int w, int d, int h);
    Tile*** id_array;
    int w;
    int d;
    int h;
    ImageList image_list;
};

#endif // __SPACEH_GUARD
