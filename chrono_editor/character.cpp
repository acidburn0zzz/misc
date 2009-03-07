#include "character.h"

Character::Character() {
    //DUDE!
}

Character::Character(s_ioCharacter c) {
    _ioChar = c;
}

unsigned char Character::getWho() {
    return _ioChar.who;
}

unsigned short Character::getCurrentHP() {
    return _ioChar.currentHP;
}

unsigned short Character::getMaxHP() {
    return _ioChar.maxHP;
}

unsigned short Character::getCurrentMP() {
    return _ioChar.currentMP;
}

unsigned short Character::getMaxMP() {
    return _ioChar.maxMP;
}

unsigned char Character::getPower() {
    return _ioChar.power;
}

unsigned char Character::getStamina() {
    return _ioChar.stamina;
}

unsigned char Character::getSpeed() {
    return _ioChar.speed;
}

unsigned char Character::getMagic() {
    return _ioChar.magic;
}

unsigned char Character::getHit() {
    return _ioChar.hit;
}

unsigned char Character::getEvade() {
    return _ioChar.evade;
}

unsigned char Character::getMagicDef() {
    return _ioChar.magicDef;
}

unsigned char Character::getLevel() {
    return _ioChar.level;
}

unsigned int Character::getExp() {
    return _ioChar.exp;
}

unsigned char Character::getHelmet() {
    return _ioChar.helmet;
}

unsigned char Character::getArmor() {
    return _ioChar.armor;
}

unsigned char Character::getWeapon() {
    return _ioChar.weapon;
}

unsigned char Character::getRelic() {
    return _ioChar.relic;
}

unsigned short Character::getXpForLevelUp() {
    return _ioChar.xpForLevelUp;
}

unsigned short Character::getSpForNextTech() {
    return _ioChar.spForNextTech;
}

s_ioCharacter Character::getIOCharacter() {
    return _ioChar;
}

void Character::setCurrentHP(unsigned short currentHP) {
    if (currentHP > 999)
        currentHP = 999;
    
    _ioChar.currentHP = currentHP;
}

void Character::setMaxHP(unsigned short maxHP) {
    if (maxHP > 999)
        maxHP = 999;
    
    _ioChar.maxHP = maxHP;
}

void Character::setCurrentMP(unsigned short currentMP) {
    if (currentMP > 99)
        currentMP = 99;
    
    _ioChar.currentMP = currentMP;
}

void Character::setMaxMP(unsigned short maxMP) {
    if (maxMP > 99)
        maxMP = 99;
    
    _ioChar.maxMP = maxMP;
}

void Character::setPower(unsigned char power) {
    if (power > 99)
        power = 99;
    
    _ioChar.power = power;
}


void Character::setStamina(unsigned char stamina) {
    if (stamina > 99)
        stamina = 99;
    
    _ioChar.stamina = stamina;
}

void Character::setSpeed(unsigned char speed) {
    if (speed > 16)
        speed = 16;
    
    _ioChar.speed = speed;
}

void Character::setMagic(unsigned char magic) {
    if (magic > 99)
        magic = 99;
    
    _ioChar.magic = magic;
}

void Character::setHit(unsigned char hit) {
    if (hit > 99)
        hit = 99;
    
    _ioChar.hit = hit;
}

void Character::setEvade(unsigned char evade) {
    if (evade > 99)
        evade = 99;
    
    _ioChar.evade = evade;
}

void Character::setMagicDef(unsigned char magicDef) {
    if (magicDef > 99)
        magicDef = 99;
    
    _ioChar.magicDef = magicDef;
}

void Character::setLevel(unsigned char level) {
    if (level > 99)
        level = 99;
    
    _ioChar.level = level;
}

void Character::setExp(unsigned int exp) {
    if (exp > 9999999)
        exp = 9999999;
    
    _ioChar.exp = exp;
}

void Character::setHelmet(unsigned char helmet) {
    if (helmet < 0x7c)
        helmet = 0x7c;
    else if (helmet > 0x93)
        helmet = 0x93;
    
    _ioChar.helmet = helmet;
}

void Character::setArmor(unsigned char armor) {
    if (armor < 0x5b)
        armor = 0x5b;
    else if (armor > 0x7a)
        armor = 0x7a;
    
    _ioChar.armor = armor;
}

void Character::setWeapon(unsigned char weapon) {
    if (checkIfWeaponOK(weapon))
        _ioChar.weapon = weapon;
}

void Character::setRelic(unsigned char relic) {
    if (relic < 0x95)
        relic = 0x95;
    else if (relic > 0xbb)
        relic = 0xbb;
    
    _ioChar.relic = relic;
}

bool Character::checkIfWeaponOK(unsigned char wpn) {
    switch (_ioChar.who) {
        case CRONO:
            if ((wpn >= 0x01 && wpn <= 0x10) || wpn == 0x4f || (wpn >= 0x53 && wpn <= 0x55))
                return true;
            else
                return false;
        case MARLE:
            if (wpn >= 0x11 && wpn <= 0x1a)
                return true;
            else
                return false;
        case LUCCA:
            if (wpn >= 0x1f && wpn <= 0x29)
                return true;
            else
                return false;
        case ROBO:
            if (wpn >= 0x2e && wpn <= 0x39)
                return true;
            else
                return false;
        case FROG:
            if ((wpn >= 0x3b && wpn <= 0x43) || (wpn >= 0x50 && wpn <= 0x52))
                return true;
            else
                return false;
        case AYLA:
            if (wpn >= 0x44 && wpn <= 0x48)
                return true;
            else
                return false;
        case MAGUS:
            if (wpn >= 0x4b && wpn <= 0x4e)
                return true;
            else
                return false;
        default:
            return false;
    }
}
