#ifndef __IDISPLAYH_GUARD
#define __IDISPLAYH_GUARD

#include "ImageList.h"
#include "Space.h"

class IDisplay
{
    /**
    IDisplay
    Purely an interface for displays.
    **/
public:
    IDisplay()
    {
        //
    }
    virtual void draw() = 0; //this should take the 3D space object
    virtual bool load(std::string name) = 0;
    virtual void changeOffset(float x, float y) = 0;
    virtual void runEvents(ALLEGRO_EVENT &ev) = 0;
    virtual void runLogic() = 0;
};

#endif // __IDISPLAYH_GUARD
