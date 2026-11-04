# Pointer tests by address bits (PSX_IS_PTR and relatives)

Ports (ff9-ds, ff9-3ds) compile `src/` with native 32-bit pointers that are not in KSEG0 (0x80000000+). Code that
tells a pointer from a number, or a relocated pointer from a file offset, by the value's sign, top bits or address
range breaks there. This lists every such site in the C (`src/main`, `src/ovl_*`, `src/fxmod`, `src_equiv`,
`include/`), as of main 7f0eb477.

How the list was made: scripts over the C with comments stripped (`< 0`/`>= 0`/`> 0` on values that receive pointer
casts at a call site or assignment; integer parameters that receive pointers at one call site and numbers at
another; functions returning pointers as integers, and sign tests on their results; values cast to a pointer type
and sign- or high-bit-tested in the same function; `& 0x80000000`/`0x90000000`/`0xA0000000`, `>> 31`/`>> 28`/`>> 24`,
`0xFF000000`/`0xFFFFFF`, `0x1F800000`, `> 0x80000000`-style range tests; unions of pointer and integer), plus gcc's
"integer from pointer"/"pointer from integer" warnings from a full rebuild. Every hit was read. The hand-written asm
modules (`*_asm.c`, the 24 `INCLUDE_ASM` game functions left) are not covered; their `src_equiv` versions are.

Port column: "breaks" = a native-pointer port takes the wrong branch unless it patches the site or defines the hook.

## 1. A value that is either a number or a pointer

| Site | Function | Expression | The two cases | Status | Port |
|---|---|---|---|---|---|
| src/main/event.c:450 | evt_moveObjectTo | `PSX_IS_PTR(flags)` | `EVT_MOVETO_*` flag bits (0, `EVT_MOVETO_NO_TURN`, ...) vs the target `EvtObject *` that `EVT_OP_MOVE_TO_OBJ` passes (event.c:1190) | **PSX_IS_PTR** (7f0eb477, 3DS F38) | fine once the port defines PSX_IS_PTR |
| src/ovl_0b/btlfx_op.c:2151 | btl_soundPan | `unit & 0x90000000` | a btlfx unit id (0-0x11, `-1` handled before) vs an `SVECTOR *` (btl_playUnitSound passes `(s32)&pos`, a stack address, btl_status.c:1235). The mask takes KSEG0 (bit 31) and the scratchpad 0x1F800000 (bit 28) | **not converted**: `PSX_IS_PTR` compiles to `bltz`, the original is `lui 0x9000; and; beqz`, so ovl_0b would change. Needs a mask-form hook (below) | breaks: the stack pointer is taken for a unit id, `btl_getUnit` calls SystemError(0x53, 0x136) or reads past the target list |

No other C site passes a pointer and a number through the same integer and then decides by sign: the
callee scan found exactly these two functions (other mixed-argument functions, e.g. `akao_stopSe`, `btl_setBanner`,
`menu_open`, `preload_setHook`, `btlfx_drawTrail`, `win_setTitle`, compare with 0 or use the value one way only).

## 2. A file offset that is relocated into a pointer in place

| Site | Function | Expression | The two cases | Status | Port |
|---|---|---|---|---|---|
| src/ovl_0b/btl_main.c:1289 | btl_registerModel | `IS_OFFSET(motion->rotKeys)` | offset vs relocated pointer | already hooked (`IS_OFFSET`, ff9.h) | fine if the port defines IS_OFFSET |
| src/ovl_0b/btl_model.c:432 | btl_initEnemyType | `IS_OFFSET(motion->rotKeys)` | same | IS_OFFSET | same |
| src/ovl_0b/hi_model.c:298 | Hi_DrawSummonModel | `IS_OFFSET(motion->rotKeys)` | same | IS_OFFSET | same |
| src/ovl_0b/btlseq.c:24 | btlseq_init | `IS_OFFSET(motion->rotKeys)` | same | IS_OFFSET | same |
| src/ovl_0b/btl_init.c:203 | btl_initEnemyState | `IS_OFFSET(motion->rotKeys)` | same | IS_OFFSET | same |
| src/ovl_0b/btl_unit.c:84 | btl_updateUnitModel | `IS_OFFSET(motion->rotKeys)` | same | IS_OFFSET | same |
| src/ovl_0c/w_movement.c:870 | wm_setActorFog | `IS_OFFSET(h->rotKeys)` | same | IS_OFFSET | same |
| src/ovl_0c/wm_main.c:584, 596 | wm_loadActorModel | `IS_OFFSET(h->rotKeys)`, `IS_OFFSET(grp)` | same | IS_OFFSET | same |
| src/ovl_0c/wm_model.c:98 | wm_initModel | `IS_OFFSET(g2)` | same | IS_OFFSET | same |
| src/ovl_0a/fld_movie.c:181, 192 | fld_setupActorModel | `IS_OFFSET(anim->bones)`, `IS_OFFSET(grp)` | same | IS_OFFSET | same |
| src/ovl_0a/fld_movie.c:341 | fld_collectVisibleModels | `IS_OFFSET(grp)` | same | IS_OFFSET | same |
| src/ovl_0a/fld_movie.c:420 | fld_drawModels | `IS_OFFSET(w->bones)` | same | IS_OFFSET | same |
| src/ovl_0a/fld_movie.c:589 | fld_modelCommand | `IS_OFFSET(grp)` | same | IS_OFFSET | same |
| src/ovl_0a/tu_800B61A4.c:98 | fldmdl_initModel | `IS_OFFSET(g2)` | same | IS_OFFSET | same |
| src/ovl_0b/btlfx_heap.c:541, 546 | btlfx_spawnEmitter | `!(el->tex & BTLFX_TEX_ADDR)`, same for `tex2` | `EffElem.tex` as a file offset (relocated by adding `data`) vs an address already stored there (btlfx_getCommonData, sprites) | **not converted**: `IS_OFFSET`/`!PSX_IS_PTR` was tried and ovl_0b FAILED verify (the mask is `lui 0x8000; and`, not `bltz`) | breaks: a native address is taken for an offset and `data` is added again |
| src/ovl_0b/btl_model.c:196 | btl_relocTexAnims | `(u32)ptr > 0x80000000` → return | `TexAnim.rects` as an offset (relocated: `+ set`) vs already relocated (the set was relocated before: stop) | **not converted**: an unsigned range test (`sltu` against 0x80000001), no sign test | breaks: relocates twice |
| src/ovl_0a/tu_800B61A4.c:1786 | fldmdl_initTexAnims | `addr > 0x80000000` (u32) → return | same as btl_relocTexAnims, field model version | **not converted**, same reason | breaks: relocates twice |
| src/ovl_0c/w_frame.c:725, 729 | w_frameExtruct | `*pack & WM_PACK_EXTRACTED` | a flag bit the code itself sets in the count word once it has relocated the pack, not an address bit | nothing to do | portable |

