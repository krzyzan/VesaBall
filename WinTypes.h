#ifndef WIN_TYPES_H
#define WIN_TYPES_H

//! Portable stand-ins for the small set of Win32 basic types the game logic
//! uses (DWORD, BYTE, LONG, HRESULT, POINT, ...). These are just integer
//! typedefs and a couple of macros -- no Win32 API calls are involved -- so
//! keeping the familiar names let the actual game-logic files (collision,
//! bonuses, level format, etc.) go untouched by the SDL2 port, which only
//! needed to replace the rendering/input/windowing layer that these types
//! never depended on in the first place.

#include <cstdint>
#include <cstring>

#define MAX_PATH 260

inline void ZeroMemory(void* p, size_t size)
{
	memset(p, 0, size);
}

typedef uint8_t BYTE;
typedef uint32_t DWORD;
typedef int32_t LONG;
typedef int HRESULT;

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#define S_OK ((HRESULT)0)
#define S_FALSE ((HRESULT)1)
#define E_FAIL ((HRESULT)0x80004005)

#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)
#define FAILED(hr) ((HRESULT)(hr) < 0)

struct POINT
{
	LONG x;
	LONG y;
};

#endif
