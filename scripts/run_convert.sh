#!/usr/bin/env bash
# Reproduit: source path_SDL.sh && make && ./convert2 "Game Files" data/
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD="${ROOT}/build/decode"
SRC_GAME="${1:-${ROOT}/decode/Game Files}"
OUT_DATA="${2:-${ROOT}/data}"

mkdir -p "${BUILD}" "${OUT_DATA}"
cmake -S "${ROOT}/decode" -B "${BUILD}"
cmake --build "${BUILD}" -j"$(nproc)"

cd "${BUILD}"
./convert2 "${SRC_GAME}" "${OUT_DATA}"
echo "Données écrites dans: ${OUT_DATA}"
