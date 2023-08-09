#ifndef icube_matchmaking_h__
#define icube_matchmaking_h__

#ifdef _WIN32
#pragma once
#endif

#include "./base/cube_types.h"

namespace Cube {

// 定义大厅访问权限
enum LobbyAuthorityType {
    PrivateLoobyType = 0,		// 对外不可见,仅能通过邀请进入
    PublicLoobyType = 1,	    // 对全员可见,任何人均可加入
    FriendsOnlyLoobyType = 2,	// 仅好友可见及进入
};

// 定义大厅搜索时使用的比较运算符
enum LobbyComparisonType {
    LobbyComparisonNotEqual             = 0,
    LobbyComparisonLessThan             = 1,
    LobbyComparisonEqualToOrLessThan    = 2,
    LobbyComparisonEqualTo              = 3,
    LobbyComparisonEqualToOrGreaterThan = 4,
    LobbyComparisonGraterThan           = 5,
};

// 定义大厅搜索的距离筛选器
enum LobbyDistanceFilterType {
    LobbyDistanceFilterDefault,		// 仅返回附近或距离较近的
    LobbyDistanceFilterSameRegion,  // 返回本地区全部大厅
    LobbyDistanceFilterNear,		// 返回本地区或最近地区
    LobbyDistanceFilterFar,			// 返回位置更远的大厅,可用于对延迟不敏感的游戏
    LobbyDistanceFilterWorld,	    // 返回全部大厅,不进行任何筛选
};

//////////////////////////////////////////////////////////////////////////
// 说明:用于操作游戏大厅,并提供匹配功能
//
//////////////////////////////////////////////////////////////////////////
class ICubeMatchmaking {
public:

    // 根据条件获取大厅列表
    // 该函数为异步函数
    // 调用后,将重置筛选器
    virtual CubeResult SearchLobby() = 0;

    // 添加字符串筛选器
    virtual void AddSearchLobbyStringFilter(const char* fieldToSearch, const char* valueToMatch, LobbyComparisonType comparisonType) = 0;

    // 添加整形筛选器
    virtual void AddSearchLobbyIntFilter(const char* fieldToSearch, int32_t valueToMatch, LobbyComparisonType comparisonType) = 0;

    // 添加整形范围(闭区间)筛选器
    virtual void AddSearchLobbyIntRangeFilter(const char* fieldToSearch, int32_t minValueToMatch, int32_t maxValueToMatch) = 0;

    // 设置距离筛选器,基于ip定位
    virtual void SetSearchLobbyDistanceFilter(LobbyDistanceFilterType distanceFilter) = 0;

    // 设置搜索结果的期望数, 该值大小与搜索速度成正比, 默认为10, 最低为1
    virtual void SetSearchLobbyResultCount(int32_t maxResults) = 0;

    // 创建大厅
    virtual CubeResult CreateLobby(const char* name, LobbyAuthorityType lobbyAuthority, int32_t maxMembers) = 0;

    virtual CubeResult JoinLooby(CubeId_t lobbyId) = 0;

    virtual void LeaveLooby(CubeId_t lobbyId) = 0;

    // 邀请
    virtual bool InviteUserToLooby(CubeId_t lobbyId, CubeId_t inviteeCubeId) = 0;

    virtual void KickUser(CubeId_t lobbyId, CubeId_t kickUserCubeId) = 0;

    // 返回大厅成员的数量
    virtual int32_t GetLoobyMemberCount(CubeId_t lobbyId) = 0;

    virtual CubeId_t GetLoobyMemberByIndex(CubeId_t lobbyId, int32_t memberIndex) = 0;

    // 获取自定义大厅数据, 同步函数. 指定字段不存在时, 返回false
    virtual bool GetLoobyData(CubeId_t lobbyId, const char* field, char* valueBuffer, int32_t valueBufferSize) = 0;

    // 重新从服务器获取大厅数据
    virtual bool ReloadLoobyData(CubeId_t lobbyId) = 0;

    // 设置大厅自定义数据, 设置后将向全部成员广播新值, 并向游戏推送LobbyDataUpdate通知
    virtual bool SetLoobyData(CubeId_t lobbyId, const char* field, const char* value) = 0;

};

}

#endif // icube_matchmaking_h__
