#ifndef icube_matchmaking_h__
#define icube_matchmaking_h__

#ifdef _WIN32
#pragma once
#endif

#include "./base/cube_types.h"

namespace Cube {

// �����������Ȩ��
enum LobbyAuthorityType {
    PrivateLoobyType = 0,		// ���ⲻ�ɼ�,����ͨ���������
    PublicLoobyType = 1,	    // ��ȫԱ�ɼ�,�κ��˾��ɼ���
    FriendsOnlyLoobyType = 2,	// �����ѿɼ�������
};

// �����������ʱʹ�õıȽ������
enum LobbyComparisonType {
    LobbyComparisonNotEqual             = 0,
    LobbyComparisonLessThan             = 1,
    LobbyComparisonEqualToOrLessThan    = 2,
    LobbyComparisonEqualTo              = 3,
    LobbyComparisonEqualToOrGreaterThan = 4,
    LobbyComparisonGraterThan           = 5,
};

// ������������ľ���ɸѡ��
enum LobbyDistanceFilterType {
    LobbyDistanceFilterDefault,		// �����ظ��������Ͻ���
    LobbyDistanceFilterSameRegion,  // ���ر�����ȫ������
    LobbyDistanceFilterNear,		// ���ر��������������
    LobbyDistanceFilterFar,			// ����λ�ø�Զ�Ĵ���,�����ڶ��ӳٲ����е���Ϸ
    LobbyDistanceFilterWorld,	    // ����ȫ������,�������κ�ɸѡ
};

//////////////////////////////////////////////////////////////////////////
// ˵��:���ڲ�����Ϸ����,���ṩƥ�书��
//
//////////////////////////////////////////////////////////////////////////
class ICubeMatchmaking {
public:

    // ����������ȡ�����б�
    // �ú���Ϊ�첽����
    // ���ú�,������ɸѡ��
    virtual CubeResult SearchLobby() = 0;

    // ����ַ���ɸѡ��
    virtual void AddSearchLobbyStringFilter(const char* fieldToSearch, const char* valueToMatch, LobbyComparisonType comparisonType) = 0;

    // �������ɸѡ��
    virtual void AddSearchLobbyIntFilter(const char* fieldToSearch, int32_t valueToMatch, LobbyComparisonType comparisonType) = 0;

    // ������η�Χ(������)ɸѡ��
    virtual void AddSearchLobbyIntRangeFilter(const char* fieldToSearch, int32_t minValueToMatch, int32_t maxValueToMatch) = 0;

    // ���þ���ɸѡ��,����ip��λ
    virtual void SetSearchLobbyDistanceFilter(LobbyDistanceFilterType distanceFilter) = 0;

    // �������������������, ��ֵ��С�������ٶȳ�����, Ĭ��Ϊ10, ���Ϊ1
    virtual void SetSearchLobbyResultCount(int32_t maxResults) = 0;

    // ��������
    virtual CubeResult CreateLobby(const char* name, LobbyAuthorityType lobbyAuthority, int32_t maxMembers) = 0;

    virtual CubeResult JoinLooby(CubeId_t lobbyId) = 0;

    virtual void LeaveLooby(CubeId_t lobbyId) = 0;

    // ����
    virtual bool InviteUserToLooby(CubeId_t lobbyId, CubeId_t inviteeCubeId) = 0;

    virtual void KickUser(CubeId_t lobbyId, CubeId_t kickUserCubeId) = 0;

    // ���ش�����Ա������
    virtual int32_t GetLoobyMemberCount(CubeId_t lobbyId) = 0;

    virtual CubeId_t GetLoobyMemberByIndex(CubeId_t lobbyId, int32_t memberIndex) = 0;

    // ��ȡ�Զ����������, ͬ������. ָ���ֶβ�����ʱ, ����false
    virtual bool GetLoobyData(CubeId_t lobbyId, const char* field, char* valueBuffer, int32_t valueBufferSize) = 0;

    // ���´ӷ�������ȡ��������
    virtual bool ReloadLoobyData(CubeId_t lobbyId) = 0;

    // ���ô����Զ�������, ���ú���ȫ����Ա�㲥��ֵ, ������Ϸ����LobbyDataUpdate֪ͨ
    virtual bool SetLoobyData(CubeId_t lobbyId, const char* field, const char* value) = 0;

};

}

#endif // icube_matchmaking_h__
