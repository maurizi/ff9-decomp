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
# gcc 2.8.x matches FF9 game code (2.8.0 vs 2.8.1 not yet distinguished) -- see CLAUDE.md
CC_VER   ?= gcc-2.8.0-psx
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
CPP_FLAGS := -nostdinc -Iinclude -undef -D__GNUC__=2 -Dmips -D__mips__ -D_LANGUAGE_C -DLANGUAGE_C
ASFLAGS   := -march=r3000 -mabi=32 -EL -no-pad-sections -O0 -Iinclude
LDFLAGS   := -T $(LD_SCRIPT) \
             -T config/undefined_funcs_auto.$(VERSION).txt \
             -T config/undefined_syms_auto.$(VERSION).txt \
             --no-check-sections -nostdlib \
             -Map $(ELF:.elf=.map)

.PHONY: all extract extract-img split build verify clean distclean

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

split: $(DISKS)/$(MAIN)
	rm -rf asm/$(VERSION)
	$(SPLAT) $(SPLAT_YAML)

# splat's ld_dependencies rule lists every object the ELF needs.
-include $(LD_SCRIPT:.ld=.d)

$(BUILD_DIR)/%.s.o: %.s
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -o $@ $<

# cpp -> cc1 -> maspsx (emulates ASPSX expansions) -> GNU as
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CPP) $(CPP_FLAGS) $< -o $(@:.o=.i)
	$(CC1) -quiet $(CC_FLAGS) $(@:.o=.i) -o $(@:.o=.cc1.s)
	$(MASPSX) --aspsx-version=$(ASPSX_VERSION) --expand-div --run-assembler $(ASFLAGS) -o $@ < $(@:.o=.cc1.s)

$(ELF): $(LD_SCRIPT)
	$(LD) $(LDFLAGS) -o $@ $(filter %.o,$^)

$(EXE): $(ELF)
	$(OBJCOPY) -O binary $< $@

build: $(EXE)

verify: $(EXE)
	sha1sum -c config/checksums.$(VERSION).sha1

clean:
	rm -rf build asm config/undefined_*_auto*.txt

distclean: clean
	rm -rf disks
