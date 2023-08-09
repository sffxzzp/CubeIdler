#ifndef icubenotifyhandler_h__
#define icubenotifyhandler_h__

#ifdef _WIN32
#pragma once
#endif

#include "./base/cube_types.h"
#include "./base/cubestring.hpp"

namespace Cube {
#pragma pack(push, 1)


class ICubeNotify {
public:
    ICubeNotify() : result(kUnknownError) {}
    virtual ~ICubeNotify() {}


    CubeNotifyType GetNotifyId() const { return notifyId; }
    void SetNotifyId(CubeNotifyType rhs) { notifyId = rhs; }

    CubeAppId_t GetAppId() const { return appId; }
    void SetAppId(CubeAppId_t rhs) { appId = rhs; }

    CubeResult GetResult() const { return result; }
    void SetResult(CubeResult rhs) { result = rhs; }

    CubeString GetResultData() const { return resultData; }
    void SetResultData(const CubeString& rhs) { resultData = rhs; }

    CubeString GetExtraDataData() const { return extraData; }
    void SetExtraDataData(const CubeString& rhs) { extraData = rhs; }

    CubeString GetUserData() const { return userData; }
    void SetUserData(const CubeString& rhs) { userData = rhs; }

    CubeNotifyType notifyId;
    CubeAppId_t appId;
    CubeResult result;
    CubeString resultData;
    CubeString extraData;
    CubeString userData;
};


class ICubeNotifyHandler {
public:    
    virtual void OnNotify(ICubeNotify* param) = 0;
};

#pragma pack(pop)
}

#endif // icubenotifyhandler_h__
