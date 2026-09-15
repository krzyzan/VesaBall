#!/bin/sh
# Builds a universal (x86_64 + arm64) macOS VesaBall binary and packages it,
# with its SDL2/SDL2_image frameworks and game assets, into a zip ready to
# attach to a GitHub release.
#
# Why not just `cmake -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"`? Homebrew's
# SDL2/SDL2_image are single-arch, matching whatever machine built them --
# there's no local library to link the other slice against. This script
# instead links against the official prebuilt *universal* SDL2/SDL2_image
# frameworks published by libsdl.org, so the result is architecture-complete
# even when built on only one kind of Mac. (The build machine can only
# execute-test the slice matching its own architecture -- e.g. built on
# Intel, only the x86_64 slice can be run and verified here.)
#
# Usage: scripts/package-macos-universal.sh [output-dir]
# Requires: clang++, curl, hdiutil, lipo, codesign (all standard on macOS).

set -e

REPO_ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT_DIR="${1:-$REPO_ROOT/dist-macos-universal}"
WORK_DIR="$(mktemp -d)"
trap 'rm -rf "$WORK_DIR"' EXIT

SDL2_VERSION=2.32.10
SDL2_IMAGE_VERSION=2.8.12

echo "==> Downloading SDL2 $SDL2_VERSION and SDL2_image $SDL2_IMAGE_VERSION (universal frameworks)"
curl -sL -o "$WORK_DIR/SDL2.dmg" \
	"https://github.com/libsdl-org/SDL/releases/download/release-$SDL2_VERSION/SDL2-$SDL2_VERSION.dmg"
curl -sL -o "$WORK_DIR/SDL2_image.dmg" \
	"https://github.com/libsdl-org/SDL_image/releases/download/release-$SDL2_IMAGE_VERSION/SDL2_image-$SDL2_IMAGE_VERSION.dmg"

SDL2_MOUNT="$WORK_DIR/sdl2_mount"
SDL2_IMAGE_MOUNT="$WORK_DIR/sdl2image_mount"
hdiutil attach "$WORK_DIR/SDL2.dmg" -mountpoint "$SDL2_MOUNT" -nobrowse -quiet
hdiutil attach "$WORK_DIR/SDL2_image.dmg" -mountpoint "$SDL2_IMAGE_MOUNT" -nobrowse -quiet

FW_DIR="$WORK_DIR/Frameworks"
mkdir -p "$FW_DIR"
cp -R "$SDL2_MOUNT/SDL2.framework" "$FW_DIR/"
cp -R "$SDL2_IMAGE_MOUNT/SDL2_image.framework" "$FW_DIR/"
hdiutil detach "$SDL2_MOUNT" -quiet
hdiutil detach "$SDL2_IMAGE_MOUNT" -quiet

for f in SDL2 SDL2_image; do
	archs=$(lipo -info "$FW_DIR/$f.framework/Versions/A/$f" | grep -o 'x86_64\|arm64' | sort -u | tr '\n' ' ')
	echo "==> $f.framework architectures: $archs"
	case "$archs" in
	*arm64*x86_64* | *x86_64*arm64*) ;;
	*)
		echo "error: $f.framework is not universal (got: $archs)" >&2
		exit 1
		;;
	esac
done

echo "==> Compiling universal binary"
rm -rf "$OUT_DIR"
mkdir -p "$OUT_DIR"
clang++ -std=c++11 -O2 \
	-arch x86_64 -arch arm64 \
	-F"$FW_DIR" -I"$FW_DIR/SDL2.framework/Headers" -I"$FW_DIR/SDL2_image.framework/Headers" \
	"$REPO_ROOT"/*.cpp \
	-framework SDL2 -framework SDL2_image -framework Cocoa \
	-Wl,-rpath,@executable_path/Frameworks \
	-o "$OUT_DIR/VesaBall"

lipo -info "$OUT_DIR/VesaBall"

echo "==> Assembling package"
mkdir -p "$OUT_DIR/Frameworks"
cp -R "$FW_DIR/SDL2.framework" "$OUT_DIR/Frameworks/"
cp -R "$FW_DIR/SDL2_image.framework" "$OUT_DIR/Frameworks/"
cp -R "$REPO_ROOT/gfx" "$REPO_ROOT/lev" "$OUT_DIR/"

echo "==> Ad-hoc code-signing"
codesign --force --sign - "$OUT_DIR/Frameworks/SDL2.framework"
codesign --force --sign - "$OUT_DIR/Frameworks/SDL2_image.framework"
codesign --force --deep --sign - "$OUT_DIR/VesaBall"

echo "==> Done: $OUT_DIR"
echo "    Run with: (cd '$OUT_DIR' && ./VesaBall)"
echo "    Zip for release with: (cd '$OUT_DIR/..' && zip -r -y VesaBall-macos-universal.zip '$(basename "$OUT_DIR")')"
