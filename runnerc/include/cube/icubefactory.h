#ifndef icubefactory_h__
#define icubefactory_h__

#ifdef _WIN32
#pragma once
#endif


namespace Cube {
#pragma pack(push, 1)

class ICubeUtils;
class ICubeAchievement;
class ICubeCloudStorage;
class ICubeDLCs;
class ICubeFriends;
class ICubeLeaderboard;
class ICubeMatchmaking;
class ICubePersona;
class ICubeApps;
class ICubeInAppPurchase;
class ICubeWindow;

class ICubeFactory {
public:
    virtual ICubeUtils* Utils() = 0;

    virtual ICubeAchievement* Achievement() = 0;

    virtual ICubeApps* Apps() = 0;

    virtual ICubeCloudStorage* CloudStorage() = 0;

    virtual ICubeDLCs* DLCs() = 0;

    virtual ICubeFriends* Friends() = 0;

    virtual ICubeLeaderboard* Leaderboard() = 0;

    virtual ICubeMatchmaking* Matchmaking() = 0;

    virtual ICubePersona* Persona() = 0;

    virtual ICubeInAppPurchase* InAppPurchase() = 0;

    virtual ICubeWindow* Window() = 0;
};

#pragma pack(pop)
}


#endif // icubefactory_h__
