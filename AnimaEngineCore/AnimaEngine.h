//
//  AnimaEngine.h
//  Anima
//
//  Created by Marco Zille on 08/11/14.
//
//

#ifndef __Anima__AnimaEngine__
#define __Anima__AnimaEngine__

#include "AnimaEngineCore.h"
#include "AnimaAllocator.h"
#include "AnimaStackAllocator.h"
#include "AnimaWindow_Base.h"

#define _ANIMA_ENGINE_CORE_REQUIRE_INIT()					\
    if (!Anima::AnimaEngine::IsInitialized())				\
		{													\
		/*_glfwInputError(GLFW_NOT_INITIALIZED, NULL);*/	\
		return;												\
		}

#define _ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(x)		\
    if (!Anima::AnimaEngine::IsInitialized())				\
		{													\
        /*_glfwInputError(GLFW_NOT_INITIALIZED, NULL);*/	\
        return x;											\
		}

#define _ANIMA_ENGINE_CORE_SWAP_POINTERS(x, y, type)	\
		{												\
        void* t;										\
        t = (void*)x;									\
        x = y;											\
        y = (type)t;									\
		}

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

class ANIMA_ENGINE_CORE_EXPORT AnimaEngine
{
public:
	AnimaEngine();
	~AnimaEngine();

public:
	bool Initialize();
	void Terminate();

	AnimaEngineWindow_Base* GetWindowListHead() { return _windowListHead;	}

	AnimaEngineWindow_Base* GetFocusedWindow()			{ return _focusedWindow;	}
	void SetFocusedWindow(AnimaEngineWindow_Base* w)	{ _focusedWindow = w;		}

	template<class T>
	T* CreateAnimaWindow(int width, int height, const char* title, AnimaEngineWindowmonitor* monitor = NULL, AnimaEngineWindow_Base* share = NULL);

	void DestroyAnimaWindow(AnimaEngineWindow_Base* window);

	void PollEvents(void);
	void WaitEvents(void);
	void PostEmptyEvent(void);

	double GetTime(void);
	void SetTime(double time);
	void MakeContextCurrent(AnimaEngineWindow_Base* window);
	AnimaEngineWindow_Base* GetCurrentContext(void);
	void SwapInterval(int interval);
	
	static void GetVersion(int* major, int* minor, int* rev);
	static const char* GetVersionString(void);

	static void DefaultWindowHints(void);
	static void SetWindowHint(int target, int hint);
	static void SetWindowHint(int target, bool hint);

	static AnimaEngineWindowmonitor* GetPrimaryMonitor(void);
	static void GetMonitorPos(AnimaEngineWindowmonitor* monitor, int* xpos, int* ypos);
	static void GetMonitorPhysicalSize(AnimaEngineWindowmonitor* monitor, int* width, int* height);
	static const char* GetMonitorName(AnimaEngineWindowmonitor* monitor);
	
	static AnimaEngineWindowmonitorfun SetMonitorCallback(AnimaEngineWindowmonitorfun cbfun);
	
	static const AnimaEngineWindowvidmode* GetVideoModes(AnimaEngineWindowmonitor* monitor, int* count);
	static const AnimaEngineWindowvidmode* GetVideoMode(AnimaEngineWindowmonitor* monitor);
	
	static void SetGamma(AnimaEngineWindowmonitor* monitor, float gamma);
	static const AnimaEngineWindowgammaramp* GetGammaRamp(AnimaEngineWindowmonitor* monitor);
	static void SetGammaRamp(AnimaEngineWindowmonitor* monitor, const AnimaEngineWindowgammaramp* ramp);
	
	static AnimaEngineWindowcursor* CreateCursor(const AnimaEngineWindowimage* image, int xhot, int yhot);
	static void DestroyCursor(AnimaEngineWindowcursor* cursor);

	static bool JoystickPresent(int joy);
	
	static const float* GetJoystickAxes(int joy, int* count);
	static const unsigned char* GetJoystickButtons(int joy, int* count);
	static const char* GetJoystickName(int joy);
	static bool ExtensionSupported(const char* extension);
	
public:
	static bool IsInitialized() { return _animaEngineInitialized; }

