#ifndef icubeapps_h__
#define icubeapps_h__


#ifdef _WIN32
#pragma once
#endif

#include "./base/cube_types.h"

namespace Cube {
#pragma pack(push, 1)

enum OwnershipType {

};

struct AppOwnership {
    int64_t timestamp;
    int64_t unused1;
    int32_t unused2;
    int32_t unused3;
};

class ICubeApps {
public:
    
    virtual bool CheckAppOwnership(AppOwnership* ownership) = 0;

    virtual CubeId_t GetAppOriginalOwner() = 0;

//     virtual uint64_t GetEarliestPurchaseUnixTime(CubeAppId_t appId) = 0;

};

#pragma pack(pop)

}

#endif // icubeapps_h__
