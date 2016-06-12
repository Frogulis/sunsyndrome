#include "ImageList.h"

ImageList::ImageList()
{
    image_array = nullptr;
    this->length = 0;
}

ImageList::~ImageList()
{
    if (image_array != nullptr)
    {
        delete image_array;
        image_array = nullptr;
    }
}

bool ImageList::loadImagesFromIndexFile(std::string filename)
{
    return true;
}

int ImageList::getLength()
{
    return this->length;
}

ALLEGRO_BITMAP* ImageList::getImageFromID(int ID)
{
    if (ID < this->length)
    {
        return this->image_array[ID];
    }
    else
    {
        return nullptr;
    }
}

