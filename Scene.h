#ifndef SCENE_H
#define SCENE_H

#include <stack>
using namespace std;

#include <SDL.h>
#include "InputEvent.h"

//! Scene
/*!
	\par
	Abstract class describing a single scene - a logically self-contained part of the program,
	such as the game's main menu, the score screen, the options menu, etc. Serves as a "skeleton" for classes that inherit from it.
	The only interface to a scene should be its constructor.
	Calling the functions in the correct order and ending the scene is handled by the friend class #CApp.
	\par
	The class also provides simple texture management, releasing them automatically when the scene ends.

	\par
	Unlike the original DirectX 8 version's CD3DScene, this doesn't need a
	separate device-loss lifecycle (OnInitDevice/OnRestoreDevice/
	OnInvalidateDevice/OnDeleteDevice) -- SDL2's renderer doesn't lose its
	textures the way a D3D8 device could on Alt-Tab, so OnInit()/OnDestroy()
	is all that's needed.
*/
class CScene
{
public:
	//! Constructor
	CScene();

protected:
	//! Destructor
	/*!
		Automatically releases all textures loaded via #LoadTexture()
	*/
	virtual ~CScene();

	//! Initializes the scene's objects and loads its textures.
	virtual HRESULT OnInit() = 0;

	//! Releases the scene's objects.
	virtual HRESULT OnDestroy() = 0;

	//! Processes input data from the mouse
	/*!
		\param evt	The input event
	*/
	virtual HRESULT OnMouseEvent(const InputEvent* evt) = 0;

	//! Processes input data from the keyboard
	/*!
		\param evt	The input event
	*/
	virtual HRESULT OnKeyboardEvent(const InputEvent* evt) = 0;

	//! Moves the scene's objects
	virtual HRESULT FrameMove(float fElapsedTime) = 0;

	//! Renders the scene's objects
	virtual HRESULT FrameRender() = 0;

	//! Sets the pointer to the next scene
	/*!
		If a child scene needs to be created (e.g. going from the main menu to the options menu)
		this function makes that possible. To do so, create an object of a class derived from #CScene
		and pass its address as the argument. If the current scene has ended, pass \b NULL.
		\param pScene	Address of the child scene, or null if this scene should end.
	*/
	void SetCurrentScene(CScene* pScene)
	{
		pCurrentScene = pScene;
	}

	//! Loads a texture from a file.
	/*!
		The texture will be automatically released in the class destructor.
		\param strFileName	Texture file name.
		\param pTex			Address of the texture pointer that receives the loaded texture.
	*/
	HRESULT LoadTexture(const char* strFileName, SDL_Texture** pTex);

	//! Renderer shared by all scenes
	static SDL_Renderer* pRenderer;

private:
	CScene* GetNextScene();
	CScene* pCurrentScene;
	stack<SDL_Texture*> stackTextures;
	friend class CApp;
};

#endif
