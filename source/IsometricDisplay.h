#ifndef __ISOMETRICDISPLAYH_GUARD
#define __ISOMETRICDISPLAYH_GUARD

#include <iostream>
#include <memory>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>

#include "IDisplay.h"
#include "Space.h"
#include "HashTable.h"
#include "AStar.h"

class IsometricDisplay : public IDisplay
{
public:
    class CombatUnit
    {
    public:
        class SkillFunctor
        {
        public:
            SkillFunctor();
            void operator()(CombatUnit* user, CombatUnit* target); //pass this to user
            void call(CombatUnit* user, CombatUnit* target);
            void setName(std::string name);
            std::string getName();
            std::string getDesc();
        protected:
            virtual void func(CombatUnit* user, CombatUnit* target) = 0;
            std::string name;
            std::string desc;
        };
        class Buff
        {
        public:
            Buff(int length);
            virtual void initEffect(CombatUnit* user);
            virtual void turnEffect(CombatUnit* user);
            virtual void endEffect(CombatUnit* user);
            void takeTurn(); //decrements turn counter
            bool toKeep();
            std::string getName();
            std::string getDesc();
        protected:
            int turns_remaining;
            std::string name;
            std::string desc;
            //dispellabe?
        };
        static CombatUnit* getInstance(std::string unit_class);
        ~CombatUnit();
        void setW(int w);
        void setD(int d);
        void setH(int h);
        int getW();
        int getD();
        int getH();
        void setStat(std::string stat, float value);
        float getStat(std::string stat);
        void applyBuff(Buff* b);
        void useSkill(unsigned int number, CombatUnit* target);
        void takeTurn(); //applies buffs etc.
    protected:
        CombatUnit();
        int w, d, h;
        JH::HashTable<std::string, float> stats;
        std::vector<SkillFunctor*> skills; //0 is always basic attack
        std::vector<Buff*> buffs;
    };
    struct ArenaObject
    {
        CombatUnit* unit;
    };
    IsometricDisplay();
    void draw();
    bool load(std::string name);
    void setSpace(Space* space);
    void setCursorMoveTime(int value);
    std::shared_ptr<Space> getSpacePtr();
    void changeOffset(float x, float y);
    void runEvents(ALLEGRO_EVENT &ev);
    void runLogic();
private:
    int mode; //0 = out of unit, 1 = move select, 2 = target select, 3 = animation (no inputs)
    bool** generateWalkableArrayFor(CombatUnit* unit);
    bool keys[ALLEGRO_KEY_MAX];
    int mouse_button_state;
    ALLEGRO_MOUSE_STATE mouse_state;
    bool mouse_moved;
    std::shared_ptr<Space> space;
    std::pair<std::vector<CombatUnit*>,std::vector<CombatUnit*> > parties; //first is player party
    CombatUnit* focused_unit;
    void callAStar(std::pair<int,int> goal);
    JH::AStar* focused_unit_pf;
    float x, y;
    void setCursorColour();
    int cursorOverUnit(); //-1 ally, 0 no, 1 enemy
    int cursor_x, cursor_y; //tile cursor in tile units
    bool cursor_moved;
    JH::HashTable<std::string,ALLEGRO_BITMAP*> cursors;
    int cursor_move_step;
    int cursor_move_time;
    ALLEGRO_BITMAP* cur_cursor; //cursor currently in use
};

#endif // __ISOMETRICDISPLAYH_GUARD
