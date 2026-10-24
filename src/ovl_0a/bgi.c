#include "ovl_0a.h"

s32 bgi_pointInTriangle(Unk800C9DF8 *b, Unk800C9DF8Tri *t, Unk800C9E00 *n, s16 *p) {
    s8 c[2];
    s32 d[2];
    Unk800C9DFCv *pv[2];
    s16 o[3];
    Unk800C9DFCv v[3];
    u8 i;
    s32 r;
    u8 x;
    u8 z;
    Unk800C9DFCv *vtx;
    Unk800C9E04 *pl;
    s32 a;
    s32 cz;

    x = 0;
    z = 2;
    if (n->unk6 >= 0) {
        pl = &g_fieldBgi.unk14[n->unk6];
        if ((u32)(pl->unk4 + 0x50F4) <= 0xA1E8) {
            a = pl->unk0;
            cz = pl->unk8;
            if (ABS(a) > ABS(cz)) {
                x = 1;
                z = 2;
            } else {
                x = 0;
                z = 1;
            }
        }
    }
    r = 0;
    i = 0;
    vtx = (Unk800C9DFCv *)g_fieldBgi.unkC;
    o[x] = b->unk8[x] + t->unkA[x];
    o[z] = b->unk8[z] + t->unkA[z];
    c[0] = 0;
    c[1] = 0;
    for (; i < 3; i++) {
        v[i].unk0[x] = o[x] + vtx[n->unkC[i]].unk0[x];
        v[i].unk0[z] = o[z] + vtx[n->unkC[i]].unk0[z];
        if (v[i].unk0[x] == p[x]) {
            c[0]++;
            if (v[i].unk0[z] <= p[z]) {
                c[0] = -c[0];
            }
        }
        if (v[i].unk0[z] == p[z]) {
            c[1]++;
            if (v[i].unk0[x] <= p[x]) {
                c[1] = -c[1];
            }
        }
    }
    if (c[0] == -2) {
        return 1;
    }
    if (c[1] == -2) {
        return 1;
    }
    pv[0] = &v[2];
    c[0] = p[z] >= pv[0]->unk0[z];
    for (i = 0; i < 3; i++) {
        pv[1] = &v[i];
        c[1] = p[z] >= pv[1]->unk0[z];
        if (c[0] != c[1]) {
            d[0] = (pv[1]->unk0[z] - p[z]) * (pv[0]->unk0[x] - pv[1]->unk0[x]);
            d[1] = (pv[1]->unk0[x] - p[x]) * (pv[0]->unk0[z] - pv[1]->unk0[z]);
            if (d[0] == d[1]) {
                return 1;
            }
            if ((d[1] < d[0]) == c[1]) {
                r ^= 1;
            }
        }
        pv[0] = pv[1];
        c[0] = c[1];
    }
    return r;
}

s32 bgi_pointInTriangleXZ(Unk800C9DF8 *b, Unk800C9DF8Tri *t, Unk800C9E00 *n, SVECTOR *sv) {
    s16 *p = (s16 *)sv;
    s8 c[2];
    s32 d[2];
    Unk800C9DFCv *pv[2];
    s16 o[3];
    Unk800C9DFCv v[3];
    u8 i;
    u8 r;
    u8 x;
    u8 z;
    Unk800C9DFCv *vtx;

    x = 0;
    z = 2;
    r = 0;
    i = 0;
    vtx = (Unk800C9DFCv *)g_fieldBgi.unkC;
    o[x] = b->unk8[x] + t->unkA[x];
    o[z] = b->unk8[z] + t->unkA[z];
    c[0] = 0;
    c[1] = 0;
    for (; i < 3; i++) {
        v[i].unk0[x] = o[x] + vtx[n->unkC[i]].unk0[x];
        v[i].unk0[z] = o[z] + vtx[n->unkC[i]].unk0[z];
        if (v[i].unk0[x] == p[x]) {
            c[0]++;
            if (v[i].unk0[z] <= p[z]) {
                c[0] = -c[0];
            }
        }
        if (v[i].unk0[z] == p[z]) {
            c[1]++;
            if (v[i].unk0[x] <= p[x]) {
                c[1] = -c[1];
            }
        }
    }
    if (c[0] == -2) {
        return 1;
    }
    if (c[1] == -2) {
        return 1;
    }
    pv[0] = &v[2];
    c[0] = p[z] >= pv[0]->unk0[z];
    for (i = 0; i < 3; i++) {
        pv[1] = &v[i];
        c[1] = p[z] >= pv[1]->unk0[z];
        if (c[0] != c[1]) {
            d[0] = (pv[1]->unk0[z] - p[z]) * (pv[0]->unk0[x] - pv[1]->unk0[x]);
            d[1] = (pv[1]->unk0[x] - p[x]) * (pv[0]->unk0[z] - pv[1]->unk0[z]);
            if (d[0] == d[1]) {
                return 1;
            }
            if ((d[1] < d[0]) == c[1]) {
                r = !r;
            }
        }
        pv[0] = pv[1];
        c[0] = c[1];
    }
    return r;
}

