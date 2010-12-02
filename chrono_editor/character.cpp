#include "character.h"

Character::Character() {
}

unsigned char Character::getId() {
    return id;
}

unsigned char Character::getCharId() {
    return charId;
}

unsigned char *Character::getName() {
    return name;
}

unsigned short Character::getCurrentHP() {
    return currentHP;
}

unsigned short Character::getMaxHP() {
    return maxHP;
}

unsigned short Character::getCurrentMP() {
    return currentMP;
}

unsigned short Character::getMaxMP() {
    return maxMP;
}

unsigned char Character::getBasePower() {
    return basePower;
}

unsigned char Character::getBaseStamina() {
    return baseStamina;
}

unsigned char Character::getBaseSpeed() {
    return baseSpeed;
}

unsigned char Character::getBaseMagic() {
    return baseMagic;
}

unsigned char Character::getBaseHit() {
    return baseHit;
}

unsigned char Character::getBaseEvade() {
    return baseEvade;
}

unsigned char Character::getBaseMagicDef() {
    return baseMagicDef;
}

unsigned char Character::getLevel() {
    return level;
}

unsigned int Character::getExp() {
    return exp;
}

unsigned char Character::getHelmet() {
    return helmet;
}

unsigned char Character::getArmor() {
    return armor;
}

unsigned char Character::getWeapon() {
    return weapon;
}

unsigned char Character::getRelic() {
    return relic;
}

unsigned short Character::getXpForLevelUp() {
    return xpForLevelUp;
}

unsigned short Character::getSpForNextTech() {
    return spForNextTech;
}

void Character::setId(unsigned char id) {
    this->id = id;
}

void Character::setCharId(unsigned char charId) {
    this->charId = charId;
}

void Character::setNameFromSRAM(unsigned char *name) {
    for (int i=0; i<5; i++) {
        if (name[i] >= 0xa0 && name[i] <= 0xb9)
            this->name[i] = name[i] - 95;
        else if (name[i] >= 0xba && name[i] <= 0xd3)
            this->name[i] = name[i] - 89;
        else
            this->name[i] = name[i];
    }
    this->name[5] = '\0';
}

void Character::setCurrentHP(unsigned short currentHP) {
    if (currentHP > 999)
        currentHP = 999;

    this->currentHP = currentHP;
}

void Character::setMaxHP(unsigned short maxHP) {
    if (maxHP > 999)
        maxHP = 999;

    this->maxHP = maxHP;
}

void Character::setCurrentMP(unsigned short currentMP) {
    if (currentMP > 99)
        currentMP = 99;

    this->currentMP = currentMP;
}

void Character::setMaxMP(unsigned short maxMP) {
    if (maxMP > 99)
        maxMP = 99;

    this->maxMP = maxMP;
}

void Character::setBasePower(unsigned char basePower) {
    if (basePower > 99)
        basePower = 99;

    this->basePower = basePower;
}


void Character::setBaseStamina(unsigned char baseStamina) {
    if (baseStamina > 99)
        baseStamina = 99;

    this->baseStamina = baseStamina;
}

void Character::setBaseSpeed(unsigned char baseSpeed) {
    if (baseSpeed > 16)
        baseSpeed = 16;

    this->baseSpeed = baseSpeed;
}

void Character::setBaseMagic(unsigned char baseMagic) {
    if (baseMagic > 99)
        baseMagic = 99;

    this->baseMagic = baseMagic;
}

void Character::setBaseHit(unsigned char baseHit) {
    if (baseHit > 99)
        baseHit = 99;

    this->baseHit = baseHit;
}

void Character::setBaseEvade(unsigned char baseEvade) {
    if (baseEvade > 99)
        baseEvade = 99;

    this->baseEvade = baseEvade;
}

void Character::setBaseMagicDef(unsigned char baseMagicDef) {
    if (baseMagicDef > 99)
        baseMagicDef = 99;

    this->baseMagicDef = baseMagicDef;
}

void Character::setLevel(unsigned char level) {
    if (level > 99)
        level = 99;

    this->level = level;
}

void Character::setExp(unsigned int exp) {
    if (exp > 9999999)
        exp = 9999999;

    this->exp = exp;
}

void Character::setHelmet(unsigned char helmet) {
    if (helmet < 0x7c)
        helmet = 0x7c;
    else if (helmet > 0x93)
        helmet = 0x93;

    this->helmet = helmet;
}

void Character::setArmor(unsigned char armor) {
    if (armor < 0x5b)
        armor = 0x5b;
    else if (armor > 0x7a)
        armor = 0x7a;

    this->armor = armor;
}

void Character::setWeapon(unsigned char weapon) {
    if (checkIfWeaponOK(weapon))
        this->weapon = weapon;
}

void Character::setRelic(unsigned char relic) {
    if (relic < 0x95)
        relic = 0x95;
    else if (relic > 0xbb)
        relic = 0xbb;

    this->relic = relic;
}

bool Character::checkIfWeaponOK(unsigned char wpn) {
    switch (id) {
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
