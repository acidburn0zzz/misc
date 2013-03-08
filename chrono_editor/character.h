/***************************************************************************
 * Copyright (C) 2010 Lemay, Mathieu                                       *
 *                                                                         *
 * This program is free software; you can redistribute it and/or modify    *
 * it under the terms of the GNU General Public License as published by    *
 * the Free Software Foundation; either version 2 of the License, or       *
 * (at your option) any later version.                                     *
 *                                                                         *
 * This program is distributed in the hope that it will be useful,         *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the            *
 * GNU General Public License for more details.                            *
 *                                                                         *
 * You should have received a copy of the GNU General Public License along *
 * with this program; if not, write to the Free Software Foundation, Inc., *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.             *
 *                                                                         *
 * You can contact the original author at acidrain1@gmail.com              *
 ***************************************************************************/

#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "struct.h"

enum {CRONO, MARLE, LUCCA, ROBO, FROG, AYLA, MAGUS};

class Character {
public:
    Character(character_t character);

    character_t getCharStruct();
    void setCharStruct(character_t character);

    u8   getId();
    u8   getCharId();
    u16  getCurrentHP();
    u16  getMaxHP();
    u16  getCurrentMP();
    u16  getMaxMP();
    u8   getBasePower();
    u8   getBaseStamina();
    u8   getBaseSpeed();
    u8   getBaseMagic();
    u8   getBaseHit();
    u8   getBaseEvade();
    u8   getBaseMagicDef();
    u8   getLevel();
    u32  getExp();
    u8   getHelmet();
    u8   getArmor();
    u8   getWeapon();
    u8   getRelic();
    u16  getXpForLevelUp();
    u16  getSpForNextTech();

    void setId(u8 id);
    void setCharId(u8 charId);
    void setCurrentHP(u16 currentHP);
    void setMaxHP(u16 maxHP);
    void setCurrentMP(u16 currentMP);
    void setMaxMP(u16 currentHP);
    void setBasePower(u8 basePower);
    void setBaseStamina(u8 baseStamina);
    void setBaseSpeed(u8 baseSpeed);
    void setBaseMagic(u8 baseMagic);
    void setBaseHit(u8 baseHit);
    void setBaseEvade(u8 baseEvade);
    void setBaseMagicDef(u8 baseMagicDef);
    void setLevel(u8 level);
    void setExp(u32 exp);
    void setHelmet(u8 helmet);
    void setArmor(u8 armor);
    void setWeapon(u8 weapon);
    void setRelic(u8 relic);
    void setXpForLevelUp(u16 xp);
    void setSpForNextTech(u16 sp);

    bool checkIfWeaponOK(u8 wpn);
private:
    character_t _char;
};

#endif //__CHARACTER_H__
