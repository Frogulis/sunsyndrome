#ifndef __SPACEH_GUARD
#define __SPACEH_GUARD

class ImageList;

#include <string>

#include <allegro5/allegro.h>

#include "ImageList.h"

class Space
{
public:
    Space();

    bool loadFromFile(std::string filename);

    void setImageList(ImageList* il);
    int getWidth();
    int getDepth();
    int getHeight();

    ALLEGRO_BITMAP* getImageFromLocation(int w, int d, int h);

private:
    int*** id_array;
    int w;
    int d;
    int h;
    ImageList* image_list;
};

#endif // __SPACEH_GUARD
