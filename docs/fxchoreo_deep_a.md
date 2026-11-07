# Effect-module choreography: deep pass A

Six of the big state-machine modules, read one state at a time (scheme: docs/fxdata.md, "Choreography literals";
slice docs: docs/fxchoreo_s1.md, _s3.md, _s4.md, _s5.md). fx_07a, fx_07d and fx_086 had no choreography block at
all after slice 1, so they got the mechanical names and the inner timings in one go.

## Deep pass

Counts are `tools/fxlits.py fx_XXX` (actionable literals) before (main at 89782b2a) and after.

| module | spell | before | after | defines added | twin names reused |
|---|---|---|---|---|---|
| fx_07a | Demi Shock | 133 | 52 | 59 | fx_079 (`FX_DEMI_SHOCK_*`: slots, scales, strand counts) |
| fx_07d | Flare | 155 | 57 | 68 | fx_0df (Sword Magic Flare, `FX_SWDMAG_FLARE_*` with this module's values) |
| fx_086 | Meteor | 225 | 110 | 65 | fx_08f (Meteor alt: states, textures, sky rect) |
| fx_134 | Doomsday | 290 | 248 | 40 | fx_0e0 (already shared in the slice pass) |
| fx_0e2 | Fenrir (2nd) | 396 | 173 | 103 | fx_0d2 (op84 `DROP_HEIGHT` / `DROP_FRAMES` / `LAND_FRAMES`, `CAM_KEY_SLOTS`) |
| fx_105 | Odin | 146 | 90 | 32 | - |
| total | | 1345 | 730 | 367 | |

### What was named

- **fx_07a**: the 16 work timers as `FX_DEMI_SHOCK_T_*` slots (strands, sphere, fly, grow / shrink, lift / drop,
  backdrop, and the twisted disc's flag / open / twist / close), the six keyframes (`START_FRAME` 1 after the
  frame-0 background capture, `DISC_FRAME`, `FLASH_RING_FRAME`, `SHRINK_FRAME`, `WISP_FRAME`, `FLASH_FRAME`), the
  lengths written into each slot, the sphere height, the background capture rect and the disc's texture centre,
  radii, ring / segment counts (prim buffer `ft4 + SEGMENTS * RINGS`, `poly + SEGMENTS * 2`), GPU words.
- **fx_07d**: states CHARGE (new, not in fx_0df) / TWIST / DOME / FLASH / BLAST with their lengths, flashes and
  tints, the callback emitter's keyframe, the grid centre's screen box, fire dome scale / offset, cloud pool item
  timings, model textures, the scrolled fire rect, op83's arc variants, lengths and buffers, and the radial grid
  helper's GPU words / screen size.
- **fx_086**: STATE_SKY / FALL / IMPACT, every keyframe inside them (embers, glow / shake, streak speed-up, red tint,
  camera dive, fall glow, shake, streak flash, explosion, glowing embers, the blast and its flash / dust / dome
  sub-lengths), screen tints, the streak field (count, radius, "no point" sentinel), ember sprite positions, model
  textures, the sky and fire-dome scroll rects, and the flame sheath (`fxm_drawTexCylinder`: segments, texture,
  radii, width, length).
- **fx_134**: the inner keys the slice pass left: space-layer fade-in and camera bob, STAR_ARCH column pieces /
  spacing / ring counts, the cone's grid pulse and flare frames, cone sparks, riser period, gather bolts; the orb
  pool items' kinds (ORB / ORB_2 / DRIFT, the last never spawned here) and phases (FLY until fx_134_catchRadius,
  then CAUGHT with embers and a ring burst) with their trail / ring lengths; the sprite / mirror / smoke-strip /
  bulge-grid textures (the bulge grid samples the streamed TUNNEL rect).
- **fx_0e2**: op80's whole timeline (camera shot frames, summon window / dim / additive, red moon, motion 1, wind
  model morphs and fades, wind trail, strips' blend / hold / path phases with their key, fade and morph sub-frames,
  the heads' emitters, drops), op81's tower (stages, pieces, blocks, position, smoke keyframes, stage switches,
  background rect), op82's whirlwind (keyframes, fade, ribbon strands / copies / key cycles, spiral length,
  ribbon textures and their scrolled rects), op83's lift, op84's drop (fx_0d2's names), op85's white fade.
- **fx_105**: inside the countdowns: moon fade-in, night fade-out and blue, the stars' area and rise window, the
  cut's black / WHITE_FOLD_0 / _1 / morph / fade counts, the blade ribbon's bone and extent, the flash and dimming
  bands' sweeps and level, the subtractive bottom band, wind / smoke fades, camera ribbon growth, the camera key
  helper's target count and extra-slot base, and the wind / ribbon texture boxes; `case 0x9C ... 0xA6` became
  `FX_ODIN_SHOW_FRAME ... FX_ODIN_SHOT1_FRAME - 1`.

### Findings worth knowing

- fx_07a: the disc's close phase (T_DISC_CLOSE) lerps the outer band and the disc radius on `timers0[T_TWIST]`, not
  its own count; T_TWIST is already 0 by then, so both snap shut on the first close frame while only the inner band
  shrinks over the 16 frames (commented at the site).
- fx_105: timer24's fade-in branch (`timer24 >= 0x31`) can never run, since the count starts at FX_ODIN_WIND_FRAMES
  (0x30); the wind streaks start at full brightness. Kept numeric.
- fx_105: the "light band" (timer18) is drawn subtractive (abr 2) with white at the bottom: it darkens the bottom of
  the screen.
- fx_086: the flame sheaths sample the dust ring's 4-bit page (0x300, 0x100) at u / v 0x80 with their own CLUT.

### Still numeric (by the brief) and open questions

- OT biases, random spreads, angle / fixed-point math, one-off scales and the camera offsets throughout.
- fx_07a: the disc ring twists 0x1000 / 0xEAB / 0xD56 / 0xC00 (one turn less 1/12 per ring) and drawBgOffscreen's
  last argument 0x1208.
- fx_134: drawSpriteAtOt / drawScreenGrid / drawRippleGrid placement args of the space layers and planets; the
  mirror strip (fxm_drawMirrorSprite, METEOR page v 0xC0) is named by kind (look pending).
- fx_0e2: op80's hold phase path clamp (`base >= 0x23 ? 0x22`) and path time offset (`k + 0x37`), the head rotation
  0x113, the emitter positions; op81's drawBgOffscreen camera args; the ribbon key chains' key indices (0 / 6 / 12
  ... are strand counts times the key, left literal in the if-chains).
- fx_105: Odin's bones 0x3E / 0x43 / 0x4C / 0x5C / 0x60 (the debris spirals' anchors) need an emulator look to say
  which body parts they are; the 0x408 argument that clears the blade ribbon (fxm_drawRibbonTrail's n >= 1000 path,
  clearing n - 999 segments).