s32 bgi_findEdgeCrossing(Unk800C9DF8 *b, Unk800C9DF8Tri *t, Unk800C9E00 *n, s16 *arg3, s16 *arg4, SVECTOR *arg5, s16 *arg6, s32 arg7) {
    s16 o[3];
    Unk800C9DFCv *pv[3];
    Unk800C9DFCv v[3];
    s16 hit[3][3];
    s16 idx[3];
    Unk800C9DFCv *vtx;
    Unk800C9E04 *pl;
    s16 i;
    s16 cnt;
    s16 edge;
    u8 x;
    u8 y;
    u8 z;
    u8 best;
    s32 a;
    s32 c;
    s32 d;
    s32 e;
    s32 f;
    s32 ax;
    s32 m;
    s32 min;
    s32 ex;
    s32 ez;

    edge = arg7;
    x = 0;
    z = 2;
    y = 1;
    if (n->unk6 >= 0) {
        pl = &g_fieldBgi.unk14[n->unk6];
        if ((u32)(pl->unk4 + 0x50F4) <= 0xA1E8) {
            a = pl->unk0;
            c = pl->unk8;
            if (ABS(a) > ABS(c)) {
                x = 1;
                z = 2;
                y = 0;
            } else {
                x = 0;
                z = 1;
                y = 2;
            }
        }
    }
    vtx = (Unk800C9DFCv *)((u8 *)b + b->unk3A);
    o[0] = b->unk8[0] + t->unkA[0];
    o[1] = b->unk8[1] + t->unkA[1];
    o[2] = b->unk8[2] + t->unkA[2];
    for (i = 0; i < 3; i++) {
        v[i].unk0[0] = o[0] + vtx[n->unkC[i]].unk0[0];
        v[i].unk0[1] = o[1] + vtx[n->unkC[i]].unk0[1];
        v[i].unk0[2] = o[2] + vtx[n->unkC[i]].unk0[2];
    }
    pv[0] = &v[2];
    cnt = 0;
    for (i = 0; i < 3; i++) {
        pv[1] = &v[i];
        if (i != edge) {
            d = (pv[1]->unk0[x] - pv[0]->unk0[x]) * (ez = arg3[z] - arg4[z]);
            ex = arg3[x] - arg4[x];
            e = (pv[0]->unk0[z] - arg4[z]) * ex;
            d -= (pv[1]->unk0[z] - pv[0]->unk0[z]) * ex;
            e -= (pv[0]->unk0[x] - arg4[x]) * ez;
            if (ABS(e) <= ABS(d)) {
                f = (pv[0]->unk0[z] - arg4[z]) * (pv[1]->unk0[x] - pv[0]->unk0[x]);
                f -= (pv[0]->unk0[x] - arg4[x]) * (pv[1]->unk0[z] - pv[0]->unk0[z]);
                if (ABS(f) <= ABS(d)) {
                    if ((e >= 0 && d >= 0) || (e <= 0 && d < 0)) {
                        if ((f >= 0 && d >= 0) || (f <= 0 && d < 0)) {
                            if (d != 0) {
                                while (e > 0x80000) {
                                    e >>= 1;
                                    d >>= 1;
                                }
                                hit[cnt][x] = pv[0]->unk0[x] + e * (pv[1]->unk0[x] - pv[0]->unk0[x]) / d;
                                hit[cnt][z] = pv[0]->unk0[z] + e * (pv[1]->unk0[z] - pv[0]->unk0[z]) / d;
                                ax = pv[1]->unk0[x] - pv[0]->unk0[x];
                                if (ax < 0) {
                                    ax = pv[0]->unk0[x] - pv[1]->unk0[x];
                                }
                                if ((pv[1]->unk0[z] - pv[0]->unk0[z] >= 0 ? pv[1]->unk0[z] - pv[0]->unk0[z] : pv[0]->unk0[z] - pv[1]->unk0[z]) < ax) {
                                    hit[cnt][y] = pv[1]->unk0[y] - (pv[1]->unk0[x] - hit[cnt][x]) * (pv[0]->unk0[y] - pv[1]->unk0[y]) / (pv[0]->unk0[x] - pv[1]->unk0[x]);
                                } else {
                                    hit[cnt][y] = pv[1]->unk0[y] - (pv[1]->unk0[z] - hit[cnt][z]) * (pv[0]->unk0[y] - pv[1]->unk0[y]) / (pv[0]->unk0[z] - pv[1]->unk0[z]);
                                }
                            }
                            idx[cnt] = i;
                            cnt++;
                        }
                    }
                }
            }
        }
        pv[0] = pv[1];
    }
    if (cnt == 1) {
        *arg6 = idx[0];
        arg5->vx = hit[0][0];
        arg5->vy = hit[0][1];
        arg5->vz = hit[0][2];
    } else if (cnt < 2) {
        *arg6 = -1;
        return 0;
    } else switch (cnt) {
    case 2:
        switch (idx[0] + idx[1]) {
        case 1:
            pv[0] = &v[1];
            pv[1] = &v[2];
            pv[2] = &v[0];
            break;
        case 2:
            pv[0] = &v[1];
            pv[1] = &v[0];
            pv[2] = &v[2];
            break;
        case 3:
            pv[0] = &v[2];
            pv[1] = &v[0];
            pv[2] = &v[1];
            break;
        }
        if (bgi_segmentsCross(pv[0]->unk0, arg3, pv[2]->unk0, pv[1]->unk0, x, z)) {
            *arg6 = idx[0];
            arg5->vx = hit[0][0];
            arg5->vy = hit[0][1];
            arg5->vz = hit[0][2];
        } else {
            *arg6 = idx[1];
            arg5->vx = hit[1][0];
            arg5->vy = hit[1][1];
            arg5->vz = hit[1][2];
        }
        break;
    case 3:
        best = 0;
        min = 0x7FFFFFFF;
        for (i = 0; i < 3; i++) {
            m = hit[i][0] * hit[i][0] + hit[i][1] * hit[i][1] + hit[i][2] * hit[i][2];
            if (m < min) {
                best = i;
                min = m;
            }
        }
        *arg6 = best;
        arg5->vx = hit[best][0];
        arg5->vy = hit[best][1];
        arg5->vz = hit[best][2];
        break;
    default:
        *arg6 = -1;
        return 0;
    }
    return 1;
}

