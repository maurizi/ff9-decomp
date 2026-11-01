#ifndef FF9_IDS_H
#define FF9_IDS_H
/* Game-data ids named from the game's own text (item, ability, command, status, element, character names;
 * tools/ff9text.py decodes them) or from the data that defines them. Included by ff9.h. docs/game-ids.md describes
 * the tables. Only ids the code uses get a macro. Tags: [text TABLE] = the name in that table at this id
 * (`tools/ff9text.py dump TABLE`); [code] / [runtime] as in docs/naming.md. A name the table repeats gets the
 * command that lists it (`dump cmdabilities`) or a number. */

/* ---- characters: Character.info.b.id = GameData.characters index = (BtlUnit.flags >> 1) & 0xF for party units.
 * [text chrnames]: g_chrDefaultNames[g_chrDefaultSets[id][0]] (chr_getDefaultSet; slots 5-7 hold Cinna, Marcus and
 * Blank while Character.classFlags 0x10 is set, chr_init). */
#define CHR_ZIDANE  0
#define CHR_VIVI    1
#define CHR_DAGGER  2
#define CHR_STEINER 3
#define CHR_FREYA   4
#define CHR_QUINA   5
#define CHR_EIKO    6
#define CHR_AMARANT 7
#define CHR_BEATRIX 8
#define CHR_COUNT   9 /* GameData.characters entries (party_initNewGame, chr_setRecruited) */
/* stat class (chr_getClass: g_charClassStats; chr_getClassFlags: g_chrClassFlags). 0-7 are the CHR_* above; the guests
 * follow g_chrDefaultNames' order ([text chrnames] 8-11), which g_chrInitInfo confirms: slots 5-8 start on ability
 * rows 8, 10, 12, 14 with default sets 8-11 */
#define CHR_STATS_CINNA   8
#define CHR_STATS_MARCUS  9
#define CHR_STATS_BLANK   10
#define CHR_STATS_BEATRIX 11 /* the female guest class (g_chrClassFlags 0x16); rows 14/15 hold Beatrix's abilities */
/* ability row (Character.info.b.row: g_abilityRows, g_btlCharCmds). Rows 0-7 are the CHR_* above; each guest has two
 * (the second's use not known; g_chrInitInfo starts the guests on the first) */
#define CHR_ROW_CINNA     8
#define CHR_ROW_CINNA_2   9
#define CHR_ROW_MARCUS    10
#define CHR_ROW_MARCUS_2  11
#define CHR_ROW_BLANK     12
#define CHR_ROW_BLANK_2   13
#define CHR_ROW_BEATRIX   14
#define CHR_ROW_BEATRIX_2 15
#define CHR_ROW_EXTRA     16 /* 16-19: past the 16 g_abilityRows rows; classes Zidane, Cinna, Marcus, Blank */

/* ---- appearance kinds: Character.info.b.kind, the index of ovl_0b g_btlCharModelIds / g_btlPartyMotionIds and the
 * battle weapon bone (btl_initWeapon). [code] chr_calcAppearance (main) picks them from g_chrAppearanceTypes, the
 * weapon's ITEM_ICON_* and the story counter (docs/r32/3ds-requests-5-models.md section 3.1 has the models). */
#define CHR_KIND_ZIDANE_DAGGER        0  /* Zidane with ITEM_ICON_DAGGER */
#define CHR_KIND_ZIDANE               1  /* Zidane with other weapons (thief swords) */
#define CHR_KIND_VIVI                 2
#define CHR_KIND_GARNET_LONG          3  /* story < 10300 */
#define CHR_KIND_GARNET_LONG_RACKET   4  /* story < 10300, ITEM_ICON_RACKET */
#define CHR_KIND_GARNET_SHORT         5  /* story >= 10300 */
#define CHR_KIND_GARNET_SHORT_RACKET  6  /* story >= 10300, ITEM_ICON_RACKET */
#define CHR_KIND_STEINER              7
#define CHR_KIND_STEINER_DARK         8  /* btl_initBattleState: Steiner in GameData.darkBattle battles (model 286) */
#define CHR_KIND_QUINA                9
#define CHR_KIND_EIKO                 10
#define CHR_KIND_EIKO_RACKET          11 /* ITEM_ICON_RACKET */
#define CHR_KIND_FREYA                12
#define CHR_KIND_AMARANT              13
#define CHR_KIND_CINNA                14 /* guest in slot 5 */
#define CHR_KIND_MARCUS               15 /* guest in slot 6 */
#define CHR_KIND_BLANK                16 /* guest in slot 7 */
#define CHR_KIND_BLANK_ALT            17 /* guest in slot 7, story 1500-1599 */
#define CHR_KIND_BEATRIX              18
#define CHR_KIND_COUNT                19
#define CHR_KIND_NONE                 CHR_KIND_COUNT /* the kind ovl_0b gives a non-party unit (`party ? kind : 0x13`) */
#define CHR_KIND_TRANCE               0x13 /* g_btlCharModelIds[kind + this]: the trance model */

/* ---- items: GameData.items ids, Character.equip, BtlCmd.subNo of Item commands. [text items] */
#define ITEM_SLOT_COUNT 0x100 /* GameData.items slots */
#define ITEM_NONE       0xFF  /* empty item id / equipment slot */
#define ITEM_FIRST_USABLE 0xE0 /* items from here have a use entry in g_usableItemTable (index id - this) */
/* ItemInfo.icon (g_itemTable +8): the item kind, drawn as sprite 0x5C + it (text_buildBlock); weapon kinds are tested
 * by chr_calcAppearance and Throw (btl_startCmdEffect). Names from the items carrying them ([text items]). */
