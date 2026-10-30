/* ovl_0b effect-model vertex projection: btlfx_projectSliceMesh (0x800DDF10) and btlfx_projectMorphMesh
 * (0x800DF15C), hand-written asm in the original. Both fill g_vtxScreenXY / g_vtxScreenZ for one mesh of a
 * model whose bone matrices btl_calcBoneMatrices has built (BtlUnit9C.unk20), for btl_drawMesh*.
 *
 * Register convention: standard C ABI. GTE: H (control 26) = the camera's projection distance
 * (GameData.unkA); RT/TR end as the last bone's camera-space matrix; V0-V2, IR, MAC and the RTPT outputs as
 * the last vertex left them.
 *
 * Per bone b with n = BtlMesh.unk14[b] vertices (none: skipped), the vertices continue the mesh's SVECTOR
 * array (unk18) and the screen arrays from where the previous bone stopped. Projection runs in triples, so a
 * bone whose count isn't a multiple of 3 also projects (and writes) up to two entries past its last vertex,
 * which the next bone then overwrites; these overruns are kept.
 *
 * Checked with tools/equiv.py against the original (src_equiv/ovl_0b/check.py).
 */
#include "ovl_0b.h"
#include "../src_equiv.h"

#define W(p, o) (*(u32 *)((u8 *)(p) + (o)))
#define H(p, o) (*(u16 *)((u8 *)(p) + (o)))

static void projLoadMatrix(u8 *mat) {
    GTE_CTC2(0, W(mat, 0x0));
    GTE_CTC2(1, W(mat, 0x4));
    GTE_CTC2(2, W(mat, 0x8));
    GTE_CTC2(3, W(mat, 0xC));
    GTE_CTC2(4, W(mat, 0x10));
    GTE_CTC2(5, W(mat, 0x14));
    GTE_CTC2(6, W(mat, 0x18));
    GTE_CTC2(7, W(mat, 0x1C));
}

/* Slice: each vertex goes through its bone's matrix (MVMVA RT * V0 + TR); y is clamped to the model's slice
 * plane BtlUnit9C.unk5A (y >= unk5A becomes unk5A and the z word gets bit 28 as a clipped mark), then the
 * camera (GameData+0x18) projects the triples in place (SXY over the xy, SZ into the low halfword of the z
 * word, keeping the mark). */
void btlfx_projectSliceMesh(BtlUnit9C *m, u8 meshIdx) {
    u32 *xyTab = (u32 *)g_vtxScreenXY;
    u32 *zTab = (u32 *)g_vtxScreenZ;
    BtlMesh *mesh;
    u8 *bone, *v;
    u32 nb, b, n, k, base, *xy, *z, y, w;
    s32 ylim;

    GTE_CTC2(26, *(u16 *)&g_gameState->gameData->projDist);
    base = 0;
    mesh = &m->header->meshes[meshIdx];
    bone = (u8 *)m->bones;
    nb = m->header->boneCount;
    ylim = m->sliceY;
    v = (u8 *)mesh->vertices;
    if (nb == 0) {
        return;
    }
    b = 0;
    do {
        n = mesh->vertexCounts[b];
        b++;
        if (n != 0) {
            xy = xyTab + base;
            z = zTab + base;
            projLoadMatrix(bone);
            k = 0;
            do {
                GTE_MTC2(0, W(v, 0));
                GTE_MTC2(1, W(v, 4));
                k++;
                v += 8;
                GTE_CMD(0x4A480012);
                H(xy, 0) = GTE_MFC2(9);
                y = GTE_MFC2(10);
                w = GTE_MFC2(11) & 0xFFFF;
                if (!((s32)y < ylim)) {
                    y = ylim;
                    w |= 0x10000000;
                }
                H(xy, 2) = y;
                *z = w;
                xy++;
                z++;
            } while (k < n);

            xy = xyTab + base;
            z = zTab + base;
            projLoadMatrix((u8 *)&g_gameState->gameData->viewMatrix);
            k = 0;
            do {
                GTE_MTC2(0, xy[0]);
                GTE_MTC2(1, z[0]);
                GTE_MTC2(2, xy[1]);
                GTE_MTC2(3, z[1]);
                GTE_MTC2(4, xy[2]);
                GTE_MTC2(5, z[2]);
                k += 3;
                GTE_CMD(0x4A280030);
                xy[0] = GTE_MFC2(12);
                xy[1] = GTE_MFC2(13);
                xy[2] = GTE_MFC2(14);
                xy += 3;
                H(z, 0) = GTE_MFC2(17);
                H(z, 4) = GTE_MFC2(18);
                H(z, 8) = GTE_MFC2(19);
                z += 3;
            } while (k < n);
            base += n;
        }
        bone += 0x20;
    } while (b < nb);
}

