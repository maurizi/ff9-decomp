#!/bin/bash
# start Redux on disc 3, boot, load a state, install coverage + freeze helpers
cd "$(dirname "$0")/../.."
tools/emu.py start --disc 3 --paused && tools/emu.py frames 1800 && tools/emu.py load ${1:-disc3_malboro_lv99_calm}
tools/emu.py lua -f tools/rt/freeze.lua
python3 tools/rt/cov.py start --bin ovl_0b --bin main