#define ITEM_ICON_HAMMER       0  /* Hammer */
#define ITEM_ICON_DAGGER       1  /* Dagger .. Orichalcon */
#define ITEM_ICON_THIEF_SWORD  2  /* Butterfly Sword .. Ultima Weapon */
#define ITEM_ICON_SWORD        3  /* Broadsword .. Defender */
#define ITEM_ICON_KNIGHT_SWORD 4  /* Save the Queen, Ultima Sword, Excalibur, Ragnarok, Excalibur II */
#define ITEM_ICON_SPEAR        5  /* Javelin .. Dragon's Hair */
#define ITEM_ICON_CLAW         6  /* Cat's Claws .. Rune Claws */
#define ITEM_ICON_RACKET       7  /* Air Racket .. Tiger Racket */
#define ITEM_ICON_ROD          8  /* Rod .. Whale Whisker */
#define ITEM_ICON_FLUTE        9  /* Golem's Flute .. Angel Flute */
#define ITEM_ICON_STAFF        10 /* Mage Staff .. Mace of Zeus */
#define ITEM_ICON_FORK         11 /* Fork .. Gastro Fork */
#define ITEM_ICON_THROWING     12 /* Pinwheel, Rising Sun, Wing Edge */
#define ITEM_HEALING_ROD    0x3C /* [text items] "Healing Rod" */
#define ITEM_MAIDEN_PRAYER  0xDE /* [text items] "Maiden Prayer" */
#define ITEM_GARNET         0xE0 /* [text items] "Garnet" */
#define ITEM_AMETHYST       0xE1 /* [text items] "Amethyst" */
#define ITEM_AQUAMARINE     0xE2 /* [text items] "Aquamarine" */
#define ITEM_DIAMOND        0xE3 /* [text items] "Diamond" */
#define ITEM_EMERALD        0xE4 /* [text items] "Emerald" */
#define ITEM_MOONSTONE      0xE5 /* [text items] "Moonstone" */
#define ITEM_PERIDOT        0xE7 /* [text items] "Peridot" */
#define ITEM_SAPPHIRE       0xE8 /* [text items] "Sapphire" */
#define ITEM_OPAL           0xE9 /* [text items] "Opal" */
#define ITEM_TOPAZ          0xEA /* [text items] "Topaz" */
#define ITEM_LAPIS_LAZULI   0xEB /* [text items] "Lapis Lazuli" */
#define ITEM_PHOENIX_PINION 0xF9 /* [text items] "Phoenix Pinion" */
#define ITEM_GYSAHL_GREENS  0xFB /* [text items] "Gysahl Greens" */
#define ITEM_ORE            0xFE /* [text items] "Ore" */

/* ---- key items: keyItem_has/add/remove ids (msg ids 0x100 + id). [text keyitems] */
#define KEY_ITEM_COUNT 0x50 /* key item ids (keyItem_has) */
/* code: one id space for items, key items and cards in the event script and messages (EVT_OP_ADD_ITEM, event
 * expression 0x64, msg_emitItemName): ids below 0x100 are items, then key items (id - ITEM_ID_KEY_FIRST), then
 * Tetra Master cards (id - ITEM_ID_CARD_FIRST) */
#define ITEM_ID_KEY_FIRST  0x100
#define ITEM_ID_CARD_FIRST 0x200
#define CARD_NONE 0xFF /* empty CardEntry.id (card_add, card_remove) */

/* ---- abilities: ids 0x00-0xBF are action abilities (BtlCmd.subNo, Battle.abilities), 0xC0-0xFF support abilities
 * (g_abilityRows, AbilityEntry.id). [text abilities] = the menus' action ability names (ovl_33
 * s_menuAbilityActionNames, the same texts as ovl_0b g_btlAbilityNames) */
#define ABILITY_MAX 0x30 /* entries in a character's g_abilityRows ability row (Character.ap) */
#define ABILITY_SUPPORT_FIRST 0xC0 /* ability ids from here are support abilities (g_supportAbilities) */
#define SUPPORT_COUNT 0x40
#define ABIL_METEOR            0x2E /* [text abilities] "Meteor" */
#define ABIL_SHIVA             0x31 /* [text abilities] "Shiva" */
#define ABIL_IFRIT             0x33 /* [text abilities] "Ifrit" */
#define ABIL_RAMUH             0x35 /* [text abilities] "Ramuh" */
#define ABIL_ATOMOS            0x37 /* [text abilities] "Atomos" */
#define ABIL_ODIN              0x3A /* [text abilities] "Odin" */
#define ABIL_LEVIATHAN         0x3C /* [text abilities] "Leviathan" */
#define ABIL_BAHAMUT           0x3E /* [text abilities] "Bahamut" */
#define ABIL_ARK               0x40 /* [text abilities] "Ark" */
#define ABIL_FENRIR            0x42 /* [text abilities] "Fenrir"; Summon2 list */
#define ABIL_FENRIR2           0x43 /* [text abilities] "Fenrir"; second entry, in no command list: Fenrir with
                                    * ITEM_MAIDEN_PRAYER equipped (btlui_resolveAbility) */
