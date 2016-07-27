#ifndef __GAMEH_GUARD
#define __GAMEH_GUARD


#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <allegro5/allegro.h>

#include "Space.h"
#include "IDisplay.h"
#include "IsometricDisplay.h"
#include "MenuDisplay.h"
#include "HashTable.h"

class Game
{
    /***
    Handles logic and everything else.
    Basically a hub that manages the game behind the screen.
    ***/
public:
    Game();
    bool init();
    void runEvents(ALLEGRO_EVENT &ev);
    void runLogic();
    void runDisplay();
    class CombatUnit
    {
    public:
        class SkillFunctor
        {
        public:
            void operator()(CombatUnit* user, CombatUnit* target); //pass this to user
            void call(CombatUnit* user, CombatUnit* target);
            void setName(std::string name);
            std::string getName();
        protected:
            virtual void func(CombatUnit* user, CombatUnit* target) = 0;
            std::string name;
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
            void setName(std::string name);
            std::string getName();
        protected:
            int turns_remaining;
            std::string name;
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
private:
    bool loadArea(std::string area_name);
    bool loadIsoDisplay(std::string area_name);
    bool loadMenuDisplay(std::string menu_name);
    int mode; // -1 = nothing placeholder?, 0 = menu, 1 = combat,
    bool keys[ALLEGRO_KEY_MAX];
    int mouse_button_state;
    std::vector<CombatUnit> combat_units;
    std::shared_ptr<IDisplay> drawer;
    std::shared_ptr<IsometricDisplay> cur_isometric_display;
    std::shared_ptr<Space> cur_map;
    std::shared_ptr<MenuDisplay> cur_menu_display;
};

#endif // __GAMEH_GUARD
