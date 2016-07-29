#ifndef __IDISPLAYH_GUARD
#define __IDISPLAYH_GUARD

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

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
    virtual void draw() =0; //this should take the 3D space object
<<<<<<< HEAD
    virtual void changeOffset(float x, float y) = 0;
=======
    virtual void setSpace(Space* space) =0;
>>>>>>> 966dc2addb2e6ae5b7ccfee62eeab0d12748ff73
};

#endif // __IDISPLAYH_GUARD