#define ABIL_CARBUNCLE         0x44 /* [text abilities] "Carbuncle"; Summon list */
#define ABIL_CARBUNCLE_EMERALD 0x45 /* [text abilities] "Carbuncle"; Carbuncle with ITEM_EMERALD equipped (btlui_resolveAbility) */
#define ABIL_CARBUNCLE_MOONSTONE 0x46 /* [text abilities] "Carbuncle"; with ITEM_MOONSTONE equipped */
#define ABIL_CARBUNCLE_DIAMOND 0x47 /* [text abilities] "Carbuncle"; with ITEM_DIAMOND equipped */
#define ABIL_PHOENIX           0x48 /* [text abilities] "Phoenix" */
#define ABIL_REBIRTH_FLAME     0x49 /* [text abilities] "Rebirth Flame" */
#define ABIL_MADEEN            0x4A /* [text abilities] "Madeen" */
#define ABIL_ROULETTE          0x52 /* [text abilities] "Roulette" */
#define ABIL_ANGELS_SNACK      0x5D /* [text abilities] "Angel's Snack" */
#define ABIL_SPARE_CHANGE      0x7E /* [text abilities] "Spare Change"; Flair list */
#define ABIL_CURSE             0x81 /* [text abilities] "Curse"; Flair list */
#define ABIL_SPARE_CHANGE_ELAN 0x86 /* [text abilities] "Spare Change"; Elan list */
#define ABIL_CURSE_ELAN        0x89 /* [text abilities] "Curse"; Elan list */
#define ABIL_DIAMOND_DUST      0x99 /* [text abilities] "Diamond Dust" */
#define ABIL_FLAMES_OF_HELL    0x9A /* [text abilities] "Flames of Hell" */
#define ABIL_JUDGEMENT_BOLT    0x9B /* [text abilities] "Judgement Bolt" */
#define ABIL_WORM_HOLE         0x9C /* [text abilities] "Worm Hole" */
#define ABIL_ZANTETSUKEN       0x9D /* [text abilities] "Zantetsuken" */
#define ABIL_TSUNAMI           0x9E /* [text abilities] "Tsunami" */
#define ABIL_MEGA_FLARE        0x9F /* [text abilities] "Mega Flare" */
#define ABIL_ETERNAL_DARKNESS  0xA0 /* [text abilities] "Eternal Darkness" */
#define ABIL_ATTACK            0xB0 /* [text abilities] "Attack" */
#define ABIL_FLEE              0xB4 /* [text abilities] "Flee"; Flee command (ABIL_FLEE_SKILL 0x65 is the Skill one) */
#define ABIL_MUG               0xB5 /* [text abilities] "Mug" */
#define ABIL_SPEAR             0xB9 /* [text abilities] "Spear"; code: BTL_CMD_SPEAR casts it after Jump */
#define ABIL_SPEAR_TRANCE      0xBA /* [text abilities] "Spear"; code: BTL_CMD_SPEAR_TRANCE casts it after the trance Jump */
#define ABIL_TERRA_HOMING      0xBB /* [text abilities] "Terra Homing" */
#define ABIL_THROW             0xBE /* [text abilities] "Throw" */

/* ---- support abilities: ids 0xC0-0xFF. [text support] (g_supportAbilities name offsets into the menus' support
 * name text). Character.supportAbilities / BtlUnit.support hold them as bits: word (id - 0xC0) >> 5,
 * bit (id - 0xC0) & 0x1F (chr_testSupportBit). */
#define SA_BIT(id) ((u32)1 << (((id) - ABILITY_SUPPORT_FIRST) & 0x1F))
#define SA_AUTO_REFLECT   0xC0 /* [text support] "Auto-Reflect" */
#define SA_AUTO_FLOAT     0xC1 /* [text support] "Auto-Float" */
#define SA_AUTO_HASTE     0xC2 /* [text support] "Auto-Haste" */
#define SA_AUTO_REGEN     0xC3 /* [text support] "Auto-Regen" */
#define SA_AUTO_LIFE      0xC4 /* [text support] "Auto-Life" */
#define SA_HP_10          0xC5 /* [text support] "HP+10%" */
#define SA_HP_20          0xC6 /* [text support] "HP+20%" */
#define SA_MP_10          0xC7 /* [text support] "MP+10%" */
#define SA_MP_20          0xC8 /* [text support] "MP+20%" */
#define SA_ACCURACY_PLUS  0xC9 /* [text support] "Accuracy+" */
#define SA_DISTRACT       0xCA /* [text support] "Distract" */
#define SA_LONG_REACH     0xCB /* [text support] "Long Reach" */
#define SA_MP_ATTACK      0xCC /* [text support] "MP Attack" */
#define SA_BIRD_KILLER    0xCD /* [text support] "Bird Killer"; BTL_ENEMY_AERIAL (btl_applyKillerBonus) */
#define SA_BUG_KILLER     0xCE /* [text support] "Bug Killer"; BTL_ENEMY_INSECT */
#define SA_STONE_KILLER   0xCF /* [text support] "Stone Killer"; BTL_ENEMY_STONE */
#define SA_UNDEAD_KILLER  0xD0 /* [text support] "Undead Killer"; BTL_ENEMY_UNDEAD */
#define SA_DRAGON_KILLER  0xD1 /* [text support] "Dragon Killer"; BTL_ENEMY_DRAGON */
#define SA_DEVIL_KILLER   0xD2 /* [text support] "Devil Killer"; BTL_ENEMY_DEMON */
#define SA_BEAST_KILLER   0xD3 /* [text support] "Beast Killer"; BTL_ENEMY_BEAST */
#define SA_MAN_EATER      0xD4 /* [text support] "Man Eater"; BTL_ENEMY_HUMANOID */
#define SA_HIGH_JUMP      0xD5 /* [text support] "High Jump" */
#define SA_MASTER_THIEF   0xD6 /* [text support] "Master Thief" */
#define SA_STEAL_GIL      0xD7 /* [text support] "Steal Gil" */
#define SA_HEALER         0xD8 /* [text support] "Healer" */
#define SA_ADD_STATUS     0xD9 /* [text support] "Add Status" */
#define SA_GAMBLE_DEFENSE 0xDA /* [text support] "Gamble Defense" */
#define SA_CHEMIST        0xDB /* [text support] "Chemist" */
#define SA_POWER_THROW    0xDC /* [text support] "Power Throw" */
#define SA_POWER_UP       0xDD /* [text support] "Power Up" */
#define SA_REFLECT_NULL   0xDE /* [text support] "Reflect-Null" */
#define SA_MAG_ELEM_NULL  0xE0 /* [text support] "Mag Elem Null" */
#define SA_CONCENTRATE    0xE1 /* [text support] "Concentrate" */
#define SA_HALF_MP        0xE2 /* [text support] "Half MP" */
#define SA_HIGH_TIDE      0xE3 /* [text support] "High Tide" */
#define SA_COUNTER        0xE4 /* [text support] "Counter" */
#define SA_COVER          0xE5 /* [text support] "Cover" */
#define SA_PROTECT_GIRLS  0xE6 /* [text support] "Protect Girls" */
#define SA_EYE_4_EYE      0xE7 /* [text support] "Eye 4 Eye" */
#define SA_BODY_TEMP      0xE8 /* [text support] "Body Temp" */
#define SA_ALERT          0xE9 /* [text support] "Alert" */
#define SA_INITIATIVE     0xEA /* [text support] "Initiative" */
#define SA_LEVEL_UP       0xEB /* [text support] "Level Up" */
#define SA_ABILITY_UP     0xEC /* [text support] "Ability Up" */
#define SA_MILLIONAIRE    0xED /* [text support] "Millionaire" */
#define SA_FLEE_GIL       0xEE /* [text support] "Flee-Gil" */
#define SA_GUARDIAN_MOG   0xEF /* [text support] "Guardian Mog" */
#define SA_INSOMNIAC      0xF0 /* [text support] "Insomniac" */
#define SA_ANTIBODY       0xF1 /* [text support] "Antibody" */
#define SA_BRIGHT_EYES    0xF2 /* [text support] "Bright Eyes" */
#define SA_LOUDMOUTH      0xF3 /* [text support] "Loudmouth" */
#define SA_RESTORE_HP     0xF4 /* [text support] "Restore HP" */
#define SA_JELLY          0xF5 /* [text support] "Jelly" */
#define SA_RETURN_MAGIC   0xF6 /* [text support] "Return Magic" */
#define SA_ABSORB_MP      0xF7 /* [text support] "Absorb MP" */
#define SA_AUTO_POTION    0xF8 /* [text support] "Auto-Potion" */
#define SA_LOCOMOTION     0xF9 /* [text support] "Locomotion" */
#define SA_CLEAR_HEADED   0xFA /* [text support] "Clear Headed" */
#define SA_BOOST          0xFB /* [text support] "Boost" */
#define SA_ODINS_SWORD    0xFC /* [text support] "Odin's Sword" */
#define SA_MUG            0xFD /* [text support] "Mug" */
#define SA_BANDIT         0xFE /* [text support] "Bandit" */

