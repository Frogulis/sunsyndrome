#include "TopDownDisplay.h"

#include <iostream>

TopDownDisplay::TopDownDisplay()
{
    std::cout << "making this\n";
}

void TopDownDisplay::draw()
{
    for (int i = 0; i < this->image_list->getLength(); i++)
    {
        al_draw_bitmap(image_list->getImageFromID(i), i*64, 0, 0);
    }
}

void TopDownDisplay::setImageList(ImageList* il)
{
    this->image_list = il;
}

void TopDownDisplay::setSpace(Space* space)
{
    //
}
