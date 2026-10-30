#include "ovl_64.h"

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", DecDCTReset);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", DecDCTGetEnv);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", DecDCTPutEnv);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", DecDCTin);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", DecDCTout);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", DecDCTinSync);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", DecDCToutSync);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", DecDCTinCallback);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", DecDCToutCallback);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", MDEC_reset);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", MDEC_in);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", MDEC_out);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", MDEC_in_sync);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", MDEC_out_sync);

u32 mdec_getStatus(void) {
    return *g_mdec1Reg;
}

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/psyq_8019CD60", mdec_timeout);
