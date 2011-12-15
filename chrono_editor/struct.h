#ifndef __STRUCT_H__
#define __STRUCT_H__

#include <stdint.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t  s8;
typedef int16_t s16;
typedef int32_t s32;

//~ struct s_ioCharacter {
    //~ uint8_t   useless0; //Pour commencer a lire le fichier 1 octet + tot, fonctionne pas sans
    //~ uint8_t   who;
    //~ uint8_t   useless1[2];
    //~ uint16_t  currentHP;
    //~ uint16_t  maxHP;
    //~ uint16_t  currentMP;
    //~ uint16_t  maxMP;
    //~ uint8_t   power;
    //~ uint8_t   stamina;
    //~ uint8_t   speed;
    //~ uint8_t   magic;
    //~ uint8_t   hit;
    //~ uint8_t   evade;
    //~ uint8_t   magicDef;
    //~ uint8_t   level;
    //~ uint32_t  exp; //TODO: S'assurer que l'exp est bien 4 bytes
    //~ uint8_t   useless2[16]; // +2 bytes si l'exp est 2 bytes
    //~ uint8_t   helmet;
    //~ uint8_t   armor;
    //~ uint8_t   weapon;
    //~ uint8_t   relic;
    //~ uint16_t  xpForLevelUp;
    //~ uint16_t  spForNextTech;
    //~ uint8_t   useless3[32];
//~ };

struct character_s {
    uint8_t   Id;
    uint8_t   charId;
    uint16_t  currentHP;
    uint16_t  maxHP;
    uint16_t  currentMP;
    uint16_t  maxMP;
    uint8_t   basePower;
    uint8_t   baseStamina;
    uint8_t   baseSpeed;
    uint8_t   baseMagic;
    uint8_t   baseHit;
    uint8_t   baseEvade;
    uint8_t   baseMagicDef;
    uint8_t   level;
    uint32_t  exp;
    uint8_t   dummy2[16];
    uint8_t   helmet;
    uint8_t   armor;
    uint8_t   weapon;
    uint8_t   relic;
    uint16_t  xpForLevelUp;
    uint16_t  spForNextTech;
    uint8_t   dummy3[7];
    uint8_t   currentPower;
    uint8_t   currentStamina;
    uint8_t   currentSpeed;
    uint8_t   currentMagic;
    uint8_t   currentHit;
    uint8_t   currentEvade;
    uint8_t   currentMagicDef;
    uint8_t   currentDefense;
    uint16_t  currentMaxHP;
    uint8_t   dummy4[13];
};

struct s_items {
    uint8_t item[256];
    uint8_t count[256];
};

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

#endif //__STRUCT_H__
