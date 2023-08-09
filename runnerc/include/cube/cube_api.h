#ifndef cube_api_h__
#define cube_api_h__

#ifdef _WIN32
#pragma once
#endif

#include "./base/cube_types.h"
#include "icubefactory.h"
// #include "icubematchmaking.h"
#include "icubenotifyhandler.h"

#ifdef CUBE_API_EXPORTS
#define CUBE_API extern "C" __declspec(dllexport)
#else
#define CUBE_API extern "C" __declspec(dllimport)
#endif

namespace Cube {

CUBE_API bool CUBE_CALLTYPE CubeInitialize();

CUBE_API void CUBE_CALLTYPE CubeShutdown();

CUBE_API bool CUBE_CALLTYPE IsNeedRestartApp(uint64_t appId);

CUBE_API void CUBE_CALLTYPE RegisterNotify(CubeNotifyType notifyId, ICubeNotifyHandler* notifyHandler);

CUBE_API void CUBE_CALLTYPE UnRegisterNotify(CubeNotifyType notifyId);

CUBE_API bool CUBE_CALLTYPE IsCubeRunning();

CUBE_API const char* CUBE_CALLTYPE GetCubeInstallPath();

CUBE_API ICubeFactory* CUBE_CALLTYPE GetCubeFactory();

CUBE_API const char* CUBE_CALLTYPE GetApiVersion();

}


#endif // cube_api_h__