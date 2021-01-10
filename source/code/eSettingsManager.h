#pragma once
#include<string>

// as usual, pluginmh/mugenhook

class eSettingsManager {
public:
	void Init();
	int StringToVK(std::string);

	// Booleans
	bool bEnableConsoleWindow;
	bool bPauseOnStart;
	bool bDisableAntiCheatEngine;
	bool bDisableAntiCVD1;
	bool bDisableAntiCVD2;
	bool bPatchUnlocker;
	bool bModLoader;
	bool bEnableCameraMod;
	bool bEnableCheats;

	// Addresses
	std::string iCVD1;
	std::string iCVD2;
	std::string iModLoader;
	std::string iCamOffset;

	// Patterns
	std::string pCam;
	// Cheats
	std::string pMercyAnyTime;
	std::string pNoGroundReq;
	std::string pNoBrutalityReq;
	std::string pBrutalityAlwaysCorrect;
	std::string pMeteorAlwaysSpawns;

	std::string hotkey_cheats;
	int iVKCheats;

	/// Camera Mod Section
	std::string hotkey_toggle_camera;
	std::string hotkey_xp;
	std::string hotkey_xm;
	std::string hotkey_yp;
	std::string hotkey_ym;
	std::string hotkey_zp;
	std::string hotkey_zm;
	std::string hotkey_fovp;
	std::string hotkey_fovm;
	std::string hotkey_rotp;
	std::string hotkey_rotm;
	std::string hotkey_pitchp;
	std::string hotkey_pitchm;
	std::string hotkey_yawp;
	std::string hotkey_yawm;
	std::string hotkey_timestop;
	int iVKCamToggle;
	int iVKxp;
	int iVKxm;
	int iVKyp;
	int iVKym;
	int iVKzp;
	int iVKzm;
	int iVKfovp;
	int iVKfovm;
	int iVKrotp;
	int iVKrotm;
	int iVKpitchp;
	int iVKpitchm;
	int iVKyawp;
	int iVKyawm;
	int iVKtimestop;
	float fSpeed;
	float fCamHold;
	/// End of Cam Mod Section

	// Menu Section
	std::string hotkey_toggle_menu;
	int iVKMenuToggle;

};

extern eSettingsManager* SettingsMgr;