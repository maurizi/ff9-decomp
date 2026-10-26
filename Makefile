# FF9 PS1 decompilation (NTSC-U). Build targets need the toolchain container:
#   tools/docker.sh make verify
# `make extract` / `make extract-img` are stdlib-only Python and also run on the host.

VERSION  ?= us
# Raw 2352-byte disc images ("FF9 CD1.img" .. "FF9 CD4.img").
DISC_DIR ?= disks/$(VERSION)/raw

DISKS    := disks/$(VERSION)
MAIN     := SLUS_012.51
SPLAT_YAML := config/slus_012.51.yaml
BUILD_DIR := build/$(VERSION)
LD_SCRIPT := $(BUILD_DIR)/slus_012.51.ld
ELF      := $(BUILD_DIR)/slus_012.51.elf
EXE      := $(BUILD_DIR)/$(MAIN)

### Toolchain (paths are inside the docker image) ###
CC_DIR   ?= /opt/cc
# gcc 2.8.1: everything matches under 2.8.0 too, except func_80039C38 (2.8.1 only) -- see CLAUDE.md
CC_VER   ?= gcc-2.8.1-psx
CC1      := $(CC_DIR)/$(CC_VER)/cc1
CPP      := cpp
AS       := mipsel-linux-gnu-as
LD       := mipsel-linux-gnu-ld
OBJCOPY  := mipsel-linux-gnu-objcopy
PYTHON   := python3
SPLAT    := $(PYTHON) -m splat split
MASPSX   := $(PYTHON) tools/maspsx/maspsx.py

CC_FLAGS      ?= -O2 -G0
ASPSX_VERSION ?= 2.77
# Checks that never change code: every call needs a declaration in scope (a port compiles src/ with a
# modern compiler, which rejects implicit declarations; tools/decl_check.py compares them with the definitions).
CC_CHECKS := -Werror-implicit-function-declaration
CPP_FLAGS := -nostdinc -Iinclude -undef -D__GNUC__=2 -Dmips -D__mips__ -D_LANGUAGE_C -DLANGUAGE_C
ASFLAGS   := -march=r3000 -mabi=32 -EL -no-pad-sections -O0 -Iinclude
LDFLAGS   := -T $(LD_SCRIPT) \
             -T config/undefined_funcs_auto.$(VERSION).txt \
             -T config/undefined_syms_auto.$(VERSION).txt \
             -T config/xrefs.$(VERSION).txt \
             --no-check-sections -nostdlib \
             -Map $(ELF:.elf=.map)

# FF9.IMG dir-0 code overlays (identical on all discs); see config/overlays.us.txt
OVLS      := $(shell awk '/^[0-9a-f]/ {print $$1}' config/overlays.$(VERSION).txt)
OVL_BINS  := $(foreach o,$(OVLS),$(BUILD_DIR)/ovl_$(o).bin)
YAMLS     := $(SPLAT_YAML) $(foreach o,$(OVLS),config/ovl_$(o).yaml)

# FF9.IMG dir-13 battle effect modules (one binary each; docs/ff9img.md, config/fxmods.us.txt).
# Built and verified once `make extract-fxmod` has put their images in disks/us/fxmod/.
FXMOD_DIR := $(DISKS)/fxmod
ifneq ($(wildcard $(FXMOD_DIR)/fx_000.bin),)
FXMODS    := $(shell awk '/^fx_/ {print $$1}' config/fxmods.$(VERSION).txt)
endif
FX_BINS   := $(foreach m,$(FXMODS),$(BUILD_DIR)/$(m).bin)
FX_YAMLS  := $(foreach m,$(FXMODS),config/fxmod/$(m).yaml)
FX_SUMS   := $(if $(FXMODS),config/checksums.fxmod.$(VERSION).sha1)

.PHONY: all extract extract-img extract-ovl extract-fxmod split resplit build verify progress clean distclean

all: verify

# Boot exe is identical on all discs; take it from disc 1.
extract: $(DISKS)/$(MAIN)
$(DISKS)/$(MAIN):
	python3 tools/extract_disc.py "$(DISC_DIR)/FF9 CD1.img" $(DISKS) $(MAIN)
	cd $(DISKS) && echo "e37e8c4dc7c374a06375f312b366f79b8fbd1764  $(MAIN)" | sha1sum -c

# FF9.IMG (overlays/assets) differs per disc and is ~330-430MB each.
extract-img:
	for n in 1 2 3 4; do \
		python3 tools/extract_disc.py "$(DISC_DIR)/FF9 CD$$n.img" $(DISKS)/disc$$n FF9.IMG || exit 1; \
	done

# Code overlays from disc 1's FF9.IMG (needs extract-img first).
extract-ovl:
	python3 tools/ff9img.py $(DISKS)/disc1/FF9.IMG extract-dir 0 $(DISKS)/ovl

# Battle effect modules from FF9.IMG dir 13 (needs extract-img; identical on all discs, read from disc 1),
# then their splat configs (existing yamls are kept).
extract-fxmod:
	python3 tools/effect_modules.py extract $(FXMOD_DIR)
	python3 tools/gen_fxmod_configs.py

