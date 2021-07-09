#pragma once
#include<string>

class eSettingsManager {
public:
	void Init();

public:
	// Booleans
	bool bEnableConsoleWindow;
	bool bPauseOnStart;
	bool bDisableAntiCheatEngine;
	bool bDisableAntiCVD;
	bool bPatchUnlocker;
	bool bModLoader;
	bool bEnableCameraMod;
	bool bEnableCheats;
	bool bEnableIntroSwap;
	bool bEnableTimeStop;

	// Addresses

	// Patterns
	std::string pGameVerFull;
	std::string pGameVer;
	std::string pCam;
	std::string pTimestop;
	std::string pIntroSwap;
	std::string pCVD1;
	std::string pCVD2;
	// Cheats
	std::string pMercyAnyTime;
	std::string pNoGroundReq;
	std::string pNoBrutalityReq;
	std::string pBrutalityAlwaysCorrect;
	std::string pMeteorAlwaysSpawns;
	std::string pFatalityAlwaysAvailable;
	std::string pDizzyAlwaysAvailable;
	std::string pComboInFatality;
	std::string pBlockingDisabled;
	std::string pFatalBlowAtMaxHealth;

	std::string hkCheats;
	int iVKCheats;

	std::string hkTimestop;
	int iVKtimestop;

	// Menu Section
	std::string hkMenu;
	std::string hkInfo;
	int iVKMenuToggle;
	int iVKMenuInfo;

	//Other
	int iLogSize;
	bool FORCE_CHECK_VER = true;
	std::string szGameVer;
	std::string szModLoader;
	std::string szAntiCheatEngine;

};

extern eSettingsManager* SettingsMgr;