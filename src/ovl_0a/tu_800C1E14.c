#include "ovl_0a.h"

/* does the segment from arg0 to arg1 cross the edge arg2-arg3 (on axes x, z)? */
s32 bgi_segmentsCross(s16 *arg0, s16 *arg1, s16 *arg2, s16 *arg3, u8 x, u8 z) {
    s32 t2;
    s32 t3;
    s32 t4;
    s32 ex;
    s32 ez;

    t2 = (arg1[x] - arg0[x]) * (ez = arg3[z] - arg2[z]);
    ex = arg3[x] - arg2[x];
    t3 = (arg0[z] - arg2[z]) * ex;
    t2 -= (arg1[z] - arg0[z]) * ex;
    t3 -= (arg0[x] - arg2[x]) * ez;
    if (ABS(t3) <= ABS(t2)) {
        if ((t3 >= 0 && t2 >= 0) || (t3 <= 0 && t2 < 0)) {
            t4 = (arg0[z] - arg2[z]) * (arg1[x] - arg0[x]);
            t4 -= (arg0[x] - arg2[x]) * (arg1[z] - arg0[z]);
            if ((t4 >= 0 && t2 >= 0) || (t4 <= 0 && t2 < 0)) {
                return 1;
            }
        }
    }
    return 0;
}

Unk800C9E00 *bgi_getPassableNeighbor(Unk800C9DF4 *arg0, Unk800C9E00 *arg1, s32 arg2) {
    Unk800C9E00 *n;
    Unk8004AB64 *a;
    s16 id;
    u8 m;
    u8 mode;

    id = arg1->unk18[arg2];
    if (id == -1) {
        goto fail;
    }
    n = &g_fieldBgi.unk10[id];
    if (!(n->unk0 & 1)) {
        goto fail;
    }
    if ((m = (n->unk0 >> 8) & g_fieldBgi.unk1) & 0xC0) {
        mode = g_gameState->mode;
        if (mode == 5) {
            a = (Unk8004AB64 *)g_gameState->unk1C->unk8E0[g_gameState->unk1C->unk8E4.field->unk8].unkC;
        } else if (mode == 1) {
            a = func_8004AE00();
        } else {
            a = NULL;
        }
        if (arg0->unk24 == a) {
            if (m & 0x80) {
            fail:
                return NULL;
            }
            return n;
        }
        if (m & 0x40) {
            return NULL;
        }
    }
    return n;
}

/* closest point to arg0 on segment arg1-arg2 -> arg3; returns squared distance */
s32 bgi_closestPointOnSegment(SVECTOR *arg0, SVECTOR *arg1, SVECTOR *arg2, SVECTOR *arg3, s16 *arg4) {
    SVECTOR c;
    s32 ta;
    s32 tb;
    s32 s;
    s32 dx;
    s32 dy;
    s32 dz;
    s16 px;
    s16 py;
    s16 pz;
    s16 ax;
    s16 ay;
    s16 az;
    s16 bx;
    s16 by;
    s16 bz;

    px = arg0->vx;
    ax = arg1->vx;
    bx = arg2->vx;
    py = arg0->vy;
    ay = arg1->vy;
    by = arg2->vy;
    pz = arg0->vz;
    az = arg1->vz;
    bz = arg2->vz;
    ta = (px - ax) * (bx - ax);
    ta += (py - ay) * (by - ay);
    ta += (pz - az) * (bz - az);
    if (ta <= 0) {
        c.vx = arg1->vx;
        c.vy = arg1->vy;
        c.vz = arg1->vz;
        if (ta == 0) {
            *arg4 = 1;
        } else {
            *arg4 = 0;
        }
    } else {
        tb = (px - bx) * (ax - bx);
        tb += (py - by) * (ay - by);
        tb += (pz - bz) * (az - bz);
        if (tb <= 0) {
            c.vx = arg2->vx;
            c.vy = arg2->vy;
            c.vz = arg2->vz;
            if (tb == 0) {
                *arg4 = 1;
            } else {
                *arg4 = 0;
            }
        } else {
            while (ta > 0x10000 || tb > 0x10000) {
                ta >>= 1;
                tb = tb >> 1;
            }
            s = ta + tb;
            c.vx = arg1->vx + (arg2->vx - arg1->vx) * ta / s;
            c.vy = arg1->vy + (arg2->vy - arg1->vy) * ta / s;
            c.vz = arg1->vz + (arg2->vz - arg1->vz) * ta / s;
            *arg4 = 1;
        }
    }
    dx = c.vx - arg0->vx;
    dy = c.vy - arg0->vy;
    dz = c.vz - arg0->vz;
    arg3->vx = c.vx;
    arg3->vy = c.vy;
    s = dx * dx;
    s += dy * dy;
    arg3->vz = c.vz;
    return s + dz * dz;
}

