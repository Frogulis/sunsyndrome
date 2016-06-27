#include "Actor.h"

class Animation
{
public:
    Animation()
    {
        this->cur_frame = 0;
        this->cur_time = 0;
        this->time_per_frame = nullptr;
        this->frames = nullptr;
    }
    void setAnimation(int nof, int* time_per_frame, ALLEGRO_BITMAP** frames)
    {
        this->number_of_frames = nof;
        this->time_per_frame = time_per_frame;
        this->frames = frames;
    }
    void startAnimation()
    {
        cur_time = 0;
        cur_frame = 0;
    }
    ALLEGRO_BITMAP* getFrame()
    {
        if (cur_time == time_per_frame[cur_frame])
        {
            if (cur_frame == number_of_frames - 1) //last frame
            {
                this->startAnimation();
                throw std::exception();
            }
            cur_frame++;
            cur_time = 0;
        }
        return frames[cur_frame];
    }
private:
    int cur_time;
    int cur_frame;
    int* time_per_frame;
    int number_of_frames;
    ALLEGRO_BITMAP** frames;
};

Actor::Actor()
{
    this->cur_animation = nullptr;
    this->x_off = 0;
    this->y_off = 0;
}

ALLEGRO_BITMAP* Actor::getFrame()
{
    ALLEGRO_BITMAP* frame = nullptr;
    try
    {
        frame = this->cur_animation->getFrame();
    }
    catch (std::exception& e)
    {
        this->cur_animation = this->animations.get("idle");
        this->cur_animation->startAnimation();
        frame = this->cur_animation->getFrame();
    }
    return frame;
}
