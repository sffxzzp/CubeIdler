#ifndef icubepersona_h__
#define icubepersona_h__


#ifdef _WIN32
#pragma once
#endif

#include "./base/cube_types.h"

namespace Cube {
#pragma pack(push, 1)

	enum PersonaState {
		UserStateOffline = 0,
		UserStateOnline = 1,
		UserStateBusy = 2,
		UserStateAway = 3,
		UserStateMax = 0xFF,
	};

	enum AvatarQuality {
		MiniAvatar,     //32X32
		SmallAvatar,    //64X64
		MediumAvatar,   //128X128
		LargeAvatar,    //256X256
	};


	class ICubePersona {
	public:
		//ªÒ»°∑ΩøÈID
		virtual CubeId_t GetMyCubeId() = 0;

		virtual const char* GetMyCubeToken() = 0;

		virtual const char* GetMyAvatarFile(AvatarQuality quality) = 0;

		virtual const char* GetPersonaNickname(CubeId_t userCubeId) = 0;

		virtual const char* GetPersonaAvatarFile(CubeId_t userCubeId, AvatarQuality quality) = 0;

		virtual PersonaState GetPersonaState(CubeId_t userCubeId) = 0;
	};

#pragma pack(pop)
}

#endif // icubepersona_h__
