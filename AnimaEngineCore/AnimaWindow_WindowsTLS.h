#ifndef _AnimaWindow_WindowsTLS_h
#define _AnimaWindow_WindowsTLS_h

#include "AnimaEngineCore.h"

#define _ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE _AnimaEngineWindowtlsWin32 win32_tls

#define _INIT_STATIC_ANIMA_ENGINE_CORE_PLATFORM_LIBRARY_TLS_STATE(class) _AnimaEngineWindowtlsWin32 class::win32_tls;

BEGIN_ANIMA_ENGINE_CORE_NAMESPACE

typedef struct _AnimaEngineWindowtlsWin32
{
	bool	_allocated;
	DWORD	_context;

} _AnimaEngineWindowtlsWin32;

extern "C" ANIMA_ENGINE_CORE_EXPORT bool _AnimaEngineWindowInitTLS(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowTerminateTLS(void);
extern "C" ANIMA_ENGINE_CORE_EXPORT void _AnimaEngineWindowSetCurrentContext(_AnimaEngineWindowwindow* context);

END_ANIMA_ENGINE_CORE_NAMESPACE

#endif