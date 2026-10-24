#include "ovl_0b.h"

void func_800C23C8(BtlUnit *arg0, s32 arg1, u32 arg2) {
    s32 k;
    k = arg0->unk68 >> 1;
    k &= 0xF;
    if (!(arg0->unk68 & 1)) {
        k += 9;
    }
    arg0->unkBF = (arg1 & 0xFFFF) / 14;
    arg0->unkC0 = arg2 / 12;
    g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_0 = 0;
    g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_8 = arg0->unkBF * 14;
    g_gameState->unk1C->unk8E4.battle->unk11C0->unk28[k].unk0_20 = arg0->unkC0 * 12;
}

void btl_setUnitFade(BtlUnit *arg0, s32 arg1) {
    Unk800CEE74 *p = btlbg_getHeader();
    btl_setModelAbr(arg0->unk9C, 0x60);
    func_800C8A3C(arg0->unk9C, (p->unkC * arg1) << 11 >> 16, (p->unkD * arg1) << 11 >> 16, (p->unkE * arg1) << 11 >> 16);
    if (arg1 == 0) {
        arg0->unk68 |= 0x400000;
    }
}

void btl_playCmdEffect(BtlCmd *arg0, s32 arg1, s16 *arg2) {
    Battle *b = g_gameState->unk1C->unk8E4.battle;
    s32 sum[4];
    BtlUnitList *l;
    BtlUnit *u;
    BtlUnit *c;
    s32 mask;
    u8 e;

    memset(sum, 0, 0x10);
    mask = 0;
    b->unk116C.unk0 = arg0->unk4;
    l = &b->unk116C;
    if (arg2 != NULL) {
        l->unk25[0] = arg2[0];
        l->unk25[1] = arg2[1];
        l->unk42 = arg2[2];
        l->unk40 = arg2[3];
    } else {
        l->unk42 = 0;
        l->unk40 = 0;
        l->unk25[1] = 0;
        l->unk25[0] = 0;
    }
    l->unk27 = 0;
    l->unk24 = 0;
    c = arg0->unk4;
    c->unk7E = 0;
    c->unk82 = 0;
    c->unk80 = 0;
    if (arg0->unk10.b.unk0_7 == 0) {
        mask = btl_applyReflect(arg0);
        e = arg0->cmdNo;
        if (e == 0xE || e == 0x33) {
            l->unk40 = 2;
        } else if (e == 0x1F) {
            l->unk40 = 4;
        } else if ((u8)(e - 0x39) < 2) {
            l->unk40 = 1;
        }
    } else if (arg0->unk10.b.unk0_7 == 2) {
        arg0->unk10.b.unk0_7 = 1;
        if (!(arg0->unk4->unk68 & 1)) {
            arg0->unk10.b.unk0_11 = 1;
        }
        l->unk40 = 0x11;
    }
    for (u = g_gameState->unk1C->unk8E4.battle->unk6F8; u != NULL; u = u->unk0) {
        if (u->unkB0 & (u16)arg0->targets) {
            l->unk4[l->unk24++] = u;
            u->unk7E = 0;
            u->unk82 = 0;
            u->unk80 = 0;
            sum[0] += u->unk98->unk0[0] >> 16;
            sum[2] += u->unk98->unk0[2] >> 16;
        }
        if (u->unkB0 & mask) {
            l->unk28[l->unk27++] = u;
        }
        if (arg0->cmdNo == 0x1F && (u->unk68 & 1) && g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4 == 2) {
            l->unk44 = u;
        }
    }
    l->unk38 = sum[0] / (s8)l->unk24;
    l->unk3A = 0;
    l->unk3C = sum[2] / (s8)l->unk24;
    btlfx_resetAll();
    btlfx_setTargets(l);
    btlfx_start(arg1);
}

