#ifndef D3DSCENE_H
#define D3DSCENE_H


#include <stack>
using namespace std;

#include <d3dx8.h>
#include <dinput.h>

	//! Scene
	/*!
		\par
		Abstract class describing a single scene - a logically self-contained part of the program, 
		such as the game's main menu, the score screen, the options menu, etc. Serves as a "skeleton" for classes that inherit from it.
		The only interface to a scene should be its constructor. 
		Calling the functions in the correct order and ending the scene is handled by the friend class #CD3DApp.
		This makes it easier to correctly allocate objects in graphics card memory, 
		which must be released when the card's device is reset 
		(e.g. the user minimized the application by pressing Alt-Tab). 
		\par
		The class also provides simple texture management, releasing them automatically when the scene ends.
	*/
class CD3DScene
{
public:
		//! Constructor
	CD3DScene();
		
protected:
		//! Destructor
		/*!
			Automatically releases all textures loaded via #LoadTexture()
		*/
	virtual ~CD3DScene();

		//! Initializes objects that use system memory
		/*!
			In this function, the derived class should initialize all objects, 
			except those that reside in graphics card memory. In particular, this
			is where any needed textures should be loaded via #LoadTexture().
			This is allowed because they are automatically moved to system memory, 
			when the graphics device is reset.
		*/
	virtual HRESULT OnInitDevice() = 0;
		
		//! Initializes objects that use card memory
		/*!
			In this function, the derived class should initialize only those objects, 
			that reside in graphics card memory. In particular, this applies to vertex buffers, 
			and therefore also to \e ID3DXSprite objects.
		*/
	virtual HRESULT OnRestoreDevice() = 0;
		
		//! Releases objects that use card memory
		/*!
			In this function, the derived class should release the objects initialized in #OnRestoreDevice().
		*/
	virtual HRESULT OnInvalidateDevice() = 0;
		
		//! Releases objects that use system memory
		/*!
			In this function, the derived class should release the objects initialized in #OnInitDevice().
		*/
	virtual HRESULT OnDeleteDevice() = 0;

		//! Processes input data from the mouse
		/*!
			This function is called for each element in the device's buffer.
			\param didod	Data from the buffer
		*/
	virtual HRESULT OnMouseEvent( LPDIDEVICEOBJECTDATA didod ) = 0;
		
		//! Processes input data from the keyboard
		/*!
			This function is called for each element in the device's buffer.
			\param didod	Data from the buffer
		*/
	virtual HRESULT OnKeyboardEvent( LPDIDEVICEOBJECTDATA didod ) = 0;
		
		//! Moves the scene's objects
	virtual HRESULT FrameMove( float fElapsedTime ) = 0;
		
		//! Renders the scene's objects
	virtual HRESULT FrameRender() = 0;
		
		//! Sets the pointer to the next scene
		/*!
			If a child scene needs to be created (e.g. going from the main menu to the options menu) 
			this function makes that possible. To do so, create an object of a class derived from #CD3DScene
			and pass its address as the argument. If the current scene has ended, pass \b NULL. 
			\param pScene	Address of the child scene, or null if this scene should end.
		*/
	void SetCurrentScene( CD3DScene* pScene ) 
		{ pCurrentScene = pScene; }

		//! Loads a texture from a file. 
		/*!
			The texture will be automatically released in the class destructor.
			\param strFileName	Texture file name. Supported formats: .bmp, .dds, .dib, .jpg, .png, and .tga
			\param pTex			Address of the texture pointer that receives the loaded texture.
		*/
	HRESULT LoadTexture( char* strFileName, LPDIRECT3DTEXTURE8* pTex );

		//! Direct3D object shared by all scenes
	static LPDIRECT3DDEVICE8 pD3DDevice;

private:
	CD3DScene* GetNextScene();
	CD3DScene* pCurrentScene;
	stack<LPDIRECT3DTEXTURE8> stackTextures;
	friend class CD3DApp;
};

#endif
