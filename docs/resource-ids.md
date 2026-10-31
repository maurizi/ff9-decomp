# Resource and graphics constants

`include/res_ids.h` (included by `ff9.h`, so also by every overlay and effect module) names the constants of the
resource system and the graphics setup. GPU primitive codes and libgte values live in `include/psyq.h`. Game-data
ids are in `ff9_ids.h`, sound ids in `snd_ids.h`. Every macro is the literal it replaced (same value and type),
so codegen doesn't change.

| group | macros | evidence |
|---|---|---|
| db lookup modes | `DB_GET_BLOCK` .. `DB_GET_COUNT_IN` | the `mode` switch of `db_lookup` (main db.c) |
| db key kinds | `DB_OVERLAY`, `DB_MODEL` .. `DB_LIGHTS`, `DB_BTL_BG`, `DB_BTL_BG_HEADER`, `DB_VRAM_ALLOC` | what the callers do with the item; docs/encounters.md, battle-backgrounds.md |
| single resources | `DB_SHADOW_TIM` | both shadow loaders (battle, world map) |
| FF9.IMG directories | `DIR_*` | docs/ff9img.md, the contents of each directory, the `file_find` callers |
| overlay file ids | `OVL_*`, `OVL_KEY(ovl)` | config/overlays.us.txt; `game_loadModeOverlay`'s mode table |
| DIR_COMMON files | `FILE_*` | named after the one function that loads each |
| screen | `SCREEN_W`, `SCREEN_H`, `SCREEN_H_NTSC` | `game_setScreenHeight` |
| ordering tables | `OT_LEN`, `OT_LAST`, `OT_PAD`, `BTL_BG_OT_LEN` | docs/r29/battle-render.md |
| VRAM | `VRAM_SHADOW_CLUT_*`, `VRAM_MODEL_CLUT_X`, `VRAM_IMAGE_*`, `VRAM_BTLUI_CACHE_*` | only locations several functions share |
| RAM regions | `RAM_SUB_OVL(type)`, `RAM_BTLFX_MODULE(type)`, `RAM_BTLFX_MODULE_HI(type)`, `RAM_BTLFX_RING(type)` | PSX_RAM pointers; docs/r29/battle-arena-memory.md |
| GPU (psyq.h) | `GPU_POLY_*`, `GPU_SPRT*`, `GPU_TILE*`, `GPU_ABE`, `GPU_TGE`, `GPU_CODE`, `GPU_RGBC`, `GPU_RGB_NEUTRAL`, `ONE` | libgpu / libgte |

Rules used when replacing literals:
- A literal is replaced only where its context shows the domain: the mode argument of `db_lookup*`, a key passed to
  `db_lookup*` / `dep_find` / the preload functions, the first argument of `file_find`, a primitive's code byte or
  colour word, a fixed-point matrix element. Values are never replaced on their own.
- One-off coordinates (a window's x/y/w/h, a sprite's position in some overlay's VRAM page) stay literals. Tetra
  Master and Blackjack DR_TPAGE and CLUT words are written as `get_mode(dfe, 0, GetTPage(...))` and
  `getClut(x, y)` so their coordinates can be read, but they aren't named.
- Combined music / sound-bank keys (`0x70005`, `0x90164`) are left to the sound naming.
- Not named yet: db kinds 0x00, 0x10 (encounter `unk20`), 0x18, 0x1E; FF9.IMG dir 9; dir 1 files whose loader says
  nothing about their content (disc-change pictures, attract images come from tables).
