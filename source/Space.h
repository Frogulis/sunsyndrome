#ifndef __SPACEH_GUARD
#define __SPACEH_GUARD

class ImageList;

#include <string>

#include <allegro5/allegro.h>


class Space
{
public:
    Space();

    bool loadFromFile(std::string filename);

    void setImageList(ImageList* il);
    int getWidth();
    int getDepth();
    int getHeight();

    int getIDFromLocation(int w, int d, int h);
    ALLEGRO_BITMAP* getImageOfType(int type);

private:
    int*** id_array;
    ImageList* il;
};

#endif // __SPACEH_GUARD
