#ifndef __STRINGS_H__
#define __STRINGS_H__

#ifdef __cplusplus
extern "C" {
#endif

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

static const char *techniquesList[7][9] = {
    /*CRONO*/ {"[NONE]", "Cyclone", "Slash", "Lightning", "Spincut", "Lightning 2", "Life", "Confuse", "Luminaire"},
    /*MARLE*/ {"[NONE]", "Aura", "Provoke", "Ice", "Cure", "Haste", "Ice 2", "Cure 2", "Life 2"},
    /*LUCCA*/ {"[NONE]", "Flame Toss", "Hypno Wave", "Fire", "Napalm", "Protect", "Fire 2", "Mega Bomb", "Flare"},
    /*ROBO*/  {"[NONE]", "Slurp", "Slurp Cut", "Water", "Heal", "Leap Slash", "Water 2", "Cure 2", "Frog Squash"},
    /*FROG*/  {"[NONE]", "Rocket Punch", "Cure Beam", "Laser Spin", "Robo Tackle", "Heal Beam", "Uzzi Punch", "Area Bomb", "Shock"},
    /*AYLA*/  {"[NONE]", "Kiss", "Rollo Kick", "Cat Attack", "Rock Throw", "Charm", "Tail Spin", "Dino Tail", "Triple Kick"},
    /*MAGUS*/ {"[NONE]", "Lightning 2", "Ice 2", "Fire 2", "Dark Bomb", "Magic Wall", "Dark Mist", "Black Hole", "Dark Matter"}
};

#ifdef __cplusplus
}  /* extern "C" */
#endif

#endif //__STRINGS_H__