/* debug: print a walkmesh */
void bgi_dumpText(Unk800C9DF8 *arg0) {
    Unk800C9E00 *tri;
    Unk800C9DF8Link *edge;
    Unk800C9E10 *anim;
    Unk800C9E10Frame *frame;
    Unk800C9DF8Tri *floor;
    Unk800C9DFC *vtx;
    Unk800C9E04 *nrm;
    s32 *ids;
    s32 i;
    s32 j;
    s32 k;

    if (arg0 == NULL) {
        return;
    }
    printf("BGI Text Dump\n");
    printf("=============\n");
    printf("Data Size\t\t : %d [+4 for file magic]\n", arg0->unk0);
    printf("Original Position: (%6d,%6d,%6d)\n", arg0->unk2, arg0->unk4, arg0->unk6);
    printf("Current Position : (%6d,%6d,%6d)\n", arg0->unk8[0], arg0->unk8[1], arg0->unk8[2]);
    printf("Minimum Values   : (%6d,%6d,%6d)\n", arg0->unkE, arg0->unk10, arg0->unk12);
    printf("Maximum Values   : (%6d,%6d,%6d)\n", arg0->unk14, arg0->unk16, arg0->unk18);
    printf("Default Char Pos : (%6d,%6d,%6d)\n", arg0->unk1A, arg0->unk1C, arg0->unk1E);
    printf("Active Floor\t : %d\n", arg0->unk20);
    printf("Active Triangle  : %d\n", arg0->unk22);
    printf("Triangle Count   : %d\n", arg0->unk24);
    printf("Triangle Offset  : %d\n", arg0->unk26);
    tri = (Unk800C9E00 *)((u8 *)arg0 + arg0->unk26);
    for (i = 0; i < arg0->unk24; tri++, i++) {
        printf("  %4d: Flags\t\t\t= 0x%04X\n", i, tri->unk0);
        printf("\t\tData\t\t\t = %d\n", tri->unk2);
        printf("\t\tFloor Index \t = %d\n", tri->unk4);
        printf("\t\tNormal Index\t = %d\n", tri->unk6);
        printf("\t\tTheta X \t\t = %d\n", (s16)tri->unk8);
        printf("\t\tTheta Z \t\t = %d\n", tri->unkA);
        printf("\t\tD\t\t\t\t = %ld\n", tri->unk24);
        printf("\t\tCenter  \t\t = (%6d,%6d,%6d)\n", tri->unk1E, tri->unk20, tri->unk22);
        printf("\t\tVertex Indices   = (%6d,%6d,%6d)\n", tri->unkC[0], tri->unkC[1], tri->unkC[2]);
        printf("\t\tEdge Indices\t = (%6d,%6d,%6d)\n", tri->unk12[0], tri->unk12[1], tri->unk12[2]);
        printf("\t\tNeighbor Indices = (%6d,%6d,%6d)\n", tri->unk18[0], tri->unk18[1], tri->unk18[2]);
    }
    printf("Edge Count  \t : %d\n", arg0->unk28);
    printf("Edge Offset \t : %d\n", arg0->unk2A);
    edge = (Unk800C9DF8Link *)((u8 *)arg0 + arg0->unk2A);
    for (i = 0; i < arg0->unk28; edge++, i++) {
        printf("  %4d: Flags\t   = 0x%04X\n", i, edge->unk0);
        printf("\t\tEdge Clone  = %d\n", edge->unk2);
    }
    printf("Keyframe Animation Count  : %d\n", arg0->unk2C);
    printf("Keyframe Animation Offset : %d\n", arg0->unk2E);
    anim = (Unk800C9E10 *)((u8 *)arg0 + arg0->unk2E);
    for (i = 0; i < arg0->unk2C; anim++, i++) {
        printf("  %4d: Flags\t\t  = 0x%04X\n", i, anim->unk0);
        printf("\t\tFrame Count    = %d\n", anim->unk2);
        printf("\t\tCurrent Frame  = %ld\n", anim->unk8);
        printf("\t\tFrame Rate     = %d\n", anim->unk4);
        printf("\t\tCounter \t   = %d\n", (u16)anim->unk6);
        printf("\t\tFrame Offset   = %ld\n", anim->unkC);
        frame = (Unk800C9E10Frame *)((u8 *)arg0 + anim->unkC);
        for (j = 0; j < anim->unk2; frame++, j++) {
            printf("\t\t%4d: Frame Flags     = 0x%04X\n", j, frame->unk0);
            printf("              Value           = %d\n", frame->unk2);
            printf("              Triangle Count  = %d\n", frame->unk4);
            printf("\t\t\t  Triangle Offset = %d\n", frame->unk6);
            ids = (s32 *)((u8 *)arg0 + frame->unk6);
            for (k = 0; k < frame->unk4; ids++, k++) {
                printf("\t\t\t\t%4d: Triangle Ndx = %ld\n", j, *ids);
            }
        }
    }
    printf("Floor Count \t : %d\n", arg0->unk30);
    printf("Floor Offset\t : %d\n", arg0->unk32);
    floor = (Unk800C9DF8Tri *)((u8 *)arg0 + arg0->unk32);
    for (i = 0; i < arg0->unk30; floor++, i++) {
        printf("  %4d: Flags\t\t\t\t= %d\n", i, floor->unk0);
        printf("\t\tFloor Index \t\t  = %d\n", floor->unk2);
        printf("\t\tOriginal Position\t  = (%6d,%6d,%6d)\n", floor->unk4[0], floor->unk4[1], floor->unk4[2]);
        printf("\t\tCurrent Position\t  = (%6d,%6d,%6d)\n", (s16)floor->unkA[0], (s16)floor->unkA[1], (s16)floor->unkA[2]);
        printf("\t\tMinumim Values  \t  = (%6d,%6d,%6d)\n", floor->unk10[0], floor->unk10[1], floor->unk10[2]);
        printf("\t\tMaximum Values  \t  = (%6d,%6d,%6d)\n", floor->unk16[0], floor->unk16[1], floor->unk16[2]);
        printf("\t\tTriangle Count  \t  = %d\n", floor->unk1C);
        printf("\t\tTriangle Index Offset = %d\n", floor->unk1E);
        ids = (s32 *)((u8 *)arg0 + floor->unk1E);
        for (j = 0; j < floor->unk1C; ids++, j++) {
            printf("\t\t  %4d: Triangle Ndx = %ld\n", j, *ids);
        }
    }
    printf("Vertex Count\t : %d\n", arg0->unk38);
    printf("Vertex Offset\t : %d\n", arg0->unk3A);
    vtx = (Unk800C9DFC *)((u8 *)arg0 + arg0->unk3A);
    for (i = 0; i < arg0->unk38; vtx++, i++) {
        printf("  %4d: (%d,%d,%d)\n", i, vtx->unk0, vtx->unk2, vtx->unk4);
    }
    printf("Normal Count\t : %d\n", arg0->unk34);
    printf("Normal Offset\t : %d\n", arg0->unk36);
    nrm = (Unk800C9E04 *)((u8 *)arg0 + arg0->unk36);
    for (i = 0; i < arg0->unk34; nrm++, i++) {
        printf("  %4d: (%lx,%lx,%lx)\n", i, nrm->unk0, nrm->unk4, nrm->unk8);
        printf(D_800A7814, nrm->unkC);
    }
}

