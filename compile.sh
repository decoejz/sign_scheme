#!/bin/bash

set -e  # Aborta se algo der errado

TARGET="desktop"
CFLAGS="-Wall -Wextra -Wpedantic"

for arg in "$@"; do
    case $arg in
        --target=stm32)
            TARGET="stm32"
            ;;
        --target=desktop)
            TARGET="desktop"
            ;;
        *)
            echo "Usage: $0 [--target=stm32|--target=desktop]"
            exit 1
            ;;
    esac
done

# Define OPENSSL_NO_STDIO só para desktop
if [[ "$TARGET" == "desktop" ]]; then
    CFLAGS="$CFLAGS -DOPENSSL_NO_STDIO"
fi

# Caminho do projeto
ROOT_DIR=$(dirname "$(realpath "$0")")

# Recria pastas
rm -rf "$ROOT_DIR/build" "$ROOT_DIR/install"
mkdir -p "$ROOT_DIR/build" "$ROOT_DIR/install"

# Gera o build com CMake
cmake -DTARGET_PLATFORM="${TARGET}" \
      -DCMAKE_C_FLAGS="${CFLAGS}" \
      -DCMAKE_INSTALL_PREFIX="${ROOT_DIR}/install" \
      -S "${ROOT_DIR}" -B "${ROOT_DIR}/build"

# Compila e instala
cmake --build "${ROOT_DIR}/build" --parallel
cmake --install "${ROOT_DIR}/build"

echo "✅ Build completed for platform: ${TARGET}"