# First split only: splat won't touch existing .c files. Use resplit afterwards.
# The effect modules are many small binaries: split them in parallel.
split: $(DISKS)/$(MAIN)
	rm -rf asm/$(VERSION)
	for y in $(YAMLS); do $(SPLAT) $$y || exit 1; done
	@echo "splitting $(words $(FX_YAMLS)) effect modules"
	@$(if $(FX_YAMLS),printf '%s\n' $(FX_YAMLS) | xargs -P 8 -n 1 $(SPLAT) >/dev/null)

# Re-split after symbol/config changes, keeping decompiled C.
resplit: $(DISKS)/$(MAIN)
	$(PYTHON) tools/resplit.py $(YAMLS) $(FX_YAMLS)

# splat's ld_dependencies rule lists every object the ELF needs.
-include $(LD_SCRIPT:.ld=.d)

$(BUILD_DIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

# Binaries whose C is split along the original translation units (docs/source-files.md): each
# object's .rodata is linked at 4-byte alignment (subalign: 4) like psylink did, so gcc's own
# 8-alignment of jump tables reproduces the original padding words. That is now every binary
# (single-TU overlays included); only the effect modules still get the rewrite below.
TU_SPLIT := src/main $(foreach o,$(OVLS),src/ovl_$(o))

# Some original source files were built with -O1: name them *_o1.c (splat subsegment name).
$(BUILD_DIR)/%_o1.c.o: CC_FLAGS := -O1 -G0

# cpp -> cc1 -> maspsx (emulates ASPSX expansions) -> GNU as
$(BUILD_DIR)/%.c.o: %.c $(wildcard include/*.h include/fxmod/*.h)
	@mkdir -p $(dir $@)
	$(CPP) $(CPP_FLAGS) $< -o $(@:.o=.i)
	$(CC1) -quiet $(CC_FLAGS) $(CC_CHECKS) $(@:.o=.i) -o $(@:.o=.cc1.s)
	@# gcc 8-aligns jump tables (.align 3); originals were separate objects packed at 4, so
	@# in a C file that merges several of them that alignment lands differently. Use 4 there.
	@# Binaries split into their original translation units keep .align 3 (TU_SPLIT).
	$(if $(filter $(TU_SPLIT),$(patsubst %/,%,$(dir $<))),,sed -i 's/^\t\.align\t3$$/\t.align\t2/' $(@:.o=.cc1.s))
	$(MASPSX) --aspsx-version=$(ASPSX_VERSION) --expand-div --run-assembler $(ASFLAGS) -o $@ < $(@:.o=.cc1.s)

$(ELF): $(LD_SCRIPT)
	$(LD) $(LDFLAGS) -o $@ $(filter %.o,$^)

$(EXE): $(ELF)
	$(OBJCOPY) -O binary $< $@

define OVL_RULES
-include $(BUILD_DIR)/ovl_$(1).d
$(BUILD_DIR)/ovl_$(1).elf: $(BUILD_DIR)/ovl_$(1).ld
	$(LD) -T $$< -T config/undefined_funcs_auto.ovl_$(1).$(VERSION).txt \
		-T config/undefined_syms_auto.ovl_$(1).$(VERSION).txt \
		--no-check-sections -nostdlib -Map $$(@:.elf=.map) -o $$@ $$(filter %.o,$$^)
$(BUILD_DIR)/ovl_$(1).bin: $(BUILD_DIR)/ovl_$(1).elf
	$(OBJCOPY) -O binary $$< $$@
endef
$(foreach o,$(OVLS),$(eval $(call OVL_RULES,$(o))))

define FX_RULES
-include $(BUILD_DIR)/$(1).d
$(BUILD_DIR)/$(1).elf: $(BUILD_DIR)/$(1).ld
	$(LD) -T $$< -T config/fxmod/undefined_funcs_auto.$(1).txt -T config/fxmod/undefined_syms_auto.$(1).txt \
		--no-check-sections -nostdlib -Map $$(@:.elf=.map) -o $$@ $$(filter %.o,$$^)
$(BUILD_DIR)/$(1).bin: $(BUILD_DIR)/$(1).elf
	$(OBJCOPY) -O binary $$< $$@
endef
$(foreach m,$(FXMODS),$(eval $(call FX_RULES,$(m))))

build: $(EXE) $(OVL_BINS) $(FX_BINS)

# Effect modules: one OK line each is noise, so only failures are listed, then a count.
verify: build
	sha1sum -c config/checksums.$(VERSION).sha1 config/checksums.ovl.$(VERSION).sha1
	$(if $(FX_SUMS),@sha1sum --quiet -c $(FX_SUMS) && echo "effect modules: $(words $(FXMODS)) OK", \
		@echo "effect modules: not extracted (make extract-fxmod), skipped")

# Function/byte counts for game code (PsyQ libs excluded). `make progress CSV=1` logs to docs/progress.csv
progress:
	python3 tools/progress.py $(if $(CSV),--csv)

clean:
	rm -rf build asm config/undefined_*_auto*.txt config/fxmod/undefined_*_auto.*.txt

distclean: clean
	rm -rf disks
