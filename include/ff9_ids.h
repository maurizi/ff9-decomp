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

/* ---- items: GameData.items ids, Character.equip, BtlCmd.subNo of Item commands. [text items] */
#define ITEM_SLOT_COUNT 0x100 /* GameData.items slots */
#define ITEM_NONE       0xFF  /* empty item id / equipment slot */
#define ITEM_FIRST_USABLE 0xE0 /* items from here have a use entry in g_usableItemTable (index id - this) */
#define ITEM_HEALING_ROD    0x3C /* [text items] "Healing Rod" */
#define ITEM_MAIDEN_PRAYER  0xDE /* [text items] "Maiden Prayer" */
#define ITEM_GARNET         0xE0 /* [text items] "Garnet" */
#define ITEM_AMETHYST       0xE1 /* [text items] "Amethyst" */
#define ITEM_AQUAMARINE     0xE2 /* [text items] "Aquamarine" */
#define ITEM_PERIDOT        0xE7 /* [text items] "Peridot" */
#define ITEM_SAPPHIRE       0xE8 /* [text items] "Sapphire" */
#define ITEM_OPAL           0xE9 /* [text items] "Opal" */
#define ITEM_TOPAZ          0xEA /* [text items] "Topaz" */
#define ITEM_LAPIS_LAZULI   0xEB /* [text items] "Lapis Lazuli" */
#define ITEM_PHOENIX_PINION 0xF9 /* [text items] "Phoenix Pinion" */
#define ITEM_ORE            0xFE /* [text items] "Ore" */

/* ---- key items: keyItem_has/add/remove ids (msg ids 0x100 + id). [text keyitems] */
#define KEY_ITEM_COUNT 0x50 /* key item ids (keyItem_has) */

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
#define ABIL_FENRIR2           0x43 /* [text abilities] "Fenrir"; second entry, in no command list */
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
#define SA_AUTO_LIFE      0xC4 /* [text support] "Auto-Life" */
#define SA_ACCURACY_PLUS  0xC9 /* [text support] "Accuracy+" */
#define SA_DISTRACT       0xCA /* [text support] "Distract" */
#define SA_LONG_REACH     0xCB /* [text support] "Long Reach" */
#define SA_MP_ATTACK      0xCC /* [text support] "MP Attack" */
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
#define SA_HALF_MP        0xE2 /* [text support] "Half MP" */
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

/* ---- status: BtlUnit.status bits (docs/runtime-findings.md "Battle STATUS bits"). BtlUnit.autoStatus uses the same
 * bits (btl_hasStatus tests both words). The low 7 bits are also Character.status, the statuses that last after
 * battle (btl_init copies them in): ui_drawStatusIcons draws icon k for bit k, and ovl_38's status help lists them
 * in that order, [text ovl_38 g_statusMenuText] "Petrified", "Venom", "Virus", "Silence", "Darkness", "Trouble",
 * "Zombie". */
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
