#ifndef icubewindow_define_h__
#define icubewindow_define_h__



#ifdef _WIN32
#pragma once
#endif
 
#include "./base/cube_types.h"
#include "icubenotifyhandler.h"

namespace Cube {
    enum CubeWindowType {
        kCubeWindowUnknown = 0,
        kCubeWindowAntiAddiction = 10,
    };

    class CubeWindowNotify : public ICubeNotify {
    public:
        CubeWindowNotify() {
            result = kFailed;
            windowType = kCubeWindowAntiAddiction;
        }
        
        CubeString content;
        CubeWindowType windowType;
        bool isShowWindow;
    };
}

#endif // icubewindow_define_h__
