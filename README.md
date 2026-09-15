# VesaBall

VesaBall is a Breakout/Arkanoid-style block-breaking game written in C++, using SDL2 for rendering, input, and windowing. It features mouse-controlled paddle play, 50 built-in levels, a variety of bonus power-ups, and a built-in level editor.

The game was originally written for Windows and DirectX 8 (2005-2006) and later ported to SDL2 for cross-platform portability (Windows, macOS, Linux). The original DirectX 8 platform layer is preserved for reference in [`legacy/`](legacy/) and at git tag `v0.97`, the last commit before the SDL2 port.

## Features

- **Classic brick-breaking gameplay** — bounce the ball off a mouse-controlled paddle to clear boards of bricks.
- **50 levels** included under `lev/` (`00.lev`–`49.lev`).
- **Bonus power-ups**, including expanding/shrinking paddle and ball, extra life, fireball, multi-ball split, exploding bricks, level warp, thru-brick, and more (see `gfx/Bonus_*.png`).
- **Level editor** (`CGameEditor`), accessible from the main menu, for creating or modifying levels.
- **Fullscreen**, matching the desktop's resolution — the game's fixed 640x480 pixel art is scaled up (letterboxed to preserve its aspect ratio) to fit whatever the real display turns out to be.

## Installing / building from source

See [`INSTALL.md`](INSTALL.md) for full prerequisites, per-OS setup, build, run, and troubleshooting steps. Quick version:

```
cmake -S . -B build
cmake --build build
cd build && ./VesaBall   # Windows: VesaBall.exe
```

## Controls

- **Mouse move** — move the paddle.
- **Left mouse button** — launch a caught ball.
- **Esc** — back out to the previous screen (from the main menu, quits the game).
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

### v1.0

- **Changed:**
  - The game now always runs fullscreen at the desktop's resolution, instead of a fixed-size window. Internally it still renders at a fixed 640x480 logical resolution (matching its pixel art) which SDL scales up and letterboxes to fit the real display, so sprites stay crisp and correctly proportioned regardless of the actual screen's aspect ratio. Mouse sensitivity (paddle movement, cursor speed) is rescaled to match, so it feels the same as before regardless of the real display's resolution.
  - Removed `cfg/res.cfg` and the resolution-from-file logic it enabled — there's no longer a window size to configure.
  - Doubled the resolution of every sprite in `gfx/` and enabled bilinear texture filtering, so the art looks meaningfully sharper when scaled up to fill a fullscreen display instead of blocky/nearest-neighbor. Upscaled with alpha-premultiplied Lanczos resampling (the source art is smoothly-shaded rather than hard-edged pixel art, so a pixel-art scaler like Scale2x makes no visible difference here); the two sprite sheets (`Explosion.png`, `Digits.png`) were upscaled frame-by-frame rather than as whole images, so individual animation frames don't bleed into each other.

### v0.99 — Xcode build & universal macOS binary

- **Added:**
  - Documented generating an Xcode project via CMake's Xcode generator (`cmake -G Xcode`) — see `INSTALL.md`.
  - `scripts/package-macos-universal.sh`, which builds and packages a universal (x86_64 + arm64) macOS binary by linking against the official prebuilt universal SDL2/SDL2_image frameworks, for Macs without SDL2 installed separately.

### v0.98 — SDL2 port

Ported the game from DirectX 8 (Direct3D 8 + DirectInput 8) to SDL2, making it buildable on Windows, macOS, and Linux instead of Windows only.

- **Fixed:**
  - A segfault returning to a suspended parent scene (e.g. backing out of a game or the level editor to the main menu): the scene being suspended had its sprites destroyed as if it were ending for good, so it came back with dangling pointers in its render list. Only a scene actually leaving the stack for good is torn down now; a merely-suspended one keeps its resources and resumes where it left off, matching the original DirectX 8 code's own suspend/resume distinction.
  - Incidental fixes found during the port: `CGameEditor` no longer leaks its brick-legend array on scene teardown; a dangling-else and a deprecated `sprintf` call were cleaned up.
- **Changed:**
  - Replaced `D3DApp`/`D3DScene` with an SDL2 `App`/`Scene` application shell. DirectX 8's four-stage device-loss lifecycle (`OnInitDevice`/`OnRestoreDevice`/`OnInvalidateDevice`/`OnDeleteDevice`) doesn't apply to SDL2's renderer, so scenes now use a simpler `OnInit`/`OnDestroy`.
  - Replaced `ID3DXSprite`-based rendering with `SDL_RenderCopyExF` (float-precision, so sprite motion stays as smooth as the original's), and DirectInput's buffered mouse/keyboard events with SDL's event queue feeding the same event-handling code paths.
  - Replaced the small set of D3DX8 math/color types (`D3DXVECTOR2`, `D3DCOLOR`, `D3DXCOLOR`, `D3DXMATRIX`) with portable equivalents (`Vec2`, `Color`) — see `Vec2.h`/`Color.h`.
  - Replaced `CTimer`'s Windows `QueryPerformanceCounter` with SDL's always-available equivalent.
  - Moved the DirectX 8 / Win32-specific files to `legacy/`; see that folder's `README.md`.
  - The game logic itself (physics, collisions, the bonus system, the level file format) is unchanged — only the platform layer was replaced.
- **Added:**
  - `INSTALL.md`, with detailed per-OS build/install/troubleshooting steps.

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
