#pragma once
#include<string>

class eSettingsManager {
public:
	void Init();

public:
	// Debug
	bool bEnableConsoleWindow;
	bool bPauseOnStart;
	int	iVerbose;
	bool bDebug;

	// Toggles
	bool bDisableAntiCheatEngine;
	bool bDisableAntiCVD;
	bool bModLoader;
	bool bEnableCheats;
	bool bEnableIntroSwap;
	bool bPatchUnlocker;
	bool bPatchCurl;

	// Addresses

	// Patterns
	std::string pGameVerFull;
	std::string pGameVer;
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
	// Unlocker
	std::string pGetItemName;
	std::string pGetItemPriceString;
	std::string pGetItemPrice;
	std::string pKollectableUnlocker;
	std::string pVR2Function;
	std::string pVR2s;
	std::string pVR2x;

	std::string hkCheats;
	int iVKCheats;

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
	std::string szCurlSetOpt;
	std::string szCurlPerform;

	//Private Server
	std::string szMITMUrl;
	bool bMITM;

};

extern eSettingsManager* SettingsMgr;