/* ovl_0b battle-effect math helpers (0x800CF5A4-0x800CFA6C, 0x800DFE4C-0x800E00DC), hand-written asm in the
 * original.
 *
 * Register convention: all take their arguments in $a0-$a3 and return in $v0 like C, but they don't keep a
 * frame (except btlfx_aimAngles/btlfx_transformPolar, which call ratan2) and clobber only $t0-$t7, $v1, $a1-$a3
 * and $at. None of them saves or restores GTE state: the GTE registers they write (listed per function) stay
 * as they leave them, and btlfx_transformPoint/btlfx_transformPolar use the rotation and translation the caller
 * loaded. The C below writes the same GTE registers with the same 32-bit values in the same command order
 * (tools/equiv.py compares the whole GTE register file).
 *
 * The callers' prototypes (include/ovl_0b.h, include/btlfx_mod.h) are kept, including the ones that pass a
 * pointer as an s32 (btlfx_pushTrailPoint's position, btlfx_transformPoint's out, btlfx_transformPolar's
 * outputs).
 *
 * Checked with tools/equiv.py against the original (see each function).
 */
#include "ovl_0b.h"
#include "../src_equiv.h"

#define GTE_SQR0 0x4AA00428  /* SQR sf=0: MAC = IR * IR */
#define GTE_RTV0TR 0x4A480012 /* MVMVA sf=1 RT * V0 + TR */
#define GTE_INTPL 0x4A980011 /* INTPL sf=1 */
#define GTE_GPF1 0x4B98003D  /* GPF sf=1: MAC = IR0 * IR >> 12 */
#define GTE_GPL0 0x4BA0003E  /* GPL sf=0: MAC += IR0 * IR */

/* The 32-bit seed of the battle effects' random generator, in the scratchpad. */
#define BTLFX_SEED (*(u32 *)(SCRATCHPAD + 0x3FC))

static u32 btlfx_nextSeed(void) {
    BTLFX_SEED = BTLFX_SEED * 0x41C64E6D + 0x3039;
    return BTLFX_SEED >> 16;
}

/* The square-root loop the asm repeats inline in btlfx_distance, btlfx_aimAngles and btlfx_transformPolar:
 * restoring digit-by-digit root of a signed 32-bit sum (compares are signed, so a sum >= 2^31 gives 0). */
static s32 btlfx_isqrt(s32 n) {
    s32 root = 0;
    u32 trial = 1;
    s32 shift = 30;

    do {
        u32 bit = trial << shift;

        root <<= 1;
        if (n >= (s32)bit) {
            root |= 4;
            n -= bit;
        }
        shift -= 2;
        trial = root + 1;
    } while (shift >= 0);
    return root >> 2;
}

/* Background brightness word and OT shift: btlbg_setBrightness etc. are in background.c. */

/* Pushes a point on a trail ring buffer: buf[1] is the head index (moves down, wrapping from 0 to `max`),
 * buf[2] the count (saturating at `max`), entries are 8 bytes from buf + 4: the position's first word (x, y)
 * and z | width << 17. Returns the count before the push. */
s32 btlfx_pushTrailPoint(u8 *buf, s32 max, s32 width, s32 pos) {
    s32 head = buf[1] - 1;
    s32 count = buf[2];
    u32 xy, z;
    u32 *e;

    if (head < 0) {
        head = max;
    }
    e = (u32 *)(buf + head * 8);
    xy = *(u32 *)pos;
    z = *(u16 *)(pos + 4);
    e[1] = xy;
    e[2] = ((u32)width << 17) | z;
    buf[1] = head;
    if (count != max) {
        buf[2] = count + 1;
    } else {
        buf[2] = count;
    }
    return count;
}

/* Distance between two points, at least 1. The asm squares IR1 = dx, IR2 = a->vx (not dz, which it computes
 * and drops) and IR3 = dy, so this is sqrt(dx^2 + a->vx^2 + dy^2) with each term the square of the low 16
 * bits. GTE: IR1-IR3 and MAC1-MAC3. */
s32 btlfx_distance(SVECTOR *a, SVECTOR *b) {
    s32 r;

    GTE_MTC2(9, b->vx - a->vx);
    GTE_MTC2(10, a->vx);
    GTE_MTC2(11, b->vy - a->vy);
    GTE_CMD(0x4AA00428);
    r = btlfx_isqrt((s32)(GTE_MFC2(25) + GTE_MFC2(26) + GTE_MFC2(27)));
    if (r == 0) {
        r = 1;
    }
    return r;
}

