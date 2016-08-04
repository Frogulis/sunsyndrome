#include "IsometricDisplay.h"

IsometricDisplay::IsometricDisplay()
{
    this->mode = 0;

    this->x = 200;
    this->y = 200;

    this->cursor_x = 0;
    this->cursor_y = 0;

    for (int i = 0; i < ALLEGRO_KEY_MAX; i++)
    {
        this->keys[i] = false;
    }
    this->mouse_button_state = 0;
    this->mouse_moved = false;
    this->cursor_move_step = 0;
    this->cursor_move_time = 7;
    this->cursor_moved = false;

    this->focused_unit = nullptr;
    this->focused_unit_pf = nullptr;
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
                this->drawCallWithCoords(space->getImageFromLocation(cw,cd,ch), cw, cd, ch);
                if (ch == this->space->getArenaHeight())
                {
                    for (unsigned int a = 0; a < this->objects.size(); a++)
                    {
                        if (this->objects[a].first.first == cw && this->objects[a].first.second == cd)
                        {
                            this->drawCallWithCoords(this->objects[a].second->getFrame(), cw, cd, this->space->getArenaHeight());
                        }
                    }
                    for (std::vector<CombatUnit*>::iterator ally = this->parties.first.begin(); ally != this->parties.first.end(); ally++)
                    {
                        if ((*ally)->getW() == cw && (*ally)->getD() == cd)
                        {
                            ALLEGRO_BITMAP* sprite = (*ally)->actor.getFrame();
                            if (sprite)
                            {
                                al_draw_bitmap(sprite,
                                               this->x + (*ally)->actor.getX() - al_get_bitmap_width(sprite)/2,
                                               this->y + (*ally)->actor.getY() + 18.0 * this->space->getArenaHeight() - al_get_bitmap_height(sprite)/2,
                                               0);
                            }
                        }
                    }
                    for (std::vector<CombatUnit*>::iterator enemy = this->parties.second.begin(); enemy != this->parties.second.end(); enemy++)
                    {
                        if ((*enemy)->getW() == cw && (*enemy)->getD() == cd)
                        {
                            ALLEGRO_BITMAP* sprite = (*enemy)->actor.getFrame();
                            if (sprite)
                            {
                                al_draw_bitmap(sprite,
                                               this->x + (*enemy)->actor.getX() - al_get_bitmap_width(sprite)/2,
                                               this->y + (*enemy)->actor.getY() + 18.0 * this->space->getArenaHeight() - al_get_bitmap_height(sprite)/2,
                                               0);
                            }
                        }
                    }
                }
            }
        }
    }

    if (mode == 2)
    {
        if (this->focused_unit_pf)
        {
            std::vector<std::pair<int,int> >* pf_result = &this->focused_unit_pf->getPath();
            if (pf_result->size() == 0)
            {
                this->cur_cursor = this->cursors.get("red");
            }
            else
            {
                for (std::vector<std::pair<int,int> >::reverse_iterator tile = pf_result->rbegin(); tile != pf_result->rend(); tile++)
                {
                    ALLEGRO_BITMAP* cursor_to_use;
                    if (tile + 1 == pf_result->rend()) //last element
                    {
                        cursor_to_use = this->cursors.get("lgreen");
                    }
                    else
                    {
                        cursor_to_use = this->cursors.get("white");
                    }
                    this->drawCallWithCoords(cursor_to_use, tile->first, tile->second, this->space->getArenaHeight());
                }
            }
        }
    }

    this->drawCallWithCoords(this->cur_cursor, this->cursor_x, this->cursor_y, this->space->getArenaHeight());
}

void IsometricDisplay::drawCallWithCoords(ALLEGRO_BITMAP* sprite, float w, float d, float h)
{
    if (sprite)
    {
        al_draw_bitmap(sprite,
                       this->x + 32.0 * (w - d) - al_get_bitmap_width(sprite)/2,
                       this->y + 16.0 * (w + d) + 18.0 * h - al_get_bitmap_height(sprite)/2,
                       0);
    }
}

