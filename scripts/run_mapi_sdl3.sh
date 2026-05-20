#!/usr/bin/env bash
# Usage: ./scripts/run_mapi_sdl3.sh [x] [y] [zone]
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD="${ROOT}/build"
X="${1:-2880}"
Y="${2:-1083}"
ZONE="${3:-0}"

cmake -S "${ROOT}" -B "${BUILD}"
cmake --build "${BUILD}" -j"$(nproc)"

EXE="${BUILD}/TnC_dev/mapi_sdl3"
if [[ ! -x "${EXE}" ]]; then
    echo "Binaire introuvable: ${EXE}" >&2
    exit 1
fi

if [[ ! -d "${ROOT}/data/sprites" ]]; then
    echo "data/sprites manquant. Lancez d'abord: ./scripts/run_convert.sh" >&2
    exit 1
fi

cd "${BUILD}/TnC_dev"
exec ./mapi_sdl3 "${X}" "${Y}" "${ZONE}"
