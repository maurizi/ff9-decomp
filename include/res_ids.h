#ifndef RES_IDS_H
#define RES_IDS_H
/* Resource and graphics constants: database kinds/ids (db_lookup*), FF9.IMG directories and file ids (file_find),
 * VRAM layout (tpage/clut coordinates), OT sizes, screen geometry. Included by ff9.h. docs/resource-ids.md */

/* db_lookup(db, mode, key, subKey) / db_lookupRegistered(mode, key, subKey) modes (main db.c). A failed lookup in
 * db_lookupRegistered returns -1 for the odd modes and NULL for the even ones. The *_IN modes find the container
 * holding `key` and then run the plain mode on subKey in that container. */
#define DB_GET_BLOCK 0 /* the block of kind key >> 16 */
#define DB_GET_COUNT 1 /* items in that block */
#define DB_GET_AT 2 /* address of item number key & 0xFFFF */
#define DB_GET_KEY_AT 3 /* key of item number key & 0xFFFF */
#define DB_GET_ADDR 4 /* address of the item with this key */
#define DB_GET_SIZE 5 /* size of the item with this key */
#define DB_GET_AT_IN 8 /* DB_GET_AT of subKey */
#define DB_GET_KEY_AT_IN 9 /* DB_GET_KEY_AT of subKey */
#define DB_GET_COUNT_IN 11 /* DB_GET_COUNT of subKey */

/* db keys: kind << 16 | id (db_lookup, docs/encounters.md). Preload entries and dependency tables use the same
 * keys. */
#define DB_KEY_MASK 0xFFFFFF /* kind and id of a key; dependency and preload entries keep flags in the top byte */
/* db container (db_lookup, db_getSize): byte 0 DB_MAGIC, byte 1 the block count, then a u32 entry per block; a block
 * holds byte 1 the item count, u16 ids, then (word-aligned) a u32 entry per item and one past the end. Entries keep
 * the offset from the entry itself in the low 24 bits. Evidence: [code] db_lookup, db_getSize */
#define DB_MAGIC 0xDB
#define DB_ENTRY_OFS 0xFFFFFF
#define DB_ENTRY_KIND(e) (((e) >> 24) & 0x7F) /* block entries: the kind (key >> 16) the block holds */
#define DB_OVERLAY 0x10000 /* code overlay, id = its FF9.IMG dir 0 file id (game_loadModeOverlay, preloads) */
#define DB_MODEL 0x20000 /* evt_createModel: an object's model id */
#define DB_ANIM 0x30000 /* model animation (evt_getAnimFrameCount) */
#define DB_TIM 0x40000 /* TIM images (file_loadTim, game_loadCommonImages) */
#define DB_FIELD 0x50000 /* field / world file and its event script (also the encounter bundle's script) */
#define DB_AREA 0x60000 /* area bundle */
#define DB_MUSIC 0x70000 /* music sequences */
#define DB_SE 0x80000 /* sound effects */
#define DB_SOUND_BANK 0x90000 /* AKAO banks (game_loadCommonAkao, game_loadCommonDb's db) */
#define DB_BG 0xA0000 /* field background */
#define DB_WALKMESH 0xB0000 /* field walkmesh (bgi) */
#define DB_BTL_BG 0xC0000 /* battle background geometry, id = background id (Battle11C0.bgData) */
#define DB_FONT 0xD0000 /* the font (GameData.font, game_loadCommonImages) */
#define DB_BTL_SCENE 0x100000 /* the encounter's scene data inside its DB_BATTLE bundle (btl_initEncounter: Battle11C0.encounter) */
#define DB_BATTLE 0x110000 /* encounter bundle */
#define DB_MODEL_INFO 0x120000 /* per-model tables */
#define DB_DEP_TABLE 0x140000 /* dependency table (GameData.depTable) */
#define DB_SPRITE 0x160000
#define DB_IMAGE_PACK 0x170000
#define DB_SND_STREAM 0x180000 /* streamed sound (SND_TYPE_STREAM, fld_soundCommand) */
#define DB_TEX_ANIM 0x190000 /* model texture animations */
#define DB_BTL_BG_HEADER 0x1A0000 /* battle background header (BtlBgHeader, Battle11C0.bgHeader) */
#define DB_PACK 0x1B0000 /* nested container inside field/world files */
#define DB_VRAM_ALLOC 0x1C0000 /* field VRAM allocation cursors (FieldVramPos; fld_getShadowTexPos) */
#define DB_PAL_ANIM 0x1D0000 /* palette animation table in a world file's second pack (wm_loadFile: u8 count, then WmPalAnim[]) */
#define DB_VIBRATION 0x1E0000 /* field vibration data, id = field id (fld_loadVibration -> vib_init) */
#define DB_LIGHTS 0x1F0000

/* single resources */
#define DB_SHADOW_TIM (DB_TIM | 0x2774) /* unit shadow texture (btl_loadShadowTexture, wm_loadShadowTexture) */

/* FF9.IMG directories: file_find(dir, id), GameState.dirs (docs/ff9img.md; tools/encounter_catalog.py,
 * field_models.py and battle_budget.py use the same indexes) */