bool IsometricDisplay::load(std::string name)
{
    Space* s = new Space;
    if (!s->loadFromFile(name))
    {
        return false;
    }
    this->space.reset(s);
    for (int i = 0; i < 2; i++)
    {
        CombatUnit* temp = CombatUnit::getInstance("shadowman");
        temp->setW(1);
        temp->setD(4-i);
        temp->setH(this->space->getArenaHeight());
        temp->alignActorToCoords();
        this->parties.first.push_back(temp);
    }
    CombatUnit* temp2 = CombatUnit::getInstance("shadowman");
    temp2->setW(3);
    temp2->setD(4);
    temp2->alignActorToCoords();
    this->parties.second.push_back(temp2);

    if (!this->loadObjects(name))
    {
        return false;
    }

    ALLEGRO_BITMAP* red_c = al_load_bitmap("resources/sprites/UI/cursor/cursor_red.png");
    ALLEGRO_BITMAP* blue_c = al_load_bitmap("resources/sprites/UI/cursor/cursor_blue.png");
    ALLEGRO_BITMAP* lred_c = al_load_bitmap("resources/sprites/UI/cursor/cursor_lred.png");
    ALLEGRO_BITMAP* yellow_c = al_load_bitmap("resources/sprites/UI/cursor/cursor_yellow.png");
    ALLEGRO_BITMAP* green_c = al_load_bitmap("resources/sprites/UI/cursor/cursor_green.png");
    ALLEGRO_BITMAP* white_c = al_load_bitmap("resources/sprites/UI/cursor/cursor_white.png");
    ALLEGRO_BITMAP* lgreen_c = al_load_bitmap("resources/sprites/UI/cursor/cursor_lgreen.png");

    if (red_c && blue_c && lred_c && yellow_c && green_c && white_c && lgreen_c)
    {
        this->cursors.add("red", red_c);
        this->cursors.add("blue", blue_c);
        this->cursors.add("lred", lred_c);
        this->cursors.add("yellow", yellow_c);
        this->cursors.add("green", green_c);
        this->cursors.add("white", white_c);
        this->cursors.add("lgreen", lgreen_c);
    }
    else
    {
        std::cout << "Cursor images not found.\n";
        al_destroy_bitmap(red_c);
        al_destroy_bitmap(blue_c);
        al_destroy_bitmap(lred_c);
        al_destroy_bitmap(yellow_c);
        al_destroy_bitmap(green_c);
        al_destroy_bitmap(white_c);
        al_destroy_bitmap(lgreen_c);
        return false;
    }
    this->setCursorColour();

    return true;
}

void IsometricDisplay::setSpace(Space* space)
{
    this->space.reset(space);
}

void IsometricDisplay::setCursorMoveTime(int value)
{
    this->cursor_move_time = value;
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

void IsometricDisplay::runEvents(ALLEGRO_EVENT &ev)
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
    else if (ev.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        this->mouse_moved = true;
        this->mouse_state.x = ev.mouse.x;
        this->mouse_state.y = ev.mouse.y;
    }
}