`IS_OFFSET` stays its own hook (not defined as `!PSX_IS_PTR`): a file offset can be far larger than the small numbers
PSX_IS_PTR's sites hold, so a port's PSX_IS_PTR threshold need not suit it.

## 3. Pointers squeezed into fewer bits

| Site | Function | Expression | What | Status | Port |
|---|---|---|---|---|---|
| src_equiv/main/script_text.c:1060 / 1009 | evt expression evaluator (`EVT_EXPR_SYSVAR` push; ops 0x60/0x61) | `push((v & 0x3FFFFFF) \| 0x1C000000)`, `SCRIPT_PTR_RESTORE(evt_popValue())` | an object pointer from `evt_getSysVar` (op 0x5F) travels through the script stack as a 26-bit immediate and is rebuilt with `\| 0x80000000` | already hooked (`SCRIPT_PTR_RESTORE`, src_equiv.h); in `src/` these are asm (`evt_expr_asm`) | fine with the hook, as long as the port's pointers survive 26 bits or it replaces the push |
| include/psyq.h:8-10, 64 | `setaddr`/`getaddr`/`addPrim`, `PRIM_ADDR_MASK` | `P_TAG.addr` (24-bit) | OT links keep only the low 24 bits of a packet address (GPU DMA format) | known port area, listed only | port replaces the OT layer |
| src/ovl_0b/btlui_frame.c:51 | btlui_clearOtLinked | `ot[i] = (u32)&ot[i - 1] & 0xFFFFFF` | hand-built OT chain | listed (OT) | OT layer |
| src/main/text.c:411; src/main/tu_80053924.c:133, 185, 296; src/ovl_0c/tu_800BA528.c:2020; src/ovl_0c/wm_main.c:1430; src/ovl_0a/fld_movie.c:1586 | various | `getaddr(...)` / `setaddr(p, getaddr(ot))` | splicing OT chains by hand | listed (OT) | OT layer |
| src/ovl_0a/tu_800B61A4.c:1175, 1185, 1195, 1212, 1227, 1244, 1259, 1269 | fldmdl_tintGroupCur | `*(u32 *)prim & PRIM_ADDR_MASK` | "this packet is linked into an OT this frame" (non-zero 24-bit link) | listed (OT) | portable if the port's links are non-zero there too |
| src_equiv/ovl_0b/frames.c:26-27, trails.c:29-30, sprites.c:31-32, rings.c:29-30, mesh_packets.c:63, 74; src_equiv/main/script_text.c:85, 1710-1719; src_equiv/ovl_0a/model_link.c:97, field_sprites.c:78-79; src_equiv/ovl_0c/model_link.c:96, world_faces.c:49-50, world_sprites.c:81-82 | asm equivalents | `(e & 0xFF000000) \| ((u32)p & 0xFFFFFF)`, `*slot = (u32)p & 0xFFFFFF` | OT insertion of packets | listed (OT) | OT layer |

## 4. Fixed scratchpad addresses

`tools/raw_addr.py` only checks 0x80010000-0x801FFFFF, so these raw scratchpad (0x1F800000) addresses are not in
`PSX_RAM`. Not converted: a port's `PSX_RAM` may map only main RAM (e.g. `& 0x1FFFFF` into its RAM copy, which
would send 0x1F800000 to RAM offset 0 silently).

