#ifndef iachievement_h__
#define iachievement_h__

#ifdef _WIN32
#pragma once
#endif

#include "./base/cube_types.h"


namespace Cube {
#pragma pack(push, 1)

class ICubeAchievement {
public:

    virtual bool AsyncSetAchievement(const char* apiName) = 0;

    virtual bool AsyncSetAchievementProgress(const char* apiName, int64_t currentValue, int64_t unlockValue) = 0;

    virtual bool AsyncClearAchievement(const char* apiName) = 0;

    virtual bool GetAchievement(const char* apiName, bool* isUnlock, uint64_t* unlockTime, int64_t* currentValue) = 0;

    virtual bool GetUserAchievement(CubeId_t userCubeId, const char* apiName, bool* isUnlock) = 0;

//     virtual const char* GetAchievementLocalizedDisplayAttribute(const char* name, const char* field) = 0;
// 
//     virtual uint32_t GetAchievementCount() = 0;
// 
//     virtual const char* GetAchievementInfo(uint32_t achievementIndex) = 0;
};



#pragma pack(pop)
}
#endif // iachievement_h__
