/* ovl_0b btl_calcBoneMatrices (0x800C7DAC), hand-written asm in the original (compiler-like code around
 * hand-placed GTE sequences).
 *
 * Register convention: standard C ABI. GTE: RT and TR (control 0-7) are left as the last bone composed them,
 * plus IR/MAC/V0.
 *
 * Builds a model's bone matrices (32 bytes each: MATRIX m[3][3], pad, t[3]) at `out`, records `out` at
 * BtlUnit9C.unk20 and returns the end:
 * - Without an animation (unkC == 0): one matrix, the parent model's bone unk4 matrix (unk1C->unk20) moved
 *   by RT * (attachX, attachY, attachZ), or else the model's own unk24 with the y and z columns negated.
 * - With one: per bone, a rotation decoded from the animation keys for `frame` (math_rotMatrixZYX_gte turns it into
 *   a matrix), bone 0 gets the root translation; then bone 0 is put into the model's frame (unk24, or the
 *   parent's bone) and every other bone i into its parent's (bone table byte 3) with an offset of
 *   (0, 0, bone table s16 0) along the parent's z. With fewer than 31 bones the work is done in the
 *   scratchpad (0x1F800018) and copied out.
 * Quirks kept: both bone loops are do-while (a model with 0 bones decodes one, a model with 1 bone composes a
 * second, out of range); the root translation decoded for bone 0 is discarded when the model has a parent;
 * the offset vector is read back from the scratchpad (0x1F800008-0x1F800013) each bone.
 *
 * Checked with tools/equiv.py against the original (src_equiv/ovl_0b/check.py).
 */
#include "ovl_0b.h"
#include "../src_equiv.h"

#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))

static void boneLoadRT(u8 *mat) {
    GTE_CTC2(0, W(mat, 0x0));
    GTE_CTC2(1, W(mat, 0x4));
    GTE_CTC2(2, W(mat, 0x8));
    GTE_CTC2(3, W(mat, 0xC));
    GTE_CTC2(4, W(mat, 0x10));
}

static void boneLoadTR(u8 *mat) {
    GTE_CTC2(5, W(mat, 0x14));
    GTE_CTC2(6, W(mat, 0x18));
    GTE_CTC2(7, W(mat, 0x1C));
}

/* Rotates one matrix column (halfwords 0, 6, 0xC from col) by RT: MVMVA sf=1 RT * IR. */
static void boneRotColumn(u8 *col) {
    GTE_MTC2(9, H(col, 0x0));
    GTE_MTC2(10, H(col, 0x6));
    GTE_MTC2(11, H(col, 0xC));
    GTE_CMD(0x4A49E012);
    H(col, 0x0) = GTE_MFC2(9);
    H(col, 0x6) = GTE_MFC2(10);
    H(col, 0xC) = GTE_MFC2(11);
}

/* Copies the parent model's bone matrix and moves it by RT * (attachX, attachY, attachZ) (IR, saturated s16). */
static void boneFromParent(BtlUnit9C *m, u8 *mat) {
    u8 *src = (u8 *)m->parent->bones + m->parentBone * 32;
    u32 a, b, c, d;

    a = W(src, 0x0);
    b = W(src, 0x4);
    c = W(src, 0x8);
    d = W(src, 0xC);
    W(mat, 0x0) = a;
    W(mat, 0x4) = b;
    W(mat, 0x8) = c;
    W(mat, 0xC) = d;
    a = W(src, 0x10);
    b = W(src, 0x14);
    c = W(src, 0x18);
    d = W(src, 0x1C);
    W(mat, 0x10) = a;
    W(mat, 0x14) = b;
    W(mat, 0x18) = c;
    W(mat, 0x1C) = d;
    boneLoadRT(mat);
    GTE_MTC2(0, W(m, 0x74));
    GTE_MTC2(1, W(m, 0x78));
    GTE_CMD(0x4A486012);
    a = (s16)GTE_MFC2(9);
    b = (s16)GTE_MFC2(10);
    c = (s16)GTE_MFC2(11);
    W(mat, 0x14) += a;
    W(mat, 0x18) += b;
    W(mat, 0x1C) += c;
}

/* One rotation key: the u16 itself when the bone's flag bit is set, else the s8 at anim + key + index. */
#define KEY(k, bit, index) ((flags & (bit)) ? (k) : (u32)(s32)(s8)anim[(k) + (index)])