void btl_startCmdEffect(BtlCmd *cmd) {
    BtlUnit *u = cmd->unk4;
    s32 id;
    s32 k;
    u16 t;

    switch (cmd->cmdNo) {
    case 0x1:
    case 0x31:
    case 0x34:
        if (u->unk68 & 1) {
            id = g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4 + 0x64;
        } else {
            id = 0x77;
        }
        btl_playCmdEffect(cmd, id, 0);
        break;
    case 0x2:
        if (u->unk68 & 1) {
            k = g_gameState->unk1C->characters[(u->unk68 >> 1) & 0xF].unk34.b.unk4;
        } else {
            k = 0x13;
        }
        switch (k) {
        case 0:
            btl_playCmdEffect(cmd, 0xC8, 0);
            break;
        case 1:
            btl_playCmdEffect(cmd, 0x111, 0);
            break;
        case 0xE:
            btl_playCmdEffect(cmd, 0x13, 0);
            break;
        case 0xF:
            btl_playCmdEffect(cmd, 0x77, 0);
            break;
        default:
            btl_playCmdEffect(cmd, 0x14, 0);
            break;
        }
        break;
    case 0x4:
        btl_showCmdName(cmd);
        u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[12];
        u->unkA0->unk3A = 0;
        btl_execCmd(u);
        return;
    case 0x7:
        btl_showCmdName(cmd);
        u->unk9C->unkC = (BtlUnit9CC *)u->unkA4[9];
        return;
    case 0xE:
    case 0x33:
        btl_playCmdEffect(cmd, D_8006B1F4[cmd->subNo - 0xE0].unk0_8, 0);
        break;
    case 0xF:
        /* padded: the first jump table is 8-aligned in its object */
        __asm__(".section .rodata\n\t.word 0\n\t.section .text");
        switch (D_80069018[cmd->subNo].unk8) {
        case 1:
            btl_playCmdEffect(cmd, 0x110, 0);
            break;
        case 2:
            btl_playCmdEffect(cmd, 0x10A, 0);
            break;
        case 3:
        case 4:
            btl_playCmdEffect(cmd, 0x10B, 0);
            break;
        case 5:
            btl_playCmdEffect(cmd, 0x10C, 0);
            break;
        case 6:
            btl_playCmdEffect(cmd, 0x10D, 0);
            break;
        case 7:
            btl_playCmdEffect(cmd, 0x109, 0);
            break;
        case 8:
        case 9:
        case 10:
            btl_playCmdEffect(cmd, 0x10E, 0);
            break;
        case 11:
            btl_playCmdEffect(cmd, 0x10F, 0);
            break;
        case 12:
            btl_playCmdEffect(cmd, 0x115, 0);
            break;
        }
        break;
    case 0x2F:
        if ((u16)cmd->targets < 0x10 && (cmd->ability->unk8 & 8) && !cmd->unk10.b.unk0_0) {
            t = btl_findCover((u16)cmd->targets);
            if (t) {
                cmd->targets = t;
                cmd->unk10.b.unk0_5 = 1;
            }
        }
    case 0x35:
    case 0x36:
    case 0x37:
        btl_startEnemySeq(cmd);
        return;
    case 0x3B:
        id = 0x1E9;
        if (u->status & 0x4000) {
            id = 0x101;
        }
        btl_playCmdEffect(cmd, id, 0);
        break;
    case 0x12:
        switch (cmd->subNo) {
        case 0x31:
            g_gameState->unk1C->unk8E4.battle->unk11BB = 0x99;
            break;
        case 0x33:
            g_gameState->unk1C->unk8E4.battle->unk11BB = 0x9A;
            break;
        case 0x35:
            g_gameState->unk1C->unk8E4.battle->unk11BB = 0x9B;
            break;
        case 0x37:
            g_gameState->unk1C->unk8E4.battle->unk11BB = 0x9C;
            break;
        case 0x3A:
            g_gameState->unk1C->unk8E4.battle->unk11BB = 0x9D;
            break;
        case 0x3C:
            g_gameState->unk1C->unk8E4.battle->unk11BB = 0x9E;
            break;
        case 0x3E:
            g_gameState->unk1C->unk8E4.battle->unk11BB = 0x9F;
            break;
        case 0x40:
            g_gameState->unk1C->unk8E4.battle->unk11BB = 0xA0;
            break;
        }
    default:
        if ((cmd->ability->unk0_0 == 3 && !cmd->unk10.b.unk0_0) || (cmd->unk10.w & 0x600) ||
            cmd->cmdNo == 0x20 || cmd->cmdNo == 0x21) {
            btl_playCmdEffect(cmd, cmd->ability->unkC, 0);
        } else {
            btl_playCmdEffect(cmd, cmd->ability->unk0_8, 0);
        }
        break;
    }
}