/* Upper 16 bits of the next seed (0-0xFFFF). */
s32 btlfx_rand(void) {
    return btlfx_nextSeed();
}

/* Newton's method for sqrt(v), `n` rounds (at least one) or until the estimate is 0. The asm's divide has
 * the compiler-style guards: break 6 on 0x80000000 / -1 (v = 0x80000000 with an estimate of -1). */
s32 btlfx_sqrt(s32 v, s32 n) {
    s32 x = v;

    while (x != 0) {
        x = (u32)(x + v / x) >> 1;
        if (--n <= 0) {
            break;
        }
    }
    return x;
}

/* lo + rand % (hi - lo), or lo when they are equal (no draw then). Raw div: hi - lo is never 0 here. */
s32 btlfx_randRange(s32 lo, s32 hi) {
    if (lo == hi) {
        return lo;
    }
    return lo + (s32)btlfx_nextSeed() % (hi - lo);
}

/* rand % span - span / 2 (the half is a logical shift), 0 without a draw when span is 0. */
s32 btlfx_randCentered(s32 span) {
    if (span == 0) {
        return 0;
    }
    return (s32)btlfx_nextSeed() % span - ((u32)span >> 1);
}

/* Random angles: phase = (phase + 1) & 7, vx = rand & 0x3FF in the quadrant (phase >> 1) & 3,
 * vy = (rand >> 6) & 0x3FF, negated on even phases, vz = 0. The asm stores vx/vy as one word and leaves
 * it in $v0, but no caller reads it (the module export is a word-returning slot, see btlfx_mod.h). */
void btlfx_randRotation(SVECTOR *v) {
    u32 r = btlfx_nextSeed();
    u32 phase;
    s32 y;

    phase = (g_btlfxRotPhase + 1) & 7;
    v->vz = 0;
    g_btlfxRotPhase = phase;
    y = (r >> 6) & 0x3FF;
    if (!(phase & 1)) {
        y = -y;
    }
    *(u32 *)v = ((u32)y << 16) | ((r & 0x3FF) + ((phase << 9) & 0xC00));
}

/* Rotation that points from `from` to `to`: out[1] = 0x400 - ratan2(dz, dx), out[2] = 0,
 * out[0] = -ratan2(dy, sqrt(dx^2 + dz^2)) (no minimum of 1 here). GTE: IR1 = dx, IR2 = dz, SQR (IR3 is
 * whatever the caller left), MAC1-MAC3. */
void btlfx_aimAngles(s16 *from, s16 *to, s16 *out) {
    s32 dx = to[0] - from[0];
    s32 dz = to[2] - from[2];
    s32 dy = to[1] - from[1];
    s32 r;

    GTE_MTC2(9, dx);
    GTE_MTC2(10, dz);
    GTE_CMD(0x4AA00428);
    out[1] = 0x400 - ratan2(dz, dx);
    r = btlfx_isqrt((s32)(GTE_MFC2(25) + GTE_MFC2(26)));
    out[2] = 0;
    out[0] = -ratan2(dy, r);
}

/* out = RT * (xy, z) + TR through the current GTE matrices (s16 results, MAC truncated). GTE: VXY0 = xy,
 * VZ0 = z, MVMVA, MAC1-MAC3/IR1-IR3. */
void btlfx_transformPoint(s32 z, s32 xy, s32 out) {
    s16 *o = (s16 *)out;

    GTE_MTC2(0, xy);
    GTE_MTC2(1, z);
    GTE_CMD(0x4A480012);
    o[0] = GTE_MFC2(25);
    o[1] = GTE_MFC2(26);
    o[2] = GTE_MFC2(27);
}

/* Transforms v (RT * v + TR), stores z in *depth and sqrt(IR1^2 + IR2^2) of the saturated x/y in *radius,
 * returns ratan2(x, y) of the unsaturated MACs. GTE: V0 (both words of v), MVMVA, then SQR. */
s32 btlfx_transformPolar(SVECTOR *v, s32 depth, s32 radius) {
    s32 x, y;

    GTE_MTC2(0, ((u32 *)v)[0]);
    GTE_MTC2(1, ((u32 *)v)[1]);
    GTE_CMD(0x4A480012);
    x = GTE_MFC2(25);
    y = GTE_MFC2(26);
    *(s32 *)depth = GTE_MFC2(27);
    GTE_CMD(0x4AA00428);
    *(s32 *)radius = btlfx_isqrt((s32)(GTE_MFC2(25) + GTE_MFC2(26)));
    return ratan2(x, y);
}

