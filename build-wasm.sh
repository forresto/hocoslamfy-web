#!/bin/bash
set -e

if ! command -v emcc &>/dev/null; then
    if [ -n "$EMSDK_DIR" ] && [ -f "$EMSDK_DIR/emsdk_env.sh" ]; then
        source "$EMSDK_DIR/emsdk_env.sh"
    else
        echo "Error: emcc not found. Set EMSDK_DIR or activate emsdk first."
        exit 1
    fi
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
echo "Serve with: npx serve ."
