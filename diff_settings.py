# asm-differ config: tools/docker.sh python3 tools/asm-differ/diff.py -mwo func_XXXXXXXX
def apply(config, args):
    config["arch"] = "mipsel"
    config["baseimg"] = "disks/us/SLUS_012.51"
    config["myimg"] = "build/us/SLUS_012.51"
    config["mapfile"] = "build/us/slus_012.51.map"
    config["source_directories"] = ["src", "include"]
    config["objdump_executable"] = "mipsel-linux-gnu-objdump"
    config["make_command"] = ["make"]
    config["build_dir"] = "build/us"
