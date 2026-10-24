#include "ff9.h"

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdSearchFile);

s32 func_8001E698(char *a, char *b) {
    return strncmp(a, b, 12) == 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_newmedia);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_8001E97C);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_cachefile);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_8001ECBC);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", strcmp);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", strncmp);

INCLUDE_RODATA("asm/us/main/nonmatchings/psyq_8001E3C0", D_800105B4);

INCLUDE_RODATA("asm/us/main/nonmatchings/psyq_8001E3C0", D_800105B8);

INCLUDE_RODATA("asm/us/main/nonmatchings/psyq_8001E3C0", D_800105BC);

INCLUDE_RODATA("asm/us/main/nonmatchings/psyq_8001E3C0", D_800105D8);

INCLUDE_RODATA("asm/us/main/nonmatchings/psyq_8001E3C0", D_80010738);

INCLUDE_RODATA("asm/us/main/nonmatchings/psyq_8001E3C0", D_80010748);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_8001ED50);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_sync);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_ready);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_cw);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_vol);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_flush);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_initvol);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_initintr);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_init);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_datasync);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_set_test_parmnum);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_800201F4);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdIntToPos);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdControl);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdControlF);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdControlB);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_800207A0);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_800207D4);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_80020A48);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_80020B18);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdReadBreak);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdRead);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdReadSync);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdInit);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_800211D0);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_8002120C);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_80021234);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_8002125C);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdPosToInt);

u8 CdStatus(void) {
    return D_8006825C;
}

s32 CdMode(void) {
    return D_8006826C;
}

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdLastPos);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdFlush);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdReady);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdSyncCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdReadyCallback);

s32 CdGetSector(void *a, s32 b) {
    return CD_getsector(a, b) == 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_getsector);

s32 CdGetSector2(void *a, s32 b) {
    return CD_getsector2(a, b) == 0;
}

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CD_getsector2);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", CdDataCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_8001E3C0", func_80021620);
