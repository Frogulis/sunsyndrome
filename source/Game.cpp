#include "Game.h"

Game::Game()
{
    this->loadIsoDisplay("practice");
    for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
    {
        this->keys[i] = false;
    }
    this->mouse_button_state = 0;
}

/*void Game::loadMenuDisplay(std::string menu_name)
{
    if (menu_name == "main")
    {
        MenuDisplay* result = new MenuDisplay;
        this->cur_menu_display = result;
    }
    else
    {
        std::cout << "Invalid menu name: " << menu_name << ".\n";
    }
}*/

bool Game::loadArea(std::string area_name)
{
    if (!this->loadIsoDisplay(area_name))
    {
        return false;
    }
    return true;
}

bool Game::loadIsoDisplay(std::string area_name)
{
    if (!this->cur_isometric_display)
    {
        this->cur_isometric_display.reset(new IsometricDisplay);
        Space* s = new Space;
        if (!s->loadFromFile(area_name))
        {
            return false;
        }
        this->cur_isometric_display->setSpace(s);
        this->cur_map.reset(s);
    }
    this->drawer = this->cur_isometric_display;
    this->mode = 1;
    return true;
}

void Game::runEvents(ALLEGRO_EVENT &ev)
{
    if (ev.type == ALLEGRO_EVENT_KEY_UP)
    {
        this->keys[ev.keyboard.keycode] = false;
    }
    else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        this->keys[ev.keyboard.keycode] = true;
    }
    else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
        this->mouse_button_state |= 1 << (ev.mouse.button - 1);
    }
    else if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
    {
        this->mouse_button_state &= ~(1 << (ev.mouse.button - 1));
    }
}

void Game::runLogic()
{
    if (mode == 0)
    {

    }
    else if (mode == 1)
    {
        if (this->keys[ALLEGRO_KEY_UP])
        {
            this->drawer->changeOffset(0, -5);
        }
        if (this->keys[ALLEGRO_KEY_DOWN])
        {
            this->drawer->changeOffset(0, 5);
        }
        if (this->keys[ALLEGRO_KEY_LEFT])
        {
            this->drawer->changeOffset(-5, 0);
        }
        if (this->keys[ALLEGRO_KEY_RIGHT])
        {
            this->drawer->changeOffset(5, 0);
        }
    }
}

void Game::runDisplay()
{
    al_clear_to_color(al_map_rgb(0,0,0));
    this->drawer->draw();
}

//Buff definitions
Game::CombatUnit::Buff::Buff(int length)
{
    this->turns_remaining = length;
};

void Game::CombatUnit::Buff::initEffect(CombatUnit* user)
{
    //do nothing by default
}

void Game::CombatUnit::Buff::turnEffect(CombatUnit* user)
{
    //do nothing by default
}

void Game::CombatUnit::Buff::endEffect(CombatUnit* user)
{
    //do nothing by default
}

void Game::CombatUnit::Buff::takeTurn()
{
    this->turns_remaining--;
}

