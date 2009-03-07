#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "struct.h"

class Character {
private:
    s_ioCharacter _ioChar;

public:
    Character();
    Character(s_ioCharacter c);
    
    unsigned char   getWho();
    unsigned short  getCurrentHP();
    unsigned short  getMaxHP();
    unsigned short  getCurrentMP();
    unsigned short  getMaxMP();
    unsigned char   getPower();
    unsigned char   getStamina();
    unsigned char   getSpeed();
    unsigned char   getMagic();
    unsigned char   getHit();
    unsigned char   getEvade();
    unsigned char   getMagicDef();
    unsigned char   getLevel();
    unsigned int    getExp();
    unsigned char   getHelmet();
    unsigned char   getArmor();
    unsigned char   getWeapon();
    unsigned char   getRelic();
    unsigned short  getXpForLevelUp();
    unsigned short  getSpForNextTech();
    s_ioCharacter   getIOCharacter();
    
    void setCurrentHP(unsigned short currentHP);
    void setMaxHP(unsigned short maxHP);
    void setCurrentMP(unsigned short currentMP);
    void setMaxMP(unsigned short currentHP);
    void setPower(unsigned char power);
    void setStamina(unsigned char stamina);
    void setSpeed(unsigned char speed);
    void setMagic(unsigned char magic);
    void setHit(unsigned char hit);
    void setEvade(unsigned char evade);
    void setMagicDef(unsigned char magicDef);
    void setLevel(unsigned char level);
    void setExp(unsigned int exp);
    void setHelmet(unsigned char helmet);
    void setArmor(unsigned char armor);
    void setWeapon(unsigned char weapon);
    void setRelic(unsigned char relic);
    
    bool checkIfWeaponOK(unsigned char wpn);
};
    
#endif //__CHARACTER_H__
