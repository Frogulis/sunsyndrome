#ifndef __IMAGELISTH_GUARD
#define __IMAGELISTH_GUARD

#include <string>
#include <fstream>
#include <iostream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class ImageList
{
    /**
    ImageList
    Holds and manages an array of all the bitmaps in the game.
    This is relatively memory-intensive but perfectly
    acceptable in an absolute sense.
    Requires a index to specify which textures to use,
    in the format:
    filename.jpg ; comments here
    **/
public:
    ImageList();

    bool loadImagesFromIndexFile(std::string filename);

    int getLength();
    ALLEGRO_BITMAP* getImageFromID(int ID);
private:
    ~ImageList();
    ALLEGRO_BITMAP** image_array;
    int length;
};

#endif // __IMAGELISTH_GUARD
