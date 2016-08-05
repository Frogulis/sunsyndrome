#include "IsometricDisplay.h"

//Buffs

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

//SkillFunctors

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

//Units

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
    IsometricDisplay::CombatUnit* temp;
    if (unit_class == "shadowman")
    {
        temp = new Unit_Shadowman;
        temp->actor.loadByName("shadowman");
    }
    else if (unit_class == "grunt")
    {
        temp = new Unit_Grunt;
        temp->actor.loadByName("shadowman");
    }
    else
    {
        //Come up with a suitable default/error class...?
        temp = new Unit_Shadowman;
        temp->actor.loadByName("shadowman");
    }
    return temp;
}