void IsometricDisplay::runLogic()
{
    if (this->mouse_moved)
    {
        this->mouse_moved = false;
        this->cursor_y = std::round((this->mouse_state.y - this->y - ((this->mouse_state.x - this->x) / 2) - 18.0 * this->space->getArenaHeight()) / 32);
        this->cursor_x = std::round((this->mouse_state.y - this->y  - 18.0 * this->space->getArenaHeight()) / 16 - this->cursor_y);
        setCursorColour();
        this->callAStar(std::pair<int,int>(this->cursor_x, this->cursor_y));
    }
    if (this->keys[ALLEGRO_KEY_W])
    {
        this->changeOffset(0, -5);
    }
    if (this->keys[ALLEGRO_KEY_S])
    {
        this->changeOffset(0, 5);
    }
    if (this->keys[ALLEGRO_KEY_A])
    {
        this->changeOffset(-5, 0);
    }
    if (this->keys[ALLEGRO_KEY_D])
    {
        this->changeOffset(5, 0);
    }
    if (cursor_move_step > cursor_move_time)
    {
        if (this->keys[ALLEGRO_KEY_UP])
        {
            this->cursor_y--;
            cursor_move_step = 0;
            setCursorColour();
            this->callAStar(std::pair<int,int>(this->cursor_x, this->cursor_y));
        }
        if (this->keys[ALLEGRO_KEY_DOWN])
        {
            this->cursor_y++;
            cursor_move_step = 0;
            setCursorColour();
            this->callAStar(std::pair<int,int>(this->cursor_x, this->cursor_y));
        }
        if (this->keys[ALLEGRO_KEY_LEFT])
        {
            this->cursor_x--;
            cursor_move_step = 0;
            setCursorColour();
            this->callAStar(std::pair<int,int>(this->cursor_x, this->cursor_y));
        }
        if (this->keys[ALLEGRO_KEY_RIGHT])
        {
            this->cursor_x++;
            cursor_move_step = 0;
            setCursorColour();
            this->callAStar(std::pair<int,int>(this->cursor_x, this->cursor_y));
        }
    }
    cursor_move_step++;
    if (this->keys[ALLEGRO_KEY_SPACE] || (this->mouse_button_state & 1))
    {
        this->mouse_button_state &= ~(1); //safe to reset these in this case
        this->keys[ALLEGRO_KEY_SPACE] = false;
        if (this->mode == 0)
        {
            int c_state = this->cursorOverUnit();
            if (c_state)
            {
                if (c_state == -1)
                {
                    for (std::vector<CombatUnit*>::iterator u = this->parties.first.begin(); u != this->parties.first.end(); u++)
                    {
                        if ((*u)->getW() == this->cursor_x && (*u)->getD() == this->cursor_y)
                        {
                            this->focused_unit = *u;
                            this->mode = 2;
                            if (this->focused_unit_pf)
                            {
                                delete this->focused_unit_pf;
                            }
                            this->focused_unit_pf = new JH::AStar(this->generateWalkableArrayFor(this->focused_unit), this->space->getWidth(), this->space->getDepth());
                            break;
                        }
                        else
                        {
                            this->focused_unit = nullptr;
                        }
                    }
                }
            }
        }
        else if (this->mode == 1)
        {
            //?
        }
        else if (this->mode == 2)
        {
            this->mode = 0;
        }
        this->setCursorColour();
    }
}

void IsometricDisplay::setCursorColour()
{
    if (this->mode == 0)
    {
        int cursor_unit = this->cursorOverUnit();
        if (cursor_unit == -1) //ally
        {
            this->cur_cursor = this->cursors.get("green");
        }
        else if (cursor_unit == 1) //enemy
        {
            this->cur_cursor = this->cursors.get("red");
        }
        else
        {
            this->cur_cursor = this->cursors.get("lred");
        }
    }
    else if (mode == 1)
    {
        this->cur_cursor = this->cursors.get("yellow");
    }
    else if (mode == 2)
    {
        if (this->cursor_x >= this->space->getWidth() || this->cursor_x < 0 ||
            this->cursor_y >= this->space->getDepth() || this->cursor_y < 0)
        {
            this->cur_cursor = this->cursors.get("red");
        }
        else
        {
            this->cur_cursor = this->cursors.get("lgreen");
        }
    }
}

bool IsometricDisplay::loadObjects(std::string name)
{
    name = "resources/levels/" + name + "/objects";
    std::ifstream f;
    f.open(name);
    if (!f)
    {
        std::cout << "Failed to load objects at " << name << ".\n";
        return false;
    }
    while (!f.eof())
    {
        Actor* temp = new Actor();
        std::string a_name;
        int x, y;
        if (!(f >> a_name >> x >> y))
        {
            while (f.get() != '\n' && !f.eof())
            {
                //boop
            }
        }
        if (f.eof())
        {
            return true;
        }
        temp->loadByName(a_name);
        temp->setX(32.0 * (x - y));
        temp->setY(16.0 * (x + y));
        temp->startAnimation("idle");
        this->objects.push_back(std::pair<std::pair<int,int>,Actor*>(std::pair<int,int>(x,y), temp));
    }
    return true;
}

void IsometricDisplay::callAStar(std::pair<int,int> goal)
{
    if (this->focused_unit &&
        goal.first >= 0 && goal.first < this->space->getWidth() &&
        goal.second >= 0 && goal.second < this->space->getWidth())
    {
        this->focused_unit_pf->generatePath(std::pair<int,int>(this->focused_unit->getW(),this->focused_unit->getD()), goal);
    }
}

int IsometricDisplay::cursorOverUnit()
{
    for (std::vector<CombatUnit*>::iterator i = this->parties.first.begin(); i != this->parties.first.end(); i++)
    {
        if (this->cursor_x == (*i)->getW() && this->cursor_y == (*i)->getD())
        {
            return -1;
        }
    }
    for (std::vector<CombatUnit*>::iterator i = this->parties.second.begin(); i != this->parties.second.end(); i++)
    {
        if (this->cursor_x == (*i)->getW() && this->cursor_y == (*i)->getD())
        {
            return 1;
        }
    }
    return 0;
}