s32 bgi_updateAnims(void) {
    Unk800C9DF8 *b;
    Unk800C9E10 *e;
    u16 cnt;
    Unk800C9E00 *nodes;
    u16 i;
    s16 old;
    Unk800C9DF8Pt *pts;
    Unk800C9DF8Pt *pt;
    u16 j;
    u16 k;
    s32 *ids;
    Unk800C9E00 *n;
    s32 pos;

    s32 v;
    u16 f;
    u16 num;
    u16 m;

    b = g_fieldBgi.unk8;
    e = g_fieldBgi.unk20;
    cnt = b->unk2C;
    nodes = g_fieldBgi.unk10;
    for (i = 0; i < cnt; e++, i++) {
        if (!(e->unk0 & 1)) {
            continue;
        }
        if (!(e->unk0 & 0xC)) {
            continue;
        }
        old = -1;
        pts = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
        num = e->unk2;
        for (j = 0; j < num; j++) {
            ids = (s32 *)((u8 *)b + pts[j].unk6);
            m = pts[j].unk4;
            for (k = 0; k < m; k++, ids++) {
                f = nodes[*ids].unk0;
                if (f & 1) {
                    old = j;
                }
                nodes[*ids].unk0 = f & ~1;
            }
        }
        pos = e->unk8;
        j = pos >> 8;
        ids = (s32 *)((u8 *)b + pts[j].unk6);
        m = pts[j].unk4;
        for (k = 0; k < m; k++) {
            nodes[*ids++].unk0 |= 1;
        }
        if (old != j) {
            pt = &pts[j];
            bgi_remapActors(b, e, pt, old, j);
            bgi_linkFrameNeighbors(b, e, pt);
        }
        if (pts[j].unk2 < 0) {
            pts[j].unk2 = 0;
            e->unk6 = 0;
            e->unk0 &= ~4;
            return 1;
        }
        e->unk6++;
        if ((u16)e->unk6 >= pts[j].unk2) {
            e->unk6 = 0;
            e->unk8 = pos + e->unk4;
            v = e->unk8 >> 8;
            if (v >= e->unk2) {
                if (e->unk0 & 0x10) {
                    e->unk8 = ((e->unk2 - 1) << 8) | 0xFF;
                    bgi_setAnimRate(i, (s16)-e->unk4);
                } else {
                    e->unk8 = 0;
                }
                e->unk0 &= ~4;
            } else if (v < 0) {
                if (e->unk0 & 0x10) {
                    e->unk8 = 0;
                    bgi_setAnimRate(i, (s16)-e->unk4);
                } else {
                    e->unk8 = ((e->unk2 - 1) << 8) | 0xFF;
                }
                e->unk0 &= ~4;
            }
        }
    }
    return 1;
}

s32 bgi_showAnimFrame(u32 arg0, u32 arg1) {
    Unk800C9DF8 *b;
    Unk800C9E10 *e;
    Unk800C9DF8Pt *pt;
    s32 *ids;
    s32 i;
    s32 j;
    s16 old;
    Unk800C9E00 *n;
    u16 f;

    b = g_fieldBgi.unk8;
    if (arg0 >= b->unk2C) {
        return 0;
    }
    e = &g_fieldBgi.unk20[arg0];
    old = -1;
    if (arg1 >= e->unk2) {
        return 0;
    }
    pt = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
    for (i = 0; i < e->unk2; pt++, i++) {
        ids = (s32 *)((u8 *)b + pt->unk6);
        for (j = 0; j < pt->unk4; ids++, j++) {
            n = &g_fieldBgi.unk10[*ids];
            if (n->unk0 & 1) {
                old = i;
                n->unk0 &= ~1;
            } else {
                n->unk0 &= ~1;
            }
        }
    }
    pt = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
    pt += arg1;
    ids = (s32 *)((u8 *)b + pt->unk6);
    for (i = 0; i < pt->unk4; ids++, i++) {
        n = &g_fieldBgi.unk10[*ids];
        n->unk0 |= 1;
    }
    if (arg1 != old) {
        bgi_remapActors(b, e, pt, old, arg1 & 0xFFFF);
        bgi_linkFrameNeighbors(b, e, pt);
    }
    e->unk8 = arg1 << 8;
    return 1;
}

/* walkmesh triangle transfer: express the actor position relative to triangle n0 by cross products (nv, over
   the triangle's doubled area) and re-project it onto triangle n1; the height comes from n1's plane or, without
   one, from n1's first vertex */
