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

#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;

static const int CHARACTER_SIZE = 0x0050;
static const int NAME_SIZE = 0x0006;
static const int GAME_SIZE = 0x0A00;
static const int SRAM_SIZE = 0x2000;
static const int CHECKSUM_OFFSET = 0x1FF0;

// Size: 80
struct character_s {
    u8  id;
    u8  charId;
    u8  unk1;
    u16 currentHP;
    u16 maxHP;
    u16 currentMP;
    u16 maxMP;
    u8  basePower;
    u8  baseStamina;
    u8  baseSpeed;
    u8  baseMagic;
    u8  baseHit;
    u8  baseEvade;
    u8  baseMagicDef;
    u8  level;
    u32 exp;    //  Exp uses only 24bit
    u8  unk2[16];
    u8  helmet;
    u8  armor;
    u8  weapon;
    u8  relic;
    u16 xpForLevelUp;
    u16 spForNextTech;
    u8  unk3[7];
    u8  currentPower;
    u8  currentStamina;
    u8  currentSpeed;
    u8  currentMagic;
    u8  currentHit;
    u8  currentEvade;
    u8  currentMagicDef;
    u8  currentDefense;
    u16 currentMaxHP;
    u8  unk4[16];
} __attribute__((packed));
typedef struct character_s character_t;

// Size: 512
struct items_s {
    uint8_t item[256];
    uint8_t count[256];
};
typedef struct items_s items_t;

// Size: 2560
struct game_s {
    items_t     items;
    character_t characters[7];
    u8  unk0[7];
    u8  techLevel[7];
    u8  unk1[66];
    u8  unk2[256];
    u8  partyMember1;
    u8  partyMember2;
    u8  partyMember3;
    u8  unk3[25];
    u16 saveCount;
    u8  unk4[18];
    u8  names[8][6];
    u8  gold1;
    u8  gold2;
    u8  gold3;
    u8  unk5[2];
    u8  min2;
    u8  min1;
    u8  hour2;
    u8  hour1;
    u8  fill[1047];
};// __attribute__((packed));
typedef struct game_s game_t;

struct sram_s {
    game_t games[3];
    u8     unk1[480];
    u8     lastGame;
    u8     unk2;
    u8     ngPlus;
    u8     unk3[13];
    u16    crc[3];
    u16    unk4;
    u16    isUsed[3]; // 0xE41B if slot used
    u16    unk5;
};
typedef struct sram_s sram_t;

struct s_time {
    uint8_t min2; //2eme chiffre des minutes
    uint8_t min1; //1er chiffre des minutes
    uint8_t hour2; //2eme chiffre des heures
    uint8_t hour1; //1er chiffre des heures
};

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //__STRUCT_H__