#define DIR_OVERLAYS 0 /* code overlays, id = OVL_* (key DB_OVERLAY | id) */
#define DIR_COMMON 1 /* the modes' common files (FILE_*), menu and title images, world data */
#define DIR_AREAS 2 /* area bundles (DB_AREA) */
#define DIR_WORLDS 3 /* world map "fields" 9000-9012 */
#define FIELD_ID_WORLD_FIRST 9000 /* DB_FIELD ids from here on are world maps (DIR_WORLDS, EVT_OP_GOTO_WORLD); */
                                  /* fld_preloadRequest and fld_queueFieldPreloads only preload fields below it */
#define DIR_FIELDS 4 /* fields, id = area << 16 | field */
#define DIR_ENCOUNTERS 5 /* encounter bundles, id = battle id */
#define DIR_BTL_BGS 6 /* battle backgrounds, id = background id */
#define DIR_ENEMY_MODELS 7 /* battle (enemy) models, id = model id */
#define DIR_WEAPON_MODELS 8 /* weapon models (btl_loadWeaponModelsStep, btl_getWeaponSoundBank) */
#define DIR_CHAR_MODELS 10 /* party models, field and battle (fld_loadModelBundle, btl_loadPartyModelsStep) */
#define DIR_SOUND_BANKS 11 /* AKAO banks, DB_SOUND_BANK keys (btl_getSoundFileSectors, wm_findSoundFile) */
#define DIR_WORLD_IMAGES 12 /* world map streamed images ("Read CD Image", wm_initDiscImage) */
#define DIR_BTL_EFFECTS 13 /* battle effect files (type 3 directory; btlfx_loadFile) */

/* dir 0 file ids (config/overlays.us.txt) */
#define OVL_FIELD 0x0A
#define OVL_BATTLE 0x0B
#define OVL_WORLD 0x0C
#define OVL_TETRA_MASTER 0x0D
#define OVL_OPENING 0x14 /* opening / logos / title */
#define OVL_ENDING 0x15 /* ending + Blackjack */
#define OVL_MOVIE 0x64 /* FMV player, loaded at 0x80198700 */
#define OVL_BATTLE_END 0x65 /* battle rewards and game over, loaded at 0x801EDF00 */
#define OVL_KEY(ovl) (DB_OVERLAY | (ovl)) /* preload key of an overlay */

/* DIR_COMMON file ids, named after the function that loads them */
#define FILE_GAMEOVER_IMAGE 0x0B /* gameover_loadImage */
#define FILE_MENU_PORTRAITS 0x1E /* menu_readPortraits */
#define FILE_CARD_IMAGES 0x32 /* menuCard_init: the card images (s_menuCard.imageFile) */
#define FILE_TITLE_IMAGES 0x3C /* opening_titleLoadImages */
#define FILE_BTLFX_COMMON 0x2BF /* btlfx_reloadCommonData: four 64x128 VRAM pages from (0x300, 0x100) */
#define FILE_TM_IMAGES 0x44C /* tm_loadImageFile */
#define FILE_WM_DATA 0x2712 /* wm_getWorldDataFileId, before story 0x2B52 */
#define FILE_BTL_END_DB 0x2713 /* battleEnd_loadDb, preloaded by btl_preloadExit */
#define FILE_FLD_COMMON 0x2714 /* fld_loadCommon */
#define FILE_TM_DATA 0x2715 /* tm_loadDataFile */
#define FILE_WM_DATA_LATE 0x2716 /* wm_getWorldDataFileId, from story 0x2B52 on */
#define FILE_OPENING_DATA 0x2717 /* opening_loadData */
#define FILE_BOOT_SCREEN 0x2718 /* opening_bootScreenLoad */
#define FILE_COMMON_DB 0x271A /* game_loadCommonDb: the resident db (font, common TIMs, sound) */
#define FILE_BTL_COMMON 0x271B /* btl_loadCommonData */
#define FILE_ENDING_DATA 0x2F44 /* ending_loadData */

/* Screen: two SCREEN_W-wide buffers side by side at VRAM (0, 0) and (SCREEN_W, 0) (game_setScreenHeight).
 * SCREEN_H is the default height (field, menus); battle and world map use 0xDA. The display is centred in the
 * SCREEN_H_NTSC lines of an NTSC frame. */
#define SCREEN_W 320
#define SCREEN_H 224
#define SCREEN_H_NTSC 240