s32 bgi_transferActorPos(Unk800C9DF8 *b, Unk800C9DF4 *a, Unk800C9E00 *n0, Unk800C9E00 *n1) {
    SVECTOR o;
    SVECTOR v[3];
    SVECTOR p;
    VECTOR nv;
    VECTOR r;
    Unk800C9DF8Tri *t;
    Unk800C9DFC *vtx;
    Unk800C9E04 *pl;
    s32 dx0, dx1, dx2, dz1, dz2;
    s32 d;
    s32 area;
    s32 k;
    s32 c;

    t = &g_fieldBgi.unk1C[n0->unk4];
    o.vx = b->unk8[0] + t->unkA[0];
    o.vy = b->unk8[1] + t->unkA[1];
    o.vz = b->unk8[2] + t->unkA[2];
    vtx = (Unk800C9DFC *)((u8 *)b + b->unk3A);
    v[0].vx = vtx[n0->unkC[0]].unk0;
    v[1].vx = vtx[n0->unkC[1]].unk0;
    v[2].vx = vtx[n0->unkC[2]].unk0;
    p.vx = a->unk24->pos.vx - o.vx;
    v[0].vy = vtx[n0->unkC[0]].unk2;
    v[1].vy = vtx[n0->unkC[1]].unk2;
    v[2].vy = vtx[n0->unkC[2]].unk2;
    p.vy = a->unk24->pos.vy - o.vy;
    v[0].vz = vtx[n0->unkC[0]].unk4;
    v[1].vz = vtx[n0->unkC[1]].unk4;
    v[2].vz = vtx[n0->unkC[2]].unk4;
    p.vz = a->unk24->pos.vz - o.vz;
    dx0 = v[0].vx - p.vx;
    dx1 = v[1].vx - p.vx;
    area = (v[1].vx - v[0].vx) * (v[2].vz - v[0].vz);
    area -= (v[2].vx - v[0].vx) * (v[1].vz - v[0].vz);
    dx2 = v[2].vx - p.vx;
    d = v[0].vz - p.vz;
    dz1 = v[1].vz - p.vz;
    dz2 = v[2].vz - p.vz;
    nv.vx = dx1 * dz2 - dx2 * dz1;
    nv.vy = dx2 * d - dx0 * dz2;
    nv.vz = dx0 * dz1 - dx1 * d;
    t = &g_fieldBgi.unk1C[n1->unk4];
    o.vx = b->unk8[0] + t->unkA[0];
    o.vy = b->unk8[1] + t->unkA[1];
    o.vz = b->unk8[2] + t->unkA[2];
    v[0].vx = vtx[n1->unkC[0]].unk0;
    v[1].vx = vtx[n1->unkC[1]].unk0;
    v[2].vx = vtx[n1->unkC[2]].unk0;
    v[0].vy = vtx[n1->unkC[0]].unk2;
    v[1].vy = vtx[n1->unkC[1]].unk2;
    v[2].vy = vtx[n1->unkC[2]].unk2;
    v[0].vz = vtx[n1->unkC[0]].unk4;
    v[1].vz = vtx[n1->unkC[1]].unk4;
    v[2].vz = vtx[n1->unkC[2]].unk4;
    r.vx = (nv.vx * v[0].vx + nv.vy * v[1].vx + nv.vz * v[2].vx) / area;
    r.vz = (nv.vx * v[0].vz + nv.vy * v[1].vz + nv.vz * v[2].vz) / area;
    a->unk24->pos.vx = r.vx + o.vx;
    a->unk24->pos.vz = r.vz + o.vz;
    k = n1->unk6;
    if (k >= 0) {
        pl = &g_fieldBgi.unk14[k];
        c = func_800C1DC4(pl->unk0, a->unk24->pos.vx << 16);
        a->unk24->pos.vy = func_800C1DC4(n1->unk24 - c - func_800C1DC4(pl->unk8, a->unk24->pos.vz << 16), pl->unkC) >> 16;
        a->unk24->pos.vy += (s16)g_fieldBgi.unk1C[n1->unk4].unkA[1] - g_fieldBgi.unk1C[n1->unk4].unk4[1];
    } else {
        Unk800C9DF8Tri *t3;
        Unk800C9DFC *vt;
        t3 = g_fieldBgi.unk1C + n1->unk4;
        vt = g_fieldBgi.unkC + n1->unkC[0];
        a->unk24->pos.vy = (s16)(vt->unk2 + (g_fieldBgi.unk8->unk8[1] + t3->unkA[1]));
    }
    return 1;
}

s32 bgi_remapActors(u8 *arg0, Unk800C6C04A *arg1, Unk800C6C04B *arg2, s16 arg3) {
    Unk800C6C04E *e;
    s32 *tbl;
    s32 *ids;
    Unk800C9DF4 *a;
    u16 i;
    u16 j;

    if (arg3 == -1) {
        return 0;
    }
    e = (Unk800C6C04E *)(arg0 + arg1->unkC);
    e += arg3;
    tbl = (s32 *)(arg0 + arg2->unk6);
    a = g_fieldBgi.unk4;
    for (i = 0; i < g_fieldBgi.unk2; a++, i++) {
        if (a->unk0 & 1) {
            ids = (s32 *)(arg0 + e->unk6);
            for (j = 0; j < e->unk4; j++, ids++) {
                if (a->unk6 == *ids) {
                    bgi_transferActorPos(arg0, a, &g_fieldBgi.unk10[a->unk6], &g_fieldBgi.unk10[tbl[j]]);
                    a->unk6 = tbl[j];
                    a->unk4 = g_fieldBgi.unk10[tbl[j]].unk4;
                }
            }
        }
    }
    return 1;
}

/* link the neighbour edges between the nodes of different triangles */
s32 bgi_linkFrameNeighbors(Unk800C9DF8 *arg0, Unk800C9E10 *arg1, Unk800C9DF8Pt *arg2) {
    s32 *ids = (s32 *)((u8 *)arg0 + arg2->unk6);
    Unk800C9E00 *n;
    Unk800C9E00 *m;
    Unk800C9E08 *e;
    s16 nb;
    u16 i;
    u16 j;
    u16 k;

    for (i = 0; i < arg2->unk4; i++, ids++) {
        n = &g_fieldBgi.unk10[*ids];
        for (j = 0; j < 3; j++) {
            nb = n->unk18[j];
            if (nb >= 0) {
                m = &g_fieldBgi.unk10[nb];
                if (n->unk4 != m->unk4) {
                    e = &g_fieldBgi.unk18[n->unk12[j]];
                    if (e->unk2 >= 0) {
                        k = e->unk2;
                        m->unk18[k] = *ids;
                        e = &g_fieldBgi.unk18[m->unk12[k]];
                        e->unk2 = j;
                    }
                }
            }
        }
    }
    return 1;
}

s32 bgi_resetFloorMoves(void) {
    s32 i;

    for (i = 0; i < 30; i++) {
        g_fieldBgiFloorMoves[i].unk0 = 2;
        g_fieldBgiFloorMoves[i].unk2 = 0;
        g_fieldBgiFloorMoves[i].unk4 = 0;
        g_fieldBgiFloorMoves[i].unk6 = 0;
        g_fieldBgiFloorMoves[i].unk10 = 0;
        g_fieldBgiFloorMoves[i].unk14 = 0;
        g_fieldBgiFloorMoves[i].unk12 = 0x100;
        g_fieldBgiFloorMoves[i].unk8 = 0;
        g_fieldBgiFloorMoves[i].unkA = 0;
        g_fieldBgiFloorMoves[i].unkC = 0;
        g_fieldBgiFloorMoves[i].unkE = 0;
    }
    return 1;
}

