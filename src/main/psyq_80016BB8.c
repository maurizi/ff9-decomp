#include "ff9.h"

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", InitGeom);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", SetGeomOffset);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _patch_gte);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", func_80016D0C);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", SpuInit);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _SpuInit);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", SpuStart);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", OpenEvent);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", EnableEvent);

INCLUDE_RODATA("asm/us/main/nonmatchings/psyq_80016BB8", D_80010330);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _spu_init);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", func_80017160);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _spu_FiDMA);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _spu_Fr_);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _spu_t);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _spu_Fw);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _spu_Fr);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _spu_FsetRXX);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _spu_FsetRXXa);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _spu_FgetRXXa);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _spu_FsetPCR);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", func_80017968);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", func_80017990);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _spu_Fw1ts);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", DeliverEvent);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _SpuDataCallback);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", SpuSetReverbModeParam);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _SpuIsInAllocateArea);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _SpuIsInAllocateArea_);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _spu_setReverbAttr);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", SpuSetReverbDepth);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", SpuClearReverbWorkArea);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", WaitEvent);

void SpuGetReverbModeType(s32 *a) {
    *a = D_8006798C;
}

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", InitCARD);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", StartCARD);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", StopCARD);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", SetInitPadFlag);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", ReadInitPadFlag);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", PAD_init);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", InitPAD);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", StartPAD);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", func_800189CC);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", func_80018A44);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", func_80018AAC);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", InitPAD2);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", StartPAD2);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", PAD_init2);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", SysEnqIntRP);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", SysDeqIntRP);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", EnablePAD);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", DisablePAD);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _patch_pad);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _remove_ChgclrPAD);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", InitCARD2);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", StartCARD2);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", StopCARD2);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _patch_card_info);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", func_80018CC4);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", func_80018CF0);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", func_80018D34);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _patch_card);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _patch_card2);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _copy_memcard_patch);

INCLUDE_ASM("asm/us/main/nonmatchings/psyq_80016BB8", _ExitCard);
