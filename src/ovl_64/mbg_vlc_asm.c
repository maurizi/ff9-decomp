#include "ovl_64.h"

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/mbg_vlc_asm", mbg_vlcBuildTable);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/mbg_vlc_asm", mbg_vlcSetSize);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/mbg_vlc_asm", mbg_vlcDecode);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/mbg_vlc_asm", mbg_vlcDecodeBlock);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/mbg_vlc_asm", mbg_vlcDecodeLumaBlockV3);

INCLUDE_ASM("asm/us/ovl_64/nonmatchings/mbg_vlc_asm", mbg_applyMask);