/* old-style definition: bgi_startFloorMove/bgi_setFloorMovePlaying call it without arguments */
s32 bgi_stepFloorMove(arg0)
s32 arg0;
{
    Unk800CAE98 *e;
    s32 step;
    s16 range;
    s16 v;

    e = &g_fieldBgiFloorMoves[arg0];
    e->unk14 += e->unk12;
    step = e->unk14 >> 8;
    range = e->unkA - e->unk8;
    if (e->unk10 != 0 && e->unk12 != 0 && range > 0) {
        switch (e->unk6) {
        case 0:
            v = e->unk8 + step;
            break;
        case 1:
            v = e->unk8 + range * ((s16)math_cos((s16)((step << 12) / range) + 0x800) + 0x1000) / 0x2000;
            break;
        default:
            v = e->unk8 + step;
            break;
        }
        e->unkE = e->unkC;
        e->unkC = v;
        if (e->unkA < v) {
            if (e->unk0 & 0x10) {
                e->unkC = e->unkA;
                e->unk12 = -e->unk12;
            } else {
                e->unkC = e->unk8;
            }
            e->unk14 = (range << 8) - 0x100;
            e->unk0 &= ~4;
        } else if (v < e->unk8) {
            if (e->unk0 & 0x10) {
                e->unkC = e->unk8;
                e->unk12 = -e->unk12;
            } else {
                e->unkC = e->unkA;
            }
            e->unk14 = 0;
            e->unk0 &= ~4;
        }
    } else {
        return 0;
    }
    return 1;
}

s32 bgi_updateFloorMoves(void) {
    Unk800CAE98 *e;
    Unk800C9DF4 *a;
    Unk800C9DF8Tri *t;
    s32 i;
    s32 j;
    Unk8004AB64 *p;
    u16 flags;

    e = g_fieldBgiFloorMoves;
    for (i = 0; i < 30; e++, i++) {
        if (!(e->unk0 & 1) || !(e->unk0 & 0xC)) {
            continue;
        }
        t = &g_fieldBgi.unk1C[e->unk2];
        t->unkA[e->unk4] = t->unk4[e->unk4] + e->unkC;
        a = g_fieldBgi.unk4;
        for (j = 0; j < g_fieldBgi.unk2; a++, j++) {
            flags = a->unk0;
            if ((flags & 1) && a->unk4 == e->unk2 && a->unk6 >= 0) {
                p = a->unk24;
                ((ActorPosView *)p)->unk14[e->unk4] += e->unkC - e->unkE;
                switch (e->unk4) {
                case 0:
                    a->unk24->unk40.vx = a->unk24->pos.vx;
                    break;
                case 1:
                    a->unk24->unk40.vy = a->unk24->pos.vy;
                    break;
                case 2:
                    a->unk24->unk40.vz = a->unk24->pos.vz;
                    break;
                }
            }
        }
        bgi_stepFloorMove(i);
    }
    return 1;
}

s32 bgi_setMesh(Unk800C9DF8 *arg0) {
    s32 i;

    g_fieldBgi.unk8 = arg0;
    g_fieldBgi.unkC = (Unk800C9DFC *)((u8 *)arg0 + arg0->unk3A);
    g_fieldBgi.unk10 = (Unk800C9E00 *)((u8 *)arg0 + arg0->unk26);
    g_fieldBgi.unk14 = (Unk800C9E04 *)((u8 *)arg0 + arg0->unk36);
    g_fieldBgi.unk18 = (u8 *)arg0 + arg0->unk2A;
    g_fieldBgi.unk1C = (Unk800C9DF8Tri *)((u8 *)arg0 + arg0->unk32);
    g_fieldBgi.unk20 = (Unk800C9E10 *)((u8 *)arg0 + arg0->unk2E);
    for (i = 0; i < g_fieldBgi.unk8->unk2C; i++) {
        bgi_initAnim(i);
    }
    bgi_resetFloorMoves();
    return 1;
}

s32 bgi_updateActors(void) {
    s32 n = 0;
    s16 i = 0;

    if (g_fieldBgi.unk2 != 0) {
        do {
            if (bgi_updateActor(i) > 0) {
                n++;
            }
            i++;
        } while (i < g_fieldBgi.unk2);
    }
    return n & 0xFFFF;
}

s32 bgi_clear(void) {
    g_fieldBgi.unk8 = 0;
    g_fieldBgi.unk2 = 0;
    g_fieldBgi.unk4 = 0;
    g_fieldBgi.unk0 = 0;
    return 1;
}

s32 func_800C7450(void) {
    return 1;
}

void func_800C7458(s32 *arg0) {
    *arg0 = 1;
}

s32 bgi_setMaskFilter(u8 arg0) {
    g_fieldBgi.unk1 = arg0;
    return 1;
}

s32 bgi_getMaskFilter(u8 *arg0) {
    *arg0 = g_fieldBgi.unk1;
    return 1;
}

Unk800C9DF4 *bgi_getActor(s32 arg0) {
    return &g_fieldBgi.unk4[arg0];
}

s32 bgi_bindActor(s32 arg0, FieldObjSlot *arg1) {
    g_fieldBgi.unk4[arg0].unk0 = 0;
    g_fieldBgi.unk4[arg0].unk2 = 0;
    g_fieldBgi.unk4[arg0].unk24 = arg1->unkC;
    return 1;
}

s32 bgi_addActor(FieldObjSlot *arg0) {
    g_fieldBgi.unk4[g_fieldBgi.unk2].unk0 = 0;
    g_fieldBgi.unk4[g_fieldBgi.unk2].unk2 = 0;
    g_fieldBgi.unk4[g_fieldBgi.unk2].unk24 = arg0->unkC;
    g_fieldBgi.unk2++;
    return 1;
}

s32 bgi_setActorRadius(s32 arg0, u16 arg1) {
    g_fieldBgi.unk4[arg0].unk1C = arg1;
    g_fieldBgi.unk4[arg0].unk20 = arg1 * arg1;
    return 1;
}

s32 bgi_getActorRadius(s32 arg0, s32 *arg1) {
    *arg1 = g_fieldBgi.unk4[arg0].unk1C;
    return 1;
}