bool** IsometricDisplay::generateWalkableArrayFor(IsometricDisplay::CombatUnit* unit)
{
    if (!space)
    {
        std::cout << "Failed to generate walkability array without a space.\n";
        return nullptr;
    }
    bool** result = new bool*[this->space->getWidth()];
    for (int r_i = 0; r_i < this->space->getWidth(); r_i++)
    {
        result[r_i] = new bool[this->space->getDepth()];
        for (int r_j = 0; r_j < this->space->getDepth(); r_j++)
        {
            result[r_i][r_j] = true;
        }
    }
    for (int i = 0; i < this->space->getWidth(); i++)
    {
        for (int j = 0; j < this->space->getDepth(); j++)
        {
            result[i][j] = this->space->getWalkableFromLocation(i, j, this->space->getArenaHeight());
        }
    }
    for (std::vector<CombatUnit*>::iterator i = this->parties.first.begin(); i != this->parties.first.end(); i++)
    {
        if ((*i)->getW() != unit->getW() || (*i)->getD() != unit->getD()) //i.e. in a different spot i.e. different unit
        {
            result[(*i)->getW()][(*i)->getD()] = false;
        }
    }
    for (std::vector<CombatUnit*>::iterator i = this->parties.second.begin(); i != this->parties.second.end(); i++)
    {
        if ((*i)->getW() != unit->getW() || (*i)->getD() != unit->getD())
        {
            result[(*i)->getW()][(*i)->getD()] = false;
        }
    }
    for (unsigned int i = 0; i < this->objects.size(); i++)
    {
        result[this->objects[i].first.first][this->objects[i].first.second] = false;
    }
    return result;
}

//Buff definitions
IsometricDisplay::CombatUnit::Buff::Buff(int length)
{
    this->turns_remaining = length;
};

void IsometricDisplay::CombatUnit::Buff::initEffect(CombatUnit* user)
{
    //do nothing by default
}

void IsometricDisplay::CombatUnit::Buff::turnEffect(CombatUnit* user)
{
    //do nothing by default
}

void IsometricDisplay::CombatUnit::Buff::endEffect(CombatUnit* user)
{
    //do nothing by default
}

void IsometricDisplay::CombatUnit::Buff::takeTurn()
{
    this->turns_remaining--;
}

bool IsometricDisplay::CombatUnit::Buff::toKeep()
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

std::string IsometricDisplay::CombatUnit::Buff::getName()
{
    return this->name;
}

std::string IsometricDisplay::CombatUnit::Buff::getDesc()
{
    return this->desc;
}

class Buff_Lower2Defence : public IsometricDisplay::CombatUnit::Buff
{
public:
    Buff_Lower2Defence(int length) : IsometricDisplay::CombatUnit::Buff(length)
    {
        this->name = "Minor Acid.";
        this->desc = "Lowers defence by 2.";
    }
    void initEffect(IsometricDisplay::CombatUnit* user)
    {
        user->setStat("defence", user->getStat("defence") - this->strength);
    }
    void endEffect(IsometricDisplay::CombatUnit* user)
    {
        user->setStat("defence", user->getStat("defence") + this->strength);
    }
protected:
    const float strength = 2;
};

//SkillFunctor definitions

IsometricDisplay::CombatUnit::SkillFunctor::SkillFunctor()
{
    this->name = "";
    this->desc = "";
}

void IsometricDisplay::CombatUnit::SkillFunctor::operator()(IsometricDisplay::CombatUnit* user, IsometricDisplay::CombatUnit* target)
{
    this->call(user, target);
}

void IsometricDisplay::CombatUnit::SkillFunctor::call(IsometricDisplay::CombatUnit* user, IsometricDisplay::CombatUnit* target)
{
    this->func(user, target);
}

std::string IsometricDisplay::CombatUnit::SkillFunctor::getName()
{
    return this->name;
}

std::string IsometricDisplay::CombatUnit::SkillFunctor::getDesc()
{
    return this->desc;
}

