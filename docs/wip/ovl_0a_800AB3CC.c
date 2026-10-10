/*
 * ovl_0a func_800AB3CC (0x6CC bytes): per-frame update/draw of the field models.
 * WIP: 435 of 435 instructions, 4 aligned diff rows: only the prologue order of `li s4, 100`
 * (lim) and `move s1, zero` (j); the original does the s4 pair first. sched1 already puts j = 0
 * (insn 12) ahead of lim = 100 (insn 9) before register allocation, and swapping or moving the
 * two statements doesn't change that (moving them after the if/else costs 5-11 rows).
 * Needs c = list[i], lim/cnt/j/j1 as separate locals and two s16 locals ax/az for the frame.
 * Pass 9 (cause found with -dS): sched1 ranks `birthing` insns (dest set once, REG_N_SETS == 1)
 * at 0x7f000001, so lim = 100 (set once) is placed after j = 0 (j is also the first f-loop
 * counter, so it is set several times). In the original lim must be set more than once: it is
 * the same variable as cnt (s4 holds 100 and later `lbu s4, 3(a0)`). With lim used as cnt and
 * `f = 1 << j;` in the first f-loop (permuter) the prologue is right and only lim/n swap
 * s4/s5 (31 rows): n (refs 28, len 285, priority 0.393) outranks lim (16, 222, 0.288).
 * To get that variant: replace cnt by lim, and in the first f-loop
 * `f = 1 << j; if (!(m->unk14 & (u32)f))`. Making j set once instead folds `o->unk22 + j`.
 */
#include "ovl_0a.h"

#define SetSpadStack12(addr) \
    __asm__ volatile("move $8, %0\n\tsw $29, 0($8)\n\taddiu $8, $8, -12\n\tmove $29, $8" : : "r"(addr) : "$8", "memory")
#define ResetSpadStack12() __asm__ volatile("addiu $29, $29, 12\n\tlw $29, 0($29)" : : : "memory")

void func_800B5958(Unk800C95C8 *, u8);
void func_800B61A4(Unk800C95C8 *, u8);
void func_800BB508(Unk800C95C8 *, u8);
void func_800BB714(Unk800C95C8 *, u8);
void func_80031244(SVECTOR *, MATRIX *);
MATRIX *func_80030EA4(SVECTOR *, MATRIX *);

void func_800AB3CC(void) {
    FieldEvtCtx *list[16];
    SVECTOR rot;
    DVECTOR sxy;
    s32 ofs[2];
    s16 ax;
    s16 az;
    s32 z;
    u32 *ot;
    s32 n;
    s32 i;
    s32 j;
    s32 j1;
    s32 lim;
    s32 d;
    s32 cnt;
    s32 f;
    Unk800C95C8 *m;
    FieldEvtObj *o;
    FieldEvtCtx *c;
    WmModel *w;

    lim = 100;
    j = 0;
    if (D_800C9D44 & 0x40) {
        func_8019AE0C(&ofs[0], &ofs[1]);
        SetGeomOffset(ofs[0], ofs[1]);
    } else {
        func_800C0F08();
    }
    i = 0;
    ot = D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8];
    D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8] = ot + D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk30;
    n = func_800AB110((FieldObjSlot **)list);
    for (; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->unk8;
            o = c->unkC;
            if ((c->unk0 & 2) && func_800C768C(o->unk3B, &ax, &az)) {
                rot.vx = ax;
                rot.vy = o->unk22;
                rot.vz = az;
                func_80030EA4(&rot, &m->unk24);
            } else {
                rot.vx = o->unk20;
                rot.vy = (o->unk22 + j) & 0xFFF;
                rot.vz = o->unk24;
                func_80031244(&rot, &m->unk24);
            }
            m->unk24.t[0] = o->unk14.vx;
            m->unk24.t[1] = o->unk14.vy;
            m->unk24.t[2] = o->unk14.vz;
            func_800B7CEC(m);
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            o = c->unkC;
            m = c->unk8;
            w = func_8001C7FC(4, o->unk36 | 0x30000, 0);
            m->unkC = w;
            if (w != NULL && (s32)w->unkC >= 0) {
                w->unkC = (void *)((u8 *)w + (s32)w->unkC);
                if (w->unk10 != NULL) {
                    w->unk10 = (void *)((u8 *)m->unkC + (s32)w->unk10);
                }
            }
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->unk8;
            o = c->unkC;
            if (m->unk0 & 1) {
                func_800BA560(m, o->unk3A);
            } else {
                func_800B7F14(m, o->unk3A);
            }
        }
    }
    gte_SetRotMatrix(&D_8006794C->unk1C->unk18);
    gte_SetTransMatrix(&D_8006794C->unk1C->unk18);
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->unk8;
            rot.vx = ((Unk8004AB64 *)m->unk20)->unk14.vx;
            rot.vy = ((Unk8004AB64 *)m->unk20)->unk14.vy;
            rot.vz = ((Unk8004AB64 *)m->unk20)->unk14.vz;
            gte_ldv0(&rot);
            gte_rtps();
            gte_stsxy(&sxy);
            gte_stsz(&z);
            z = z / 4 + D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk30;
            if (z < lim || z > 0x1000 - lim) {
                list[i] = NULL;
            }
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            m = c->unk8;
            f = c->unk0 & 0x100000;
            cnt = m->unk8->unk3;
            SetSpadStack12(0x1F8003FC);
            if (f) {
                for (j = 0; j < cnt; j++) {
                    if (!(m->unk14 & (u32)(1 << j))) {
                        func_800BB508(m, j);
                        func_800BB714(m, j);
                    }
                }
            } else {
                for (j1 = 0; j1 < cnt; j1++) {
                    if (!(m->unk14 & (u32)(1 << j1))) {
                        func_800B61A4(m, j1);
                        func_800B5958(m, j1);
                    }
                }
            }
            ResetSpadStack12();
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            func_800AC6C0(D_8006794C->unk1C->unk8E4.field->unkC->unk14->unk2C, c->unkC->unk3B);
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL && !(c->unk0 & 0x1000)) {
            m = c->unk8;
            if (m->unk50->unk0 >= 2 && func_800AB0B0(m) == 0) {
                m->unk50->unk4[2].unk0 |= 1;
                m->unk50->unk4[2].unk8 = 0;
                m->unk50->unk4[2].unkE = 0x1000;
            }
        }
    }
    for (i = 0; i < n; i++) {
        c = list[i];
        if (c != NULL) {
            func_800BB090((BgAnimHdr *)((Unk800C95C8 *)c->unk8)->unk50);
        }
    }
    func_800B2D10(list, n);
    if (!(D_8006794C->unk1C->unk0 & 2)) {
        func_800B1E8C(list, n);
    }
    func_800AC02C(list, n);
    D_8006794C->unk1C->unk10[D_8006794C->unk1C->unk8] = ot;
    if (!(D_800C9D44 & 0x40)) {
        func_800C0FB0();
    }
}