s32 bgi_getActorRadiusSq(s32 arg0, s32 *arg1) {
    *arg1 = g_fieldBgi.unk4[arg0].unk20;
    return 1;
}

s32 bgi_getActorNormal(s32 arg0, s32 *arg1, s32 *arg2, s32 *arg3, s32 *arg4) {
    Unk800C9E04 *e;
    Unk800C9E00 *n;
    struct Unk800C9DF4 *a;

    a = g_fieldBgi.unk4 + arg0;
    n = g_fieldBgi.unk10 + a->unk6;
    e = g_fieldBgi.unk14 + n->unk6;
    *arg1 = e->unk0;
    *arg2 = e->unk4;
    *arg3 = e->unk8;
    *arg4 = e->unkC;
    return 1;
}

s32 bgi_getActorTilt(s32 arg0, s16 *arg1, u16 *arg2) {
    Unk800C9DF4 *a = &g_fieldBgi.unk4[arg0];
    Unk800C9E00 *e = &g_fieldBgi.unk10[a->unk6];

    *arg1 = -e->unkA;
    *arg2 = e->unk8;
    return 1;
}

s32 bgi_getActorTriangle(s32 arg0, u16 *arg1, u16 *arg2) {
    Unk800C9DF4 *e = &g_fieldBgi.unk4[arg0];

    *arg1 = e->unk6;
    *arg2 = e->unk4;
    return 1;
}

s32 bgi_revertActor(s32 arg0) {
    Unk800C9DF4 *e = &g_fieldBgi.unk4[arg0];

    e->unk24->pos.vx = e->unk24->unk40.vx;
    e->unk24->pos.vy = e->unk24->unk40.vy;
    e->unk24->pos.vz = e->unk24->unk40.vz;
    e->unk4 = e->unk8;
    e->unk6 = e->unkA;
    return 1;
}

s32 bgi_setTriangleActive(s32 arg0, s32 arg1) {
    Unk800C9DF8Node *n = func_800C9DF8_node(arg0);
    u16 v;

    if (arg1 != 0) {
        v = n->unk0 | 1;
    } else {
        v = n->unk0 & ~1;
    }
    n->unk0 = v;
    return 1;
}

s32 func_800C77DC(u16 *arg0, s16 **arg1) {
    *arg0 = D_800C9DDE;
    *arg1 = D_800CAA98;
    return 1;
}

s32 bgi_getTriangleFlags(s16 arg0, u16 *arg1) {
    *arg1 = func_800C9DF8_node(arg0)->unk0 & ((g_fieldBgi.unk1 << 8) | 0xFF);
    return 1;
}

s32 bgi_setTriangleMask(s16 arg0, s32 arg1) {
    Unk800C9DF8Node *n = func_800C9DF8_node(arg0);

    n->unk0 = ((arg1 & 0xFF) << 8) | (n->unk0 & 0xFF);
    return 1;
}

s32 bgi_getTriangleMask(s16 arg0, u8 *arg1) {
    *arg1 = (func_800C9DF8_node(arg0)->unk0 >> 8) & g_fieldBgi.unk1;
    return 1;
}

s32 bgi_getTriangleFloor(s16 arg0, u16 *arg1) {
    *arg1 = func_800C9DF8_node(arg0)->unk4;
    return 1;
}

s32 func_800C7914(u16 *arg0, s16 **arg1) {
    *arg0 = D_800C9DE0;
    *arg1 = D_800CAAB8;
    return 1;
}

s32 bgi_getEdgeFlags(s16 arg0, u16 *arg1) {
    *arg1 = func_800C9DF8_link(arg0)->unk0;
    return 1;
}

s32 bgi_getFloorPosition(s32 arg0, u16 *arg1, u16 *arg2, u16 *arg3) {
    Unk800C9DF8Tri *t = func_800C9DF8_tri(arg0);

    *arg1 = t->unkA[0];
    *arg2 = t->unkA[1];
    *arg3 = t->unkA[2];
    return 1;
}

s32 bgi_initAnim(u32 arg0) {
    Unk800C9DF8 *b;
    Unk800C9E10 *e;
    Unk800C9DF8Pt *pt;
    s32 *ids;
    s32 i;
    s32 j;
    Unk800C9E00 *n;

    b = g_fieldBgi.unk8;
    if (arg0 >= b->unk2C) {
        return 0;
    }
    e = &g_fieldBgi.unk20[arg0];
    pt = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
    for (i = 0; i < e->unk2; pt++, i++) {
        ids = (s32 *)((u8 *)b + pt->unk6);
        for (j = 0; j < pt->unk4; j++) {
            n = &g_fieldBgi.unk10[*ids];
            if (i == 0) {
                n->unk0 |= 1;
                bgi_linkFrameNeighbors(b, e, pt);
            } else {
                n->unk0 &= ~1;
            }
            ids++;
        }
    }
    return 1;
}

s32 bgi_setAnimFrame(u32 arg0, u32 arg1) {
    Unk800C9E10 *e;

    if (arg0 >= g_fieldBgi.unk8->unk2C) {
        return 0;
    }
    e = &g_fieldBgi.unk20[arg0];
    if (arg1 >= e->unk2) {
        return 0;
    }
    e->unk8 = arg1 << 8;
    e->unk6 = 0;
    e->unk0 |= 1;
    return 1;
}

s32 bgi_setAnimPlaying(u32 arg0, s32 arg1) {
    Unk800C9E10 *e;

    if (arg0 >= g_fieldBgi.unk8->unk2C) {
        return 0;
    }
    e = &g_fieldBgi.unk20[arg0];
    if (arg1 != 0) {
        e->unk0 |= 5;
    } else {
        e->unk0 &= ~5;
    }
    return 1;
}

s32 bgi_setAnimRate(u32 arg0, s16 arg1) {
    Unk800C9E10 *e;

    if (arg0 >= g_fieldBgi.unk8->unk2C) {
        return 0;
    }
    e = &g_fieldBgi.unk20[arg0];
    e->unk4 = arg1;
    return 1;
}