u8 *btl_calcBoneMatrices(BtlUnit9C *m, s32 frame, u8 *out) {
    u8 *anim = (u8 *)m->motion;
    u8 *base, *mat, *end, *keys, *nib, *bone, *parent;
    u32 *rot = (u32 *)SCRATCHPAD;      /* SVECTOR for math_rotMatrixZYX_gte */
    u32 *ofs = (u32 *)(SCRATCHPAD + 8); /* (0, 0, length) */
    u32 n, aflags, flags, k0, k1, x, y, z, w0, w1;

    m->bones = (BtlBone *)out;
    if (anim == NULL) {
        if (m->parent != NULL) {
            boneFromParent(m, out);
        } else {
            H(out, 0x0) = H(m, 0x24);
            H(out, 0x2) = -H(m, 0x26);
            H(out, 0x4) = -H(m, 0x28);
            H(out, 0x6) = H(m, 0x2A);
            H(out, 0x8) = -H(m, 0x2C);
            H(out, 0xA) = -H(m, 0x2E);
            H(out, 0xC) = H(m, 0x30);
            H(out, 0xE) = -H(m, 0x32);
            H(out, 0x10) = -H(m, 0x34);
            W(out, 0x14) = W(m, 0x38);
            W(out, 0x18) = W(m, 0x3C);
            W(out, 0x1C) = W(m, 0x40);
        }
        return out + 0x20;
    }

    n = m->header->boneCount;
    base = n < 31 ? SCRATCHPAD + 0x18 : out;
    ofs[0] = 0;
    ofs[1] = 0;
    keys = (u8 *)((BtlUnit9CC *)anim)->rotKeys;
    nib = (u8 *)((BtlUnit9CC *)anim)->rotKeysLo;
    aflags = H(anim, 0xA);
    W(base, 0x14) = (aflags & 1) ? (s32)(s16)H(anim, 4) : *(s16 *)(anim + H(anim, 4) + (frame & 0xFFFF) * 2);
    W(base, 0x18) = (aflags & 2) ? (s32)(s16)H(anim, 6) : *(s16 *)(anim + H(anim, 6) + (frame & 0xFFFF) * 2);
    W(base, 0x1C) = (aflags & 4) ? (s32)(s16)H(anim, 8) : *(s16 *)(anim + H(anim, 8) + (frame & 0xFFFF) * 2);
    end = base + n * 32;

    mat = base;
    do {
        w1 = W(keys, 4);
        w0 = W(keys, 0);
        flags = w1 >> 16;
        x = KEY(w0 & 0xFFFF, 1, frame) << 4;
        y = KEY(w0 >> 16, 2, frame) << 20;
        z = KEY(w1 & 0xFFFF, 4, frame) << 4;
        rot[0] = y | (x & 0xFFFF);
        rot[1] = z;
        if (nib != NULL) {
            u32 half = (u32)frame >> 1;
            u32 odd = frame & 1;

            w1 = W(nib, 4);
            w0 = W(nib, 0);
            flags = w1 >> 16;
            k0 = w0 & 0xFFFF;
            if (!(flags & 1)) {
                k0 = (s8)anim[k0 + half];
                if (odd) {
                    k0 >>= 4;
                }
            }
            k1 = w0 >> 16;
            if (!(flags & 2)) {
                k1 = (s8)anim[k1 + half];
                if (odd) {
                    k1 >>= 4;
                }
            }
            z = w1 & 0xFFFF;
            if (!(flags & 4)) {
                z = (s8)anim[z + half];
                if (odd) {
                    z >>= 4;
                }
            }
            rot[0] = rot[0] | (k0 & 0xF) | ((k1 & 0xF) << 16);
            rot[1] = rot[1] | (z & 0xF);
            nib += 8;
        }
        keys += 8;
        math_rotMatrixZYX_gte(rot, mat);
        mat += 0x20;
    } while (mat < end);

    if (m->parent != NULL) {
        boneFromParent(m, base);
    } else {
        u8 *own = (u8 *)&m->matrix;

        boneLoadRT(own);
        boneLoadTR(own);
        boneRotColumn(base);
        boneRotColumn(base + 2);
        boneRotColumn(base + 4);
        GTE_MTC2(0, H(base, 0x14) | (H(base, 0x18) << 16));
        GTE_MTC2(1, W(base, 0x1C));
        GTE_CMD(0x4A480012);
        W(base, 0x14) = GTE_MFC2(25);
        W(base, 0x18) = GTE_MFC2(26);
        W(base, 0x1C) = GTE_MFC2(27);
    }

    bone = (u8 *)m->header->bones + 4;
    parent = base + bone[3] * 32;
    mat = base + 0x20;
    do {
        boneLoadRT(parent);
        GTE_MTC2(9, H(mat, 0x0));
        GTE_MTC2(10, H(mat, 0x6));
        GTE_MTC2(11, H(mat, 0xC));
        GTE_CMD(0x4A49E012);
        boneLoadTR(parent);
        H(mat, 0x0) = GTE_MFC2(9);
        H(mat, 0x6) = GTE_MFC2(10);
        H(mat, 0xC) = GTE_MFC2(11);
        GTE_MTC2(9, H(mat, 0x2));
        GTE_MTC2(10, H(mat, 0x8));
        GTE_MTC2(11, H(mat, 0xE));
        GTE_CMD(0x4A49E012);
        ofs[2] = *(s16 *)bone;
        H(mat, 0x2) = GTE_MFC2(9);
        H(mat, 0x8) = GTE_MFC2(10);
        H(mat, 0xE) = GTE_MFC2(11);
        boneRotColumn(mat + 4);
        bone += 4;
        GTE_MTC2(0, H(ofs, 0) | (H(ofs, 4) << 16));
        GTE_MTC2(1, ofs[2]);
        GTE_CMD(0x4A480012);
        parent = base + bone[3] * 32;
        W(mat, 0x14) = GTE_MFC2(25);
        W(mat, 0x18) = GTE_MFC2(26);
        W(mat, 0x1C) = GTE_MFC2(27);
        mat += 0x20;
    } while (mat < end);

    if (n < 31) {
        u8 *dst = (u8 *)m->bones;
        u8 *srcm = base;
        u32 a, b, c, d;

        do {
            a = W(srcm, 0x0);
            b = W(srcm, 0x4);
            c = W(srcm, 0x8);
            d = W(srcm, 0xC);
            W(dst, 0x0) = a;
            W(dst, 0x4) = b;
            W(dst, 0x8) = c;
            W(dst, 0xC) = d;
            a = W(srcm, 0x10);
            b = W(srcm, 0x14);
            c = W(srcm, 0x18);
            d = W(srcm, 0x1C);
            W(dst, 0x10) = a;
            W(dst, 0x14) = b;
            W(dst, 0x18) = c;
            W(dst, 0x1C) = d;
            srcm += 0x20;
            dst += 0x20;
        } while (srcm < end);
    }
    return out + n * 32;
}
