#ifndef icubewindow_h__
#define icubewindow_h__


#ifdef _WIN32
#pragma once
#endif

#include "./base/cube_types.h"
#include "icubewindow_define.h"

namespace Cube {
#pragma pack(push, 1)

class ICubeWindow {
public:
    // @desc To enable or disable the notifications. 
    // @window_type Overlay type
    // @param enable If you would like to use your own notifications rather than the WeGame
    // platform's, please set it to false
    // @return kSuccess on success
    virtual CubeResult EnableCubeWindow(CubeWindowType windowType, bool isEnable) = 0;
};

#pragma pack(pop)
}


#endif // icubewindow_h__