/* ---- battle commands: BtlCmd.cmdNo. 0x00-0x2F index the command table g_btlCmdTable; [text commands] = its menu
 * texts (g_btlCmdNames; a repeated text gets a number). Ids from 0x30 up are queued by the battle code itself. */
#define BTL_CMD_NONE          0x00
#define BTL_CMD_ATTACK        0x01 /* runtime: Attack (ability 0xB0) */
#define BTL_CMD_STEAL         0x02 /* ability 0xB1 Steal (0xB5 Mug with a support bit, btl_setCmd) */
#define BTL_CMD_JUMP          0x03 /* runtime: Jump; ability 0xB2 */
#define BTL_CMD_DEFEND        0x04 /* runtime: Defend */
#define BTL_CMD_FLEE          0x05 /* runtime: Flee */
#define BTL_CMD_CHANGE        0x07 /* runtime: Change (row) */
#define BTL_CMD_EAT           0x08 /* runtime: Eat */
#define BTL_CMD_COOK          0x09
#define BTL_CMD_SPEAR         0x0A /* code: the landing of Jump (cmd 3) queues it with ability 0xB9 Spear (text "None") */
#define BTL_CMD_SPEAR_TRANCE  0x0B /* code: the landing of cmd 0xC queues it with ability 0xBA Spear (text "Jump") */
#define BTL_CMD_JUMP_TRANCE   0x0C /* table text Jump, ability 0xAC */
#define BTL_CMD_FOCUS         0x0D /* runtime: Focus */
#define BTL_CMD_ITEM          0x0E
#define BTL_CMD_THROW         0x0F /* runtime: Throw */
#define BTL_CMD_SUMMON        0x10 /* code: btl_rollShortSummon handles 0x10/0x12/0x14 */
#define BTL_CMD_WHT_MAG       0x11 /* runtime: Wht Mag */
#define BTL_CMD_EIDOLON       0x12
#define BTL_CMD_WHT_MAG2      0x13
#define BTL_CMD_SUMMON2       0x14
#define BTL_CMD_DBL_WHT       0x15
#define BTL_CMD_BLK_MAG       0x16 /* runtime: Blk Mag */
#define BTL_CMD_DBL_BLK       0x17
#define BTL_CMD_BLU_MAG       0x18 /* runtime: Blu Mag */
#define BTL_CMD_SKILL         0x19 /* runtime: Skill */
#define BTL_CMD_DYNE          0x1A
#define BTL_CMD_DRAGON        0x1B
#define BTL_CMD_FLAIR         0x1C
#define BTL_CMD_ELAN          0x1D
#define BTL_CMD_SWD_ART       0x1E
#define BTL_CMD_SWD_MAG       0x1F
#define BTL_CMD_SEIKEN        0x20
#define BTL_CMD_SEIKEN2       0x21
#define BTL_CMD_WHT_MAG3      0x22
#define BTL_CMD_ENEMY_TURN    0x2F /* runtime: the enemy AI's command (event-script trigger 0x2F, btl_setEnemyCmd) */
#define BTL_CMD_TABLE_COUNT   0x30 /* entries in g_btlCmdTable; lower ids are chosen commands, higher ones automatic */
#define BTL_CMD_COUNTER       0x31 /* runtime: Counter queues it with Attack against the attacker (btl_tryCounter) */
#define BTL_CMD_RETURN_MAGIC  0x32 /* code: Return Magic queues it with the spell that hit (btl_tryCounter) */
#define BTL_CMD_AUTO_POTION   0x33 /* code: btl_tryAutoPotion queues it with the item; handled like Item */
#define BTL_CMD_CHARGE_ATTACK 0x34 /* code: formula 0x3D (ability Charge!) queues an Attack by each low-HP party unit */
#define BTL_CMD_ENEMY_COUNTER 0x35 /* code: enemy command from the event script (trigger 0x35 = hit) */
#define BTL_CMD_ENEMY_DEATH   0x36 /* code: enemy command from the event script (trigger 0x36 = killed) */
#define BTL_CMD_ENEMY_REACT   0x37 /* code: btl_startCmdEffect plays a sequence for it like 0x2F/0x35/0x36 */
#define BTL_CMD_ESCAPE        0x38 /* runtime: queued by btl_rollFlee / btl_tryEscape; ability 0xB4 Flee */
#define BTL_CMD_TRANCE_EIDOLON 0x39 /* code: trance Eidolon timer (btl_updateUnitStatus) casts Battle.eidolonAbility */
#define BTL_CMD_REBIRTH_FLAME 0x3A /* code: the item 0xF9 chance in btl_checkSideDefeated, ability 0x49 Rebirth Flame */
#define BTL_CMD_TRANCE        0x3B /* code: adding/removing BTL_STATUS_TRANCE queues it; effect 0x101 in trance */
#define BTL_CMD_KO            0x3C /* code: adding BTL_STATUS_KO queues it (btl_addStatus) */
#define BTL_CMD_AUTO_LIFE     0x3D /* code: KO with BTL_STATUS_AUTO_LIFE queues it */
#define BTL_CMD_PETRIFY       0x3E /* code: adding BTL_STATUS_PETRIFY during combat queues it */