/* VRAM layout shared by several functions (the framebuffers fill x 0..2*SCREEN_W-1 at the top) */
#define VRAM_SHADOW_CLUT_X 0x100 /* unit shadow palette: btl_loadShadowTexture uploads it, the battle shadow quads */
#define VRAM_SHADOW_CLUT_Y 0xFF  /* and fld_drawShadow use it */
#define VRAM_MODEL_CLUT_X 0x100 /* battle model palettes (btl_uploadModelTextures, btl_swapPartyModelStep) */
#define VRAM_IMAGE_X 0x280 /* full-screen still images, right of the framebuffers (opening attract and boot */
#define VRAM_IMAGE_Y 0     /* screens, THE END) */
#define VRAM_IMAGE_CLUT_X 0 /* their palettes, below the framebuffers */
#define VRAM_IMAGE_CLUT_Y 0xE0
#define VRAM_FONT_X 0x380 /* 4-bit page of the menu font's glyphs (text_buildFontTpage, text_draw, text_buildBlock, */
#define VRAM_FONT_Y 0x100 /* text_buildPlain, btlui names) */
#define VRAM_UI_SHEET_X 0x3C0 /* 4-bit page next to it: label glyphs, small digits and the hand cursor (ui_buildLabel, */
#define VRAM_UI_SHEET_Y 0x100 /* text_buildSmall, text_buildDigits, ui_buildDigits, btlui_drawTargetCursors) */
#define VRAM_BTLUI_CACHE_X 0x3C0 /* 16-bit page where the battle UI caches its command icons, header and rows */
#define VRAM_BTLUI_CACHE_Y 0x100 /* (btlui_cache* copy them in with DR_MOVE, btlui_initCached* draw from it) */
/* Battle effect screen captures: a rect of the draw buffer around the target is copied here (DR_MOVE / MoveImage) and
 * drawn back as a 16-bit texture, up to 256x256. Runtime (VRAM dumps): Slow (fx_07e) copies 128x128 around the target
 * to (0x1C0, 0x180) every frame and draws it back as the swirling clock face; Stop (fx_095) copies ~192x192 of the
 * screen, HUD included, to (0x1C0, 0x100) once and shows it greyed. Also Silent Voice (fx_18b) and Eat (fx_112).
 * Leviathan (fx_0b3) scrolls its file texture rows into 0x1C0, which is no capture. */
#define VRAM_BTLFX_SCRATCH_X 0x1C0
#define VRAM_BTLFX_SCRATCH_Y 0x100
/* World map chocograph picture: the selected chocograph's picture, drawn in a navi frame above the mini-map
 * (wm_naviDrawChocograph, WM_NAVI_MINI). wm_setupWorldData (ovl_0c) loads its TIM here, and the Chocograph menu
 * (ovl_32 menuChocograph_restoreVram) copies it back here after its own pictures overwrote it. [runtime] R44 */
#define WM_CHOCOGRAPH_PICTURE_X 0x318
#define WM_CHOCOGRAPH_PICTURE_Y 0xB0
#define WM_CHOCOGRAPH_CLUT_X 0x280
#define WM_CHOCOGRAPH_CLUT_Y 0xF6

/* Ordering tables (docs/r29/battle-render.md). Each mode's scene OT GameData.ot[drawBuf] has OT_LEN entries, is
 * cleared with ClearOTagR (entry i links to i - 1) and drawn from OT_LAST, so higher entries are farther away.
 * Battle and world map clear OT_PAD extra entries on each side. */
#define OT_LEN 0x1000
#define OT_LAST (OT_LEN - 1)
#define OT_PAD 8
#define BTL_BG_OT_LEN 0x100 /* battle background OT (Battle11C0.ot, btlbg_draw; btlfx_drawBgOffscreen has its own) */

/* Fixed RAM regions, as PSX_RAM(type, addr) pointers (ports remap them). docs/ff9img.md,
 * docs/r29/battle-arena-memory.md */
#define RAM_GAME_STATE(type) PSX_RAM(type, 0x8008B000) /* GameStateBlock (game_allocState), then the allocator */
#define RAM_SUB_OVL(type) PSX_RAM(type, 0x801EDF00) /* load address of the menu overlays (0x32-0x3E) and ovl_65; ends
                                                     * below the stack. Also where menu_readPortraits reads, and
                                                     * the top of the field / world model cache */
#define RAM_BTLFX_MODULE(type) PSX_RAM(type, 0x801E7700) /* battle effect module area (0xA000 bytes, to the ring); also
                                                          * staging for FILE_BTLFX_COMMON and btlfx_runScript */
#define RAM_BTLFX_MODULE_HI(type) PSX_RAM(type, 0x801EC700) /* its second half: odd kind-1 groups (summons) */
#define RAM_BTLFX_RING(type) PSX_RAM(type, 0x801F1700) /* effect data ring (0xC800 bytes, to 0x801FDF00) */

/* The movie player in the load area (offsets from g_loadAreaEnd, which is the 0x800A7000 overlay slot). The field
 * (fld_movieStart), the opening and the ending read OVL_MOVIE to LOAD_MOVIE_OVL (= 0x80198700, its link address)
 * in one chunk of LOAD_MOVIE_OVL_SIZE (the file is 0x8800 bytes), and give MBG_init the work area right after it.
 * Field models whose packets would reach LOAD_MOVIE_OVL are not drawn during a movie. Evidence: [code] */
#define LOAD_MOVIE_OVL 0xF1700
#define LOAD_MOVIE_OVL_SIZE 0x15000
#define LOAD_MOVIE_WORK (LOAD_MOVIE_OVL + LOAD_MOVIE_OVL_SIZE) /* the MBG_init work area (FieldScene.movieWork) */

#endif
