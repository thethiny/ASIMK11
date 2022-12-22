#include "eSettingsManager.h"
#include "..\IniReader.h"
#include <Windows.h>

eSettingsManager* SettingsMgr = new eSettingsManager();

void eSettingsManager::Init()
{
	CIniReader ini("");

	// Debug
	bEnableConsoleWindow		= ini.ReadBoolean	("Debug",				"bEnableConsoleWindow",		false);
	bPauseOnStart				= ini.ReadBoolean	("Debug",				"bPauseOnStart",			false);
	iVerbose					= ini.ReadInteger	("Debug",				"iVerbose",					0);
	bDebug						= ini.ReadBoolean	("Debug",				"bDebug",					false);
	
	// Settings
	iLogSize					= ini.ReadInteger	("Settings",			"iLogSize",					50);
	szGameVer					= ini.ReadString	("Settings",			"szGameVer",				"0.386");
	szModLoader					= ini.ReadString	("Settings",			"szModLoader",				"Kernel32.CreateFileW");
	szAntiCheatEngine			= ini.ReadString	("Settings",			"szAntiCheatEngine",		"User32.EnumChildWindows");
	szCurlSetOpt				= ini.ReadString	("Settings",			"szCurlSetOpt",				"libcurl.curl_easy_setopt");
	szCurlPerform				= ini.ReadString	("Settings",			"szCurlPerform",			"libcurl.curl_easy_perform");
	// Patches
	bModLoader					= ini.ReadBoolean	("Patches",				"bModLoader",				false);
	bEnableCheats				= ini.ReadBoolean	("Patches",				"bEnableCheats",			false);
	bEnableIntroSwap			= ini.ReadBoolean	("Patches",				"bEnableIntroSwap",			false);
	bPatchUnlocker				= ini.ReadBoolean	("Patches",				"bPatchUnlocker",			false);
	bPatchCurl					= ini.ReadBoolean	("Patches",				"bPatchCurl",				false);

	// Patches.AntiCheat
	bDisableAntiCheatEngine		= ini.ReadBoolean	("Patches.AntiCheat",	"bDisableAntiCheatEngine",	true);
	bDisableAntiCVD				= ini.ReadBoolean	("Patches.AntiCheat",	"bDisableAntiCVD",			false);

	// Patches.Addresses
	
	// Patches.Patterns
	pGameVer					= ini.ReadString	("Patches.Patterns",	"pGameVer",					"49 63 4c 24 08 85 c9 74 12");
	pGameVerFull				= ini.ReadString	("Patches.Patterns",	"pGameVerFull",				"48 89 84 24 ? ? ? ? 4c 8b f1 33 DB 89 5C 24");
	pCVD1						= ini.ReadString	("Patches.Patterns",	"pCVD1",					"39 44 24 30 0F 85 37 05 00 00");
	pCVD2						= ini.ReadString	("Patches.Patterns",	"pCVD2",					"48 89 54 24 10 53 55 56 57 41 54 41 55 41 56 41 57 48 83 ec 58 48 c7 44 24 20 fe ff ff ff");
	pIntroSwap					= ini.ReadString	("Patches.Patterns",	"pIntroSwap",				"8B 46 08 4D 89 55 00");

	// Cheats
	pMercyAnyTime				= ini.ReadString	("Cheats.Patterns",		"pMercyAnyTime",			"");
	pNoGroundReq				= ini.ReadString	("Cheats.Patterns",		"pNoGroundReq",				"");
	pNoBrutalityReq				= ini.ReadString	("Cheats.Patterns",		"pNoBrutalityReq",			"");
	pBrutalityAlwaysCorrect		= ini.ReadString	("Cheats.Patterns",		"pBrutalityAlwaysCorrect",	"");
	pMeteorAlwaysSpawns			= ini.ReadString	("Cheats.Patterns",		"pMeteorAlwaysSpawns",		"");
	pFatalityAlwaysAvailable	= ini.ReadString	("Cheats.Patterns",		"pFatalityAlwaysAvailable", "");
	pDizzyAlwaysAvailable		= ini.ReadString	("Cheats.Patterns",		"pDizzyAlwaysAvailable",	"");
	pComboInFatality			= ini.ReadString	("Cheats.Patterns",		"pComboInFatality",			"");
	pBlockingDisabled			= ini.ReadString	("Cheats.Patterns",		"pBlockingDisabled",		"");
	pFatalBlowAtMaxHealth		= ini.ReadString	("Cheats.Patterns",		"pFatalBlowAtMaxHealth",	"");

	// Unlocker
	pGetItemName				= ini.ReadString	("Unlocker.Patterns",	"pGetItemName",				"48 8b d0 48 8b 4f ? E8 ? ? ? ? 48 8b cb");
	pGetItemPriceString			= ini.ReadString	("Unlocker.Patterns",	"pGetItemPriceString",		"90 4c 89 74 24 ? 48 8b 54 24 ? 4c");
	pGetItemPrice				= ini.ReadString	("Unlocker.Patterns",	"pGetItemPrice",			"48 8b d0 49 8b cc E8 ? ? ? ? 44 8b e8 48");
	pKollectableUnlocker		= ini.ReadString	("Unlocker.Patterns",	"pKollectableUnlocker",		"BA 08 00 00 00 41 b8 66 02 00 00");
	pVR2Function				= ini.ReadString	("Unlocker.Patterns",	"pVR2Function",				"48 8d 0d ? ? ? ? e8 ? ? ? ? 48 83 C4 ? 41 55");
	pVR2s						= ini.ReadString	("Unlocker.Patterns",	"pVR2s",					"48 8b d0 48 8d 0d ? ? ? ? E8 ? ? ? ? 90 48 8b 4c 24 ? E8");
	pVR2x						= ini.ReadString	("Unlocker.Patterns",	"pVR2x",					"4c 8b 2c 24 48 8d 0d");

	// Keybinds
	hkMenu						= ini.ReadString	("Keybinds",			"hkMenu",					"F1");
	hkInfo						= ini.ReadString	("Keybinds",			"hkInfo",					"TAB");
	hkCheats					= ini.ReadString	("Keybinds",			"hkCheats",					"F12");

	// Private Server
	szMITMUrl					= ini.ReadString	("Server",				"szMITMUrl",				"127.0.0.1");
	bMITM						= ini.ReadBoolean	("Server",				"bMITM",					false);

}