/* ---- battle formulas: ovl_0b BtlCmd8.formula / Unk8006B1F4.formula (g_usableItemTable) / weapon record +4, the
 * damage/effect formula of ovl_0b btl_calcCmd; main item_useOnMember runs the field versions of the item ones.
 * Evidence: [code]
 * (each name says what btl_calcCmd's case computes); the users in parentheses are the ability (ovl_33 action table
 * +4), usable item (g_usableItemTable +4) and weapon (g_weaponTable +4) records carrying it, decoded with
 * tools/ff9text.py. Formulas no player record uses (0x08, 0x58-0x6B) are enemy abilities'. Weapon formulas 1-7 are
 * also btl_setPhysParams' multiplier kind. 0x2F, 0x40, 0x4E-0x52 have no case (no effect). */
#define BTL_FORMULA_WEAPON_STR         0x01 /* weapon attack, multiplier strength + rand (swords, daggers, spears, claws, rods, flutes, staffs) */
#define BTL_FORMULA_WEAPON_STR_SPR     0x02 /* multiplier (strength + spirit) / 2 (Zidane's thief swords and Steiner's later swords) */
#define BTL_FORMULA_WEAPON_STR_SPD     0x03 /* multiplier (strength + speed) / 2 (rackets) */
#define BTL_FORMULA_WEAPON_STR_MAG     0x04 /* multiplier (strength + magic) / 2 (no weapon) */
#define BTL_FORMULA_WEAPON_RANDOM      0x05 /* multiplier rand % strength + rand (Quina's forks) */
#define BTL_FORMULA_WEAPON_DRAIN       0x06 /* weapon damage the caster drains (Blood Sword) */
#define BTL_FORMULA_WEAPON_STR_LEVEL   0x07 /* as WEAPON_STR, power + caster level (Save the Queen) */
#define BTL_FORMULA_PHYSICAL           0x08 /* ability power vs defense, multiplier strength + rand (level + str) / 4, ability element/status */
#define BTL_FORMULA_MAGIC              0x09 /* ability power vs magic defense, multiplier magic (Fire, Holy, Doomsday, Diamond Dust) */
#define BTL_FORMULA_MAGIC_HEAL         0x0A /* healing magic (btl_calcHeal: Cure, Cura, Curaga) */
#define BTL_FORMULA_ADD_STATUS         0x0B /* status roll, halved by Shell and spread (Silence, Sleep, Stop, Doom, Bad Breath) */
#define BTL_FORMULA_CURE_STATUS        0x0C /* removes the ability's statuses (Panacea, Stona, Esuna, Dispel) */
#define BTL_FORMULA_REVIVE             0x0D /* revives a party member with maxHp * (spirit + power) / 100; kills undead (Life, Full-Life, Revive, Rebirth Flame) */
#define BTL_FORMULA_DEATH              0x0E /* status roll; undead are fully healed (Death) */
#define BTL_FORMULA_OSMOSE             0x0F /* MP drain, damage / 4 (Osmose) */
#define BTL_FORMULA_DRAIN              0x10 /* HP drain magic (Drain) */
#define BTL_FORMULA_PERCENT_MAX_HP     0x11 /* power % of max HP after a status roll (Demi, Aqua Breath, Thunder Slash, Worm Hole) */
#define BTL_FORMULA_RANDOM_MAGIC       0x12 /* multiplier rand % (magic + level), no defense; effect 0x38 misses 1/3 (Comet, Meteor, Twister) */
#define BTL_FORMULA_WEAPON_SKILL       0x13 /* weapon attack * power / 10 vs defense, weapon element (Free Energy, Shock, Stock Break, No Mercy) */
#define BTL_FORMULA_WEAPON_SKILL_MDEF  0x14 /* weapon attack * power / 10 vs magic defense (Cherry Blossom, Climhazzard) */
#define BTL_FORMULA_GOBLIN_PUNCH       0x15 /* magic damage, + level and no defense when the levels are equal (Goblin Punch) */
#define BTL_FORMULA_LEVEL_SET_HP       0x16 /* target level a multiple of rate: btl_setTargetHp (LV5 Death) */
#define BTL_FORMULA_LEVEL_MAGIC        0x17 /* target level a multiple of rate: magic damage (LV4 Holy) */
#define BTL_FORMULA_LEVEL_DEFLESS      0x18 /* target level a multiple of rate: defense and magic defense to rand % themselves (LV3 Def-less) */
#define BTL_FORMULA_SET_HP             0x19 /* btl_setTargetHp, no roll (Roulette) */
#define BTL_FORMULA_FIXED_DAMAGE       0x1A /* power * 100 + rate damage (1,000 Needles; Pyro, Medeo, Poly) */
#define BTL_FORMULA_SET_HP_ROLL        0x1B /* btl_setTargetHp after a status roll (Matra Magic) */
#define BTL_FORMULA_FIXED_DAMAGE_1HP   0x1C /* FIXED_DAMAGE only while the caster has 1 HP (Limit Glove) */
#define BTL_FORMULA_HP_LOST            0x1D /* damage = caster maxHp - hp (Pumpkin Head, Minus Strike) */
#define BTL_FORMULA_THIRD_MAX_HP_HEAL  0x1E /* heals caster maxHp / 3 (White Wind) */
#define BTL_FORMULA_MP_DAMAGE_RANDOM   0x1F /* MP damage rand % target mp (Magic Hammer) */
#define BTL_FORMULA_DARKSIDE           0x20 /* weapon skill damage, caster loses maxHp / 8 (Darkside) */
#define BTL_FORMULA_ARMOR_BREAK        0x21 /* status roll: defense / 2 (Armor Break) */
#define BTL_FORMULA_POWER_BREAK        0x22 /* status roll: strength * 3 / 4 (Power Break) */
#define BTL_FORMULA_MENTAL_BREAK       0x23 /* status roll: magic defense / 2 (Mental Break) */
#define BTL_FORMULA_MAGIC_BREAK        0x24 /* status roll: magic * 3 / 4 (Magic Break) */
#define BTL_FORMULA_PERCENT_HEAL       0x25 /* heals power % of max HP and MP, x2 with Power Up (Chakra) */
#define BTL_FORMULA_SPARE_CHANGE       0x26 /* damage from power * level and gil, split by Elan (Spare Change) */
#define BTL_FORMULA_LANCER             0x27 /* weapon skill damage to HP and / 16 to MP (Lancer) */
#define BTL_FORMULA_DRAGON_BREATH      0x28 /* the target's lost HP as damage to every enemy (Dragon Breath) */
#define BTL_FORMULA_WHITE_DRAW         0x29 /* rand % (2 * target level) MP to every party member (White Draw) */
#define BTL_FORMULA_THROW              0x2A /* thrown weapon's attack * 2, x1.5 with Power Throw and on aerial enemies (Throw) */
#define BTL_FORMULA_STRENGTH_UP        0x2B /* strength += strength / power (Might) */
#define BTL_FORMULA_MAGIC_UP           0x2C /* magic += magic / power (Focus) */
#define BTL_FORMULA_SACRIFICE          0x2D /* fully heals the target, the caster dies (Sacrifice) */
#define BTL_FORMULA_SOUL_BLADE         0x2E /* adds the status set of the equipped thief sword (items 7-15, else miss) (Soul Blade) */
#define BTL_FORMULA_SPEAR              0x30 /* jump attack, multiplier x1.5 (x2 with High Jump) (Spear) */
#define BTL_FORMULA_PHOENIX            0x31 /* Rebirth Flame (REVIVE) on party targets, Phoenix (MAGIC) on enemies (Phoenix) */
#define BTL_FORMULA_SIX_DRAGONS        0x32 /* random: full HP/MP, HP or MP to 1 (Six Dragons) */
#define BTL_FORMULA_CURSE              0x33 /* makes the target weak to the ability's element (Curse) */
#define BTL_FORMULA_USE_ITEM           0x34 /* uses item `power` with its own formula (Angel's Snack) */
#define BTL_FORMULA_LUCKY_SEVEN        0x35 /* 7/77/777/7777 when caster hp ends in 7, else 1 (Lucky Seven) */
#define BTL_FORMULA_WHATS_THAT         0x36 /* turns the target round (What's That!?) */
#define BTL_FORMULA_CHANGE_ROW         0x37 /* btl_toggleRow (Change) */
#define BTL_FORMULA_FLEE               0x38 /* escape roll (Flee command; btl_tryEscape) */
#define BTL_FORMULA_FLEE_SKILL         0x39 /* sure escape: queues BTL_CMD_ESCAPE (Flee skill) */
#define BTL_FORMULA_STEAL              0x3A /* (Steal, Mug) */
#define BTL_FORMULA_SCAN               0x3B /* (Scan) */
#define BTL_FORMULA_DETECT             0x3C /* (Detect) */
#define BTL_FORMULA_CHARGE             0x3D /* every low-HP party member attacks (Charge!) */
#define BTL_FORMULA_SOFT               0x3E /* kills stone enemies, else cures the item's statuses (Soft) */
#define BTL_FORMULA_SWORD_MAGIC        0x3F /* weapon attack + power vs defense, ability element (Steiner's Sword Magic) */
#define BTL_FORMULA_EAT                0x41 /* kills a weakened enemy, learns its eat ability (Eat, Cook) */
#define BTL_FORMULA_FROG_DROP          0x42 /* frogCount * level (Frog Drop) */
#define BTL_FORMULA_THIEVERY           0x43 /* stealCount * speed / 2 (Thievery) */
#define BTL_FORMULA_DRAGONS_CREST      0x44 /* dragonKills squared (Dragon's Crest) */
#define BTL_FORMULA_ITEM_HEAL          0x45 /* item power * 15 (30 with Chemist) HP (Potion, Hi-Potion) */
#define BTL_FORMULA_ITEM_MP_HEAL       0x46 /* the same for MP (Ether) */
#define BTL_FORMULA_ELIXIR             0x47 /* full HP and MP; kills undead (Elixir) */
#define BTL_FORMULA_PHOENIX_DOWN       0x48 /* revives with 1-10 HP; undead to 0-9 HP (Phoenix Down, Phoenix Pinion) */
#define BTL_FORMULA_ITEM_CURE_STATUS   0x49 /* removes the item's statuses (Antidote, Remedy, Echo Screen, ...) */
#define BTL_FORMULA_ITEM_COUNT_HEAL    0x4A /* heals item power * (count + 1) (the gems, Ore) */
#define BTL_FORMULA_ITEM_COUNT_DAMAGE  0x4B /* damages item power * (count + 1) (Dead Pepper) */
#define BTL_FORMULA_TENT               0x4C /* half max HP/MP heal, may add the item's statuses (Tent) */
#define BTL_FORMULA_9999_DAMAGE        0x4D /* 9999 damage (Dark Matter) */
#define BTL_FORMULA_SPEAR_TRANCE       0x53 /* SPEAR with the WEAPON_RANDOM multiplier, damage split by the active units (Spear in trance) */
#define BTL_FORMULA_JEWEL              0x54 /* status roll: adds an Ore (Jewel) */
#define BTL_FORMULA_SUMMON             0x55 /* magic damage + the summon's gem count, Madeen + level (Shiva, Ifrit, Bahamut, Ark, Terra Homing) */
#define BTL_FORMULA_ATOMOS             0x56 /* PERCENT_MAX_HP + Amethyst count (Atomos) */
#define BTL_FORMULA_ODIN               0x57 /* death roll + Ore count / 2; Odin's Sword turns a miss into damage (Odin) */
#define BTL_FORMULA_SELF_DESTRUCT      0x58 /* the caster dies, its HP as damage */
#define BTL_FORMULA_SWAP_HP            0x59 /* swaps caster and target HP (each capped at its max) */
#define BTL_FORMULA_HALVE_DEFENSES     0x5A /* defense and magic defense / 2 */
#define BTL_FORMULA_PERCENT_CUR_HP     0x5B /* as PERCENT_MAX_HP on current HP */
#define BTL_FORMULA_ADD_ITEM           0x5C /* adds item `power` */
#define BTL_FORMULA_LOW_HP_STATUS      0x5D /* hp to 1-9 and the ability's status set */
#define BTL_FORMULA_ABSORB_MAGIC       0x5E /* target magic / 2, caster magic x2 */
#define BTL_FORMULA_ABSORB_STRENGTH    0x5F /* target strength / 2, caster strength x2 */
#define BTL_FORMULA_ADD_TRANCE         0x60 /* fills the trance gauge, adds Trance */
#define BTL_FORMULA_STATUS_NOT_FEMALE  0x61 /* adds the statuses unless CHR_CLASS_FEMALE */
#define BTL_FORMULA_PHYSICAL_CLASS8    0x62 /* PHYSICAL power, magic finish; misses class flag 0x8 */
#define BTL_FORMULA_LEVEL_DAMAGE       0x63 /* status roll: target level * power damage */
#define BTL_FORMULA_ENEMY_ATTACK       0x64 /* enemy physical attack (Frozen shatters) */
#define BTL_FORMULA_ENEMY_STEAL        0x65 /* hit roll: steals item `rate` */
#define BTL_FORMULA_ENEMY_ATTACK_STEAL 0x66 /* enemy physical attack, then steals item `rate` */
#define BTL_FORMULA_ADD_STATUS_SURE    0x67 /* adds the statuses, no roll (Protect, Shell, Haste, Regen, Carbuncle, Defend) */
#define BTL_FORMULA_EVT_POWER_DAMAGE   0x68 /* 1 << (event global byte 0xC0 - 1) damage, 9999 from 14 */
#define BTL_FORMULA_RANDOM_STATUS      0x69 /* each status bit at 1/8 */
#define BTL_FORMULA_REMOVE_UNIT        0x6A /* the party member leaves the battle (no exp, saved, unlinked) */
#define BTL_FORMULA_ENEMY_ATTACK_ROW   0x6B /* ENEMY_ATTACK without the shatter check; toggles the row of a target whose BTL_UNIT_ROW is set */
#define BTL_FORMULA_ADD_STATUS_GUARD   0x6C /* ADD_STATUS, death immunity guards (Iai Strike) */
#define BTL_FORMULA_MINI               0x6D /* ADD_STATUS, no roll when the target is already Mini (Mini) */

