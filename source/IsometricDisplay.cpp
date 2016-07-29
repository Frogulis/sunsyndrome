#include "IsometricDisplay.h"

IsometricDisplay::IsometricDisplay()
{
    this->x = 200;
    this->y = 200;
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
                ALLEGRO_BITMAP* cur = space->getImageFromLocation(cw,cd,ch);
                if (cur)
                {
                    al_draw_bitmap(cur,
                                   this->x + 32.0 * (cw - cd) - al_get_bitmap_width(cur)/2,
                                   this->y + 16.0 * (cw + cd) + 18.0 * ch - al_get_bitmap_height(cur)/2,
                                   0);
                }
            }
        }
    }

}

void IsometricDisplay::setSpace(Space* space)
{
    this->space.reset(space);
}

std::shared_ptr<Space> IsometricDisplay::getSpacePtr()
{
    return this->space;
}

void IsometricDisplay::changeOffset(float x, float y)
{
    this->x += x;
    this->y += y;
}

