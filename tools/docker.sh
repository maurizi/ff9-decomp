#!/usr/bin/env bash
# Run a command inside the ff9-decomp build container, e.g. `tools/docker.sh make verify`.
# The image is (re)built automatically when the Dockerfile or requirements change.
set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
IMAGE=ff9-decomp-build
HASH=$(cat "$ROOT/tools/docker/Dockerfile" "$ROOT/requirements.txt" | sha1sum | cut -c1-12)

if ! docker image inspect "$IMAGE:$HASH" >/dev/null 2>&1; then
    docker build -t "$IMAGE:$HASH" -t "$IMAGE:latest" -f "$ROOT/tools/docker/Dockerfile" "$ROOT"
fi

TTY=()
[ -t 0 ] && [ -t 1 ] && TTY=(-it)

exec docker run --rm "${TTY[@]}" -u "$(id -u):$(id -g)" -v "$ROOT:/ff9" -w /ff9 "$IMAGE:$HASH" "${@:-bash}"
