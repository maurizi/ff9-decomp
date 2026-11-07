#ifndef FX_1AC_H
#define FX_1AC_H

/* Effect module fx_1ac (dir 13 file 0x1ac): enemy Virus Crunch (Chimera). Evidence: the static sweep of the ability, item
 * and enemy-sequence effect ids (docs/effect-modules.md, tools/effect_labels.py). */
#include "btlfx_mod.h"

/* fx_1ac effect data (btlfx_getFileData indices), named from tools/fxdata.py renders of the file's
 * entries (docs/fxdata_s6.md; "(look pending)" = by kind, appearance not settled). */
#define FX_VIRUS_CRUNCH_MOTHS      0x1 /* emitter: orbiting gold / green / orange moth-wing quads, 24 frames */
#define FX_VIRUS_CRUNCH_MOTHS_2    0x2 /* emitter: gold / orange / green moth-wing quads, 24 frames */
#define FX_VIRUS_CRUNCH_CYAN_CLOUD 0x3 /* emitter: white-grey gouraud rings + teal-cyan cloud quads */
#define FX_VIRUS_CRUNCH_GREY_TRAIL 0x5 /* emitter: flat trail, grey faceted strip */

/* fx_1ac choreography (docs/fxdata.md, "Choreography literals"): op80 throws the target in an arc into the caster's
 * mouth (bone 4), spits moth swarms that follow the bone, then puts the target back home with a cloud; countdown
 * slots of work->timer0 and their lengths, keyframes, the trail buffer and the op's length. */
#define FX_VIRUS_CRUNCH_T_JUMP   0 /* work->timer0 slot: the target's arc to the mouth */
#define FX_VIRUS_CRUNCH_T_RETURN 2 /* slot: the target's step back home */
#define FX_VIRUS_CRUNCH_T_MOTHS  3 /* slots 3..6: moth swarm emit4C[slot - 3] follows the mouth bone */
#define FX_VIRUS_CRUNCH_T_CLOUD  7 /* slot: CYAN_CLOUD held pointing down */
#define FX_VIRUS_CRUNCH_TIMERS   8 /* work->timer0 slots */
#define FX_VIRUS_CRUNCH_EMITTERS 5 /* work->emit4C: four moth swarms and the cloud */
#define FX_VIRUS_CRUNCH_MOUTH_BONE 4 /* unit 0x10's bone the target is thrown to and the moths leave from */
#define FX_VIRUS_CRUNCH_GREY_TRAIL_TRAIL_SLOTS  4    /* GREY_TRAIL's trail buffer fx_1ac_op80Trails: slots of */
#define FX_VIRUS_CRUNCH_GREY_TRAIL_TRAIL_STRIDE 0x9C /* this many bytes (Pool6CEntry trailSlots / trailStride) */
#define FX_VIRUS_CRUNCH_JUMP_FRAMES  0xE /* the arc's length; at its end the target is hidden and put home */
#define FX_VIRUS_CRUNCH_JUMP_HEIGHT  0x258 /* the arc's height term (jumpFrame * this - jumpFrame^2 * 0x25) */
#define FX_VIRUS_CRUNCH_MOTHS_FRAME   20 /* MOTHS */
#define FX_VIRUS_CRUNCH_MOTHS_2_FRAME 26 /* MOTHS again */
#define FX_VIRUS_CRUNCH_MOTHS_3_FRAME 32 /* MOTHS again */
#define FX_VIRUS_CRUNCH_MOTHS_4_FRAME 46 /* MOTHS_2 */
#define FX_VIRUS_CRUNCH_MOTHS_FRAMES  0x10 /* each swarm follows the bone this many frames */
#define FX_VIRUS_CRUNCH_RETURN_FRAME  50 /* the target reappears turned back; CYAN_CLOUD at its home */
#define FX_VIRUS_CRUNCH_RETURN_FRAMES 2  /* the step home's length */
#define FX_VIRUS_CRUNCH_CLOUD_FRAMES  0x10 /* CYAN_CLOUD is held this many frames */
#define FX_VIRUS_CRUNCH_FRAMES   0x46 /* op80's length (the op ends) */

typedef struct {
    s32 timer0[FX_VIRUS_CRUNCH_TIMERS]; /* countdowns */
    SVECTOR pos20; /* caster position */
    SVECTOR pos28; /* target position */
    SVECTOR pos30; /* bone position */
    SVECTOR rot38; /* target rotation */
    u8 pad40[0x44 - 0x40];
    SVECTOR rot44; /* saved target rotation */
    Pool6CEntry *emit4C[FX_VIRUS_CRUNCH_EMITTERS];
} Fx1acWork; /* 0x60 */

extern Unk800F78A4 *fx_1ac_env;
extern Pool6CEntry *fx_1ac_emitter;
extern s32 fx_1ac_op80Trails[];
extern SVECTOR fx_1ac_emitPos0;
extern SVECTOR fx_1ac_emitPos1;
extern SVECTOR fx_1ac_emitPos2;
extern SVECTOR fx_1ac_emitPos3;
extern SVECTOR fx_1ac_emitPos4;
extern SVECTOR fx_1ac_targetPos;
extern SVECTOR fx_1ac_lastParticlePos;

static s32 fx_1ac_op80ParticleMotion(Pool30Entry *pt, Pool6CEntry *owner, SVECTOR *pos, s32 trail, s32 frame, s32 n);

#endif