/* GTE INTPL: from + (to - from) * t / 4096 with GTE saturation. GTE: IR0 = t, IR1 = from, RFC = to. */
s32 btlfx_lerp(s32 from, s32 to, s32 t) {
    GTE_MTC2(8, t);
    GTE_MTC2(9, from);
    GTE_CTC2(21, to);
    GTE_CMD(0x4A980011);
    return GTE_MFC2(25);
}

/* from + (to - from) * step / duration. Raw div without guards: duration 0 gives the MIPS quotient
 * (-1, or 1 for a negative product) and 0x80000000 / -1 gives 0x80000000. */
s32 btlfx_lerpRatio(s32 from, s32 to, s32 step, s32 duration) {
    s32 p = (to - from) * step;
    s32 q;

    if (duration == 0) {
        q = p < 0 ? 1 : -1;
    } else if (duration == -1) {
        q = -(u32)p;
    } else {
        q = p / duration;
    }
    return q + from;
}

/* GTE INTPL on three components: out = from + (to - from) * t / 4096. GTE: IR0 = t, IR1-IR3 = from,
 * RFC/GFC/BFC = to. */
void btlfx_lerpVec(s16 *from, s16 *to, s32 t, s16 *out) {
    GTE_MTC2(8, t);
    GTE_MTC2(9, from[0]);
    GTE_MTC2(10, from[1]);
    GTE_MTC2(11, from[2]);
    GTE_CTC2(21, to[0]);
    GTE_CTC2(22, to[1]);
    GTE_CTC2(23, to[2]);
    GTE_CMD(0x4A980011);
    out[0] = GTE_MFC2(25);
    out[1] = GTE_MFC2(26);
    out[2] = GTE_MFC2(27);
}

/* Particle motion step (Pool30Entry: unk0 position 20.12, unkC-unk10 velocity, unk1E time step).
 * velocity = velocity * drag >> 12 (GPF; drag is unk20 for btlfx_stepParticle, unk28.s.y for btlfx_stepParticleAltDrag);
 * velocity.y += gravity * step >> 7 (stored only: this step moves with the velocity before gravity);
 * position += velocity << 6 (GPL with IR0 = 0x40) + (accel * (step << 6) >> 12) << 6 (GPF);
 * out = position >> 12. GTE: IR0-IR3, MAC1-MAC3. btlfx_stepParticle is a two-instruction entry that jumps into
 * btlfx_stepParticleAltDrag's body with the other drag field. */
static void btlfx_particleStep(Pool30Entry *e, s32 drag, s32 gravity, s16 *accel, SVECTOR *out) {
    s32 vx, vy, vz;
    s32 px, py, pz;
    s32 step;

    GTE_MTC2(8, drag);
    GTE_MTC2(9, e->unkC);
    GTE_MTC2(10, e->unkE);
    GTE_MTC2(11, *(u32 *)&e->unk10);
    step = e->unk1E;
    GTE_CMD(0x4B98003D);
    vx = GTE_MFC2(9);
    vy = GTE_MFC2(10) + ((gravity * step) >> 7);
    vz = GTE_MFC2(11);
    GTE_MTC2(8, 0x40);
    GTE_MTC2(25, e->unk0.vx);
    GTE_MTC2(26, e->unk0.vy);
    GTE_MTC2(27, e->unk0.vz);
    GTE_CMD(0x4BA0003E);
    e->unkC = vx;
    e->unkE = vy;
    e->unk10 = vz;
    px = GTE_MFC2(25);
    py = GTE_MFC2(26);
    pz = GTE_MFC2(27);
    GTE_MTC2(8, step << 6);
    GTE_MTC2(9, accel[0]);
    GTE_MTC2(10, accel[1]);
    GTE_MTC2(11, accel[2]);
    GTE_CMD(0x4B98003D);
    px += GTE_MFC2(25) << 6;
    e->unk0.vx = px;
    py += GTE_MFC2(26) << 6;
    e->unk0.vy = py;
    pz += GTE_MFC2(27) << 6;
    e->unk0.vz = pz;
    out->vx = px >> 12;
    out->vy = py >> 12;
    out->vz = pz >> 12;
}

void btlfx_stepParticle(Pool30Entry *e, s32 gravity, s16 *accel, SVECTOR *out) {
    btlfx_particleStep(e, e->unk20, gravity, accel, out);
}