/* ---- status: BtlUnit.status bits (docs/runtime-findings.md "Battle STATUS bits"). BtlUnit.autoStatus uses the same
 * bits (btl_hasStatus tests both words). The low 7 bits are also Character.status, the statuses that last after
 * battle (btl_init copies them in). ui_drawStatusIcons draws g_statusIconSprites[k] for bit k; ovl_38's status
 * help (g_statusMenuText 0) names its icon arguments 0-6, set to the same sprites (s_menuStatusHelpArgs):
 * [text] "Petrified", "Venom", "Virus", "Silence", "Darkness", "Trouble", "Zombie". */
#define BTL_STATUS_PETRIFY         0x00000001 /* runtime: Break; model CLUT turns to stone (btl_setStoneClut) */
#define BTL_STATUS_VENOM           0x00000002 /* code: the tick drains HP and MP (btl_tickPoison, btl_tickVenomMp) */
#define BTL_STATUS_VIRUS           0x00000004 /* text: status help line 2 (see above) */
#define BTL_STATUS_SILENCE         0x00000008 /* runtime: Silence */
#define BTL_STATUS_BLIND           0x00000010 /* runtime: Blind; status help "Darkness" */
#define BTL_STATUS_TROUBLE         0x00000020 /* code: btl_spreadTroubleDamage splashes half the damage on allies */
#define BTL_STATUS_ZOMBIE          0x00000040 /* code: healing hurts, drain reversed */
#define BTL_STATUS_DEATH_IMMUNE    0x00000080 /* code: instant death/gravity formulas fail on it */
#define BTL_STATUS_KO              0x00000100 /* runtime: set when the monster died */
#define BTL_STATUS_LOW_HP          0x00000200 /* added when hp * 6 < maxHp */
#define BTL_STATUS_CONFUSE         0x00000400 /* runtime: Confuse */
#define BTL_STATUS_BERSERK         0x00000800 /* runtime: Berserk */
#define BTL_STATUS_STOP            0x00001000 /* runtime: Stop */
#define BTL_STATUS_AUTO_LIFE       0x00002000 /* runtime: Auto-Life; KO queues the revive command 0x3D */
#define BTL_STATUS_TRANCE          0x00004000 /* runtime */
#define BTL_STATUS_DEFEND          0x00008000 /* runtime: removed by btl_setCmd at every action */
#define BTL_STATUS_POISON          0x00010000 /* runtime: Poison */
#define BTL_STATUS_SLEEP           0x00020000 /* runtime: Sleep; timer at +0x48 (unk46[1]) */
#define BTL_STATUS_REGEN           0x00040000 /* runtime: the Regen spell adds it (btl_addStatus); timer at +0x44 (unk40[2]) */
#define BTL_STATUS_HASTE           0x00080000 /* runtime: Haste */
#define BTL_STATUS_SLOW            0x00100000 /* runtime: Slow */
#define BTL_STATUS_FLOAT           0x00200000 /* runtime: Float */
#define BTL_STATUS_SHELL           0x00400000 /* runtime: Shell */
#define BTL_STATUS_PROTECT         0x00800000 /* runtime: Protect */
#define BTL_STATUS_HEAT            0x01000000 /* code: acting while it is set adds KO (btl_processCmdQueue) */
#define BTL_STATUS_FREEZE          0x02000000 /* runtime */
#define BTL_STATUS_VANISH          0x04000000 /* runtime: Vanish; hides the meshes (btl_hideMeshes) */
#define BTL_STATUS_DOOM            0x08000000 /* code: countdown number (btl_drawStatusCountdown) */
#define BTL_STATUS_MINI            0x10000000 /* runtime: Mini */
#define BTL_STATUS_REFLECT         0x20000000 /* runtime: Reflect */
#define BTL_STATUS_JUMP            0x40000000 /* code: in the air (btl_checkCmdUsable) */
#define BTL_STATUS_GRADUAL_PETRIFY 0x80000000 /* code: countdown number (btl_drawStatusCountdown) */
/* status bit indexes (mask = 1 << index): Battle.statusInfo[], btl_addStatus' switch, btl_initDotTimer */
#define BTL_STATUS_PETRIFY_BIT         0
#define BTL_STATUS_VENOM_BIT           1
#define BTL_STATUS_VIRUS_BIT           2
#define BTL_STATUS_SILENCE_BIT         3
#define BTL_STATUS_BLIND_BIT           4
#define BTL_STATUS_TROUBLE_BIT         5
#define BTL_STATUS_ZOMBIE_BIT          6
#define BTL_STATUS_DEATH_IMMUNE_BIT    7
#define BTL_STATUS_KO_BIT              8
#define BTL_STATUS_LOW_HP_BIT          9
#define BTL_STATUS_CONFUSE_BIT         10
#define BTL_STATUS_BERSERK_BIT         11
#define BTL_STATUS_STOP_BIT            12
#define BTL_STATUS_AUTO_LIFE_BIT       13
#define BTL_STATUS_TRANCE_BIT          14
#define BTL_STATUS_DEFEND_BIT          15
#define BTL_STATUS_POISON_BIT          16
#define BTL_STATUS_SLEEP_BIT           17
#define BTL_STATUS_REGEN_BIT           18
#define BTL_STATUS_HASTE_BIT           19
#define BTL_STATUS_SLOW_BIT            20
#define BTL_STATUS_FLOAT_BIT           21
#define BTL_STATUS_SHELL_BIT           22
#define BTL_STATUS_PROTECT_BIT         23
#define BTL_STATUS_HEAT_BIT            24
#define BTL_STATUS_FREEZE_BIT          25
#define BTL_STATUS_VANISH_BIT          26
#define BTL_STATUS_DOOM_BIT            27
#define BTL_STATUS_MINI_BIT            28
#define BTL_STATUS_REFLECT_BIT         29
#define BTL_STATUS_JUMP_BIT            30
#define BTL_STATUS_GRADUAL_PETRIFY_BIT 31
#define BTL_STATUS_TIMED_FIRST_BIT     BTL_STATUS_POISON_BIT /* bits from here have a timer: BtlUnit.statusTimers[bit - this] */
#define BTL_STATUS_TIMED 0xEFFF0000 /* code: the statuses whose timer runs out (bits 16-31 but Mini): btl_addStatus sets
                                     * it, btl_tickStatusTimers counts it down */
/* Character.status: the same bits */
#define CHR_STATUS_PETRIFY BTL_STATUS_PETRIFY
#define CHR_STATUS_VENOM   BTL_STATUS_VENOM
#define CHR_STATUS_SILENCE BTL_STATUS_SILENCE
#define CHR_STATUS_ZOMBIE  BTL_STATUS_ZOMBIE

/* ---- elements: bits of BtlUnit.elem*, BtlCmd8.element, weapon[6]. [text messages] g_btlMessages entries 1-8: the
 * names btl_getElementName returns for bit n - 1. */
#define ELEM_FIRE    0x01
#define ELEM_ICE     0x02
#define ELEM_THUNDER 0x04
#define ELEM_EARTH   0x08 /* misses Float and flying enemies (btl_missIfAirborne) */
#define ELEM_WATER   0x10
#define ELEM_WIND    0x20
#define ELEM_HOLY    0x40
#define ELEM_SHADOW  0x80

#endif