/* Morph: per bone, the camera matrix (GameData+0x18) times the bone matrix gives the camera-space matrix
 * (rotation columns through MVMVA RT * IR, translation through RT * V0 + TR; its padding halfword is whatever
 * the stack held). Each vertex is morph + (mesh - morph) * factor / 4096 (INTPL, IR0 = factor: the morph
 * target BtlUnit9C.unk5C's vertex in IR, the mesh's in FC), stored as xy and z words (the loop reads one
 * vertex past the end of both arrays), then the triples are projected in place (SXY and whole SZ words) with
 * the next triple preloaded. */
void btlfx_projectMorphMesh(BtlUnit9C *m, u8 meshIdx, u16 factor) {
    u32 *xyTab = (u32 *)g_vtxScreenXY;
    u32 *zTab = (u32 *)g_vtxScreenZ;
    BtlMesh *mesh, *morph;
    u8 *bone, *cam, *a, *t;
    u32 lm[8];
    u32 nb, b, k, base, *xy, *z, w;
    s32 n, ax, ay, az, tx, ty, tz;

    mesh = &m->header->meshes[meshIdx];
    morph = &m->morph->meshes[meshIdx];
    GTE_CTC2(26, *(u16 *)&g_gameState->gameData->projDist);
    base = 0;
    nb = m->header->boneCount;
    bone = (u8 *)m->bones;
    if (nb == 0) {
        return;
    }
    b = 0;
    do {
        n = mesh->vertexCounts[b];
        b++;
        if (n != 0) {
            cam = (u8 *)&g_gameState->gameData->viewMatrix;
            projLoadMatrix(cam);
            for (k = 0; k < 3; k++) {
                GTE_MTC2(9, H(bone, k * 2));
                GTE_MTC2(10, H(bone, k * 2 + 6));
                GTE_MTC2(11, H(bone, k * 2 + 0xC));
                GTE_CMD(0x4A49E012);
                H(lm, k * 2) = GTE_MFC2(9);
                H(lm, k * 2 + 6) = GTE_MFC2(10);
                H(lm, k * 2 + 0xC) = GTE_MFC2(11);
            }
            xy = xyTab + base;
            z = zTab + base;
            a = (u8 *)mesh->vertices + base * 8;
            GTE_MTC2(0, H(bone, 0x14) | (H(bone, 0x18) << 16));
            GTE_MTC2(1, W(bone, 0x1C));
            GTE_CMD(0x4A480012);
            t = (u8 *)morph->vertices + base * 8;
            lm[5] = GTE_MFC2(25);
            lm[6] = GTE_MFC2(26);
            lm[7] = GTE_MFC2(27);
            GTE_MTC2(8, factor);
            k = n;
            w = W(a, 0);
            ax = (s32)(w << 16) >> 16;
            ay = (s32)w >> 16;
            az = *(s16 *)(a + 4);
            w = W(t, 0);
            tx = (s32)(w << 16) >> 16;
            ty = (s32)w >> 16;
            tz = *(s16 *)(t + 4);
            do {
                GTE_CTC2(21, ax);
                GTE_CTC2(22, ay);
                GTE_CTC2(23, az);
                GTE_MTC2(9, tx);
                GTE_MTC2(10, ty);
                GTE_MTC2(11, tz);
                a += 8;
                t += 8;
                GTE_CMD(0x4A980011);
                w = W(a, 0);
                ax = (s32)(w << 16) >> 16;
                ay = (s32)w >> 16;
                az = *(s16 *)(a + 4);
                w = W(t, 0);
                tx = (s32)(w << 16) >> 16;
                ty = (s32)w >> 16;
                tz = *(s16 *)(t + 4);
                *xy = (GTE_MFC2(25) & 0xFFFF) | (GTE_MFC2(26) << 16);
                *z = GTE_MFC2(27);
                xy++;
                z++;
            } while (--k != 0);

            xy = xyTab + base;
            z = zTab + base;
            projLoadMatrix((u8 *)lm);
            base += n;
            GTE_MTC2(0, xy[0]);
            GTE_MTC2(1, z[0]);
            GTE_MTC2(2, xy[1]);
            GTE_MTC2(3, z[1]);
            GTE_MTC2(4, xy[2]);
            GTE_MTC2(5, z[2]);
            do {
                n -= 3;
                GTE_CMD(0x4A280030);
                GTE_MTC2(0, xy[3]);
                GTE_MTC2(1, z[3]);
                GTE_MTC2(2, xy[4]);
                GTE_MTC2(3, z[4]);
                GTE_MTC2(4, xy[5]);
                GTE_MTC2(5, z[5]);
                xy[0] = GTE_MFC2(12);
                xy[1] = GTE_MFC2(13);
                xy[2] = GTE_MFC2(14);
                z[0] = GTE_MFC2(17);
                z[1] = GTE_MFC2(18);
                z[2] = GTE_MFC2(19);
                xy += 3;
                z += 3;
            } while (n > 0);
        }
        bone += 0x20;
    } while (b < nb);
}
