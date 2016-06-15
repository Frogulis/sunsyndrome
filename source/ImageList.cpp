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
    index.open(filename + "/index");
    if (!index.is_open())
    {
        std::cout << "Index not openable at " << filename + "/index"<< "\n";
        return false;
    }

    //we have to get the size of the array we're putting images into
    //so i'm using an somewhat obscure bit of code from stackoverflow for now...
    this->length = std::count(std::istreambuf_iterator<char>(index), std::istreambuf_iterator<char>(), '\n');
    std::cout << "Length is: " << this->length << "\n";
    index.seekg(0);

    this->image_array = new ALLEGRO_BITMAP* [this->length];

    int index_counter = 0;
    while (!index.eof())
    {
        std::string imagename;
        std::getline(index, imagename);
        imagename = imagename.substr(0, imagename.find(';'));
        while (*(imagename.end()-1) == ' ')
        {
            imagename.pop_back();
        }
        if (imagename == "")
        {
            continue; //don't want to do anything more with this line so i just abandon it
        }
        imagename.append(".png");
        imagename.insert(0, filename + "/");
        std::cout << "Loading:-" << imagename << "-\n";
        ALLEGRO_BITMAP* temp_bmp = al_load_bitmap(imagename.c_str());
        if (!temp_bmp)
        {
            std::cout << "Failed to load " << imagename << "!\n";
            return false;
        }
        this->image_array[index_counter] = temp_bmp;
        index_counter++;
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

