# Legacy DirectX 8 / Windows build

This folder holds the pieces of VesaBall's original DirectX 8 + Win32 platform
layer that were replaced when the game was ported to SDL2 for portability:

- `D3DApp.cpp` / `D3DApp.h` — the Direct3D 8 application shell (window creation,
  device/mode enumeration, DirectInput setup, main loop).
- `D3DScene.cpp` / `D3DScene.h` — the abstract scene base class, including the
  `OnInitDevice`/`OnRestoreDevice`/`OnInvalidateDevice`/`OnDeleteDevice`
  device-loss lifecycle that DirectX 8 required and SDL2 doesn't need.
- `VesaBall.vcproj`, `VesaBall.sln` — the Visual Studio (2003-era) project files.
- `VesaBall.dev`, `Makefile.win` — the Dev-C++ / MinGW project files.

These files are kept for historical reference — this is how the game
originally ran — but they are **not part of the active build** and won't
compile as-is: `VesaBall.cpp` and `StdAfx.h` at the repository root were
rewritten for SDL2, and the paths in the project files above assume the old
flat layout (everything at the repository root, including this folder's
contents).

To build the exact original DirectX 8 version, check out the `v0.97` git tag,
which is the last commit before the SDL2 port:

```
git checkout v0.97
```

See the root `README.md` for build instructions for the current SDL2 version.
