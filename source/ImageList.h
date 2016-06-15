#ifndef __IMAGELISTH_GUARD
#define __IMAGELISTH_GUARD

#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class ImageList
{
    /**
    ImageList

    **/
public:
    ImageList();
    ~ImageList();

    bool loadImagesFromIndexFile(std::string filename);

    int getLength();
    ALLEGRO_BITMAP* getImageFromID(int ID);
private:
    ALLEGRO_BITMAP** image_array;
    int length;
};

#endif // __IMAGELISTH_GUARD
