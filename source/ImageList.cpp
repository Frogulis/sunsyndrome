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
        std::string imagename;
        std::getline(index, imagename);
        imagename = imagename.substr(0, imagename.find(';'));
        while (imagename[imagename.length()-1] == ' ')
        {
            imagename.pop_back();
        }
        // load the image. need to get length of array from file length in lines
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

