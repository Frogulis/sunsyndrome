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
    ~Space();

    bool loadFromFile(std::string filename);

    void setImageList(ImageList* il);
    int getWidth();
    int getDepth();
    int getHeight();
    int getArenaHeight(); //returns the height of the level on which the battle will occur
    ALLEGRO_BITMAP* getImageFromLocation(int w, int d, int h);
    bool getWalkableFromLocation(int w, int d, int h);
private:
    bool within_array_bounds(int w, int d, int h);
    void deleteIDArray();
    Tile*** id_array;
    int w;
    int d;
    int h;
    int arena_height;
    ImageList image_list;
};

#endif // __SPACEH_GUARD
