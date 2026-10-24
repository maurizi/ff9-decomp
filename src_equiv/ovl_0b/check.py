#!/usr/bin/env python3
"""Runs tools/equiv.py on every routine in src_equiv/ovl_0b against the original asm.

    tools/docker.sh python3 src_equiv/ovl_0b/check.py [name ...]   # needs a built tree (make verify)

Routines that walk structures get fixed pointers (--arg-set) and --poke'd counts/offsets so the random
memory equiv.py starts from takes them through their loops instead of faulting on the first unaligned load;
everything else in those structures stays random. Prints one equiv.py summary per configuration.
"""
import subprocess
import sys

D = "src_equiv/ovl_0b/"
M = 0x80100000
RUNS = []


def run(name, src, trials=200, args=(), pokes=None):
    RUNS.append((name, src, trials, list(args), pokes or {}))


# math.c, background.c accessors: standard arguments
for f in ["btlfx_pushTrailPoint", "btlfx_distance", "btlfx_rand", "btlfx_randRange", "btlfx_randCentered",
          "btlfx_randRotation", "btlfx_aimAngles", "btlfx_transformPoint", "btlfx_transformPolar", "btlfx_lerp",
          "btlfx_lerpRatio", "btlfx_lerpVec", "btlfx_stepParticleAltDrag", "btlfx_stepParticleNoDragAlt",
          "btlfx_stepModuleParticle"]:
    run(f, "math.c")
run("btlfx_sqrt", "math.c", 300, ["--arg-mask", "1:1f"])  # a random 32-bit round count runs into the step limit
run("btlfx_stepParticle", "math.c", 200, ["--end", "800DFF5C"])  # jumps into btlfx_stepParticleAltDrag
run("btlfx_stepParticleNoDrag", "math.c", 200, ["--end", "800E0014"])
for f in ["btlbg_setBrightness", "btlbg_getBrightness", "btlbg_setOtShift"]:
    run(f, "background.c", 50)

# background.c
run("btlbg_projectVertices", "background.c", 100, ["--arg-set", "3=c"])
for flags in (0, 8, 2, 1):
    for bright, shift in ((0x80, 0), (0x40, 2)):
        R, G = M, 0x80110000
        pk = {R: (flags, 2), R + 2: (12, 2), R + 4: (0, 2), R + 6: (0x10, 2), R + 8: (0x40, 2), R + 0xA: (0x80, 2),
              R + 0xC: (3, 2), R + 0xE: (3, 2), G + 0xA: (0x10, 2), G + 0xE: (0x100, 2),
              0x800CF588: (bright, 4), 0x800CF5A0: (shift, 4)}
        idx = [(i * 7 + 3) % 12 * 4 for i in range(21)]
        for q in range(12):
            pk[R + 0x40 + 2 * q] = (idx[q], 2)
        for t in range(9):
            pk[R + 0x58 + 2 * t] = (idx[12 + t], 2)
        run("btlbg_drawRecord", "background.c", 100,
            ["--end", "800CF560", "--arg-set", f"0={R:x}", "--arg-set", f"3={G:x}"], pk)  # runs projectVertices too

# mesh_packets.c
for f in ("btl_drawMesh", "btl_drawMeshOffset"):
    for flags in (0, 0x10):
        pk = {M: (flags, 2), M + 8: (0x80101000, 4), M + 0x10: (0, 4), M + 0x18: (0x80102000, 4),
              0x80101010: (0x80103000, 4), 0x8006794C: (0x80107000, 4), 0x8010701C: (0x80108000, 4),
              0x80108008: (0, 1), 0x80108010: (0x80110000, 4), 0x8010301C: (0x80104000, 4),
              0x80103020: (0x80105000, 4), 0x80103024: (0x80106000, 4)}
        for t in range(8):
            pk[0x80103002 + 2 * t] = (3, 2)
        run(f, "mesh_packets.c", 100, ["--arg-set", f"0={M:x}", "--arg-set", "1=0", "--arg-set", "2=80200000"], pk)

# bones.c: (animation, parent, bones, root flags, nibble keys)
for anim, parent, n, aflags, nib in [(0, 0, 3, 0, 0), (0, 1, 3, 0, 0), (1, 0, 3, 7, 0), (1, 1, 3, 0, 1),
                                     (1, 0, 28, 0, 1), (1, 1, 40, 3, 1), (1, 0, 40, 0, 0)]:
    pk = {M + 0xC: (0x80101000 if anim else 0, 4), M + 8: (0x80102000, 4), 0x80102002: (n, 1),
          0x8010200C: (0x80103000, 4), M + 0x1C: (0x80104000 if parent else 0, 4), 0x80104020: (0x80105000, 4),
          0x8010100A: (aflags, 2), 0x8010100C: (0x80106000, 4), 0x80101010: (0x80107000 if nib else 0, 4),
          0x80101004: (0x100, 2), 0x80101006: (0x200, 2), 0x80101008: (0x300, 2)}
    run("btl_calcBoneMatrices", "bones.c", 60, ["--arg-set", f"0={M:x}", "--arg-set", "2=80200000"], pk)

