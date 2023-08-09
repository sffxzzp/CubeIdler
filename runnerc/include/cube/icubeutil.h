#ifndef icubeutil_h__
#define icubeutil_h__

#ifdef _WIN32
#pragma once
#endif

#include "./base/cube_types.h"

namespace Cube {
#pragma pack(push, 1)

class ICubeUtils {
public:
    virtual int64_t GetTimeCountSinceAppLaunch() = 0;

    virtual int64_t GetServerUnixTime() = 0;

//     virtual void GetNumberOfCurrentPlayers() = 0;

};

#pragma pack(pop)
}


#endif // icubeutil_h__
