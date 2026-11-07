# Effect-module choreography: deep pass E

Big state-machine modules read one state at a time (scheme: docs/fxdata.md, "Choreography literals"; slice doc:
docs/fxchoreo_s6.md). Twins already named on main (docs/fxchoreo_deep_a.md) give their names where the code matches.

## Deep pass

Counts are `tools/fxlits.py fx_XXX` (actionable literals) before (main at f81c857a) and after.

| module | spell | before | after | defines added | twin names reused |
|---|---|---|---|---|---|
| fx_1a8 | Odin (short); Zantetsuken | 100 | 27 | 59 | fx_105 (`FX_ODIN_MODEL_SLOTS`, 0xC here) |

### fx_1a8 (Zantetsuken)

The slice pass left it with no choreography block (only `sizeof(Fx1a8Work80)`), so it got the mechanical names
and the inner timings in one go.

- **Alignment with fx_105**: the short form shares no code with the long one. fx_105 is a camera-keyed timeline
  (night sky, Odin's motions, four shots, the WHITE_FOLD cut); fx_1a8 is a three-state op that never loads Odin's
  model, only the Zantetsuken blade. The only common piece is the `btlfx_initEffModels` slot count
  (`FX_ODIN_MODEL_SLOTS`). Where a meaning sits next to an fx_105 name with another meaning, this module uses a
  distinct name so the two headers don't read as twins: `FX_ODIN_BLAST_FLASH_FRAMES` (fx_105's `FLASH_FRAMES` is
  its white band) and `FX_ODIN_PINK_SMOKE_FRAME` (fx_105's `SMOKE_FRAME` is Odin leaving).
- **States**: `FX_ODIN_STATE_DESCEND` / `STRIKE` / `BLAST`. DESCEND (`DESCEND_FRAMES` 10): BLUE_RINGS, FLAME_TRAIL
  (`FLAME_TRAIL_FRAMES` 0x10, which outlasts the state), and the blade eases from `DROP_HEIGHT` 0x2000 above and
  behind the target over its first 0x1000. At its end the battle background is captured (`FX_ODIN_BG_VRAM_*`).
  STRIKE (`STRIKE_FRAMES` 5, ending on frame `STRIKE_FRAMES - 1` when the `(frame + 1)` ramp reaches the target):
  the blade is aimed and scaled to `BLADE_SCALE`, camera point 3 looks `CAMERA_HEIGHT` above the target; at the end
  `STRIKE_DARKEN_RGB` and the five blast emitters. BLAST (`BLAST_FRAMES` 0x20, the op's end): camera shake
  (`SHAKE_FRAMES`, `SHAKE_AMPLITUDE` less 4 a frame), blade bounce (`BOUNCE_FRAMES`), red additive flash
  (`BLAST_FLASH_FRAMES`), sheath fade (`SHEATH_FADE_FRAMES`), the four flame ribbons (`FLAME_RIBBONS`,
  `FLAME_FRAMES` 0x10 with a 12-frame `FLAME_FADE_FRAMES` ramp), STONE_RING twice (`STONE_RING_FRAMES`), GOLD_RING
  copies (`GOLD_RING_FRAMES`, `GOLD_RING_COPIES`), CRIMSON_RINGS (`CRIMSON_RINGS_FRAMES`) and PINK_SMOKE
  (`PINK_SMOKE_FRAME`).
- **Background capture into the crystal texture**: `btlfx_drawBgOffscreen(0x240, 0x1C0, 0x40, 0x40, ...)` writes
  into the 15-bit page 0x119 (0x240, 0x100) at v 0xC0. That is exactly the 0x40 x 0x40 rect where the file uploads
  the cloud picture (`fxdata.py --vram fx_1a8`), which the SKY_CRYSTAL / SKY_CRYSTAL_2 models (the CALLBACK_EMITTER
  elements' models) sample. At run time the crystal shards show the captured battle background, not the
  placeholder sky.
- **VRAM rects**: FLAME (0x1C0, 0x100, 0x38 x 0x70 from 0x1C0, 0x170: page 0x97 u / v 0, where the FLAME_RIBBON
  models sample), CLOUD (0x240, 0x140, 0x20 x 0x80 from 0x260, 0x140: CLOUD_TORUS's 4-bit page 0x19), SHEATH
  (0x240, 0x100, 0x20 x 0x40 from 0x260, 0x100). The sheath helper's texture box `u + 0x7F` / `v + 0x3F` is
  `FX_ODIN_SHEATH_TEX_W - 1` / `_TEX_H - 1`, the whole SHEATH rect in 4-bit texels.
- **Wind sheath** (`fxm_drawTexCylinder`, the same helper as fx_086's flame sheath): `SHEATH_SEGMENTS` 16
  (`ft4 + SEGMENTS - 2`, `seg >= SEGMENTS - 1`), radius / mid radius, wide and narrow band widths, length, texture.
  Its GPU words are now `GPU_POLY_FT4 / GT4 | GPU_ABE`, `BTLFX_ABR_OPAQUE`, `PRIM_TAG(POLY_FT4 / GT4)` (fx_0f5's
  forms).
- **Finding**: BLAST's blade bounce sets `pos40.vy` outright, without the target's y, so for `BOUNCE_FRAMES` the
  blade bounces round world y 0 (+-0x40). It only looks right because targets stand near y 0 (commented at the
  site).
- **Finding**: the flame ribbons' brightness ramp runs over 12 frames but they draw for 16, so for frames 13 .. 15
  `cos(pillarT >> 2)` is negative and the RGB offset goes below -0x80 (the ribbons are black or clamped).

### Still numeric (by the brief) and open questions

- The 1.12 fixed-point ramps and fades (0x1000, the `>= 0xC00` "last quarter" fades in the callback draws and the
  blade), the blade's tilt 0x600 / spin 0x80 / flip 0x800, the sheath tubes' phase 0x800, `frame * 0x18` (the gold
  rings' and torus's spin and rise), one-off scales (cone `/ 3`, flame 0x1800 / 0x2000, stone ring 0x555, torus
  0x1800 / 0x1000, gold ring 0x800), otZ biases (0x80 / 0x190 / 0x200), the scroll start offset 10000, the trail
  scale 0x555 and drawBgOffscreen's last argument 0x800.
- `btlfx_registerEffectModel`'s second argument (0 / 1 / 2 / 3 here) is unnamed in every module; its meaning is open.
