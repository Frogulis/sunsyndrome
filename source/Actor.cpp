#include "Actor.h"

class Actor::Animation
{
public:
    Animation()
    {
        this->cur_frame = 0;
        this->cur_time = 0;
    }
    ~Animation()
    {
        for (std::vector<ALLEGRO_BITMAP*>::iterator i = this->frames.begin(); i != this->frames.end(); i++)
        {
            al_destroy_bitmap(*i);
        }
    }
    void setAnimation(std::vector<int> time_per_frame, std::vector<ALLEGRO_BITMAP*> frames)
    {
        this->time_per_frame = time_per_frame;
        this->frames = frames;
    }
    void startAnimation()
    {
        this->cur_time = 0;
        this->cur_frame = 0;
    }
    ALLEGRO_BITMAP* getFrame()
    {
        if (this->cur_time == this->time_per_frame[this->cur_frame])
        {
            if (this->cur_frame == this->frames.size() - 1) //last frame
            {
                this->startAnimation();
                throw std::exception();
            }
            this->cur_frame++;
            this->cur_time = 0;
        }
        this->cur_time++;
        return this->frames[this->cur_frame];
    }
private:
    int cur_time;
    int cur_frame;
    std::vector<int> time_per_frame;
    std::vector<ALLEGRO_BITMAP*> frames;
};

Actor::Actor()
{
    this->cur_animation = nullptr;
    this->def_animation = nullptr;
    this->x_off = 0;
    this->y_off = 0;
    this->x = 0;
    this->y = 0;
}

Actor::~Actor()
{

}

bool Actor::loadByName(std::string actor_name)
{
    ALLEGRO_BITMAP* sheet = nullptr; //the overall image from which we'll chop frames
    sheet = al_load_bitmap(("resources/sprites/actors/" + actor_name + "/spritesheet.png").c_str());
    if (!sheet)
    {
        std::cout << "Can't load spritesheet for " << actor_name << ".\n";
        return false;
    }

    std::ifstream ssinfo;
    ssinfo.open("resources/sprites/actors/" + actor_name + "/ssinfo");
    if (!ssinfo.is_open())
    {
        std::cout << "Can't load ssinfo for " << actor_name << ".\n";
        al_destroy_bitmap(sheet);
        return false;
    }

    JH::SSInfoTokenizer tokenizer;
    tokenizer.input(ssinfo);
    tokenizer.parse();
    std::vector<JH::Tokenizer::Token> tokens = tokenizer.getTokens();

    float x_size, y_size;

    std::string name;
    std::vector<std::pair<int,int>> coords;
    std::vector<int> time_per_frame;
    int stage = 0;
    std::pair<int,int> buffer;
    char a_or_b = 0; //0 = a 1 = b this is inelegant leave me alone :c
    std::vector<JH::Tokenizer::Token>::iterator bookmark;

    x_size = JH::StringUtils::stringToInt(tokens[0].content);
    y_size = JH::StringUtils::stringToInt(tokens[2].content);

    for (std::vector<JH::Tokenizer::Token>::iterator i = tokens.begin() + 4; i != tokens.end(); i++)
    {
        //std::cout << "~" << i->type << " " << i->content << "\n";
        if (i->type == "next_section")
        {
            stage++;
        }
        else if (i->type == "newline")
        {
            stage = 0;
            if (coords.size() != time_per_frame.size())
            {

                std::cout << coords.size() << "-" << time_per_frame.size() << ":\n";
                std::cout << "Number of frames and times don't match in ssinfo for " << actor_name << ": " << name << ".\n";
                al_destroy_bitmap(sheet);
                return false;
            }
            Animation* temp = new Animation;
            std::vector<ALLEGRO_BITMAP*> frames;
            for (std::vector<std::pair<int,int>>::iterator i = coords.begin(); i != coords.end(); i++)
            {
                frames.push_back(al_clone_bitmap(al_create_sub_bitmap(sheet, i->first * x_size, i->second * y_size, x_size, y_size)));
            }
            temp->setAnimation(time_per_frame, frames);
            this->animations.add(name, temp);
            coords.clear();
            time_per_frame.clear();
            name.clear();
            continue;
        }
        if (stage == 0) //name
        {
            if (i->type != "word")
            {
                std::cout << "Improperly formatted ssinfo for " << actor_name << ".\n";
                al_destroy_bitmap(sheet);
                return false;
            }
            else
            {
                name = i->content;
            }
        }
        else if (stage == 1) //coords
        {
            if (i->type == "number")
            {
                if (a_or_b == 0)
                {
                    buffer.first = JH::StringUtils::stringToInt(i->content);
                    a_or_b = 1;
                }
                else if (a_or_b == 1)
                {
                    buffer.second = JH::StringUtils::stringToInt(i->content);
                    a_or_b = 0;
                    coords.push_back(std::pair<int,int>(buffer));
                }
            }
        }
        else if (stage == 2) //
        {
            if (i->type == "number")
            {
                time_per_frame.push_back(JH::StringUtils::stringToInt(i->content));
            }
        }

    }
    al_destroy_bitmap(sheet);
    try
    {
        this->def_animation = this->animations.get("idle");
    }
    catch (JH::HTElementNotFoundException &e)
    {
        std::cout << e.what() << "\n";
        std::cout << "No idle animation exists for " << actor_name << "\n";
        return false;
    }
    return true;
}

void Actor::startAnimation(std::string anim_name)
{
    try
    {
        this->cur_animation = this->animations.get(anim_name);
        this->cur_animation->startAnimation();
    }
    catch (JH::HTElementNotFoundException e)
    {
        std::cout << e.what() << "\n";
        std::cout << anim_name << " is not a valid animation name\n";
    }
}

void Actor::setDefaultAnimation(std::string anim_name)
{
    try
    {
        this->def_animation = this->animations.get(anim_name);
    }
    catch (JH::HTElementNotFoundException &e)
    {
        std::cout << e.what() << "\n";
        std::cout << anim_name << " is not a valid animation name\n";
        this->def_animation = this->animations.get("idle");
    }
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
        this->cur_animation = this->def_animation;
        this->cur_animation->startAnimation();
        frame = this->cur_animation->getFrame();
    }
    return frame;
}

float Actor::getXOffset()
{
    return this->x_off;
}

float Actor::getYOffset()
{
    return this->y_off;
}

float Actor::getX()
{
    return this->x;
}

float Actor::getY()
{
    return this->y;
}

void Actor::setX(float x)
{
    this->x = x;
}

void Actor::setY(float y)
{
    this->y = y;
}
