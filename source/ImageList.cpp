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
    std::ifstream index;
    index.open(filename);
    if (!index.is_open())
    {
        std::cout << "Index not openable at " << filename << "\n";
        return false;
    }
    while (!index.eof())
    {
        char imagename[256];
        index.getline(imagename, 256);
        std::ifstream f;
        f.open(imagename);
        if(!f.open())
        {
            std::cout << "File: " << imagename << " not openable.\n";
            return false;
        }
        //load the image
    }
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