class BasicAttack : public IsometricDisplay::CombatUnit::SkillFunctor
{
    /***
    The target takes the user's damage stat as damage. Simples.
    ***/
public:
    BasicAttack() : IsometricDisplay::CombatUnit::SkillFunctor::SkillFunctor()
    {
        this->name = "Basic attack.";
        this->desc = "Does the unit's damage stat as damage to the target.";
    }
protected:
    void func(IsometricDisplay::CombatUnit* user, IsometricDisplay::CombatUnit* target)
    {
        target->setStat("hp", target->getStat("hp") - user->getStat("damage"));
    }
};

class CumulativeAttack : public IsometricDisplay::CombatUnit::SkillFunctor
{
    /***
    Does increasing damage with each hit on the same target.
    ***/
public:
    CumulativeAttack()
    {
        this->name = "Cumulative Attack";
        this->desc = "Does increasing damage with each hit on the same target.";
        this->cur_target = nullptr;
        this->consec_hits = 1;
    }
protected:
    void func(IsometricDisplay::CombatUnit* user, IsometricDisplay::CombatUnit* target)
    {
        if (this->cur_target != target)
        {
            this->cur_target = target;
            this->consec_hits = 1;
        }
        target->setStat("hp", target->getStat("hp") - (user->getStat("damage") * this->consec_hits * 0.5));
        this->consec_hits++;
    }
    IsometricDisplay::CombatUnit* cur_target;
    int consec_hits;
};

class RiskyAttack : public IsometricDisplay::CombatUnit::SkillFunctor
{
    /***
    An attack that does high damage but applies a defence-lowering buff for one turn.
    ***/
public:
    RiskyAttack()
    {
        this->name = "Big Swing";
        this->desc = "Does high damage, but leaves the attack wide open, lowering defence by 2.";
    }
protected:
    void func(IsometricDisplay::CombatUnit* user, IsometricDisplay::CombatUnit* target)
    {
        target->setStat("hp", target->getStat("hp") - (user->getStat("damage") * 2));
        user->applyBuff(new Buff_Lower2Defence(1));
    }
};

//CombatUnit definitions
IsometricDisplay::CombatUnit::CombatUnit()
{
    this->w = 0;
    this->d = 0;
    this->h = 0;
    this->stats.add("damage", 10);
    this->stats.add("hp", 10);
    this->stats.add("defence", 10);
}

IsometricDisplay::CombatUnit::~CombatUnit()
{
    for (std::vector<IsometricDisplay::CombatUnit::SkillFunctor*>::iterator i = this->skills.begin(); i != skills.end(); i++)
    {
        delete *i;
    }
    for (std::vector<Buff*>::iterator i = this->buffs.begin(); i != buffs.end(); i++)
    {
        delete *i;
    }
}

void IsometricDisplay::CombatUnit::setW(int w)
{
    this->w = w;
}

void IsometricDisplay::CombatUnit::setD(int d)
{
    this->d = d;
}

void IsometricDisplay::CombatUnit::setH(int h)
{
    this->h = h;
}

int IsometricDisplay::CombatUnit::getW()
{
    return this->w;
}

int IsometricDisplay::CombatUnit::getD()
{
    return this->d;
}

int IsometricDisplay::CombatUnit::getH()
{
    return this->h;
}

void IsometricDisplay::CombatUnit::setStat(std::string stat, float value)
{
    stats.add(stat, value);
}

float IsometricDisplay::CombatUnit::getStat(std::string stat)
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

void IsometricDisplay::CombatUnit::applyBuff(Buff* b)
{
    b->initEffect(this);
    this->buffs.push_back(b);
}

void IsometricDisplay::CombatUnit::useSkill(unsigned int number, IsometricDisplay::CombatUnit* target)
{
    if (number > this->skills.size())
    {
        number = 0;
    }
    this->skills[number]->call(this, target);
}

void IsometricDisplay::CombatUnit::takeTurn()
{
    for (std::vector<IsometricDisplay::CombatUnit::Buff*>::iterator i = this->buffs.begin(); i < this->buffs.end(); i++)
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

void IsometricDisplay::CombatUnit::alignActorToCoords()
{
    this->actor.setXYFromCoords(this->w, this->d);
}

class Unit_Shadowman : public IsometricDisplay::CombatUnit
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

class Unit_Grunt : public IsometricDisplay::CombatUnit
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

IsometricDisplay::CombatUnit* IsometricDisplay::CombatUnit::getInstance(std::string unit_class)
{
    if (unit_class == "shadowman")
    {
        IsometricDisplay::CombatUnit* temp = new Unit_Shadowman;
        temp->actor.loadByName("shadowman");
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

