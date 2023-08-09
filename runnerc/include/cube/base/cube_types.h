#ifndef cube_types_h__
#define cube_types_h__

#ifdef _WIN32
#pragma once
#endif

//#define CUBE_CALLTYPE __cdecl
#define CUBE_CALLTYPE __stdcall

#if defined(__x86_64__) || defined(_WIN64)
#define X64_APP
#endif

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif // __cplusplus


#ifdef X64_APP
typedef int64_t intptr_t;
typedef uint64_t uintptr_t;
#else
typedef int32_t intptr_t;
typedef uint32_t uintptr_t;
#endif

#if defined( _WIN32 )

#elif defined(OSX)

#else

#endif

#ifndef NULL
#define NULL 0
#endif // NULL

namespace Cube {

class CubeId_t {
public:
    CubeId_t() : _id(kInvalidCubeId) {}
    CubeId_t(uint64_t id) { _id = id; }
    CubeId_t(const CubeId_t& rhs) { _id = rhs; }
    bool IsValid() const { return _id != kInvalidCubeId; }

    void Id(uint64_t id) { _id = id; }
    uint64_t Id() const { return _id; }
    
    CubeId_t operator = (uint64_t id) { _id = id; return _id; }
    bool operator == (const CubeId_t& rhs) const { return _id == rhs._id; }
    bool operator != (const CubeId_t& rhs) const { return !(*this == rhs); }
    operator uint64_t() const { return _id; }

    enum { kInvalidCubeId = 0 };
private:
    uint64_t _id;
};

// typedef uint64_t CubeId_t;

class CubeAppId_t {
public:
    CubeAppId_t() : _id(kInvalidCubeAppId) {}
    CubeAppId_t(uint64_t id) { _id = id; }
    CubeAppId_t(const CubeAppId_t& rhs) { _id = rhs; }
    bool IsValid() const { return _id != kInvalidCubeAppId; }

    void Id(uint64_t id) { _id = id; }
    uint64_t Id() const { return _id; }

    CubeId_t operator = (uint64_t id) { _id = id; return _id; }
    bool operator == (const CubeAppId_t& rhs) const { return _id == rhs._id; }
    bool operator != (const CubeAppId_t& rhs) const { return !(*this == rhs); }
    operator uint64_t() const { return _id; }

    enum { kInvalidCubeAppId = 0 };
private:
    uint64_t _id;
};

typedef uint64_t CubeLeaderboardId;

typedef uint64_t CubeLeaderboardEntries;

typedef uint64_t CubeProductId_t;

enum CubeNotifyType {
    kInvalidType                           = 0,
    kCubeSystemNotify                      = 1,
    kSearchLobbyResponse                   = 400,
    kCreateLobbyResponse                   = 401,
    kJoinLoobyResponse                     = 402,
    kLeaveLoobyResponse                    = 403,
    kInviteUserToLoobyResponse             = 404,
    kKickUserResponse                      = 405,
    kGetLoobyMembersResponse               = 406,
    kReloadLoobyDataResponse               = 407,
    kGetFriendListResponse                 = 408,
    kSetAchievementResponse                = 409,
    kClearAchievementResponse              = 410,
    kRequestAllPurchasableProductsResponse = 601,
    kCreateOrderResponse                   = 602,
    kPurchaseProductsResponse              = 603,
    kFinishOrderResponse                   = 604,
    kGetOrderInfoResponse                  = 605,
};

enum CubeResult {
    kUnknownError               = 0,
    kSucceed                    = 1,
    kFailed                     = 2,
    kBufferTooSmall             = 3,
    kParameterLengthTooLong     = 4,
    kAppNameNotFound            = 5,

    // user
    kInvalidToken               = 101,
    kInvalidCubeId              = 102,

    // apps
    kInvalidAppId               = 201,
    
    // achievement 

    // matchmaking

    // products
    kInvalidProductId           = 501,

    // inapppurchase
    kInventoryShortage          = 601,
    kUnavailableInCurrentRegion = 602,    
    kInvalidOrderId             = 603,
    kQuantityToMore             = 604,

    //
    kAntiAddictionTip           = 700,
    kAntiAddictionSystemShutdown = 701,
};

// enum CubeShowWindowFlag {
//     kCubeClose = 0,
//     kCubeShow = 1,
// };
}

#include "cubestring.hpp"
#endif // cube_types_h__