void btlfx_stepParticleAltDrag(Pool30Entry *e, s32 gravity, s16 *accel, SVECTOR *out) {
    btlfx_particleStep(e, e->unk28.s.y, gravity, accel, out);
}

/* Particle motion step without drag: *acc += speed * step >> 7 (an s16: unk10 = velocity z for
 * btlfx_stepParticleNoDrag, unk24.s.x for btlfx_stepParticleNoDragAlt; btlfx_stepParticleNoDrag jumps into btlfx_stepParticleNoDragAlt's body);
 * position += (dir * (step << 6) >> 12) << 6, with the new *acc added to y first; out = position >> 12.
 * GTE: IR0-IR3, MAC1-MAC3. */
static void btlfx_particleStepNoDrag(Pool30Entry *e, s16 *acc, s32 speed, s16 *dir, SVECTOR *out) {
    s32 step = e->unk1E;
    s32 a;
    s32 px, py, pz;

    GTE_MTC2(8, step << 6);
    GTE_MTC2(9, dir[0]);
    GTE_MTC2(10, dir[1]);
    GTE_MTC2(11, dir[2]);
    GTE_CMD(0x4B98003D);
    a = *acc + ((speed * step) >> 7);
    *acc = a;
    px = e->unk0.vx + (GTE_MFC2(25) << 6);
    e->unk0.vx = px;
    py = e->unk0.vy + ((GTE_MFC2(26) + a) << 6);
    e->unk0.vy = py;
    pz = e->unk0.vz + (GTE_MFC2(27) << 6);
    e->unk0.vz = pz;
    out->vx = px >> 12;
    out->vy = py >> 12;
    out->vz = pz >> 12;
}

void btlfx_stepParticleNoDrag(Pool30Entry *e, s32 speed, s16 *dir, SVECTOR *out) {
    btlfx_particleStepNoDrag(e, &e->unk10, speed, dir, out);
}

void btlfx_stepParticleNoDragAlt(Pool30Entry *e, s32 speed, s16 *dir, SVECTOR *out) {
    btlfx_particleStepNoDrag(e, &e->unk24.s.x, speed, dir, out);
}

/* The particle btlfx_stepModuleParticle moves (the effect modules' own records, passed as Pool6CEntry * through the
 * export table): position 20.12 words, velocity, drag. */
typedef struct BtlfxBody {
    s32 pos[3];
    s16 vel[3];
    s16 drag;
} BtlfxBody;

/* velocity = velocity * drag >> 12 (GPF); velocity.y += gravity (stored only: this step moves with the
 * velocity before gravity); position += velocity (GPL with IR0 = 0x1000) + bias << 6, with the bias
 * components taken in z, x, y order (position.x gets bias->vz, y gets vx, z gets vy); out = position >> 12.
 * Returns nothing (the export slot's word return is $v0 garbage). GTE: IR0-IR3, MAC1-MAC3. */
void btlfx_stepModuleParticle(void *arg0, s32 gravity, SVECTOR *bias, SVECTOR *out) {
    BtlfxBody *b = arg0;
    s32 vx, vy, vz;
    s32 px, py, pz;

    GTE_MTC2(8, b->drag);
    GTE_MTC2(9, b->vel[0]);
    GTE_MTC2(10, b->vel[1]);
    GTE_MTC2(11, *(u32 *)&b->vel[2]);
    GTE_CMD(0x4B98003D);
    vx = GTE_MFC2(9);
    vy = GTE_MFC2(10) + gravity;
    vz = GTE_MFC2(11);
    GTE_MTC2(8, 0x1000);
    GTE_MTC2(25, b->pos[0]);
    GTE_MTC2(26, b->pos[1]);
    GTE_MTC2(27, b->pos[2]);
    GTE_CMD(0x4BA0003E);
    b->vel[0] = vx;
    b->vel[1] = vy;
    b->vel[2] = vz;
    px = GTE_MFC2(25);
    py = GTE_MFC2(26);
    pz = GTE_MFC2(27);
    {
        s32 bz = bias->vz, bx = bias->vx, by = bias->vy;

        px += bz << 6;
        b->pos[0] = px;
        py += bx << 6;
        b->pos[1] = py;
        pz += by << 6;
        b->pos[2] = pz;
    }
    out->vx = px >> 12;
    out->vy = py >> 12;
    out->vz = pz >> 12;
}