s32 bgi_setAnimFrameValues(u32 arg0, s16 arg1) {
    Unk800C9DF8 *b = g_fieldBgi.unk8;
    Unk800C9E10 *e;
    Unk800C9DF8Pt *pt;
    s32 i;

    if (arg0 >= b->unk2C) {
        return 0;
    }
    e = &g_fieldBgi.unk20[arg0];
    pt = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
    for (i = 0; i < e->unk2; pt++, i++) {
        pt->unk2 = arg1;
    }
    return 1;
}

s32 bgi_setAnimFrameValue(u32 arg0, u32 arg1, s16 arg2) {
    Unk800C9DF8 *b = g_fieldBgi.unk8;
    Unk800C9E10 *e;
    Unk800C9DF8Pt *pt;

    if (arg0 >= b->unk2C) {
        return 0;
    }
    e = &g_fieldBgi.unk20[arg0];
    if (arg1 >= e->unk2) {
        return 0;
    }
    pt = &((Unk800C9DF8Pt *)((u8 *)b + e->unkC))[arg1];
    pt->unk2 = arg2;
    return 1;
}

s32 bgi_setAnimMode(u32 arg0, s32 arg1) {
    Unk800C9E10 *e;

    if (arg0 >= g_fieldBgi.unk8->unk2C) {
        return 0;
    }
    e = &g_fieldBgi.unk20[arg0];
    e->unk0 = (e->unk0 & 0xFFE7) | (arg1 & 0x18);
    return 1;
}

s32 bgi_playAnimRange(u32 arg0, u32 arg1, u32 arg2) {
    Unk800C9DF8 *b = g_fieldBgi.unk8;
    Unk800C9E10 *e;
    Unk800C9DF8Pt *pt;

    if (arg0 >= b->unk2C) {
        return 0;
    }
    e = &g_fieldBgi.unk20[arg0];
    if (arg1 >= e->unk2) {
        return 0;
    }
    if (arg2 >= e->unk2) {
        return 0;
    }
    pt = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
    e->unk8 = arg1 << 8;
    e->unk6 = 0;
    e->unk0 |= 5;
    pt += arg2;
    pt->unk2 = -1;
    if ((e->unk4 > 0 && arg2 < arg1) || (e->unk4 < 0 && arg1 < arg2)) {
        e->unk4 = -e->unk4;
    }
    return 1;
}

s32 bgi_resetAnim(u32 arg0, s32 arg1) {
    Unk800C9DF8 *b = g_fieldBgi.unk8;
    Unk800C9E10 *e;
    Unk800C9DF8Pt *pt;
    s32 *ids;
    s32 i;
    s32 j;
    Unk800C9E00 *n;

    if (arg0 >= b->unk2C) {
        return 0;
    }
    e = &g_fieldBgi.unk20[arg0];
    e->unk4 = 0x100;
    e->unk0 = 2;
    e->unk8 = 0;
    e->unk6 = 0;
    pt = (Unk800C9DF8Pt *)((u8 *)b + e->unkC);
    for (i = 0; i < e->unk2; pt++, i++) {
        ids = (s32 *)((u8 *)b + pt->unk6);
        for (j = 0; j < pt->unk4; ids++, j++) {
            n = &g_fieldBgi.unk10[*ids];
            if (arg1 != 0 && i == 0) {
                n->unk0 |= 1;
            } else {
                n->unk0 &= ~1;
            }
        }
    }
    return 1;
}

s32 bgi_getAnimFrame(u32 arg0, s32 *arg1) {
    Unk800C9E10 *e;

    if (arg0 >= g_fieldBgi.unk8->unk2C) {
        return 0;
    }
    e = &g_fieldBgi.unk20[arg0];
    *arg1 = e->unk8 >> 8;
    return 1;
}

s32 bgi_startFloorMove(s32 arg0, s32 arg1) {
    Unk800CAE98 *e = &g_fieldBgiFloorMoves[arg0];

    e->unk14 = arg1 << 8;
    e->unk0 |= 5;
    bgi_stepFloorMove(arg0);
    return 1;
}

s32 bgi_stopFloorMove(s32 arg0, s32 arg1) {
    Unk800CAE98 *e = bgi_getFloorMove_inline(arg0);

    arg1 <<= 8;
    e->unk14 = arg1;
    e->unk0 = 0;
    bgi_stepFloorMove(arg0);
    return 1;
}

s32 bgi_setFloorMovePlaying(s32 arg0, s32 arg1) {
    Unk800CAE98 *e = bgi_getFloorMove_inline(arg0);
    u16 v;

    if (arg1 != 0) {
        v = e->unk0 | 5;
    } else {
        v = e->unk0 & ~5;
    }
    e->unk0 = v;
    bgi_stepFloorMove(arg0);
    return 1;
}

s32 bgi_setFloorMoveMode(s32 arg0, s32 arg1) {
    Unk800CAE98 *e = bgi_getFloorMove_inline(arg0);

    e->unk0 = (e->unk0 & 0xFFE7) | (arg1 & 0x18);
    return 1;
}

s32 bgi_setFloorMoveFloor(s32 arg0, s16 arg1) {
    bgi_getFloorMove_inline(arg0)->unk2 = arg1;
    return 1;
}

s32 bgi_setFloorMoveSpeed(s32 arg0, s16 arg1) {
    bgi_getFloorMove_inline(arg0)->unk12 = arg1;
    return 1;
}

s32 bgi_setFloorMoveCurve(s32 arg0, s16 arg1) {
    bgi_getFloorMove_inline(arg0)->unk6 = arg1;
    return 1;
}

s32 bgi_setFloorMoveRange(s32 arg0, s16 arg1, s16 arg2) {
    Unk800CAE98 *e = bgi_getFloorMove_inline(arg0);

    e->unk8 = arg1;
    e->unkA = arg2;
    e->unk10 = e->unkA - e->unk8;
    return 1;
}

s32 bgi_setFloorMoveAxis(s32 arg0, s16 arg1) {
    bgi_getFloorMove_inline(arg0)->unk4 = arg1;
    return 1;
}

Unk800CAE98 *bgi_getFloorMove(s32 arg0) {
    return &g_fieldBgiFloorMoves[arg0];
}

INCLUDE_RODATA("asm/us/ovl_0a/nonmatchings/bgi", D_800A7814);
