#ifndef __ACTORH_GUARD
#define __ACTORH_GUARD

#include <exception>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <queue>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "HashTable.h"
#include "SSInfoTokenizer.h"
#include "StringUtils.h"

class Actor
{
public:
    Actor();
    Actor(Actor& a);
    ~Actor();
    bool loadByName(std::string actor_name);
    void startAnimation(std::string anim_name);
    ALLEGRO_BITMAP* getFrame();
    float getYOffset();
    float getXOffset();
    float getX();
    float getY();
    void setX(float x);
    void setY(float y);
    void setXYFromCoords(int x, int y);
    void setDefaultAnimation(std::string anim_name);
private:
    class Animation;
    JH::HashTable<std::string, Animation*> animations; //hash table of 'animation' objects
    Animation* cur_animation;
    Animation* def_animation;
    float x_off, y_off;
    float x, y;
};

#endif // __ACTORH_GUARD
