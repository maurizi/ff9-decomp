/* Draft of fx_0b4 fx_0b4_op80. Status: 323/352: mode 1 and the per-timer loop get their givs
 * (D_801E80EC/D_801E81C4/D_801E8110) in a different order and register. */
#include "btlfx_mod.h"

typedef struct {
    s16 unk0[3]; /* position */
    s16 unk6[3]; /* rotation */
} Fx0b4Key; /* 0xC */

typedef struct {
    s16 unk0;
    u16 unk2; /* keys */
    Fx0b4Key *unk4;
} Fx0b4Path;

typedef struct {
    u16 unk0; /* start frame */
    u16 unk2; /* frames since the start */
} Fx0b4Timer;

typedef struct {
    Pool6CEntry *unk0[6];
    Pool6CEntry *unk18;
    Pool6CEntry *unk1C;
    Pool6CEntry *unk20;
    Pool6CEntry *unk24;
    Pool6CEntry *unk28;
    SVECTOR unk2C; /* caster body position */
    SVECTOR unk34; /* caster position */
} Fx0b4Work;

extern Fx0b4Timer D_801E80EC[6];
extern u16 D_801E8104[6]; /* particle files */
extern u16 D_801E8110[6]; /* path files */
extern SVECTOR D_801E811C;
extern s32 (*D_801E8124[6])(); /* motion callbacks */
extern Unk800F78A4 *D_801E8184;
extern SVECTOR D_801E818C; /* path origin */
extern SVECTOR D_801E8194[6];
extern Fx0b4Path D_801E81C4[6];
extern s32 D_801E81F4[6][0x360];
extern s32 D_801ED2F4[];

s32 func_801E77BC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E77FC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E783C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E787C(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E78BC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E78FC(Pool30Entry *pt, s32 arg1, SVECTOR *pos, s32 arg3, s32 frame, s32 n);
s32 func_801E793C(Pool30Entry *pt, SVECTOR *pos, Fx0b4Timer *timer, Fx0b4Path *path, s32 frame, s32 n);
s32 func_801E8058(Fx0b4Path *path, s32 i, SVECTOR *rot, SVECTOR *pos);
typedef struct {
    s16 x, y, z;
} Fx0b4Point;
void func_801E8040(Fx0b4Path *path, u16 *data);

s32 fx_0b4_op80(s32 mode, Fx0b4Work *work, Unk800F78A4 *env, Unk80100C78Entry *entry, s32 *arg4) {
    BtlfxExports *ex = BTLFX_EXPORTS(env);
    s32 frame;
    s32 i;
    s32 s;
    Fx0b4Timer *tm;
    u16 *cnt;
    Pool6CEntry **pp;
    s32 *anim;
    SVECTOR *pos;
    u16 *file;

    if (mode == 0) {
        *arg4 = 0x3C;
    } else if (mode == 1) {
        D_801E8184 = env;
        ex->btlfx_getUnitBodyPos(0, 0, (s16 *)&work->unk2C);
        ex->btlfx_getUnitPos(0, (s16 *)&work->unk34);
        D_801E818C.vx = work->unk34.vx;
        D_801E818C.vy = work->unk34.vy;
        D_801E818C.vz = work->unk34.vz;
        for (i = 0; i < 6; i++) {
            D_801E80EC[i].unk0 = i * 2 + 0x14;
            func_801E8040(&D_801E81C4[i], ex->btlfx_getFileData(entry->unk8, D_801E8110[i]));
        }
        return 0;
    } else {
        frame = entry->unk0;
        switch (frame) {
        case 0:
            work->unk1C = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x11), (s32)&work->unk34);
            break;
        case 8:
            work->unk20 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xF), (s32)&work->unk34);
            break;
        case 20:
            work->unk18 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x19), (s32)&work->unk34);
            if (work->unk18 != NULL) {
                work->unk18->unk14 = (s32)D_801ED2F4;
                work->unk18->unk12 = 0x12;
                work->unk18->unkC = 0x98;
            }
            break;
        case 25:
            work->unk24 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0x10), (s32)&work->unk2C);
            break;
        case 45:
            work->unk28 = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, 0xE), (s32)&work->unk34);
            break;
        }
        tm = D_801E80EC;
        for (i = 0, cnt = &tm->unk2; i < 6; i++) {
            if (frame == tm->unk0) {
                work->unk0[i] = ex->func_800E3CEC(ex->btlfx_getFileData(entry->unk8, D_801E8104[i]), (s32)&D_801E8194[i]);
                if (work->unk0[i] != NULL) {
                    work->unk0[i]->unk14 = (s32)D_801E81F4[i];
                    work->unk0[i]->unk12 = 0x18;
                    work->unk0[i]->unkC = 0x90;
                }
                if (work->unk0[i] != NULL) {
                    ((EffMotion *)work->unk0[i]->unk0)->unk8 = D_801E8124[i];
                }
            }
            if (frame >= tm->unk0) {
                (*cnt)++;
            }
            tm++;
            cnt += 2;
        }
        if (frame >= 0) {
            if (work->unk1C != NULL) {
                if (work->unk1C->unk30 == -1) {
                    work->unk1C = NULL;
                }
                if (work->unk1C != NULL) {
                    work->unk1C->unk5C.vx = D_801E811C.vx;
                    work->unk1C->unk5C.vy = D_801E811C.vy;
                    work->unk1C->unk5C.vz = D_801E811C.vz;
                }
            }
        }
        if (frame >= 8) {
            if (work->unk20 != NULL) {
                if (work->unk20->unk30 == -1) {
                    work->unk20 = NULL;
                }
                if (work->unk20 != NULL) {
                    work->unk20->unk5C.vx = D_801E811C.vx;
                    work->unk20->unk5C.vy = D_801E811C.vy;
                    work->unk20->unk5C.vz = D_801E811C.vz;
                }
            }
        }
        if (frame >= 0x14) {
            if (work->unk18 != NULL) {
                if (work->unk18->unk30 == -1) {
                    work->unk18 = NULL;
                }
                if (work->unk18 != NULL) {
                    work->unk18->unk5C.vx = D_801E811C.vx;
                    work->unk18->unk5C.vy = D_801E811C.vy;
                    work->unk18->unk5C.vz = D_801E811C.vz;
                }
            }
        }
        if (frame >= 0x2D) {
            if (work->unk28 != NULL) {
                if (work->unk28->unk30 == -1) {
                    work->unk28 = NULL;
                }
                if (work->unk28 != NULL) {
                    work->unk28->unk5C.vx = D_801E811C.vx;
                    work->unk28->unk5C.vy = D_801E811C.vy;
                    work->unk28->unk5C.vz = D_801E811C.vz;
                }
            }
        }
        return frame >= 0x5A;
    }
    return 0;
}
