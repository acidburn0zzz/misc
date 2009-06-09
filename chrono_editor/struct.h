#ifndef __STRUCT_H__
#define __STRUCT_H__

enum {CRONO, MARLE, LUCCA, ROBO, FROG, AYLA, MAGUS};

struct s_ioCharacter {
    unsigned char   useless0; //Pour commencer a lire le fichier 1 octet + tot, fonctionne pas sans
    unsigned char   who;
    unsigned char   useless1[2];
    unsigned short  currentHP;
    unsigned short  maxHP;
    unsigned short  currentMP;
    unsigned short  maxMP;
    unsigned char   power;
    unsigned char   stamina;
    unsigned char   speed;
    unsigned char   magic;
    unsigned char   hit;
    unsigned char   evade;
    unsigned char   magicDef;
    unsigned char   level;
    unsigned int    exp; //TODO: S'assurer que l'exp est bien 4 bytes
    unsigned char   useless2[16]; // +2 bytes si l'exp est 2 bytes
    unsigned char   helmet;
    unsigned char   armor;
    unsigned char   weapon;
    unsigned char   relic;
    unsigned short  xpForLevelUp;
    unsigned short  spForNextTech;
    unsigned char   useless3[32];
};

struct s_items {
    unsigned char item[256];
    unsigned char count[256];
};

struct s_time {
    unsigned char min2; //2eme chiffre des minutes
    unsigned char min1; //1er chiffre des minutes
    unsigned char hour2; //2eme chiffre des heures
    unsigned char hour1; //1er chiffre des heures
};

static const char *itemList[] = {
    "Empty",
    "Wood Sword",
    "Iron Blade",
    "SteelSaber",
    "Lode Sword",
    "Red Katana",
    "Flint Edge",
    "Dark Saber",
    "Aeon Blade",
    "Demon Edge",
    "AlloyBlade",
    "Star Sword",
    "VedicBlade",
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
    "CometArrow",
    "SonicArrow",
    "Valkerye",
    "Siren",
    "",
    "",
    "",
    "",
    "Air Gun",
    "Dart Gun",
    "Auto Gun",
    "PicoMagnum",
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
    "DoomFinger",
    "Magma Hand",
    "MegatonArm",
    "Big Hand",
    "Kaiser Arm",
    "Giga Arm",
    "Terra Arm",
    "Crisis Arm",
    "",
    "BronzeEdge",
    "Iron Sword",
    "Masamune (I)",
    "FlashBlade",
    "Pearl Edge",
    "Rune Blade",
    "BraveSword",
    "Masamune (II)",
    "Demon Hit",
    "Fist (I)",
    "Fist (II)",
    "Fist (III)",
    "Iron Fist",
    "Bronze Fist",
    "",
    "",
    "DarkScythe",
    "Hurricane",
    "StarScythe",
    "DoomSickle",
    "Mop",
    "Bent Sword",
    "Bent Hilt",
    "Masamune (0)",
    "Swallow",
    "Slasher 2",
    "Rainbow",
    "",
    "",
    "",
    "",
    "",
    "Hide Tunic",
    "Karate Gi",
    "BronzeMail",
    "MaidenSuit",
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
    "ZodiacCape",
    "Nova Armor",
    "PrismDress",
    "Moon Armor",
    "Ruby Armor",
    "RavenArmor",
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
    "BronzeHelm",
    "Iron Helm",
    "Beret",
    "Gold Helm",
    "Rock Helm",
    "CeraTopper",
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
    "OzziePants",
    "Haste Helm",
    "R'bow Helm",
    "MermaidCap",
    "",
    "Bandana",
    "Ribbon",
    "PowerGlove",
    "Defender",
    "MagicScarf",
    "Amulet",
    "Dash Ring",
    "Hit Ring",
    "Power Ring",
    "Magic Ring",
    "Wall Ring",
    "SilverErng",
    "Gold Erng",
    "SilverStud",
    "Gold Stud",
    "SightScope",
    "Charm Top",
    "Rage Band",
    "FrenzyBand",
    "Third Eye",
    "Wallet",
    "GreenDream",
    "Berserker",
    "PowerScarf",
    "Speed Belt",
    "Black Rock",
    "Blue Rock",
    "SilverRock",
    "White Rock",
    "Gold Rock",
    "Hero Medal",
    "MuscleRing",
    "Flea Vest",
    "Magic Seal",
    "Power Seal",
    "Relic",
    "SeraphSong",
    "Sun Shades",
    "PrismSpecs",
    "",
    "Tonic",
    "Mid Tonic",
    "Full Tonic",
    "Ether",
    "Mid Ether",
    "Full Ether",
    "Elixir",
    "HyperEther",
    "MegaElixir",
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
    "PrsimShard",
    "C. Trigger",
    "Tools",
    "Jerky",
    "DreamStone",
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
