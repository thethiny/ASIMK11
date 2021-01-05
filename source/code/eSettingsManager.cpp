#include "eSettingsManager.h"
#include "..\IniReader.h"
#include <Windows.h>

eSettingsManager* SettingsMgr = new eSettingsManager();

void eSettingsManager::Init()
{
	CIniReader ini("");
	bEnableConsoleWindow = ini.ReadBoolean("Settings", "bEnableConsoleWindow", false);
	bPauseOnStart = ini.ReadBoolean("Settings", "bPauseOnStart", false);
	bDisableAntiCheatEngine = ini.ReadBoolean("AntiCheat", "bDisableAntiCheatEngine", true);
	bDisableAntiCVD1 = ini.ReadBoolean("AntiCheat", "bDisableAntiCVD1", false);
	bDisableAntiCVD2 = ini.ReadBoolean("AntiCheat", "bDisableAntiCVD2", false);
	bPatchUnlocker = ini.ReadBoolean("Settings", "bPatchUnlocker", false);
	bModLoader = ini.ReadBoolean("Settings", "bModLoader", false);
	bEnableCameraMod = ini.ReadBoolean("Settings", "bEnableCameraMod", false);
	bEnableCheats = ini.ReadBoolean("Settings", "bEnableCheats", false);
	iCVD1 = ini.ReadString("Addresses", "iCVD1", "");
	iCVD2 = ini.ReadString("Addresses", "iCVD2", "");
	iModLoader = ini.ReadString("Addresses", "iModLoader", "");
	pCam = ini.ReadString("Patterns", "pCam", "0F 11 44 24 30 F2 0F 11 4C 24 40 8B");
	iCamOffset = ini.ReadString("Addresses", "iCamOffset", "0x6BC");

	// Cheats
	pMercyAnyTime = ini.ReadString("Patterns.Cheats", "pMercyAnyTime", "");
	pNoGroundReq = ini.ReadString("Patterns.Cheats", "pNoGroundReq", "");
	pNoBrutalityReq = ini.ReadString("Patterns.Cheats", "pNoBrutalityReq", "");
	pBrutalityAlwaysCorrect = ini.ReadString("Patterns.Cheats", "pBrutalityAlwaysCorrect", "");
	pMeteorAlwaysSpawns = ini.ReadString("Patterns.Cheats", "pMeteorAlwaysSpawns", "");
	hotkey_cheats = ini.ReadString("Settings", "hotkey_cheats", "F12");
	iVKCheats = StringToVK(hotkey_cheats);

	// Cam Settings
	hotkey_toggle = ini.ReadString("Camera.Keys", "hotkey_toggle", "F2");
	hotkey_xp = ini.ReadString("Camera.Keys", "hotkey_x+", "NUMPAD8");
	hotkey_xm = ini.ReadString("Camera.Keys", "hotkey_x-", "NUMPAD2");
	hotkey_yp = ini.ReadString("Camera.Keys", "hotkey_y+", "RIGHT");
	hotkey_ym = ini.ReadString("Camera.Keys", "hotkey_y-", "LEFT");
	hotkey_zp = ini.ReadString("Camera.Keys", "hotkey_z+", "UP");
	hotkey_zm = ini.ReadString("Camera.Keys", "hotkey_z-", "DOWN");
	hotkey_fovp = ini.ReadString("Camera.Keys", "hotkey_fov+", "NUMPAD-");
	hotkey_fovm = ini.ReadString("Camera.Keys", "hotkey_fov-", "NUMPAD+");
	hotkey_rotp = ini.ReadString("Camera.Keys", "hotkey_rot+", "NUMPAD9");
	hotkey_rotm = ini.ReadString("Camera.Keys", "hotkey_rot-", "NUMPAD7");
	hotkey_pitchp = ini.ReadString("Camera.Keys", "hotkey_pitch+", "NUMPAD3");
	hotkey_pitchm = ini.ReadString("Camera.Keys", "hotkey_pitch-", "NUMPAD1");
	hotkey_yawp = ini.ReadString("Camera.Keys", "hotkey_yaw+", "NUMPAD6");
	hotkey_yawm = ini.ReadString("Camera.Keys", "hotkey_yaw-", "NUMPAD4");
	hotkey_timestop = ini.ReadString("Camera.Keys", "hotkey_timestop", "NUMPAD0");
	fSpeed = ini.ReadFloat("Camera.Config", "fSpeed", 5.0f);
	fCamHold = ini.ReadInteger("Camera.Config", "fCamHold", 50)/1000.0f;
	//Cam Load
	iVKCamToggle = StringToVK(hotkey_toggle);
	iVKxp = StringToVK(hotkey_xp);
	iVKxm = StringToVK(hotkey_xm);
	iVKyp = StringToVK(hotkey_yp);
	iVKym = StringToVK(hotkey_ym);
	iVKzp = StringToVK(hotkey_zp);
	iVKzm = StringToVK(hotkey_zm);
	iVKfovp = StringToVK(hotkey_fovp);
	iVKfovm = StringToVK(hotkey_fovm);
	iVKrotp = StringToVK(hotkey_rotp);
	iVKrotm = StringToVK(hotkey_rotm);
	iVKpitchp = StringToVK(hotkey_pitchp);
	iVKpitchm = StringToVK(hotkey_pitchm);
	iVKyawp = StringToVK(hotkey_yawp);
	iVKyawm = StringToVK(hotkey_yawm);
	iVKtimestop = StringToVK(hotkey_timestop);

	iHookMenuOpenKey = ini.ReadInteger("Settings", "iHookMenuOpenKey", VK_F1);

}

int eSettingsManager::StringToVK(std::string sKey)
{
	for (auto& c : sKey) c = toupper(c); // To Upper sKey

	if (sKey.substr(0, 6) == "NUMPAD") // Numpad
	{
		char cLast = sKey[6];
		switch (cLast)
		{
		case '+':
			return VK_ADD;
		case '-':
			return VK_SUBTRACT;
		case 'E': //NUMPAD Enter
			return VK_RETURN;
		case '*':
			return VK_MULTIPLY;
		case '/':
			return VK_DIVIDE;
		default:
			return cLast + 0x30; // NUMPAD Number
		}

	}
	if (sKey.length() == 1)
	{
		if (sKey[0] >= '0' && sKey[0] <= 'Z' && sKey[0] != 0x40) // Letter or Number. 0x40 is not available which is @
			return sKey[0]; // Letters and Numbers' VK are their ASCII Repr
		if (sKey[0] == '/' || sKey[0] == '?')
			return VK_OEM_2;
		if (sKey[0] == '-')
			return VK_OEM_MINUS;
		if (sKey[0] == '+')
			return VK_OEM_PLUS;
		if (sKey[0] == '`' || sKey[0] == '~')
			return VK_OEM_3;
	}
	
	if (sKey[0] == 'F' && sKey.length() > 1) // F1-F12 buttons
	{
		std::string digits = sKey.substr(1, sKey.length() - 1).c_str();
		return std::stoi(digits.c_str()) + 0x70 -1;
	}

	// Arrows
	if (sKey == "DOWN")
		return VK_DOWN;
	if (sKey == "UP")
		return VK_UP;
	if (sKey == "LEFT")
		return VK_LEFT;
	if (sKey == "RIGHT")
		return VK_RIGHT;
}