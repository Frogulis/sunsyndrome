#ifndef __IMAGELISTH_GUARD
#define __IMAGELISTH_GUARD

#include <string>

class ImageList
{
public:
    ImageList();

    bool loadImagesFromFolder(std::string foldername);
};

#endif // __IMAGELISTH_GUARD
