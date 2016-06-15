#ifndef __ISOMETRICDISPLAYH_GUARD
#define __ISOMETRICDISPLAYH_GUARD

#include <iostream>

#include "IDisplay.h"
#include "Space.h"

class IsometricDisplay : public IDisplay
{
public:
    IsometricDisplay();
    void draw();
    void setSpace(Space* space);
private:
    ImageList* image_list;
    Space* space;
};

#endif // __ISOMETRICDISPLAYH_GUARD
