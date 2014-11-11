#include "AnimaWindow_Base.h"
#include <stdlib.h>

static float normalizeAxis(DWORD pos, DWORD min, DWORD max)
{
	float fpos = (float)pos;
	float fmin = (float)min;
	float fmax = (float)max;

	return (2.f * (fpos - fmin) / (fmax - fmin)) - 1.f;
}

void _AnimaEngineWindowInitJoysticks(void)
{
}

void _AnimaEngineWindowTerminateJoysticks(void)
{
	//for (int i = 0; i < ANIMA_ENGINE_CORE_JOYSTICK_LAST; i++)
	//	free(_glfw.winmm_js[i].name);
}

bool _AnimaEngineWindowPlatformJoystickPresent(int joy)
{
	//JOYINFO ji;

	//if (_AnimaEngineWindow_joyGetPos(joy, &ji) != JOYERR_NOERROR)
	//	return GL_FALSE;

	//return GL_TRUE;
	return true;
}

const float* _AnimaEngineWindowPlatformGetJoystickAxes(int joy, int* count)
{
	//JOYCAPS jc;
	//JOYINFOEX ji;
	//float* axes = _glfw.winmm_js[joy].axes;

	//if (_AnimaEngineWindow_joyGetDevCaps(joy, &jc, sizeof(JOYCAPS)) != JOYERR_NOERROR)
	//	return NULL;

	//ji.dwSize = sizeof(JOYINFOEX);
	//ji.dwFlags = JOY_RETURNX | JOY_RETURNY | JOY_RETURNZ |
	//	JOY_RETURNR | JOY_RETURNU | JOY_RETURNV;
	//if (_AnimaEngineWindow_joyGetPosEx(joy, &ji) != JOYERR_NOERROR)
	//	return NULL;

	//axes[(*count)++] = normalizeAxis(ji.dwXpos, jc.wXmin, jc.wXmax);
	//axes[(*count)++] = normalizeAxis(ji.dwYpos, jc.wYmin, jc.wYmax);

	//if (jc.wCaps & JOYCAPS_HASZ)
	//	axes[(*count)++] = normalizeAxis(ji.dwZpos, jc.wZmin, jc.wZmax);

	//if (jc.wCaps & JOYCAPS_HASR)
	//	axes[(*count)++] = normalizeAxis(ji.dwRpos, jc.wRmin, jc.wRmax);

	//if (jc.wCaps & JOYCAPS_HASU)
	//	axes[(*count)++] = normalizeAxis(ji.dwUpos, jc.wUmin, jc.wUmax);

	//if (jc.wCaps & JOYCAPS_HASV)
	//	axes[(*count)++] = normalizeAxis(ji.dwVpos, jc.wVmin, jc.wVmax);

	//return axes;
	return NULL;
}

const unsigned char* _AnimaEngineWindowPlatformGetJoystickButtons(int joy, int* count)
{
	//JOYCAPS jc;
	//JOYINFOEX ji;
	//unsigned char* buttons = _glfw.winmm_js[joy].buttons;

	//if (_AnimaEngineWindow_joyGetDevCaps(joy, &jc, sizeof(JOYCAPS)) != JOYERR_NOERROR)
	//	return NULL;

	//ji.dwSize = sizeof(JOYINFOEX);
	//ji.dwFlags = JOY_RETURNBUTTONS | JOY_RETURNPOV;
	//if (_AnimaEngineWindow_joyGetPosEx(joy, &ji) != JOYERR_NOERROR)
	//	return NULL;

	//while (*count < (int)jc.wNumButtons)
	//{
	//	buttons[*count] = (unsigned char)
	//		(ji.dwButtons & (1UL << *count) ? ANIMA_ENGINE_CORE_PRESS : ANIMA_ENGINE_CORE_RELEASE);
	//	(*count)++;
	//}

	//// Virtual buttons - Inject data from hats
	//// Each hat is exposed as 4 buttons which exposes 8 directions with
	//// concurrent button presses
	//// NOTE: this API exposes only one hat

	//if ((jc.wCaps & JOYCAPS_HASPOV) && (jc.wCaps & JOYCAPS_POV4DIR))
	//{
	//	int i, value = ji.dwPOV / 100 / 45;

	//	// Bit fields of button presses for each direction, including nil
	//	const int directions[9] = { 1, 3, 2, 6, 4, 12, 8, 9, 0 };

	//	if (value < 0 || value > 8)
	//		value = 8;

	//	for (i = 0; i < 4; i++)
	//	{
	//		if (directions[value] & (1 << i))
	//			buttons[(*count)++] = ANIMA_ENGINE_CORE_PRESS;
	//		else
	//			buttons[(*count)++] = ANIMA_ENGINE_CORE_RELEASE;
	//	}
	//}

	//return buttons;
	return NULL;
}

const char* _AnimaEngineWindowPlatformGetJoystickName(int joy)
{
	JOYCAPS jc;

	//if (_AnimaEngineWindow_joyGetDevCaps(joy, &jc, sizeof(JOYCAPS)) != JOYERR_NOERROR)
	//	return NULL;

	//free(_glfw.winmm_js[joy].name);
	//_glfw.winmm_js[joy].name = _AnimaEngineWindowCreateUTF8FromWideString(jc.szPname);

	//return _glfw.winmm_js[joy].name;
	return NULL;
}