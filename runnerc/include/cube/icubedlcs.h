#ifndef icubedlc_h__
#define icubedlc_h__

#ifdef _WIN32
#pragma once
#endif

#include "./base/cube_types.h"

namespace Cube {
#pragma pack(push, 1)


class ICubeDLCs {
public:
    virtual bool IsDlcInstalled(CubeAppId_t dlcId) = 0;

    virtual int32_t GetDlcCount() = 0;

    virtual bool GetDlcDataByIndex(int32_t dlcIndex, CubeAppId_t* dlcId, bool* isOwn, bool* isInstalled, char* name, int nameBufferSize) = 0;

//     virtual void InstallDlc(CubeAppId_t dlcId) = 0;
// 
//     virtual void UninstallDlc(CubeAppId_t dlcId) = 0;
};

#pragma pack(pop)

}
#endif // icubedlc_h__