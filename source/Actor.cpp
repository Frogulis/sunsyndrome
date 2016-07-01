#include "Actor.h"

class Actor::Animation
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

bool Actor::loadByName(std::string actor_name)
{
    ALLEGRO_BITMAP* sheet = nullptr; //the overall image from which we'll chop frames
    sheet = al_load_bitmap(("resources/sprites/actor/" + actor_name + "/spritesheet.png").c_str());
    if (!sheet)
    {
        return false;
    }

    std::ifstream ssinfo;
    ssinfo.open("resources/sprites/actor/" + actor_name + "/ssinfo");
    if (!ssinfo.is_open())
    {
        return false;
    }
    std::string sizes = "";
    std::getline(ssinfo, sizes);
    int x_size = JH::StringUtils::stringToInt(sizes.substr(0, sizes.find(',')));
    int y_size = JH::StringUtils::stringToInt(sizes.substr(sizes.find(','), sizes.length()));
    std::cout << x_size << " " << y_size;
    return true;
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
