#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "struct.h"

enum {CRONO, MARLE, LUCCA, ROBO, FROG, AYLA, MAGUS, EPOCH};

class Character {
public:
    Character();

    unsigned char   getId();
    unsigned char   getCharId();
    unsigned char*  getName();
    unsigned char*  getNameForSRAM();
    unsigned short  getCurrentHP();
    unsigned short  getMaxHP();
    unsigned short  getCurrentMP();
    unsigned short  getMaxMP();
    unsigned char   getBasePower();
    unsigned char   getBaseStamina();
    unsigned char   getBaseSpeed();
    unsigned char   getBaseMagic();
    unsigned char   getBaseHit();
    unsigned char   getBaseEvade();
    unsigned char   getBaseMagicDef();
    unsigned char   getLevel();
    unsigned int    getExp();
    unsigned char   getHelmet();
    unsigned char   getArmor();
    unsigned char   getWeapon();
    unsigned char   getRelic();
    unsigned short  getXpForLevelUp();
    unsigned short  getSpForNextTech();

    void setId(unsigned char id);
    void setCharId(unsigned char charId);
    void setName(unsigned char *name);
    void setNameFromSRAM(unsigned char *name);
    void setCurrentHP(unsigned short currentHP);
    void setMaxHP(unsigned short maxHP);
    void setCurrentMP(unsigned short currentMP);
    void setMaxMP(unsigned short currentHP);
    void setBasePower(unsigned char basePower);
    void setBaseStamina(unsigned char baseStamina);
    void setBaseSpeed(unsigned char baseSpeed);
    void setBaseMagic(unsigned char baseMagic);
    void setBaseHit(unsigned char baseHit);
    void setBaseEvade(unsigned char baseEvade);
    void setBaseMagicDef(unsigned char baseMagicDef);
    void setLevel(unsigned char level);
    void setExp(unsigned int exp);
    void setHelmet(unsigned char helmet);
    void setArmor(unsigned char armor);
    void setWeapon(unsigned char weapon);
    void setRelic(unsigned char relic);

    bool checkIfWeaponOK(unsigned char wpn);
private:
    unsigned char   id;
    unsigned char   charId;
    unsigned char   name[6];
    unsigned short  currentHP;
    unsigned short  maxHP;
    unsigned short  currentMP;
    unsigned short  maxMP;
    unsigned char   basePower;
    unsigned char   baseStamina;
    unsigned char   baseSpeed;
    unsigned char   baseMagic;
    unsigned char   baseHit;
    unsigned char   baseEvade;
    unsigned char   baseMagicDef;
    unsigned char   level;
    unsigned int    exp;
    unsigned char   helmet;
    unsigned char   armor;
    unsigned char   weapon;
    unsigned char   relic;
    unsigned short  xpForLevelUp;
    unsigned short  spForNextTech;
    unsigned char   currentPower;
    unsigned char   currentStamina;
    unsigned char   currentSpeed;
    unsigned char   currentMagic;
    unsigned char   currentHit;
    unsigned char   currentEvade;
    unsigned char   currentMagicDef;
    unsigned char   currentDefense;
    unsigned short  currentMaxHP;
};

#endif //__CHARACTER_H__
