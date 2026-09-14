#ifndef INPUT_EVENT_H
#define INPUT_EVENT_H

#include <SDL.h>

//! A single buffered input event, replacing DirectInput's DIDEVICEOBJECTDATA.
/*!
	Keeps the original's "offset + data, with bit 0x80 meaning pressed"
	shape on purpose: every scene's OnMouseEvent()/OnKeyboardEvent() already
	switches on an offset and checks that bit, so only the code that
	produces these events (the SDL event loop) needed to change, not the
	game logic that consumes them.
*/
struct InputEvent
{
	enum OffsetEnum
	{
		AxisX,
		AxisY,
		Button0,
		Button1,
		Key
	} ofs;

	long data;			   //!< Axis: relative delta. Button/Key: 0x80 = down, 0 = up.
	SDL_Scancode scancode; //!< Meaningful when ofs == Key.

	InputEvent() : ofs(Key), data(0), scancode(SDL_SCANCODE_UNKNOWN) {}
};

#endif