| Site | Function | Expression |
|---|---|---|
| src/ovl_0c/wm_model.c:796, 801, 806 | world model bone pass | `(MATRIX *)0x1F800018`, `(VECTOR *)0x1F800008`, `(SVECTOR *)0x1F800000` |
| src/ovl_0a/tu_800B61A4.c:792, 797, 802, 1506, 1511, 1516 | field model bone passes | same three |
| src/ovl_0a/tu_800B61A4.c:2183 | field model reflection | `reflMtx = (MATRIX *)0x1F800000` |
| src/ovl_0c/w_frame.c:191, 195; src/ovl_0a/fld_movie.c:465 | stack switch | `SetSpadStack(0x1F8003FC)`, `SetSpadStack12(0x1F8003FC)` |
| include/fxmod/fx_1b6.h:69, include/fxmod/fx_1b7.h:68 | stack switch (asm) | `move $29, 0x1F8003F8` |
| src_equiv/src_equiv.h:79 | `SCRATCHPAD` | already a src_equiv hook |

## 5. Read and ruled out

`event.c:173` `(u32)saved >> 24` (the caller's uid in a call-frame word); `btl_main.c:1515`, `tu_800B0124.c:612, 851,
868, 875, 925, 1111`, `include/ovl_0b.h:4967`, `DB_ENTRY_KIND` (db keys and entry kinds); `btl_model.c:23, 27, 36`,
`tu_800B61A4.c:1333` (GPU colour words); `pad.c:150`, `rand.c:11`, `akao_init.c:157`, `MSG_INPUT_REPEAT`,
`GD_MUSIC2_PLAYING`, `BTL_STATUS_GRADUAL_PETRIFY`, `EVT_FRAME_CALLER_UID` (flag bits); `fx_197.c:337`
`overlayModel >= 0` (a u16 model handle or -1); `btl_cmd.c:2685` (support byte); `db.c:96` `(s32)ofs & 3`
(alignment, portable); `BtlPopup.data` (pointer/number union, chosen by `info.b.kind`); the 0x7FFFFFFF minimum
searches, overflow guards in `script_text.c:1342-1383` and the `world_faces.c` depth tests (numbers).

## Proposed hooks (not added)

- **Mask form of PSX_IS_PTR**, for btl_soundPan and btlfx_spawnEmitter:
  `#define PSX_IS_PTR_BITS(v, bits) ((v) & (bits))` here (identical code), and in a port
  `#define PSX_IS_PTR_BITS(v, bits) PSX_IS_PTR(v)` (with a PSX_IS_PTR that covers stack and heap addresses).
  Sites: `btlfx_op.c:2151` `PSX_IS_PTR_BITS(unit, 0x90000000)`, `btlfx_heap.c:541, 546`
  `!PSX_IS_PTR_BITS(el->tex, BTLFX_TEX_ADDR)`.
- **Range form for relocation guards**: `#define PSX_IS_RELOCATED(v) ((u32)(v) > 0x80000000)` here, `!IS_OFFSET(v)`
  in a port. Sites: `btl_model.c:196`, `tu_800B61A4.c:1786`.
- **Scratchpad**: a `PSX_SPAD(type, ofs)` (or `PSX_RAM` documented to cover 0x1F800000-0x1F8003FF and checked by
  raw_addr.py) for section 4.

## Hooks added after the survey (commit after 7a064d87)

The proposals above are now in include/ff9.h (identity here, the build is byte-identical):

| hook | default | sites |
|---|---|---|
| `PSX_IS_PTR_BITS(v, bits)` | `(v) & (bits)` | ovl_0b btlfx_op.c btl_soundPan `PSX_IS_PTR_BITS(unit, 0x90000000)` (unit id or SVECTOR *) |
| `PSX_IS_RELOCATED_BITS(v, bits)` | `(v) & (bits)` | ovl_0b btlfx_heap.c btlfx_spawnEmitter `el->tex` / `el->tex2` with BTLFX_TEX_ADDR |
| `PSX_IS_RELOCATED_ABOVE(v, addr)` | `(u32)(v) > (u32)(addr)` | ovl_0b btl_model.c btl_relocTexAnims, ovl_0a tu_800B61A4.c fldmdl_initTexAnims (`> 0x80000000`) |
| `PSX_SPAD(type, ofs)`, `PSX_SPAD_BASE` | `(type)(0x1F800000 + ofs)` | ovl_0c wm_model.c, ovl_0a tu_800B61A4.c scratchpad MATRIX/VECTOR/SVECTOR pointers; SetSpadStack(PSX_SPAD_BASE + 0x3FC) in w_frame.c / fld_movie.c |

A port defines PSX_IS_PTR_BITS like PSX_IS_PTR, the two RELOCATED hooks as `!IS_OFFSET(v)` (its relocation test),
and PSX_SPAD to its own scratch buffer. OT tag (24-bit link) handling and SCRIPT_PTR_RESTORE stay as listed above.
