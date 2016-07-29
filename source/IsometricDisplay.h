#ifndef __ISOMETRICDISPLAYH_GUARD
#define __ISOMETRICDISPLAYH_GUARD

#include <iostream>
#include <memory>

#include "IDisplay.h"
#include "Space.h"

class IsometricDisplay : public IDisplay
{
public:
    IsometricDisplay();
    void draw();
    void setSpace(Space* space);
    std::shared_ptr<Space> getSpacePtr();
    void changeOffset(float x, float y);
private:
    std::shared_ptr<Space> space;
    float x, y;
};

#endif // __ISOMETRICDISPLAYH_GUARD
