#ifndef __ACTORH_GUARD
#define __ACTORH_GUARD

#include <exception>
#include <iostream>
#include <string>
#include <fstream>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "HashTable.h"
#include "StringUtils.h"

class Actor
{
public:
    Actor();
    bool loadByName(std::string actor_name);
    ALLEGRO_BITMAP* getFrame();
    float getYOffset();
    float getXOffset();
private:
    class Animation;
    JH::HashTable<std::string, Animation*> animations; //hash table of 'animation' objects
    Animation* cur_animation;
    float x_off, y_off;
};

#endif // __ACTORH_GUARD