bool Game::CombatUnit::Buff::toKeep()
{
    if (this->turns_remaining <= 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void Game::CombatUnit::Buff::setName(std::string name)
{
    this->name = name;
}

std::string Game::CombatUnit::Buff::getName()
{
    return this->name;
}

class Buff_Lower2Defence : public Game::CombatUnit::Buff
{
public:
    Buff_Lower2Defence(int length) : Game::CombatUnit::Buff(length)
    {
        this->name = "Lowers defence by 2."; //this should go in description field
    }
    void initEffect(Game::CombatUnit* user)
    {
        user->setStat("defence", user->getStat("defence") - this->strength);
    }
    void endEffect(Game::CombatUnit* user)
    {
        user->setStat("defence", user->getStat("defence") + this->strength);
    }
protected:
    const float strength = 2;
};

//SkillFunctor definitions

void Game::CombatUnit::SkillFunctor::operator()(Game::CombatUnit* user, Game::CombatUnit* target)
{
    this->call(user, target);
}

void Game::CombatUnit::SkillFunctor::call(Game::CombatUnit* user, Game::CombatUnit* target)
{
    this->func(user, target);
}

void Game::CombatUnit::SkillFunctor::setName(std::string name)
{
    this->name = name;
}

std::string Game::CombatUnit::SkillFunctor::getName()
{
    return this->name;
}

class BasicAttack : public Game::CombatUnit::SkillFunctor
{
    /***
    The target takes the user's damage stat as damage. Simples.
    ***/
protected:
    void func(Game::CombatUnit* user, Game::CombatUnit* target)
    {
        target->setStat("hp", target->getStat("hp") - user->getStat("damage"));
    }
};

class CumulativeAttack : public Game::CombatUnit::SkillFunctor
{
    /***
    Does increasing damage with each hit on the same target.
    ***/
public:
    CumulativeAttack()
    {
        this->cur_target = nullptr;
        this->consec_hits = 1;
    }
protected:
    void func(Game::CombatUnit* user, Game::CombatUnit* target)
    {
        if (this->cur_target != target)
        {
            this->cur_target = target;
            this->consec_hits = 1;
        }
        target->setStat("hp", target->getStat("hp") - (user->getStat("damage") * this->consec_hits * 0.5));
        this->consec_hits++;
    }
    Game::CombatUnit* cur_target;
    int consec_hits;
};

class RiskyAttack : public Game::CombatUnit::SkillFunctor
{
    /***
    An attack that does high damage but applies a defence-lowering buff for one turn.
    ***/
public:
    void func(Game::CombatUnit* user, Game::CombatUnit* target)
    {
        target->setStat("hp", target->getStat("hp") - (user->getStat("damage") * 2));
        user->applyBuff(new Buff_Lower2Defence(1));
    }
};

//CombatUnit definitions
Game::CombatUnit::CombatUnit()
{
    this->w = 0;
    this->d = 0;
    this->h = 0;
    this->stats.add("damage", 10);
    this->stats.add("hp", 10);
    this->stats.add("defence", 10);
}

Game::CombatUnit::~CombatUnit()
{
    for (std::vector<Game::CombatUnit::SkillFunctor*>::iterator i = this->skills.begin(); i != skills.end(); i++)
    {
        delete *i;
    }
    for (std::vector<Buff*>::iterator i = this->buffs.begin(); i != buffs.end(); i++)
    {
        delete *i;
    }
}

void Game::CombatUnit::setW(int w)
{
    this->w = w;
}

void Game::CombatUnit::setD(int d)
{
    this->d = d;
}

void Game::CombatUnit::setH(int h)
{
    this->h = h;
}

int Game::CombatUnit::getW()
{
    return this->w;
}

int Game::CombatUnit::getD()
{
    return this->d;
}

int Game::CombatUnit::getH()
{
    return this->h;
}

void Game::CombatUnit::setStat(std::string stat, float value)
{
    stats.add(stat, value);
}

float Game::CombatUnit::getStat(std::string stat)
{
    float result;
    try
    {
        result = stats.get(stat);
    }
    catch (JH::HTElementNotFoundException &e)
    {
        std::cout << e.what();
        result = 0;
    }
    return result;
}

void Game::CombatUnit::applyBuff(Buff* b)
{
    b->initEffect(this);
    this->buffs.push_back(b);
}

void Game::CombatUnit::useSkill(unsigned int number, Game::CombatUnit* target)
{
    if (number > this->skills.size())
    {
        number = 0;
    }
    this->skills[number]->call(this, target);
}

void Game::CombatUnit::takeTurn()
{
    for (std::vector<Game::CombatUnit::Buff*>::iterator i = this->buffs.begin(); i < this->buffs.end(); i++)
    {
        (*i)->turnEffect(this); //lol double dereference ew
        (*i)->takeTurn();
        if (!(*i)->toKeep())
        {
            (*i)->endEffect(this);
            delete *i;
            this->buffs.erase(i);
        }
    }
}

class Unit_Shadowman : public Game::CombatUnit
{
    ///does increasing damage with further hits on the same target, benefits a 'pursuer' playstyle
public:
    Unit_Shadowman()
    {
        this->stats.add("hp", 50);
        this->stats.add("damage", 5);
        this->stats.add("defence", 5);
        this->skills.push_back(new CumulativeAttack);
        this->skills.push_back(new RiskyAttack);
    }
};

class Unit_Grunt : public Game::CombatUnit
{
    ///boring fat man class. punches stuff.
public:
    Unit_Grunt()
    {
        this->stats.add("hp", 50);
        this->stats.add("damage", 5);
        this->stats.add("defence", 5);
        this->skills.push_back(new RiskyAttack);
    }
};

Game::CombatUnit* Game::CombatUnit::getInstance(std::string unit_class)
{
    if (unit_class == "shadowman")
    {
        Game::CombatUnit* temp = new Unit_Shadowman;
        return temp;
    }
    else if (unit_class == "grunt")
    {
        return new Unit_Grunt;
    }
    else
    {
        //Come up with a suitable default/error class...?
        return new Unit_Shadowman;
    }
}