# mesh_project.c
for f, nb, counts in [("btlfx_projectSliceMesh", 4, [2, 0, 4, 3]), ("btlfx_projectSliceMesh", 1, [6]),
                      ("btlfx_projectMorphMesh", 4, [2, 0, 4, 3]), ("btlfx_projectMorphMesh", 2, [1, 7])]:
    pk = {M + 8: (0x80101000, 4), 0x80101002: (nb, 1), 0x80101010: (0x80102000, 4), 0x80102014: (0x80103000, 4),
          0x80102018: (0x80104000, 4), M + 0x5C: (0x80105000, 4), 0x80105010: (0x80106000, 4),
          0x80106018: (0x80107000, 4), M + 0x20: (0x80108000, 4), 0x8006794C: (0x80109000, 4),
          0x8010901C: (0x8010A000, 4)}
    for i, c in enumerate(counts):
        pk[0x80103000 + 2 * i] = (c, 2)
    run(f, "mesh_project.c", 60, ["--arg-set", f"0={M:x}", "--arg-set", "1=0", "--arg-mask", "2:ffff"], pk)

# sprites.c: context at M, points at 0x80101000, packets at 0x80200000, OT at 0x80300000
PTRS = ["--arg-set", "0=80101000", "--arg-set", f"1={M:x}", "--arg-set", "2=80200000", "--arg-set", "3=80300000"]
for f in ["btlfx_emitAnimQuads", "btlfx_emitSprites", "btlfx_emitFixedSprites", "btlfx_emitDots"]:
    for fade in (0x800, 0xFFFFFFFF):
        run(f, "sprites.c", 100, PTRS, {M: (0x80102000, 4), M + 4: (0x80103000, 4), M + 0x1C: (3, 4),
                                         M + 0x18: (fade, 4), M + 0x20: (0, 4)})
for n in (3, 0):
    run("btlfx_emitParticles", "sprites.c", 100, PTRS, {M + 0x14: (n, 4), M + 0x18: (0, 4)})
    run("btlfx_emitScreenSprites", "sprites.c", 100, PTRS, {M + 0x14: (n, 4), M + 0x18: (0x80104000, 4)})

# rings.c (btlfx_emitRingGouraud calls btlfx_fadeColor, just below it, with its own registers)
for f, extra in [("btlfx_emitRingQuads", []), ("btlfx_emitRingGouraud", ["--start", "800D4A08"])]:
    for stride, on in [(3, 1), (7, 0), (1, 1)]:
        pk = {M + 0x8: (0x80101000, 4), 0x80101000: (on, 2), M + 0xC: (0x80102000, 4), M + 0x10: (0x80103000, 4),
              M + 0x14: (0x80104000, 4), M + 0x18: (0, 4), M + 0x1C: (4, 4), M + 0x24: (2, 4)}
        if f == "btlfx_emitRingGouraud":
            pk[M + 0x20] = (0x80105000, 4)
        run(f, "rings.c", 60, ["--arg-set", f"0={stride:x}"] + PTRS[2:] + extra, pk)

# frames.c (btlfx_emitFrameFan calls btlfx_linkFrameTri, just below it, with its own registers)
for f, extra in [("btlfx_emitFrameQuad", []), ("btlfx_emitFrameFan", ["--start", "800D597C"])]:
    for fade in (0x800, 0xFFFFFFFF):
        pk = {M + 4: (0, 4), M + 8: (fade, 4), M + 0x10: (0x80104000 if f == "btlfx_emitFrameQuad" else 0x40, 4),
              M + 0x14: (1, 4)}
        run(f, "frames.c", 100, ["--arg-set", "0=80102000"] + PTRS[2:] + extra, pk)

# trails.c: ring buffer at 0x80101000 (last index, head)
for f in ["btlfx_emitTrailRibbonFlat", "btlfx_emitTrailRibbon3D", "btlfx_emitTrailLines", "btlfx_emitTrailRibbonScreen"]:
    for count, last, head in [(3, 5, 2), (1, 5, 5), (4, 2, 1)]:
        run(f, "trails.c", 80, ["--arg-set", f"0={count:x}"] + PTRS[2:],
            {M: (0, 4), M + 4: (0x80101000, 4), 0x80101000: (last, 1), 0x80101001: (head, 1)})

# effect_frame.c: the original saves $sp in g_btlfxSavedSp (the C doesn't switch stacks)
run("btlfx_runFrame", "effect_frame.c", 200, ["--ignore", "800FF468-800FF46C"])


def main():
    only = set(sys.argv[1:])
    bad = 0
    for name, src, trials, args, pokes in RUNS:
        if only and name not in only:
            continue
        cmd = ["python3", "tools/equiv.py", D + src, "--bin", "ovl_0b", "--func", name, "--trials", str(trials)] + args
        for a, (v, size) in pokes.items():
            cmd += ["--poke", f"{a:x}={v:x}:{size}"]
        p = subprocess.run(cmd, capture_output=True, text=True)
        print((p.stdout.strip().splitlines() or [p.stderr.strip()])[-1], flush=True)
        bad += p.returncode != 0
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