s32 bgi_projectOnPlane(VECTOR *arg0, SVECTOR *arg1, s32 arg2, VECTOR *arg3) {
    VECTOR a;
    VECTOR v;

    v.vx = arg1->vx << 16;
    v.vy = arg1->vy << 16;
    v.vz = arg1->vz << 16;
    a.vy = 0;
    a.vz = v.vx;
    a.vx = -v.vz;
    arg3->vx = -arg1->vz;
    arg3->vy = 0;
    arg3->vz = arg1->vx;
    if (arg0->vy >= 0) {
        v.vx = func_800C1DC4(arg0->vy, a.vz);
        v.vy = func_800C1DC4(arg0->vz, a.vx) - func_800C1DC4(arg0->vx, a.vz);
        v.vz = -func_800C1DC4(arg0->vy, a.vx);
    } else {
        v.vx = -func_800C1DC4(arg0->vy, a.vz);
        v.vy = func_800C1DC4(arg0->vx, a.vz) - func_800C1DC4(arg0->vz, a.vx);
        v.vz = func_800C1DC4(arg0->vy, a.vx);
    }
    v.vx = (v.vx + 0x8000) & 0xFFFF0000;
    v.vy = (v.vy + 0x8000) & 0xFFFF0000;
    v.vz = (v.vz + 0x8000) & 0xFFFF0000;
    arg1->vx = v.vx >> 16;
    arg1->vy = v.vy >> 16;
    arg1->vz = v.vz >> 16;
    return 1;
}

/* push a position out of the walkmesh triangle edges (recurses into neighbours) */
s32 bgi_pushOutOfEdges(Unk800C9DF4 *arg0, SVECTOR *arg1, Unk800C9E00 *arg2, s16 *arg3) {
    Unk800C9DFC *e[2];
    SVECTOR p;
    Unk800C9DFC v[3];
    VECTOR push;
    s16 on;
    Unk800C9DF8Tri *t;
    Unk800C9E00 *nb;
    s16 i;
    s32 hit;
    s32 d;
    s16 n;
    Unk800C9DFC *vt;

    t = &g_fieldBgi.unk1C[arg2->unk4];
    p.vx = g_fieldBgi.unk8->unk8[0] + t->unkA[0];
    p.vy = g_fieldBgi.unk8->unk8[1] + t->unkA[1];
    p.vz = g_fieldBgi.unk8->unk8[2] + t->unkA[2];
    vt = g_fieldBgi.unkC;
    for (i = 0; i < 3; i++) {
        v[i].unk0 = p.vx + vt[arg2->unkC[i]].unk0;
        v[i].unk2 = p.vy + vt[arg2->unkC[i]].unk2;
        v[i].unk4 = p.vz + vt[arg2->unkC[i]].unk4;
    }
    e[0] = &v[2];
    hit = 0;
    for (i = 0; i < 3; i++) {
        e[1] = &v[i];
        d = bgi_closestPointOnSegment(arg1, (SVECTOR *)e[0], (SVECTOR *)e[1], &p, &on);
        if ((u32)arg0->unk20 >= d) {
            nb = bgi_getPassableNeighbor(arg0, arg2, i);
            if (nb != NULL) {
                if (!(nb->unk0 & 0x80)) {
                    nb->unk0 |= 0x80;
                    if (bgi_pushOutOfEdges(arg0, arg1, nb, arg3) == 0) {
                        *arg3 = i;
                        hit = 1;
                    }
                }
            } else {
                do {
                    hit = 1;
                } while (0);
                if (d != 0) {
                    d = (s16)SquareRoot0(d);
                    if (p.vx == arg1->vx) {
                        push.vx = 0;
                    } else {
                        push.vx = (s16)((arg1->vx - p.vx) * arg0->unk1C / d);
                    }
                    if (p.vz == arg1->vz) {
                        push.vz = 0;
                    } else {
                        push.vz = (s16)((arg1->vz - p.vz) * arg0->unk1C / d);
                    }
                    n = g_fieldBgiPushCount;
                    if (n + 1 < 16) {
                        push.vx = p.vx - arg1->vx + push.vx;
                        push.vz = p.vz - arg1->vz + push.vz;
                        g_fieldBgiPushVecs[n][0] = push.vx;
                        g_fieldBgiPushCount++;
                        g_fieldBgiPushVecs[n][2] = push.vz;
                        g_fieldBgiPushOnEdge[n] = on;
                        g_fieldBgiPushDist[n] = d;
                    }
                    hit = 1;
                    arg2->unk0 |= 0x80;
                    *arg3 = i;
                }
            }
        }
        e[0] = e[1];
    }
    return hit ^ 1;
}

