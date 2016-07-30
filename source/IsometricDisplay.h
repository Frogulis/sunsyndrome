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
    std::shared_ptr<Space> getSpacePtr();
    void changeOffset(float x, float y);
    void runEvents(ALLEGRO_EVENT &ev);
    void runLogic();
private:
    bool** generateWalkableArrayFor(CombatUnit* unit);
    bool keys[ALLEGRO_KEY_MAX];
    int mouse_button_state;
    std::shared_ptr<Space> space;
    std::pair<std::vector<CombatUnit*>,std::vector<CombatUnit*> > parties; //first is player party
    float x, y;
};

#endif // __ISOMETRICDISPLAYH_GUARD
