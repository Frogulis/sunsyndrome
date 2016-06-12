#ifndef __IDISPLAYH_GUARD
#define __IDISPLAYH_GUARD

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

class IDisplay
{
public:
    Display();
    void draw(); //this should take the 3D space object
};

#endif // __IDISPLAYH_GUARD
