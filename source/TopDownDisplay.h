#ifndef __TOPDOWNDISPLAYH_GUARD
#define __TOPDOWNDISPLAYH_GUARD

#include "IDisplay.h"

class TopDownDisplay : public IDisplay
{
    /**
    TopDownDisplay
    A display implementation.
    Draws the space from the top down.
    Higher objects occlude lower ones.
    **/
public:
    TopDownDisplay();
    void draw();
    void setImageList(ImageList* il);
    void setSpace(Space* space);
private:
    ImageList* image_list;
};

#endif // __TOPDOWNDISPLAYH_GUARD
