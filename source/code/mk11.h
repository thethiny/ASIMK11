#pragma once
#include "mk11utils.h"
#include "mk11menu.h"

enum  PLAYER_NUM
{
	INVALID_PLAYER_NUM = 0xFFFFFFFF,
	PLAYER1 = 0x0,
	PLAYER2,
	PLAYER3,
	PLAYER4,
	MAX_PLAYERS,
	CPU_PLAYER = 0x5,
	NOT_CPU_PLAYER = 0x6,
	BACKGROUND_PLAYER = 0x7,
};

namespace MK11 {

	struct CamStruct {
		float fX = 0, fY = 0, fZ = 0; // Change to pointers
		float fPOV = 0;
		int32_t iYaw = 0, iPitch = 0, iRot = 0;
		bool bLogCam = false; //Make it a flag to control prints
		bool bCamActive = false;
		bool bCamEnabled = false;
		bool bTimestopActive = false;
		bool bEnableXYZ = false;
		bool bEnablePOV = false;
		bool bEnablePiYaRot = false;
		uint32_t uCamSpeed = 5;
	};

}

extern MK11::CamStruct sCamStruct;


namespace MK11Hooks {

}