u16 bgi_findTriangleNear(Unk800C9DF4 *arg0, Unk800C9E00 **arg1, s32 arg2, s32 arg3) {
    Unk800C9E00 *p[2];
    u16 i;

    if (arg3 < 9) {
        p[0] = *arg1;
        if ((u8)bgi_pointInTriangle(g_fieldBgi.unk8, &g_fieldBgi.unk1C[p[0]->unk4], p[0], (s16 *)arg2)) {
            return 1;
        }
        for (i = 0; i < 3; i++) {
            p[1] = bgi_getPassableNeighbor(arg0, p[0], i);
            if (p[1] != NULL && (u8)bgi_pointInTriangle(g_fieldBgi.unk8, &g_fieldBgi.unk1C[p[1]->unk4], p[1], (s16 *)arg2)) {
                arg0->unk6 = p[0]->unk18[i];
                if (arg0->unk6 >= 0) {
                    p[1] = &g_fieldBgi.unk10[arg0->unk6];
                    arg0->unk4 = p[1]->unk4;
                    *arg1 = p[1];
                    return 1;
                }
            }
        }
        for (i = 0; i < 3; i++) {
            p[1] = bgi_getPassableNeighbor(arg0, p[0], i);
            if (p[1] != NULL && !(p[1]->unk0 & 0x80)) {
                p[1]->unk0 |= 0x80;
                if (bgi_findTriangleNear(arg0, &p[1], arg2, arg3 + 1) == 1) {
                    *arg1 = p[1];
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* slide a position along the walkmesh from arg2 towards arg3, following neighbour triangles */
s32 bgi_slide(Unk800C9DF4 *arg0, Unk800C9E00 **arg1, s16 *arg2, SVECTOR *arg3, s16 arg4, s16 arg5, s32 arg6, s32 arg7) {
    SVECTOR p;
    SVECTOR *pp;
    VECTOR n;
    SVECTOR save;
    SVECTOR d;
    SVECTOR e;
    VECTOR f;
    s16 edge;
    Unk800C9E00 *cur;
    s16 prev;
    u8 done;
    s16 saved;
    s32 off;
    s16 s;
    s16 l;
    s16 nb;
    Unk800C9E00 *q;
    u8 r;

    cur = *arg1;
    prev = -1;
    p.vx = arg2[0];
    p.vy = arg2[1];
    p.vz = arg2[2];
    save.vx = arg3->vx;
    save.vy = arg3->vy;
    done = 0;
    pp = &p;
    save.vz = arg3->vz;
    edge = -1;
    saved = arg0->unk6;
    off = arg0->unk6 * sizeof(Unk800C9E00);
    do {
        d.vx = arg3->vx - p.vx;
        d.vy = arg3->vy - pp->vy;
        d.vz = arg3->vz - pp->vz;
        if (cur->unk6 >= 0) {
            bgi_projectOnPlane((VECTOR *)&g_fieldBgi.unk14[cur->unk6], &d, (s32)pp, &n);
        } else {
            bgi_projectOnPlane(&g_fieldBgiDefaultNormal, &d, (s32)pp, &n);
        }
        arg3->vx = pp->vx + d.vx;
        arg3->vy = pp->vy + d.vy;
        arg3->vz = pp->vz + d.vz;
        if ((u8)bgi_pointInTriangle(g_fieldBgi.unk8, &g_fieldBgi.unk1C[cur->unk4], cur, (s16 *)arg3)) {
            done = 1;
        } else if ((r = bgi_findEdgeCrossing(g_fieldBgi.unk8, &g_fieldBgi.unk1C[cur->unk4], cur, (s16 *)arg3, arg2, pp, &edge, edge))) {
            if (bgi_getPassableNeighbor(arg0, cur, edge) != NULL) {
                if (cur->unk6 >= 0) {
                    e.vx = arg3->vx - p.vx;
                    e.vy = arg3->vy - pp->vy;
                    e.vz = arg3->vz - pp->vz;
                    s = SquareRoot0(n.vx * n.vx + n.vz * n.vz);
                    if (s == 0) {
                        arg3->vy = pp->vy;
                    } else {
                        l = SquareRoot0(e.vx * e.vx + e.vy * e.vy + e.vz * e.vz);
                        f.vz = n.vz * l / s;
                        f.vx = n.vx * l / s;
                        f.vy = 0;
                        e.vy = 0;
                        e.vx = f.vz;
                        e.vz = -f.vx;
                        arg3->vx = p.vx + e.vx;
                        arg3->vy = pp->vy + e.vy;
                        arg3->vz = pp->vz + e.vz;
                    }
                }
                nb = cur->unk18[edge];
                if (nb == prev) {
                    arg0->unk6 = saved;
                    cur = (Unk800C9E00 *)((u8 *)g_fieldBgi.unk10 + off);
                    arg0->unk4 = cur->unk4;
                    *arg1 = cur;
                    return 1;
                }
                prev = arg0->unk6;
                arg0->unk6 = cur->unk18[edge];
                q = &g_fieldBgi.unk10[nb];
                {
                    Unk800C9E08 *ee = &g_fieldBgi.unk18[cur->unk12[edge]];
                    arg0->unk4 = q->unk4;
                    edge = ee->unk2;
                }
                cur = q;
                if (edge == -1) {
                    arg0->unk6 = saved;
                    cur = (Unk800C9E00 *)((u8 *)g_fieldBgi.unk10 + off);
                    arg0->unk4 = cur->unk4;
                    *arg1 = cur;
                    return 1;
                }
            } else {
                arg0->unk6 = saved;
                cur = (Unk800C9E00 *)((u8 *)g_fieldBgi.unk10 + off);
                arg0->unk4 = cur->unk4;
                *arg1 = cur;
                return 1;
            }
        } else {
            bgi_slide_clear();
            if ((u8)bgi_findTriangleNear(arg0, &cur, (s32)arg3, 0)) {
                done = 1;
            } else {
                if (arg5 < 6) {
                    arg3->vx = save.vx;
                    arg3->vy = save.vy;
                    arg3->vz = save.vz;
                    bgi_slide_push(arg3, arg2, &arg6, &arg7);
                    arg0->unk24->pos.vx = arg3->vx;
                    arg0->unk24->pos.vy = arg3->vy;
                    arg0->unk24->pos.vz = arg3->vz;
                    arg0->unk6 = saved;
                    cur = (Unk800C9E00 *)((u8 *)g_fieldBgi.unk10 + off);
                    arg0->unk4 = cur->unk4;
                    if (bgi_slide(arg0, &cur, arg2, arg3, arg4, arg5 + 1, arg6, arg7) == 0) {
                        break;
                    }
                }
                arg0->unk6 = saved;
                cur = (Unk800C9E00 *)((u8 *)g_fieldBgi.unk10 + off);
                arg0->unk4 = cur->unk4;
                *arg1 = cur;
                return 1;
            }
        }
    } while (!done);
    *arg1 = cur;
    return 0;
}

s32 bgi_applyPushes(Unk800C9DF4 *arg0, SVECTOR *arg1, Unk800C9E00 **arg2, s16 *arg3) {
    VECTOR sum;
    SVECTOR prev;
    u8 i;
    u8 j;
    u8 dup;
    u8 any;
    s32 n;

    if (g_fieldBgiPushCount == 0) {
        return 1;
    }
    prev.vx = arg1->vx;
    prev.vy = arg1->vy;
    prev.vz = arg1->vz;
    if (g_fieldBgiPushCount == 1) {
        arg1->vx += g_fieldBgiPushVecs[0][0];
        arg1->vz += g_fieldBgiPushVecs[0][2];
        arg0->unk24->pos.vx = arg1->vx;
        arg0->unk24->pos.vz = arg1->vz;
    } else {
        any = 0;
        i = 0;
        n = 0;
        sum.vx = 0;
        sum.vz = 0;
        for (; i < g_fieldBgiPushCount; i++) {
            if (g_fieldBgiPushOnEdge[i] != 0) {
                dup = 0;
                for (j = 0; j < i; j++) {
                    if (g_fieldBgiPushVecs[j][0] == g_fieldBgiPushVecs[i][0] && g_fieldBgiPushVecs[j][2] == g_fieldBgiPushVecs[i][2]) {
                        dup = 1;
                    }
                }
                if (!dup) {
                    any = 1;
                    sum.vx += g_fieldBgiPushVecs[i][0];
                    n++;
                    sum.vz += g_fieldBgiPushVecs[i][2];
                }
            }
        }
        if (any) {
            if (n >= 2) {
                sum.vx /= n;
                sum.vz /= n;
            }
        } else {
            n = 0;
            for (i = 0; i < g_fieldBgiPushCount; i++) {
                dup = 0;
                for (j = 0; j < i; j++) {
                    if (g_fieldBgiPushVecs[j][0] == g_fieldBgiPushVecs[i][0] && g_fieldBgiPushVecs[j][2] == g_fieldBgiPushVecs[i][2]) {
                        dup = 1;
                    }
                }
                if (!dup) {
                    sum.vx += g_fieldBgiPushVecs[i][0];
                    n++;
                    sum.vz += g_fieldBgiPushVecs[i][2];
                }
            }
            if (n > 0) {
                sum.vx /= n;
                sum.vy /= n;
            }
        }
        arg1->vx += sum.vx;
        arg1->vz += sum.vz;
        arg0->unk24->pos.vx = arg1->vx;
        arg0->unk24->pos.vz = arg1->vz;
    }
    g_fieldBgiPushCount = 0;
    return bgi_slide(arg0, arg2, &prev, arg1, 0, 0, 0, 0) == 0;
}

/* clears the visited flag of every walkmesh triangle first */
static inline s32 bgi_pushOutOfEdges_inline(Unk800C9DF4 *e, SVECTOR *pos, Unk800C9E00 *nd, s16 *edge) {
    u16 k;
    Unk800C9E00 *n;
    s32 cnt;
    u16 m = ~0x80;

    n = g_fieldBgi.unk10;
    cnt = g_fieldBgi.unk8->unk24;
    for (k = 0; k < cnt; k++) {
        n->unk0 &= m;
        n++;
    }
    g_fieldBgiPushCount = 0;
    return bgi_pushOutOfEdges(e, pos, nd, edge);
}

s32 bgi_moveActor(Unk800C9DF4 *arg0) {
    SVECTOR cur;
    SVECTOR prev;
    Unk800C9E00 *node;
    s16 edge;
    s32 ret;
    s32 id;
    s16 save;
    s32 k;
    Unk800C9DFC *vt;
    Unk800C9E04 *pl;
    s32 a;
    Unk800C9DF8Tri *t;

    ret = 2;
    D_800C9DDE = 0;
    for (k = 0; k < 10; k++) {
        D_800CAA98[k] = -1;
    }
    D_800C9DE0 = 0;
    for (k = 0; k < 10; k++) {
        D_800CAAB8[k] = -1;
    }
    cur.vx = arg0->unk24->pos.vx;
    cur.vy = arg0->unk24->pos.vy;
    cur.vz = arg0->unk24->pos.vz;
    prev.vx = arg0->unk24->unk40.vx;
    prev.vy = arg0->unk24->unk40.vy;
    prev.vz = arg0->unk24->unk40.vz;
    if (cur.vx == prev.vx && cur.vy == prev.vy && cur.vz == prev.vz) {
        arg0->unk8 = arg0->unk4;
        arg0->unkA = arg0->unk6;
        return ret | 4;
    }
    id = arg0->unk6;
    node = &g_fieldBgi.unk10[id];
    save = arg0->unk6;
    if (bgi_slide(arg0, &node, (s16 *)&prev, &cur, 1, 0, 0, 0) != 0) {
        arg0->unk6 = save;
        if (id >= 0) {
            arg0->unk4 = g_fieldBgi.unk10[id].unk4;
        } else {
            arg0->unk4 = -1;
        }
        arg0->unk24->pos.vx = arg0->unk24->unk40.vx;
        arg0->unk24->pos.vy = arg0->unk24->unk40.vy;
        arg0->unk24->pos.vz = arg0->unk24->unk40.vz;
        ret |= 1;
        if (arg0->unkA >= 0) {
            if (!(u8)bgi_pointInTriangle(g_fieldBgi.unk8, &g_fieldBgi.unk1C[arg0->unk8], &g_fieldBgi.unk10[arg0->unkA], &prev.vx)) {
                arg0->unk8 = arg0->unk4;
                arg0->unkA = arg0->unk6;
            }
        }
        return ret;
    }
    if (!(u8)bgi_pushOutOfEdges_inline(arg0, &cur, node, &edge)) {
        if ((u8)bgi_applyPushes(arg0, &cur, &node, &edge)) {
            ret |= 9;
        } else {
            arg0->unk6 = save;
            if (save >= 0) {
                arg0->unk4 = D_800C9E00[save].unk4;
            } else {
                arg0->unk4 = -1;
            }
            arg0->unk24->pos.vx = arg0->unk24->unk40.vx;
            arg0->unk24->pos.vy = arg0->unk24->unk40.vy;
            arg0->unk24->pos.vz = arg0->unk24->unk40.vz;
            ret |= 1;
            if (arg0->unkA >= 0 && arg0->unk8 >= 0) {
            if (!(u8)bgi_pointInTriangle(g_fieldBgi.unk8, &g_fieldBgi.unk1C[arg0->unk8], &g_fieldBgi.unk10[arg0->unkA], &prev.vx)) {
                arg0->unk8 = arg0->unk4;
                arg0->unkA = arg0->unk6;
            }
            }
            return ret;
        }
    }
    if (cur.vx != arg0->unk24->pos.vx || cur.vz != arg0->unk24->pos.vz) {
        arg0->unk24->pos.vx = cur.vx;
        ret |= 0x10;
        arg0->unk24->pos.vz = cur.vz;
    }
    if (node->unk6 >= 0) {
        pl = &g_fieldBgi.unk14[node->unk6];
        if ((u32)(pl->unk4 + 0x50F4) > 0xA1E8) {
            a = func_800C1DC4(pl->unk0, arg0->unk24->pos.vx << 16);
            arg0->unk24->pos.vy = func_800C1DC4(node->unk24 - a - func_800C1DC4(pl->unk8, arg0->unk24->pos.vz << 16), pl->unkC) >> 16;
            arg0->unk24->pos.vy += (s16)g_fieldBgi.unk1C[node->unk4].unkA[1] - g_fieldBgi.unk1C[node->unk4].unk4[1];
        } else {
            arg0->unk24->pos.vy = cur.vy;
        }
    } else {
        t = g_fieldBgi.unk1C + node->unk4;
        vt = g_fieldBgi.unkC + node->unkC[0];
        arg0->unk24->pos.vy = (s16)(vt->unk2 + (g_fieldBgi.unk8->unk8[1] + t->unkA[1]));
    }
    arg0->unkA = save;
    if (save >= 0) {
        arg0->unk8 = D_800C9E00[save].unk4;
    } else {
        arg0->unk8 = -1;
    }
    return ret;
}

s32 bgi_init(Unk800C9DF8 *arg0) {
    u16 i;
    Unk800C9E10 *p;

    g_fieldBgi.unk8 = arg0;
    g_fieldBgi.unkC = (Unk800C9DFC *)((u8 *)arg0 + arg0->unk3A);
    g_fieldBgi.unk10 = (Unk800C9E00 *)((u8 *)arg0 + arg0->unk26);
    g_fieldBgi.unk14 = (Unk800C9E04 *)((u8 *)arg0 + arg0->unk36);
    g_fieldBgi.unk18 = (u8 *)arg0 + arg0->unk2A;
    g_fieldBgi.unk1C = (Unk800C9DF8Tri *)((u8 *)arg0 + arg0->unk32);
    p = (Unk800C9E10 *)((u8 *)arg0 + arg0->unk2E);
    g_fieldBgi.unk1 = 0xFF;
    g_fieldBgi.unk2 = 9;
    g_fieldBgi.unk4 = g_fieldBgiActors;
    g_fieldBgi.unk20 = p;
    for (i = 0; i < 24; i++) {
        g_fieldBgi.unk4[i].unk0 = 0;
        g_fieldBgi.unk4[i].unk2 = 0;
        g_fieldBgi.unk4[i].unk4 = -1;
        g_fieldBgi.unk4[i].unk6 = -1;
        g_fieldBgi.unk4[i].unk8 = -1;
        g_fieldBgi.unk4[i].unkA = -1;
        g_fieldBgi.unk4[i].unk1C = 0x60;
        g_fieldBgi.unk4[i].unk20 = 0x2400;
        g_fieldBgi.unk4[i].unk24 = NULL;
    }
    g_fieldBgi.unk0 = 1;
    for (i = 0; i < g_fieldBgi.unk8->unk2C; i++) {
        bgi_initAnim(i);
    }
    bgi_resetFloorMoves();
    return 1;
}

Unk800C9DF4 *bgi_findPlayerActor(s32 *arg0) {
    Unk8004AB64 *a;
    s32 i;
    u8 mode;

    *arg0 = 0;
    mode = g_gameState->mode;
    if (mode == 5) {
        a = (Unk8004AB64 *)g_gameState->unk1C->unk8E0[g_gameState->unk1C->unk8E4.field->unk8].unkC;
    } else if (mode == 1) {
        a = func_8004AE00();
    } else {
        a = NULL;
    }
    if (a != NULL) {
        for (i = 0; i < g_fieldBgi.unk2; i++) {
            if (g_fieldBgi.unk4[i].unk24 == a) {
                *arg0 = i;
                return &g_fieldBgi.unk4[i];
            }
        }
    }
    return NULL;
}

s32 bgi_setActorActive(s32 arg0, s32 arg1) {
    if (arg1 != 0) {
        g_fieldBgi.unk4[arg0].unk0 |= 1;
    } else {
        g_fieldBgi.unk4[arg0].unk0 &= ~1;
        g_fieldBgi.unk4[arg0].unk4 = -1;
        g_fieldBgi.unk4[arg0].unk6 = -1;
        g_fieldBgi.unk4[arg0].unk8 = -1;
        g_fieldBgi.unk4[arg0].unkA = -1;
    }
    return 1;
}

/* put entry arg0 on the floor below (arg1, arg2, arg3) */
s32 bgi_placeActor(s32 arg0, s16 arg1, s16 arg2, s16 arg3) {
    SVECTOR pos;
    VECTOR v;
    s16 tmp;
    Unk800C9E00 *n;
    Unk800C9DF8 *b;
    Unk800C9DF4 *a;
    Unk800C9E04 *pl;
    s16 ymin;
    u16 found;
    s16 bi;
    s16 bt;
    u16 i;
    u16 hit;
    s32 t;
    s32 cnt;
    Unk800C9E00 *p;
    u16 j;
    u16 m;
    s32 k;
    Unk800C9DF8Tri *tri;
    Unk800C9DFC *vt;

    ymin = 0x7FFF;
    bi = -1;
    bt = -1;
    found = 0;
    i = 0;
    pos.vx = arg1;
    pos.vy = arg2;
    pos.vz = arg3;
    n = g_fieldBgi.unk10;
    b = g_fieldBgi.unk8;
    a = &g_fieldBgi.unk4[arg0];
    for (; i < b->unk24; i++, n++) {
        hit = 0;
        if ((u8)bgi_pointInTriangleXZ(b, &g_fieldBgi.unk1C[n->unk4], n, &pos)) {
            m = ~0x80;
            p = g_fieldBgi.unk10;
            cnt = g_fieldBgi.unk8->unk24;
            for (j = hit; j < cnt; j++) {
                p->unk0 &= m;
                p++;
            }
            g_fieldBgiPushCount = 0;
            if (bgi_pushOutOfEdges(a, &pos, n, &tmp) || bgi_applyPushes(a, &pos, &n, &tmp)) {
                hit = 1;
            }
            if (hit) {
                k = n->unk6;
                if (k >= 0) {
                    v.vx = pos.vx;
                    v.vy = pos.vy;
                    v.vz = pos.vz;
                    pl = &g_fieldBgi.unk14[k];
                    t = func_800C1DC4(pl->unk0, v.vx << 16);
                    v.vy = func_800C1DC4(n->unk24 - t - func_800C1DC4(pl->unk8, v.vz << 16), pl->unkC) >> 16;
                    v.vy += (s16)g_fieldBgi.unk1C[n->unk4].unkA[1] - g_fieldBgi.unk1C[n->unk4].unk4[1];
                    pos.vy = v.vy;
                } else {
                    tri = g_fieldBgi.unk1C + n->unk4;
                    vt = g_fieldBgi.unkC + n->unkC[0];
                    pos.vy = vt->unk2 + (g_fieldBgi.unk8->unk8[1] + tri->unkA[1]);
                }
                if (pos.vy >= arg2 && pos.vy < ymin) {
                    found = 1;
                    ymin = pos.vy;
                    bi = i;
                    bt = n->unk4;
                }
            }
        }
    }
    if (found) {
        a->unk4 = bt;
        a->unk6 = bi;
        a->unk24->pos.vx = arg1;
        a->unk24->pos.vy = ymin;
        a->unk24->pos.vz = arg3;
        return 1;
    }
    return 0;
}

s32 bgi_setActorTriangle(s32 arg0, s16 arg1) {
    Unk800C9E00 *n;
    Unk800C9DF8Tri *t;
    SVECTOR v;

    n = &g_fieldBgi.unk10[(u16)arg1];
    t = &g_fieldBgi.unk1C[n->unk4];
    v.vx = g_fieldBgi.unk8->unk8[0] + t->unkA[0];
    v.vy = g_fieldBgi.unk8->unk8[1] + t->unkA[1];
    v.vz = g_fieldBgi.unk8->unk8[2] + t->unkA[2];
    g_fieldBgi.unk4[arg0].unk4 = n->unk4;
    g_fieldBgi.unk4[arg0].unk6 = arg1;
    g_fieldBgi.unk4[arg0].unk24->pos.vx = v.vx + n->unk1E;
    g_fieldBgi.unk4[arg0].unk24->pos.vy = v.vy + n->unk20;
    g_fieldBgi.unk4[arg0].unk24->pos.vz = v.vz + n->unk22;
    g_fieldBgi.unk4[arg0].unk24->unk40.vx = v.vx + n->unk1E;
    g_fieldBgi.unk4[arg0].unk24->unk40.vy = v.vy + n->unk20;
    g_fieldBgi.unk4[arg0].unk24->unk40.vz = v.vz + n->unk22;
    return 1;
}

s32 bgi_setActorFloorTriangle(s32 arg0, u16 arg1, u16 arg2) {
    Unk800C9DF8Tri *t;
    Unk800C9E00 *n;
    s32 *ids;
    s32 id;
    SVECTOR v;

    t = &g_fieldBgi.unk1C[arg1];
    ids = (s32 *)((u8 *)g_fieldBgi.unk8 + t->unk1E);
    id = ids[arg2];
    v.vx = g_fieldBgi.unk8->unk8[0] + t->unkA[0];
    v.vy = g_fieldBgi.unk8->unk8[1] + t->unkA[1];
    v.vz = g_fieldBgi.unk8->unk8[2] + t->unkA[2];
    n = &g_fieldBgi.unk10[id];
    g_fieldBgi.unk4[arg0].unk4 = arg1;
    g_fieldBgi.unk4[arg0].unk6 = ids[arg2];
    g_fieldBgi.unk4[arg0].unk24->pos.vx = v.vx + n->unk1E;
    g_fieldBgi.unk4[arg0].unk24->pos.vy = v.vy + n->unk20;
    g_fieldBgi.unk4[arg0].unk24->pos.vz = v.vz + n->unk22;
    g_fieldBgi.unk4[arg0].unk24->unk40.vx = v.vx + n->unk1E;
    g_fieldBgi.unk4[arg0].unk24->unk40.vy = v.vy + n->unk20;
    g_fieldBgi.unk4[arg0].unk24->unk40.vz = v.vz + n->unk22;
    return 1;
}

s32 bgi_updateActor(u32 arg0) {
    Unk800C9DF4 *e;

    if (g_fieldBgi.unk0 == 0) {
        return 0x8000;
    }
    if (arg0 >= g_fieldBgi.unk2) {
        return 0x4000;
    }
    e = &g_fieldBgi.unk4[arg0];
    if (e->unk0 & 1) {
        if (e->unk4 == -1 || e->unk6 == -1) {
            return (bgi_placeActor(arg0, e->unk24->pos.vx, e->unk24->pos.vy, e->unk24->pos.vz) != 0) * 2;
        }
        return bgi_moveActor(e);
    }
    return 0;
}

s16 bgi_findTriangleBelow(s16 arg0, s16 arg1, s16 arg2, s16 *arg3, Unk800C9E00 **arg4) {
    SVECTOR p;
    VECTOR v;
    s16 y0;
    s32 best;
    s16 found;
    s16 i;
    Unk800C9E00 *n;
    Unk800C9DF8 *b;
    Unk800C9E04 *pl;
    s32 a;
    Unk800C9DF8Tri *t;
    s16 k;
    s16 y;
    Unk800C9DFC *vx;

    best = 0x7FFF;
    *arg3 = -1;
    *arg4 = NULL;
    n = g_fieldBgi.unk10;
    b = g_fieldBgi.unk8;
    found = 0;
    p.vx = arg0;
    p.vy = arg1;
    p.vz = arg2;
    i = 0;
    y0 = arg1;
    for (; i < b->unk24; n++, i++) {
        if ((u8)bgi_pointInTriangleXZ(b, &g_fieldBgi.unk1C[n->unk4], n, &p)) {
            k = n->unk6;
            if (k >= 0) {
                v.vx = p.vx;
                v.vy = p.vy;
                v.vz = p.vz;
                pl = &g_fieldBgi.unk14[k];
                a = func_800C1DC4(pl->unk0, v.vx << 16);
                v.vy = func_800C1DC4(n->unk24 - a - func_800C1DC4(pl->unk8, v.vz << 16), pl->unkC) >> 16;
                v.vy += (s16)g_fieldBgi.unk1C[n->unk4].unkA[1] - g_fieldBgi.unk1C[n->unk4].unk4[1];
                p.vy = v.vy;
            } else {
                t = &g_fieldBgi.unk1C[n->unk4];
                vx = &g_fieldBgi.unkC[n->unkC[0]];
                p.vy = vx->unk2 + (g_fieldBgi.unk8->unk8[1] + t->unkA[1]);
            }
            y = p.vy;
            if (y >= y0 && y < best) {
                found = 1;
                *arg3 = i;
                best = y;
                *arg4 = n;
            }
        }
    }
    return found;
}

s32 bgi_setFloorActive(s32 arg0, s32 arg1) {
    Unk800C9DF8 *b = g_fieldBgi.unk8;
    Unk800C9DF8Tri *t = &((Unk800C9DF8Tri *)((u8 *)b + b->unk32))[arg0];
    s32 *ids;
    s32 i;
    Unk800C9E00 *n;

    if (arg1 != 0) {
        ids = (s32 *)((u8 *)b + t->unk1E);
        t->unk0 |= 1;
        n = g_fieldBgi.unk10;
        for (i = 0; i < t->unk1C; i++) {
            n[ids[i]].unk0 |= 1;
        }
    } else {
        ids = (s32 *)((u8 *)b + t->unk1E);
        t->unk0 &= ~1;
        n = g_fieldBgi.unk10;
        for (i = 0; i < t->unk1C; i++) {
            n[ids[i]].unk0 &= ~1;
        }
    }
    return 1;
}

s16 bgi_getHeight(s16 arg0, s16 arg1) {
    SVECTOR p;
    VECTOR v;
    u16 i;
    Unk800C9E00 *n;
    Unk800C9E04 *pl;
    s32 a;
    s16 k;
    u8 hit;
    GameData *g;

    if (g_fieldBgi.unk8 == NULL) {
        if (g_gameState->mode == 5) {
            g = g_gameState->unk1C;
            return ((Unk8004AB64 *)g->unk8E0[g->unk8E4.field->unk8].unkC)->pos.vy;
        }
        if (g_gameState->mode == 1) {
            return func_8004AE00()->pos.vy;
        }
        return 0;
    }
    goto start;
plane:
    v.vx = p.vx;
    v.vy = p.vy;
    v.vz = p.vz;
    pl = &g_fieldBgi.unk14[k];
    a = func_800C1DC4(pl->unk0, v.vx << 16);
    v.vy = func_800C1DC4(n->unk24 - a - func_800C1DC4(pl->unk8, v.vz << 16), pl->unkC) >> 16;
    v.vy += (s16)g_fieldBgi.unk1C[n->unk4].unkA[1] - g_fieldBgi.unk1C[n->unk4].unk4[1];
    p.vy = v.vy;
    return p.vy;
start:
    p.vx = arg0;
    p.vy = -0x5CD7;
    p.vz = arg1;
    n = g_fieldBgi.unk10;
    for (i = 0; i < g_fieldBgi.unk8->unk24; i++, n++) {
        if ((u8)bgi_pointInTriangleXZ(g_fieldBgi.unk8, &g_fieldBgi.unk1C[n->unk4], n, &p)) {
            k = n->unk6;
            if (k >= 0) {
                goto plane;
            }
            {
                p.vy = g_fieldBgi.unk8->unk8[1];
                p.vy += g_fieldBgi.unk1C[n->unk4].unkA[1];
                p.vy += g_fieldBgi.unkC[n->unkC[0]].unk2;
                return p.vy;
            }
        }
    }
    return -0x7FFF;
}