	static _GETD_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE;
	static _GETD_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_CONTEXT_STATE;
	static _GETD_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE;
	static _GETD_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_JOYSTICK_STATE;
	static _GETD_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE;

public:
	static _AnimaEngineWindowmonitor**	_monitors;				/*!< Lista dei monitor disponibili per il sistema */
	static int							_monitorCount;			/*!< Numero di monitor disponibili per il sistema */

	static _AnimaEngineWindowMonitorCallbacks _callbacks;

private:	
	AnimaEngineWindow_Base*	_windowListHead;					/*!< Primo elemento della lista delle finestre istanziate da un'istanza di AnimaEngine */
	AnimaEngineWindow_Base*	_focusedWindow;						/*!< Puntatore alla finestra attualmente attiva tra la lista di quelle istanziate da un'istanza di AnimaEngine */

	static _AnimaEngineWindowHints _windowHints;				/*!< Contenitore di dati per inizializzare un contesto di rendering su una finestra */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_WINDOW_STATE;	/*!< Contenitore con i puntatore alle funzioni e instanze delle librerie per la gestione delle finestre */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_CONTEXT_STATE;	/*!< Contenitore con i puntatore alle funzioni e instanze delle librerie per la gestione di OpenGL */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TIME_STATE;		/*!< Contenitore con dati per la gestione del timer */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_JOYSTICK_STATE;	/*!< Contenitore con dati per la gestione del joystick */
	static _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE;		/*!< Contenitore con dati per la gestione dei thread */
	static bool _animaEngineInitialized;						/*!< Flag per indicare se AnimaEngine � stato inizializzato con successo */
	static int	_animaEngineCount;								/*!< Contatore di istanze di AnimaEngine */
};

