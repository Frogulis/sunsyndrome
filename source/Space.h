#ifndef __SPACEH_GUARD
#define __SPACEH_GUARD

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

    int lookupTypeAt(int w, int d, int h);
    ALLEGRO_BITMAP* getImageOfType(int type);

private:
    int*** type_array;
    //image_list something something
};

#endif // __SPACEH_GUARD
