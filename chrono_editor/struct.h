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

struct character_s {
    u8  id;
    u8  charId;
    u8  unk1;
    u16 currentHP;
    u16 maxHP;
    u16 currentMP;
    u16 maxMP;
    u8  power;
    u8  stamina;
    u8  speed;
    u8  magic;
    u8  hit;
    u8  evade;
    u8  magicDef;
    u8  level;
    u32 exp;
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

struct items_s {
    uint8_t item[256];
    uint8_t count[256];
};
typedef struct items_s items_t;

struct game_s {
    items_t     items;
    character_t characters[8];
    u8  unk1[256];
    u8  partyMember1;
    u8  partyMember2;
    u8  partyMember3;
    u8  unk2[25];
    u8  saveCount;
    u8  unk3[19];
    u8  names[8][6];
    u16 gold1; // gold & 0x00ffff
    u8  gold2; // gold & 0xff0000
    u8  unk4;
    u8  fill[1052];
};// __attribute__((packed));
typedef struct game_s game_t;

struct sram_s {
    game_t games[3];
    u8 unk1[496];
    u16 crc[3];
    u8 unk2[10];
};
typedef struct sram_s sram_t;

struct s_time {
    uint8_t min2; //2eme chiffre des minutes
    uint8_t min1; //1er chiffre des minutes
    uint8_t hour2; //2eme chiffre des heures
    uint8_t hour1; //1er chiffre des heures
};

static const char *itemList[] = {
    "Empty",
    "Wood Sword",
    "Iron Blade",
    "Steel Saber",
    "Lode Sword",
    "Red Katana",
    "Flint Edge",
    "Dark Saber",
    "Aeon Blade",
    "Demon Edge",
    "Alloy Blade",
    "Star Sword",
    "Vedic Blade",
    "Kali Blade",
    "Shiva Edge",
    "Bolt Sword",
    "Slasher",
    "Bronze Bow",
    "Iron Bow",
    "Lode Bow",
    "Robin Bow",
    "Sage Bow",
    "Dream Bow",
    "Comet Arrow",
    "Sonic Arrow",
    "Valkerye",
    "Siren",
    "",
    "",
    "",
    "",
    "Air Gun",
    "Dart Gun",
    "Auto Gun",
    "Pico Magnum",
    "Plasma Gun",
    "Ruby Gun",
    "Dream Gun",
    "Megablast",
    "Shock Wave",
    "Wonder Shot",
    "Graedus",
    "",
    "",
    "",
    "",
    "Tin Arm",
    "Hammer Arm",
    "Mirage Hand",
    "Stone Arm",
    "Doom Finger",
    "Magma Hand",
    "Megaton Arm",
    "Big Hand",
    "Kaiser Arm",
    "Giga Arm",
    "Terra Arm",
    "Crisis Arm",
    "",
    "Bronze Edge",
    "Iron Sword",
    "Masamune (I)",
    "Flash Blade",
    "Pearl Edge",
    "Rune Blade",
    "Brave Sword",
    "Masamune (II)",
    "Demon Hit",
    "Fist (I)",
    "Fist (II)",
    "Fist (III)",
    "Iron Fist",
    "Bronze Fist",
    "",
    "",
    "Dark Scythe",
    "Hurricane",
    "Star Scythe",
    "Doom Sickle",
    "Mop",
    "Bent Sword",
    "Bent Hilt",
    "Masamune (0)",
    "Swallow",
    "Slasher 2",
    "Rainbow Sword",
    "",
    "",
    "",
    "",
    "",
    "Hide Tunic",
    "Karate Gi",
    "Bronze Mail",
    "Maiden Suit",
    "Iron Suit",
    "Titan Vest",
    "Gold Suit",
    "Ruby Vest",
    "Dark Mail",
    "Mist Robe",
    "Meso Mail",
    "Lumin Robe",
    "Flash Mail",
    "Lode Vest",
    "Aeon Suit",
    "Zodiac Cape",
    "Nova Armor",
    "Prism Dress",
    "Moon Armor",
    "Ruby Armor",
    "Raven Armor",
    "Gloom Cape",
    "White Mail",
    "Black Mail",
    "Blue Mail",
    "Red Mail",
    "White Vest",
    "Black Vest",
    "Blue Vest",
    "Red Vest",
    "Taban Vest",
    "Taban Suit",
    "",
    "Hide Cap",
    "Bronze Helm",
    "Iron Helm",
    "Beret",
    "Gold Helm",
    "Rock Helm",
    "Cera Topper",
    "Glow Helm",
    "Lode Helm",
    "Aeon Helm",
    "Prism Helm",
    "Doom Helm",
    "Dark Helm",
    "Gloom Helm",
    "Safe Helm",
    "Taban Helm",
    "Sight Cap",
    "Memory Cap",
    "Time Hat",
    "Vigil Hat",
    "Ozzie Pants",
    "Haste Helm",
    "Rainbow Helm",
    "Mermaid Cap",
    "",
    "Bandana",
    "Ribbon",
    "Power Glove",
    "Defender",
    "Magic Scarf",
    "Amulet",
    "Dash Ring",
    "Hit Ring",
    "Power Ring",
    "Magic Ring",
    "Wall Ring",
    "Silver Earring",
    "Gold Earring",
    "Silver Stud",
    "Gold Stud",
    "Sight Scope",
    "Charm Top",
    "Rage Band",
    "Frenzy Band",
    "Third Eye",
    "Wallet",
    "Green Dream",
    "Berserker",
    "Power Scarf",
    "Speed Belt",
    "Black Rock",
    "Blue Rock",
    "Silver Rock",
    "White Rock",
    "Gold Rock",
    "Hero Medal",
    "Muscle Ring",
    "Flea Vest",
    "Magic Seal",
    "Power Seal",
    "Relic",
    "Seraph Song",
    "Sun Shades",
    "Prism Specs",
    "",
    "Tonic",
    "Mid Tonic",
    "Full Tonic",
    "Ether",
    "Mid Ether",
    "Full Ether",
    "Elixir",
    "Hyper Ether",
    "Mega Elixir",
    "Heal",
    "Revive",
    "Shelter",
    "Power Meal",
    "Lapis",
    "Barrier",
    "Shield",
    "Power Tab",
    "Magic Tab",
    "Speed Tab",
    "Petal",
    "Fang",
    "Horn",
    "Feather",
    "Seed",
    "Bike Key",
    "Pendant",
    "Gate Key",
    "Prsim Shard",
    "Chrono Trigger",
    "Tools",
    "Jerky",
    "Dream Stone",
    "Race Log",
    "Moon Stone",
    "Sun Stone",
    "Ruby Knife",
    "Yakra Key",
    "Clone",
    "Toma's Pop",
    "2 Petals",
    "2 Fangs",
    "2 Horns",
    "2 Feathers"
};

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //__STRUCT_H__
