# Installing VesaBall

This is a step-by-step build/install guide. For a quick overview, gameplay
controls, and project layout, see [`README.md`](README.md).

## 1. Prerequisites

- A C++11 (or later) compiler (GCC, Clang, or MSVC).
- [CMake](https://cmake.org/) 3.10 or later.
- [SDL2](https://www.libsdl.org/) and [SDL2_image](https://github.com/libsdl-org/SDL_image)
  development packages (headers + libraries, not just the runtime).

### Installing SDL2 / SDL2_image

**macOS (Homebrew):**

```
brew install sdl2 sdl2_image cmake
```

**Debian / Ubuntu:**

```
sudo apt install build-essential cmake libsdl2-dev libsdl2-image-dev
```

**Fedora:**

```
sudo dnf install gcc-c++ cmake SDL2-devel SDL2_image-devel
```

**Arch Linux:**

```
sudo pacman -S base-devel cmake sdl2 sdl2_image
```

**Windows (vcpkg):**

```
vcpkg install sdl2 sdl2-image
```

Then configure CMake with vcpkg's toolchain file, e.g.:

```
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=[path to vcpkg]/scripts/buildsystems/vcpkg.cmake
```

**Windows (MSYS2 / MinGW):**

```
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image
```

Run the remaining steps from an MSYS2 MinGW64 shell.

## 2. Get the source

```
git clone https://github.com/krzyzan/VesaBall.git
cd VesaBall
```

> **Network/remote-mounted checkouts (SMB, NFS, etc.):** build to a path on
> local disk instead of the network mount, e.g. `cmake -S . -B ~/vesaball-build`.
> Several platforms (macOS included) refuse to execute a binary whose file
> lives on a network share, so a build placed there will compile fine but
> fail to launch with a "permission denied" error unrelated to file
> permissions.

## 3. Configure and build

```
cmake -S . -B build
cmake --build build
```

This produces a `VesaBall` executable (`VesaBall.exe` on Windows) in
`build/`. The `gfx/`, `lev/`, and `cfg/` asset folders are copied alongside
it automatically as part of the build, so the executable is self-contained
and runnable directly from `build/`.

For a specific build type (the default is `Release`):

```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
```

## 4. Run

```
cd build
./VesaBall          # Windows: VesaBall.exe
```

Run it from inside `build/` (or the repository root, if you're running an
in-source build) so it can find `cfg/res.cfg` and the `gfx/`/`lev/` folders
next to the executable.

Screen resolution is read from `cfg/res.cfg` (width and height, one per
line) — edit it before launching to change it.

## 5. Verifying the install

On first launch you should see the VESABALL title screen with three menu
options: **start game**, **level editor**, **quit to dos**. If the window
doesn't appear or exits immediately, see Troubleshooting below.

## macOS: Xcode project and universal binaries

To open the project in Xcode instead of building from the command line, generate
an Xcode project with CMake's Xcode generator, then open it:

```
cmake -S . -B build-xcode -G Xcode
open build-xcode/VesaBall.xcodeproj
```

Build and run from Xcode as usual (`Cmd+R`), or from the command line with
`xcodebuild -project build-xcode/VesaBall.xcodeproj -target VesaBall -configuration Release build`.

A regular build (via either generator) only produces a binary for the
machine's own architecture, since Homebrew's SDL2/SDL2_image are single-arch.
To build a **universal (x86_64 + arm64) binary** that runs natively on both
Intel and Apple Silicon Macs, use `scripts/package-macos-universal.sh` — it
downloads the official prebuilt universal SDL2/SDL2_image frameworks from
libsdl.org and links against those instead:

```
scripts/package-macos-universal.sh
```

This writes a self-contained `dist-macos-universal/` (the `VesaBall`
binary, its `Frameworks/`, and `gfx/`/`lev/`/`cfg/`) that can be zipped up
and run on any Mac without installing SDL2 separately.

## Troubleshooting

- **`SDL2 not found` / `SDL2_image not found` during `cmake` configure** —
  the `-dev`/`-devel` package wasn't installed, or (on Windows) the vcpkg
  toolchain file wasn't passed to `cmake -S`. Re-check step 1.
- **Builds but the window flashes and closes immediately** — check that
  `gfx/`, `lev/`, and `cfg/` ended up next to the executable (the build's
  post-build step copies them automatically; a manual copy of just the
  binary will be missing them).
- **"Permission denied" running an executable that built without errors** —
  see the network-mounted-checkout note in step 2.
- **Everything installed but CMake still can't find SDL2 on Linux** — confirm
  `pkg-config --exists sdl2 SDL2_image` succeeds; if `pkg-config` itself
  isn't installed, install it (e.g. `apt install pkg-config`) alongside the
  dev packages.

## Building the original DirectX 8 version

The pre-port, Windows-only DirectX 8 build (Visual Studio / Dev-C++ project
files) is preserved for historical reference at git tag `v0.97`:

```
git checkout v0.97
```

See [`legacy/README.md`](legacy/README.md) for details. It requires the
DirectX 8 SDK and a period-appropriate toolchain, and is not expected to
build with modern tools.

## Uninstalling

VesaBall doesn't install anything outside its own build directory — no
registry entries, system files, or services. Delete the cloned repository
(and `build/` within it) to remove it completely.

## License

VesaBall is licensed under the [GNU General Public License v3.0](LICENSE).
