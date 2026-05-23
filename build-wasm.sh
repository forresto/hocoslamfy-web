#!/bin/bash
set -e

if ! command -v emcc &>/dev/null; then
    echo "Error: emcc not found."
    echo "Install Emscripten SDK: https://emscripten.org/docs/getting_started/downloads.html"
    exit 1
fi

SRCS="main.c init.c title.c game.c score.c audio.c bg.c text.c unifont.c platform/general.c"

emcc -O2 \
    -I. \
    -s USE_SDL=2 \
    -s USE_SDL_IMAGE=2 \
    -s SDL2_IMAGE_FORMATS='["png"]' \
    -s USE_SDL_MIXER=2 \
    --preload-file data \
    -s EXPORTED_FUNCTIONS='["_main","_TriggerBoostDown","_TriggerBoostUp","_SetMusicMuted","_SetSFXMuted","_SetFieldScrollMultiplier"]' \
    -s EXPORTED_RUNTIME_METHODS='["cwrap","ccall"]' \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ENVIRONMENT=web \
    -Wall -Wno-unused-variable \
    -o hocoslamfy.js \
    $SRCS

echo "Build complete: hocoslamfy.js + hocoslamfy.wasm + hocoslamfy.data"
echo "Serve with: python3 -m http.server 8000"
