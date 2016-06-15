#include "IsometricDisplay.h"

IsometricDisplay::IsometricDisplay()
{
    this->image_list = nullptr;
    this->space = nullptr;
}

void IsometricDisplay::draw()
{
    int w = space->getWidth(); //this isn't going to change so i might as well use shorter names lol
    int d = space->getDepth();
    int h = space->getHeight();

    for (int ch = h-1; ch >= 0; ch--)
    {
        for (int cw = 0; cw < w; cw++)
        {
            for (int cd = 0; cd < d; cd++)
            {
                al_draw_bitmap(space->getImageFromLocation(cw,cd,ch),
                               200 + 32.0 * (cw - cd), 200 + 16.0 * (cw + cd) + 18.0 * ch, 0);
            }
        }
    }

}

void IsometricDisplay::setSpace(Space* space)
{
    this->space = space;
}


