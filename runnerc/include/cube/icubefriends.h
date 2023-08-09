#ifndef icubefriends_h__
#define icubefriends_h__

#ifdef _WIN32
#pragma once
#endif

#include "./base/cube_types.h"

namespace Cube {
#pragma pack(push, 1)

enum FriendRelationship {
    FriendRelationshipNone = 0,
    FriendRelationshipFriend = 1,
    FriendRelationshipBlocked = 2,
    FriendRelationshipStranger = 3,
    FriendRelationshipRequestRecipient = 4,
    FriendRelationshipRequestInitiator = 5,
    FriendRelationshipMax = 0xFFFF,
};

enum ChatMessageType {
    ChatInvalidMessage = 0,
    ChatNormalMessage = 1,
    ChatHistoricalChatMessage = 2,
    ChatTypingMessage = 3,
    ChatEnteredMessage = 4,
    ChatLeaveSessionMessage = 5,
    ChatDisconnectedMessage = 6,
    ChatInviteAppMessage = 7,
    ChatWasKickedMessage = 8,
    ChatTypeWasBannedMessage = 9,
};

struct FriendAppInfo_t {
    CubeAppId_t appId;
    uint32_t appIp;
    uint16_t appPort;
    uint16_t queryPort;
    CubeId_t lobbyId;
};

class ICubeFriends {
public:
    virtual int32_t GetFriendCount() = 0;

    virtual CubeId_t GetFriendByIndex(int32_t index) = 0;

    virtual FriendRelationship GetFriendRelationship(CubeId_t friendCubeId) = 0;

    virtual bool InviteUserToApp(CubeId_t friendCubeId, const char* connectString) = 0;

    virtual bool SetListenForFriendsMessages(bool isInterceptEnabled) = 0;
    virtual bool ReplyToFriendMessage(CubeId_t friendCubeId, const char* messageToSend) = 0;
    virtual int32_t GetFriendMessage(CubeId_t friendCubeId, uint64_t messageId, void* dataBuff, int32_t dataBuffSize, ChatMessageType* messageType) = 0;
};
#pragma pack(pop)
}

#endif // icubefriends_h__
