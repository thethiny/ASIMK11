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
	
	// Settings
	iLogSize					= ini.ReadInteger	("Settings",			"iLogSize",					10);
	szGameVer					= ini.ReadString	("Settings",			"szGameVer",				"0.305");
	szModLoader					= ini.ReadString	("Settings",			"szModLoader",				"Kernel32.CreateFileW");
	szAntiCheatEngine			= ini.ReadString	("Settings",			"szAntiCheatEngine",		"User32.EnumChildWindows");

	// Patches
	bPatchUnlocker				= ini.ReadBoolean	("Patches",				"bPatchUnlocker",			false);
	bModLoader					= ini.ReadBoolean	("Patches",				"bModLoader",				false);
	bEnableTimeStop				= ini.ReadBoolean	("Patches",				"bEnableTimeStop",			false);
	bEnableCheats				= ini.ReadBoolean	("Patches",				"bEnableCheats",			false);
	bEnableIntroSwap			= ini.ReadBoolean	("Patches",				"bEnableIntroSwap",			false);

	// Patches.AntiCheat
	bDisableAntiCheatEngine		= ini.ReadBoolean	("Patches.AntiCheat",	"bDisableAntiCheatEngine",	true);
	bDisableAntiCVD				= ini.ReadBoolean	("Patches.AntiCheat",	"bDisableAntiCVD",			false);

	// Patches.Addresses
	
	// Patches.Patterns
	pGameVer					= ini.ReadString	("Patches.Patterns",	"pGameVer",					"49 63 4c 24 08 85 c9 74 12");
	pGameVerFull				= ini.ReadString	("Patches.Patterns",	"pGameVerFull",				"48 89 84 24 ? ? ? ? 4c 8b f1 33 DB 89 5C 24");
	pCVD1						= ini.ReadString	("Patches.Patterns",	"pCVD1",					"39 44 24 30 0F 85 37 05 00 00");
	pCVD2						= ini.ReadString	("Patches.Patterns",	"pCVD2",					"48 89 54 24 10 53 55 56 57 41 54 41 55 41 56 41 57 48 83 ec 58 48 c7 44 24 20 fe ff ff ff");
	pTimestop					= ini.ReadString	("Patches.Patterns",	"pTimeStop",				"8B 51 18 39 51 14");
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

	// Keybinds
	hkMenu						= ini.ReadString	("Keybinds",			"hkMenu",					"F1");
	hkInfo						= ini.ReadString	("Keybinds",			"hkInfo",					"TAB");
	hkCheats					= ini.ReadString	("Keybinds",			"hkCheats",					"F12");
	hkTimestop					= ini.ReadString	("Keybinds",			"hkTimestop",				"NUMPAD0");

}
