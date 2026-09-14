# VesaBall

VesaBall is a Breakout/Arkanoid-style block-breaking game written in C++, using SDL2 for rendering, input, and windowing. It features mouse-controlled paddle play, 50 built-in levels, a variety of bonus power-ups, and a built-in level editor.

The game was originally written for Windows and DirectX 8 (2005-2006) and later ported to SDL2 for cross-platform portability (Windows, macOS, Linux). The original DirectX 8 platform layer is preserved for reference in [`legacy/`](legacy/) and at git tag `v0.97`, the last commit before the SDL2 port.

## Features

- **Classic brick-breaking gameplay** — bounce the ball off a mouse-controlled paddle to clear boards of bricks.
- **50 levels** included under `lev/` (`00.lev`–`49.lev`).
- **Bonus power-ups**, including expanding/shrinking paddle and ball, extra life, fireball, multi-ball split, exploding bricks, level warp, thru-brick, and more (see `gfx/Bonus_*.png`).
- **Level editor** (`CGameEditor`), accessible from the main menu, for creating or modifying levels.
- **Configurable resolution** via `cfg/res.cfg`.

## Requirements

- A C++11 (or later) compiler.
- [CMake](https://cmake.org/) 3.10 or later.
- [SDL2](https://www.libsdl.org/) and [SDL2_image](https://github.com/libsdl-org/SDL_image) development packages.
  - macOS (Homebrew): `brew install sdl2 sdl2_image`
  - Debian/Ubuntu: `apt install libsdl2-dev libsdl2-image-dev`
  - Windows: via [vcpkg](https://vcpkg.io/) (`vcpkg install sdl2 sdl2-image`) or [MSYS2](https://www.msys2.org/) (`pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image`)

## Building from source

```
cmake -S . -B build
cmake --build build
```

This produces a `VesaBall` (or `VesaBall.exe` on Windows) executable in `build/`, with the `gfx/`, `lev/`, and `cfg/` asset folders copied alongside it so it can be run directly from there.

> **Note for network/remote-mounted checkouts:** if the repository lives on a network share (SMB/NFS), point the build directory at a local path instead, e.g. `cmake -S . -B /tmp/vesaball-build` — some platforms block executing binaries built on a network mount.

## Running

Run the built executable from its own directory (`build/`, if you followed the steps above), or from the repository root, so it can find `cfg/res.cfg` and the `gfx/`/`lev/` folders alongside it.

## Controls

- **Mouse move** — move the paddle.
- **Left mouse button** — launch a caught ball.
- **P** / **Pause** — pause the game.
- **Sys Rq** — toggle cheat mode.
- With cheats enabled: **Space** clears all bricks, **B** spawns/cycles bonus power-ups, holding **right mouse button** slows the game down.

## Project layout

| Path | Contents |
|---|---|
| `*.cpp` / `*.h` | Game source (rendering, game engine, sprites, bricks, paddle, ball, bonuses, menu, editor). |
| `Vec2.h`, `Color.h`, `InputEvent.h`, `WinTypes.h` | Small portable primitives (2D vector math, packed color, input events, and a few Win32-named integer typedefs) the game logic is built on, letting most of it stay platform-agnostic. |
| `App.cpp/h`, `Scene.cpp/h` | The SDL2 application shell and the scene base class (window/renderer setup, the main loop, texture loading and lifetime). |
| `CMakeLists.txt` | The build configuration. |
| `cfg/res.cfg` | Screen resolution (width and height, one per line). |
| `gfx/` | Sprite textures (bricks, paddle, ball, bonuses, UI). |
| `lev/` | Level data files (`00.lev`–`49.lev`). |
| `legacy/` | The original DirectX 8 / Win32 platform layer and Visual Studio / Dev-C++ project files, kept for historical reference. See [`legacy/README.md`](legacy/README.md). |
| `Doxyfile` | Doxygen configuration for generating API documentation from the source comments. |

## Documentation

The source is documented with Doxygen-style comments (in English). Generate HTML docs with:

```
doxygen Doxyfile
```

Output is written to `html/` (ignored by git).

## Changelog

### Unreleased — SDL2 port

Ported the game from DirectX 8 (Direct3D 8 + DirectInput 8) to SDL2, making it buildable on Windows, macOS, and Linux instead of Windows only:

- Replaced `D3DApp`/`D3DScene` with an SDL2 `App`/`Scene` application shell. DirectX 8's four-stage device-loss lifecycle (`OnInitDevice`/`OnRestoreDevice`/`OnInvalidateDevice`/`OnDeleteDevice`) doesn't apply to SDL2's renderer, so scenes now use a simpler `OnInit`/`OnDestroy`.
- Replaced `ID3DXSprite`-based rendering with `SDL_RenderCopyExF` (float-precision, so sprite motion stays as smooth as the original's), and DirectInput's buffered mouse/keyboard events with SDL's event queue feeding the same event-handling code paths.
- Replaced the small set of D3DX8 math/color types (`D3DXVECTOR2`, `D3DCOLOR`, `D3DXCOLOR`, `D3DXMATRIX`) with portable equivalents (`Vec2`, `Color`) — see `Vec2.h`/`Color.h`.
- Replaced `CTimer`'s Windows `QueryPerformanceCounter` with SDL's always-available equivalent.
- Moved the DirectX 8 / Win32-specific files to `legacy/`; see that folder's `README.md`.
- The game logic itself (physics, collisions, the bonus system, the level file format) is unchanged — only the platform layer was replaced.
- Incidental fixes found during the port: `CGameEditor` no longer leaks its brick-legend array on scene teardown; a dangling-else and a deprecated `sprintf` call were cleaned up.

### v0.97

- **Fixed:**
  - `CBrickArray::Load()` could read out-of-bounds when a level file was missing or corrupt, due to an unchecked file open/read and an unvalidated brick-type byte flowing into the fixed-size brick-type arrays.
  - `CD3DScene::LoadTexture()` swallowed texture-load failures and always reported success, which could queue a guaranteed null-pointer crash at scene teardown for any failed load.
  - Invalid "extra qualification" C++ syntax on `CSpriteMoving::SetSpeed()` in `SpriteMoving.h` that only compiled under GCC's `-fpermissive` and would fail outright on stricter/newer compilers.
  - A one-frame rendering glitch in the lives counter when the player loses their last life (a negative value briefly fed into digit rendering).
  - Case-mismatched `#include` directives and `gfx/` asset paths that would fail to build or load assets on case-sensitive filesystems (Linux, strict macOS/Windows).
  - A tracked filename case mismatch (`timer.h` vs. the on-disk `Timer.h`).
  - Author-machine-specific absolute paths in `Doxyfile` that broke `doxygen Doxyfile` for anyone but the original author.
  - Missing `gfx/Bonus_EightBall.png` asset for the fully-implemented EightBall bonus.
- **Changed:**
  - Translated all Polish Doxygen and inline comments to English.
  - Normalized indentation to tabs and applied a consistent `clang-format` style across all source files.
  - Normalized line endings via `.gitattributes` (CRLF for source/project files, stored as LF in the repository).
- **Added:**
  - This README, `LICENSE` (GPL-3.0), and `.gitattributes`.

## License

VesaBall is licensed under the [GNU General Public License v3.0](LICENSE).
