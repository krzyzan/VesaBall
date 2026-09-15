#ifndef APP_H
#define APP_H

#include <SDL.h>
#include <stack>
using namespace std;

#include "Scene.h"
#include "Timer.h"

//! SDL2 application
/*!
	Simplifies working with SDL2. Handles creating the window and renderer
	and running the main loop, dispatching input events to the current
	scene. Manages #CScene objects, performing their initialization and
	shutdown in the correct order.
*/
class CApp
{
public:
	//! Constructor
	/*!
		\param	WindowTitle	Application window title
	*/
	CApp(const char* WindowTitle);

	//! Creates the application window and renderer, and the random number generator.
	HRESULT Create();

	//! Sets the current scene
	/*!
		Should be called after #Create() has executed;
	*/
	HRESULT StartNewScene(CScene* pScene);

	//! Runs the main program loop
	/*!
		Handles retrieving and processing SDL events, moving and rendering
		the current scene once per frame.
	*/
	HRESULT Run();

private:
	// Internal functions managing scenes
	HRESULT ChangeScene();
	HRESULT StartParentScene();

	void ProcessEvent(const SDL_Event & event);

	bool bRunning;

	// Fractional remainder of mouse motion not yet reported to the current
	// scene, carried across events -- see ProcessEvent()'s SDL_MOUSEMOTION
	// case. Rescaling real screen-pixel deltas down to the game's fixed
	// logical resolution can shrink a single event's delta below one whole
	// logical pixel; accumulating instead of truncating each event keeps
	// slow/precise mouse movement from being silently dropped.
	float fMouseRemainderX, fMouseRemainderY;

	// Scene stack
	stack<CScene*> sScenes;

	SDL_Window* pWindow;
	SDL_Renderer* pRenderer;

	// Main timer
	CTimer Timer;

	// Application window title
	const char* strWindowTitle;
};

#endif