template<class T>
T* AnimaEngine::CreateAnimaWindow(int width, int height, const char* title, AnimaEngineWindowmonitor* monitor, AnimaEngineWindow_Base* share)
{
	_AnimaEngineWindowfbconfig fbconfig;
	_AnimaEngineWindowctxconfig ctxconfig;
	_AnimaEngineWindowwndconfig wndconfig;
	AnimaEngineWindow_Base* previous;

	T* window;

	_ANIMA_ENGINE_CORE_REQUIRE_INIT_OR_RETURN(NULL);

	if (width <= 0 || height <= 0)
	{
		//_glfwInputError(GLFW_INVALID_VALUE, "Invalid window size");
		return NULL;
	}

	// Set up desired framebuffer config
	fbconfig._redBits = _windowHints._redBits;
	fbconfig._greenBits = _windowHints._greenBits;
	fbconfig._blueBits = _windowHints._blueBits;
	fbconfig._alphaBits = _windowHints._alphaBits;
	fbconfig._depthBits = _windowHints._depthBits;
	fbconfig._stencilBits = _windowHints._stencilBits;
	fbconfig._accumRedBits = _windowHints._accumRedBits;
	fbconfig._accumGreenBits = _windowHints._accumGreenBits;
	fbconfig._accumBlueBits = _windowHints._accumBlueBits;
	fbconfig._accumAlphaBits = _windowHints._accumAlphaBits;
	fbconfig._auxBuffers = _windowHints._auxBuffers;
	fbconfig._stereo = _windowHints._stereo;
	fbconfig._samples = _windowHints._samples;
	fbconfig._sRGB = _windowHints._sRGB;
	fbconfig._doublebuffer = _windowHints._doublebuffer;

	// Set up desired window config
	wndconfig._width = width;
	wndconfig._height = height;
	wndconfig._title = title;
	wndconfig._resizable = _windowHints._resizable;
	wndconfig._visible = _windowHints._visible;
	wndconfig._decorated = _windowHints._decorated;
	wndconfig._focused = _windowHints._focused;
	wndconfig._autoIconify = _windowHints._autoIconify;
	wndconfig._floating = _windowHints._floating;
	wndconfig._monitor = (_AnimaEngineWindowmonitor*)monitor;

	// Set up desired context config
	ctxconfig._api = _windowHints._api;
	ctxconfig._major = _windowHints._major;
	ctxconfig._minor = _windowHints._minor;
	ctxconfig._forward = _windowHints._forward;
	ctxconfig._debug = _windowHints._debug;
	ctxconfig._profile = _windowHints._profile;
	ctxconfig._robustness = _windowHints._robustness;
	ctxconfig._release = _windowHints._release;
	ctxconfig._share = (AnimaEngineWindow_Base*)share;

	// Check the OpenGL bits of the window config
	if (!_AnimaEngineWindowIsValidContextConfig(&ctxconfig))
		return NULL;

	window = new T();
	window->InitCallbacks();
	window->SetEngine(this);
	window->SetNext(_windowListHead);
	_windowListHead = window;

	if (wndconfig._monitor)
	{
		wndconfig._resizable = true;
		wndconfig._visible = true;
		wndconfig._focused = true;

		// Set up desired video mode
		window->GetVideoMode()->_width = width;
		window->GetVideoMode()->_height = height;
		window->GetVideoMode()->_redBits = _windowHints._redBits;
		window->GetVideoMode()->_greenBits = _windowHints._greenBits;
		window->GetVideoMode()->_blueBits = _windowHints._blueBits;
		window->GetVideoMode()->_refreshRate = _windowHints._refreshRate;
	}

	// Transfer window hints that are persistent settings and not
	// just initial states
	window->SetMonitor(wndconfig._monitor);
	window->SetResizable(wndconfig._resizable);
	window->SetDecorated(wndconfig._decorated);
	window->SetAutoIconifiy(wndconfig._autoIconify);
	window->SetFloating(wndconfig._floating);
	window->SetCursorMode(ANIMA_ENGINE_CORE_CURSOR_NORMAL);

	// Save the currently current context so it can be restored later
	previous = _AnimaEngineWindowPlatformGetCurrentContext();

	// Open the actual window and create its context
	if (!_AnimaEngineWindowPlatformCreateWindow(window, &wndconfig, &ctxconfig, &fbconfig))
	{
		DestroyAnimaWindow((AnimaEngineWindow_Base*)window);
		_AnimaEngineWindowPlatformMakeContextCurrent(previous);
		return NULL;
	}

	_AnimaEngineWindowPlatformMakeContextCurrent(window);

	// Retrieve the actual (as opposed to requested) context attributes
	if (!_AnimaEngineWindowRefreshContextAttribs(&ctxconfig))
	{
		DestroyAnimaWindow((AnimaEngineWindow_Base*)window);
		_AnimaEngineWindowPlatformMakeContextCurrent(previous);
		return NULL;
	}

	// Verify the context against the requested parameters
	if (!_AnimaEngineWindowIsValidContext(&ctxconfig))
	{
		DestroyAnimaWindow((AnimaEngineWindow_Base*)window);
		_AnimaEngineWindowPlatformMakeContextCurrent(previous);
		return NULL;
	}

	// Clearing the front buffer to black to avoid garbage pixels left over
	// from previous uses of our bit of VRAM
	glClear(GL_COLOR_BUFFER_BIT);
	_AnimaEngineWindowPlatformSwapBuffers(window);

	// Restore the previously current context (or NULL)
	_AnimaEngineWindowPlatformMakeContextCurrent(previous);

	if (wndconfig._monitor)
	{
		int width, height;
		_AnimaEngineWindowPlatformGetWindowSize(window, &width, &height);

		window->SetCursorPosX(width / 2);
		window->SetCursorPosY(height / 2);

		_AnimaEngineWindowPlatformSetCursorPos(window, window->GetCursorPosX(), window->GetCursorPosY());
	}
	else
	{
		if (wndconfig._visible)
		{
			if (wndconfig._focused)
				_AnimaEngineWindowPlatformShowWindow(window);
			else
				_AnimaEngineWindowPlatformUnhideWindow(window);
		}
	}

	return (T*)window;
}

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif