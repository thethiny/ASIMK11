#include "includes.h"
#include "src/mk11.h"
#include "src/mk11hook.h"
#include "src/Unlocker/eItemsManager.h"
#include <tlhelp32.h> 
#include <VersionHelpers.h>

#define MKPIPE LPCSTR("\\\\.\\pipe\\MK11Unlocker")


using namespace Memory::VP;
using namespace hook;


Trampoline* GameTramp, * User32Tramp;

HANDLE hConsole = NULL;

void CreateConsole(bool = false);
void HooksMain();
void SyncAwait(std::string(*)(void), const char*, bool = false);
void SpawnError(const char*);
void AwaitSyncs();
void PreGameHooks();
void ProcessSettings();
bool OnInitializeHook();
bool InitUnlocker();

enum ConsoleColors
{
	BLACK = 0,
	BLUE,
	GREEN,
	AQUA,
	RED,
	PURPLE,
	YELLOW,
	WHITE,
	GRAY,
	LIGHTBLUE,
	LIGHTGREEN,
	LIGHTAQUA,
	LIGHTRED,
	LIGHTPURPLE,
	LIGHTYELLOW,
	BRIGHTWHITE,



	GREY = 8, // Synonym
};

#define AsyncError(text) CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)(SpawnError), text, NULL, NULL)

LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
	bool state = lParam >> 31, transition = lParam & 0x40000000;
	auto RepeatCount = LOWORD(lParam);


	if (code >= 0 && !state) // State 0 -> Down
	{
		if (!transition) // Transition 0 -> Being Pressed (PosEdge), 1 -> Being Held.
		{
			if (wParam == SettingsMgr->iVKMenuToggle)
			{
				GuiMenu->ToggleActive();
			}

			if (SettingsMgr->bDebug)
			{
				if (GetAsyncKeyState(VK_F10))
				{
					static int ctr = 0;
					char* HashResult = new char[10];
					std::stringstream ToHash("");
					ToHash << "thethiny" << ctr;
					ctr++;
					MK11Unlocker::VR2xStruct* ResultStructPtr = MK11Unlocker::GetVR2xHash(ToHash.str().c_str(), HashResult);
				}
				if (GetAsyncKeyState(VK_F11))
				{
					char* HashResult = new char[10];
					std::string ToHash = "thethiny";
					MK11Unlocker::VR2xStruct* ResultStructPtr = MK11Unlocker::GetVR2xHash(ToHash.c_str(), HashResult);
				}
			}


			// Cheats
			if (SettingsMgr->bEnableCheats)
			{
				if (GetAsyncKeyState(SettingsMgr->iVKCheats)) // Button Down
				{
					char* S_ON = "Enabled";
					char* S_OFF = "Disabled";
					if (wParam == '1' && MK11::sCheatsStruct.lpMercy)
					{
						std::cout << "pMercyAnyTime ";
						if (MK11::sCheatsStruct.bMercy)
						{
							printfRed("Off");
							Patch<uint16_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpMercy), 0x5340); // push rbx // Change this to read it automatically from memory, or verify against a hash or something
							Patch<uint8_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpMercy) + 2), 0x48);
						}
						else
						{
							printfGreen("On");
							Patch<uint16_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpMercy), 0x01B0); // mov al, 1
							Patch<uint8_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpMercy) + 2), 0xC3);
						}
						std::cout << std::endl;
						MK11::sCheatsStruct.bMercy = !MK11::sCheatsStruct.bMercy;
					}
					if (wParam == '2' && MK11::sCheatsStruct.lpGround)
					{
						std::cout << "pNoGroundReq ";
						if (MK11::sCheatsStruct.bGround)
						{
							printfRed("Off");
							Patch<uint16_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpGround), 0x5340); // mov al, 1
							Patch<uint8_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpGround) + 2), 0x48);
						}
						else
						{
							printfGreen("On");
							Patch<uint16_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpGround), 0x01B0); // mov al, 1
							Patch<uint8_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpGround) + 2), 0xC3);
						}
						std::cout << std::endl;
						MK11::sCheatsStruct.bGround = !MK11::sCheatsStruct.bGround;
					}
					if (wParam == '3' && MK11::sCheatsStruct.lpBrut)
					{
						std::cout << "pNoBrutalityReq ";
						if (MK11::sCheatsStruct.bBrut)
						{
							printfRed("Off");
							Patch<uint16_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpBrut), 0x8948);
							Patch<uint8_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpBrut) + 2), 0x6C);
							if (MK11::sCheatsStruct.lpBrutB)
							{
								Patch<uint16_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpBrutB), 0x5340);
								Patch<uint8_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpBrutB) + 2), 0x48);
							}
						}
						else
						{
							printfGreen("On");
							Patch<uint16_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpBrut), 0x01B0); // mov al, 1
							Patch<uint8_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpBrut) + 2), 0xC3);
							if (MK11::sCheatsStruct.lpBrutB)
							{
								Patch<uint16_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpBrutB), 0x01B0); // mov al, 1
								Patch<uint8_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpBrutB) + 2), 0xC3);
							}
						}
						std::cout << std::endl;
						MK11::sCheatsStruct.bBrut = !MK11::sCheatsStruct.bBrut;
					}
					if (wParam == '4' && MK11::sCheatsStruct.lpMeteor)
					{
						std::cout << "pMeteorAlwaysSpawns ";
						if (MK11::sCheatsStruct.bMeteor)
						{
							printfRed("Off");
							Patch<uint16_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpMeteor), 0x1C75); // jne +1E
						}
						else
						{
							printfGreen("On");
							Nop(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpMeteor), 2);
						}
						std::cout << std::endl;
						MK11::sCheatsStruct.bMeteor = !MK11::sCheatsStruct.bMeteor;
					}
					if (wParam == '5' && MK11::sCheatsStruct.lpDizzy && MK11::sCheatsStruct.lpFatality)
					{
						std::cout << "pFatalityAlwaysAvailable ";
						if (MK11::sCheatsStruct.bDizzy)
						{
							printfRed("Off");
							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpDizzy), 0x24748948); // mov [rsp+10],rsi
							Patch<uint64_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpDizzy) + 4), 0x48571024); // push rdi / sub rsp, 20

							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpDizzy), 0x83485340);
							Patch<uint64_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpDizzy) + 4), 0x8B4820EC);
						}
						else
						{
							printfGreen("On");
							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpDizzy), 0x1C0C748); // mov rax, 1
							Patch<uint64_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpDizzy) + 4), 0xC3000000); // ret

							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpFatality), 0x1C0C748);
							Patch<uint64_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpFatality) + 4), 0xC3000000);
						}
						std::cout << std::endl;
						MK11::sCheatsStruct.bFatality = MK11::sCheatsStruct.bDizzy = !MK11::sCheatsStruct.bDizzy;
					}
					if (wParam == '6' && MK11::sCheatsStruct.lpFatCombo)
					{
						std::cout << "pComboInFatality ";
						if (MK11::sCheatsStruct.bFatCombo)
						{
							printfRed("Off");
							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpFatCombo), 0x28EC8348);
						}
						else
						{
							printfGreen("On");
							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpFatCombo), 0xC3C03148); // xor rax, rax / ret
						}
						std::cout << std::endl;
						MK11::sCheatsStruct.bFatCombo = !MK11::sCheatsStruct.bFatCombo;
					}
					if (wParam == '7' && MK11::sCheatsStruct.lpNoBlock)
					{
						std::cout << "pBlockingDisabled ";
						if (MK11::sCheatsStruct.bNoBlock)
						{
							printfRed("Off");
							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpNoBlock), 0x245C8948);
						}
						else
						{
							printfGreen("On");
							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpNoBlock), 0xC3C03148); // xor rax, rax / ret
						}
						std::cout << std::endl;
						MK11::sCheatsStruct.bNoBlock = !MK11::sCheatsStruct.bNoBlock;
					}
					if (wParam == '8' && MK11::sCheatsStruct.lpFatalBlow)
					{
						std::cout << "pFatalBlowAtMaxHealth ";
						if (MK11::sCheatsStruct.bFatalBlow)
						{
							printfRed("Off");
							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpFatalBlow), 0xA9058B48);
							Patch<uint64_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpFatalBlow) + 4), 0x8302ED78);
						}
						else
						{
							printfGreen("On");
							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpFatalBlow), 0x1C0C748); // mov rax, 1
							Patch<uint64_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpFatalBlow) + 4), 0xC3000000); // ret
						}
						std::cout << std::endl;
						MK11::sCheatsStruct.bFatalBlow = !MK11::sCheatsStruct.bFatalBlow;
					}
				}
			}
		}
	}
	return CallNextHookEx(0, code, wParam, lParam);
}

void CreateConsole(bool bFreeze)
{
	AllocConsole();
	FILE* fNull;
	freopen_s(&fNull, "CONOUT$", "w", stdout);
	freopen_s(&fNull, "CONOUT$", "w", stderr);
	if (bFreeze)
	{
		std::cout << "Freezing Game Until User Presses Enter..." << std::endl;
		freopen_s(&fNull, "CONIN$", "r", stdin); // IDK Why this causes a freeze
		char x;
		std::cin >> x;
	}
		
	std::cout.clear();
	std::cerr.clear();
	std::clog.clear();
	std::string consoleName = "thethiny's MK11 Mod Console";
	SetConsoleTitleA(consoleName.c_str());
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hConsole, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hConsole, dwMode);
	printfCyan("ASIMK11 - Maintained by ");
	printfInfo("@thethiny\n");
	printfGreen("ESettingsManager::bEnableConsoleWindow = true\n");
}


void HooksMain()
{
	if (SettingsMgr->bEnableIntroSwap)
	{
		std::cout << "==bEnableIntroSwap==" << std::endl;
		uint64_t* lpIntroSwap = FindPattern(GetModuleHandleA(NULL), SettingsMgr->pIntroSwap);
		if (lpIntroSwap != NULL)
		{
			SetColorGreen();
			if (SettingsMgr->iVerbose)
				std::cout << "Intro Swap Pattern found at: " << std::hex << lpIntroSwap << std::endl;
			uint64_t fix_len_add = (uint64_t)lpIntroSwap;
			Patch<uint16_t>(fix_len_add, 0x428d); // lea eax, [rdx+
			Patch<uint8_t>(fix_len_add + 2, 0x2A); // 0x2A]
			uint64_t hook_address = fix_len_add + 0x35;
			InjectHook(hook_address, GameTramp->Jump(MK11Hooks::IntroSwap), PATCH_CALL);
			MK11::sActiveMods.bIntroSwap = true;
			std::cout << "Patched Anims Loader" << std::endl;
			MK11::PopulateCharList();
			ResetColors();
		}
		else
		{
			printfError("Couldn't find pattern. Intro Swap Mod Disabled\n");
		}
	}

	if (SettingsMgr->bEnableCheats)
	{
		std::cout << "==bEnableCheats==" << std::endl;
		SetCheatPattern(SettingsMgr->pMercyAnyTime,				"pMercyAnyTime",			&(MK11::sCheatsStruct.lpMercy));
		SetCheatPattern(SettingsMgr->pNoGroundReq,				"pNoGroundReq",				&(MK11::sCheatsStruct.lpGround));
		SetCheatPattern(SettingsMgr->pNoBrutalityReq,			"pNoBrutalityReq",			&(MK11::sCheatsStruct.lpBrut));
		SetCheatPattern(SettingsMgr->pBrutalityAlwaysCorrect,	"pBrutalityAlwaysCorrect",	&(MK11::sCheatsStruct.lpBrutB));
		SetCheatPattern(SettingsMgr->pMeteorAlwaysSpawns,		"pMeteorAlwaysSpawns",		&(MK11::sCheatsStruct.lpMeteor));
		SetCheatPattern(SettingsMgr->pFatalityAlwaysAvailable,	"pFatalityAlwaysAvailable", &(MK11::sCheatsStruct.lpFatality));
		SetCheatPattern(SettingsMgr->pDizzyAlwaysAvailable,		"pDizzyAlwaysAvailable",	&(MK11::sCheatsStruct.lpDizzy));
		SetCheatPattern(SettingsMgr->pComboInFatality,			"pComboInFatality",			&(MK11::sCheatsStruct.lpFatCombo));
		SetCheatPattern(SettingsMgr->pBlockingDisabled,			"pBlockingDisabled",		&(MK11::sCheatsStruct.lpNoBlock));
		SetCheatPattern(SettingsMgr->pFatalBlowAtMaxHealth,		"pFatalBlowAtMaxHealth",	&(MK11::sCheatsStruct.lpFatalBlow));
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)AwaitSyncs, NULL, NULL, NULL);
}

void SyncAwait(std::string(*function)(void), const char* string, bool not)
{
	while (function().empty() ^ not);
	std::cout << string << ": " << function() << std::endl;
}

void AwaitSyncs()
{
	SetColorYellow();
	SyncAwait(&MK11::GetGameVersion, "Game Version");
	SyncAwait(&MK11::GetGameVersionFull, "Game FullVersion");
	ResetColors();
	MK11::sGameState.bGameVersion = true;
	MK11::sGameState.szGameVersion = MK11::GetGameVersionFull();
}

void PreGameHooks()
{
	GameTramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
	if (SettingsMgr->iVerbose)
		std::cout << "Generated Trampolines" << std::endl;
	MK11::IAT = ParsePEHeader();

	if (SettingsMgr->FORCE_CHECK_VER)
	{
		std::cout << "==bGetGameVersionShort==" << std::endl;
		if (SettingsMgr->pGameVer.empty())
		{
			printfRed("GameVer Not Specified. Please Add Pattern to ini file!\n");
		}
		else
		{
			uint64_t* lpPattern = FindPattern(GetModuleHandleA(NULL), SettingsMgr->pGameVer);
			if (!lpPattern)
			{
				printfError("Couldn't find Game Version Pattern\n");
			}
			else
			{
				SetColorGreen();
				if (SettingsMgr->iVerbose)
					std::cout << "Game Version Pattern found at: " << std::hex << lpPattern << std::dec << std::endl;
				lpPattern = (uint64_t*)((uint64_t)(lpPattern)+0xF);
				uint64_t offset = 0;
				memcpy(&offset, lpPattern, 4);
				offset += ((uint64_t)lpPattern) + 4;
				if (SettingsMgr->iVerbose)
					std::cout << std::hex << "Game Version Offset: " << offset << std::endl;
				MK11::lpGameVersion = (uint64_t*)offset;
				ResetColors();
			}
		}

		std::cout << "==bGetGameVersion==" << std::endl;
		if (SettingsMgr->pGameVerFull.empty())
		{
			printfRed("GameVerFull Not Specified. Please Add Pattern to ini file!\n");
		}
		else
		{
			uint64_t* lpPattern = FindPattern(GetModuleHandleA(NULL), SettingsMgr->pGameVerFull);
			if (!lpPattern)
			{
				printfError("Couldn't find Game Full Version Pattern\n");
			}
			else
			{
				SetColorGreen();
				if (SettingsMgr->iVerbose)
					std::cout << "Game FullVersion Pattern found at: " << std::hex << lpPattern << std::dec << std::endl;
				lpPattern = (uint64_t*)((uint64_t)lpPattern + 0x1C);
				uint64_t offset = 0;
				memcpy(&offset, lpPattern, 4);
				offset += (uint64_t)lpPattern + 4;
				if (SettingsMgr->iVerbose)
					std::cout << std::hex << "Game FullVersion Offset: " << offset << std::endl;
				MK11::lpGameVersionFull = (uint64_t*)offset;
				ResetColors();
			}

		}

		if (SettingsMgr->bModLoader)
		{
			std::cout << "==bModLoader==" << std::endl;
			uint64_t CreateFileW = (uint64_t)MK11::GetLibProcFromNT(MK11::sLFS.ModLoader);
			if (!CreateFileW)
			{
				MK11::PrintErrorProcNT(MK11::sLFS.ModLoader, 1);
			}
			else
			{
				SetColorGreen();
				std::string CFWPattern = HexToString(CreateFileW);
				MK11::sActiveMods.bModLoader = true;
				int32_t patched = 0;

				if (SettingsMgr->iVerbose)
					std::cout << "Patching " << MK11::sLFS.ModLoader.LibName << "[" << MK11::sLFS.ModLoader.ProcName << "] (" << std::hex << CreateFileW << ")" << std::endl;

				uint64_t lpPattern = (uint64_t)FindPattern(GetModuleHandleA(NULL), CFWPattern);
				while (lpPattern)
				{
					Patch<uint64_t>(GetGameAddr(lpPattern), (uint64_t)MK11Hooks::CreateFileProxy);
					if (SettingsMgr->iVerbose)
						std::cout << "Patched at " << std::hex << lpPattern << std::endl;
					patched++;
					lpPattern = (uint64_t)FindPattern(GetModuleHandleA(NULL), CFWPattern);
				}
				if (!patched)
				{
					MK11::PrintErrorProcNT(MK11::sLFS.ModLoader, 0);
				}
				ResetColors();
			}
			
		}
	}

	if (SettingsMgr->bDisableAntiCheatEngine)
	{
		std::cout << "==bDisableAntiCheatEngine==" << std::endl;
		uint64_t Func = (uint64_t)MK11::GetLibProcFromNT(MK11::sLFS.AntiCheatEngine);
		if (Func)
		{
			if (SettingsMgr->iVerbose)
				std::cout << "Patching " << MK11::sLFS.AntiCheatEngine.LibName << "[" << MK11::sLFS.AntiCheatEngine.ProcName << "] (" << std::hex << Func << ")" << std::endl;
			Patch(Func, 0xC3);
			Patch(Func + 1, 0x90909090);
			MK11::sActiveMods.bAntiCheatEngine = true;
			printfSuccess("Anti Cheat Engine Patched\n");
		}
		else
		{
			MK11::PrintErrorProcNT(MK11::sLFS.AntiCheatEngine, 1);
		}
	}

	if (SettingsMgr->bDisableAntiCVD)
	{
		std::cout << "==bDisableAntiCVD==" << std::endl;
		if (SettingsMgr->pCVD1.empty())
		{
			printfRed("CVD1 Not Specified. Please Add Pattern to ini file!\n");
		}
		else if (SettingsMgr->pCVD2.empty())
		{
			printfRed("CVD2 Not Specified. Please Add Pattern to ini file!\n");
		}
		else
		{
			uint64_t* lpCVDPattern = FindPattern(GetModuleHandleA(NULL), SettingsMgr->pCVD1);
			uint64_t* lpCVD2Pattern = FindPattern(GetModuleHandleA(NULL), SettingsMgr->pCVD2);
			if (!lpCVDPattern)
			{
				printfError("Couldn't find CVD1 Pattern\n");
			}
			else if (!lpCVD2Pattern)
			{
				printfError("Couldn't find CVD2 Pattern\n");
			}
			else
			{
				uint64_t hook_address = (uint64_t)lpCVDPattern;
				if (SettingsMgr->iVerbose)
					std::cout << "CVD1 Pattern found at: " << std::hex << lpCVDPattern << std::dec << std::endl;
				Patch(GetGameAddr(hook_address), (uint16_t)0xC039); // cmp eax, eax
				Patch(GetGameAddr(hook_address) + 2, (uint16_t)0x9090); // Nop
				MK11::sActiveMods.bAntiCVD1 = true;
				printfSuccess("Anti CVD1 Patched\n");

				hook_address = (uint64_t)lpCVD2Pattern;
				if (SettingsMgr->iVerbose)
					std::cout << "CVD2 Pattern found at: " << std::hex << lpCVD2Pattern << std::dec << std::endl;
				Patch(GetGameAddr(hook_address), (uint8_t)0xC3); // ret
				MK11::sActiveMods.bAntiCVD2 = true;
				printfSuccess("Anti CVD2 Patched\n");
			}
		}

	}

	// CURL

	if (SettingsMgr->bPatchCurl)
	{
		std::cout << "==PrivateServer==" << std::endl;
		auto Func	= MK11Hooks::curl_easy_setopt	= (MK11Hooks::LibCurlEasySetOpt*)	MK11::GetLibProcFromNT(MK11::sLFS.CurlSetOpt);
		auto Func2	= MK11Hooks::curl_easy_perform	= (MK11Hooks::LibCurlEasyPerform*)	MK11::GetLibProcFromNT(MK11::sLFS.CurlPerform);
		MK11::sActiveMods.bCurl = true;
		if (Func && Func2)
		{
			SetColorGreen();
			if (SettingsMgr->iVerbose)
				std::cout << "Found " << MK11::sLFS.CurlSetOpt.LibName << "[" << MK11::sLFS.CurlSetOpt.ProcName << "] (" << std::hex << Func << ")" << std::endl;
			std::string LibCurlPat = HexToString(Func);
			uint64_t lpPattern = (uint64_t)FindPattern(GetModuleHandleA(NULL), LibCurlPat);
			uint64_t patched = 0;
			while (lpPattern)
			{
				Patch<uint64_t>(GetGameAddr(lpPattern), (uint64_t)MK11Hooks::curl_easy_setoptProxy);
				if (SettingsMgr->iVerbose)
					std::cout << "Patched at " << std::hex << lpPattern << std::endl;
				patched++;
				lpPattern = (uint64_t)FindPattern(GetModuleHandleA(NULL), LibCurlPat);
			}
			if (!patched)
			{
				MK11::PrintErrorProcNT(MK11::sLFS.CurlPerform, 2);
				MK11::sActiveMods.bCurl = false;
			}
			
			if (SettingsMgr->iVerbose)
				std::cout << "Found " << MK11::sLFS.CurlPerform.LibName << "[" << MK11::sLFS.CurlPerform.ProcName << "] (" << std::hex << Func2 << ")" << std::endl;
			LibCurlPat = HexToString(Func2);
			lpPattern = (uint64_t)FindPattern(GetModuleHandleA(NULL), LibCurlPat);
			patched = 0;
			while (lpPattern)
			{
				Patch<uint64_t>(GetGameAddr(lpPattern), (uint64_t)MK11Hooks::curl_easy_performProxy);
				if (SettingsMgr->iVerbose)
					std::cout << "Patched at " << std::hex << lpPattern << std::endl;
				patched++;
				lpPattern = (uint64_t)FindPattern(GetModuleHandleA(NULL), LibCurlPat);
			}
			if (!patched)
			{
				MK11::PrintErrorProcNT(MK11::sLFS.CurlPerform, 2);
				MK11::sActiveMods.bCurl = false;
			}
			ResetColors();
			
		}
		else
		{
			MK11::PrintErrorProcNT(MK11::sLFS.CurlPerform, 2);
			MK11::sActiveMods.bCurl = false;
		}

	}

	// Unlocker Section
	if (SettingsMgr->bPatchUnlocker)
	{
		bool bU = MK11::sActiveMods.bUnlocker = InitUnlocker();
		if (bU)
			printfSuccess("Unlocker is Enabled\n");
		else
			printfError("Unlocker Failed to Initialize!\n");
	}
	else
	{
		printfRed("Unlocker is not enabled in the settings.\n");
	}
	
}

bool InitUnlocker()
{
	uint64_t* Entry = new uint64_t;
	std::cout << "==Unlocker::DailyShopHook==" << std::endl;
	if (SettingsMgr->iVerbose)
		std::cout << "Injecting Daily Shop Function" << std::endl;

	if (!HookPattern(SettingsMgr->pGetItemName, "Get Item Name", GameTramp->Jump(MK11Unlocker::GetItemName), -0x5, PATCH_CALL, NULL, Entry))
	{
		printfError("Unlocker Failed to Initialize [%s]\n", "Get Item Name");
		return false;
	}
	else
	{
		MK11Unlocker::ProcGetItemName = (MK11Unlocker::lpwsThisFunction*) * Entry;
	}


	if (!HookPattern(SettingsMgr->pGetItemPriceString, "Get Item Price String", GameTramp->Jump(MK11Unlocker::GetItemPriceString), -0x14, PATCH_CALL, NULL, Entry))
	{
		printfError("Unlocker Failed to Initialize [%s]\n", "Get Item Price String");
		return false;
	}
			
	else
		MK11Unlocker::ProcGetItemPriceString = (MK11Unlocker::lpwsThisFunction*) * Entry;

	if (!HookPattern(SettingsMgr->pGetItemPrice, "Get Item Price", GameTramp->Jump(MK11Unlocker::GetItemPrice), 6, PATCH_CALL, NULL, Entry))
	{
		printfError("Unlocker Failed to Initialize [%s]\n", "Get Item Price");
		return false;
	}
	else
		MK11Unlocker::ProcGetItemPrice = (MK11Unlocker::ullThisFunction*) * Entry;


	std::cout << "==Unlocker::Kollections==" << std::endl;
	if (SettingsMgr->iVerbose)
		std::cout << "Injecting Kombat Kard Function" << std::endl;

	uint64_t lpKollections = HookPattern(SettingsMgr->pKollectableUnlocker, "Kollections Unlocker", GameTramp->Jump(&MK11Unlocker::eInventoryManager::UnlockItem), 0x4D, PATCH_CALL, NULL, Entry);
	if (!lpKollections)
	{
		printfError("Unlocker Failed to Initialize [%s]\n", "Kollections Unlocker");
		return false;
	}
	HookPattern(SettingsMgr->pKollectableUnlocker, "Kollections Unlocker", GameTramp->Jump(MK11::TrueFunc), 0xB, PATCH_CALL, lpKollections);
	HookPattern(SettingsMgr->pKollectableUnlocker, "Kollections Unlocker", GameTramp->Jump(MK11::DummyFunc), 0x69, PATCH_CALL, lpKollections);
	MK11Unlocker::ProcUnlockItem = (MK11Unlocker::UnlockItem*) * Entry;

	std::cout << "==Unlocker::Equipment==" << std::endl;
	if (SettingsMgr->iVerbose)
		std::cout << "Injecting VR2 Proxy Function" << std::endl;

	uint64_t addr;
	addr = (uint64_t)FindPattern(GetModuleHandleA(NULL), SettingsMgr->pVR2x);
	if (!addr)
	{
		printfError("Unlocker Failed to Initialize [%s]\n", "VR2 Result Pointer");
		return false;
	}
	uint64_t lpVR2x = GetAbsoluteDestination(addr, 10, 14);
	if (SettingsMgr->iVerbose)
		std::cout << "Found VR2x Static at " << lpVR2x << std::endl;
	MK11Unlocker::VR2xHash = (MK11Unlocker::VR2xPtrStaticStruct*)lpVR2x;

	uint64_t lpVR2Func = HookPattern(SettingsMgr->pVR2Function, "VR2 Proxy", GameTramp->Jump(MK11Unlocker::VR2Proxy), 17, PATCH_CALL, NULL, Entry);
	if (!lpVR2Func)
	{
		printfError("Unlocker Failed to Initialize [%s]\n", "VR2 Proxy");
		return false;
	}
	MK11Unlocker::ProcVR2Function = (MK11Unlocker::VR2Function*) * Entry;
	uint64_t lpVR2s = GetAbsoluteDestination(lpVR2Func, 13, 17);
	if (SettingsMgr->iVerbose)
		std::cout << "Found VR2s Static at " << lpVR2s << std::endl;
	MK11Unlocker::sVR2 = (MK11Unlocker::VR2Struct*)lpVR2s;

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MK11IPC::Server, NULL, NULL, NULL);
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)MK11Unlocker::HashQueueHandler, NULL, NULL, NULL);
	return true;
}

void ProcessSettings()
{
	// KeyBinds
	SettingsMgr->iVKCheats		= StringToVK(SettingsMgr->hkCheats);
	SettingsMgr->iVKMenuToggle	= StringToVK(SettingsMgr->hkMenu);
	SettingsMgr->iVKMenuInfo	= StringToVK(SettingsMgr->hkInfo);

	// DLL Procs
	MK11::sLFS.ModLoader		= MK11::ParseLibFunc(SettingsMgr->szModLoader);
	MK11::sLFS.AntiCheatEngine	= MK11::ParseLibFunc(SettingsMgr->szAntiCheatEngine);
	MK11::sLFS.CurlSetOpt		= MK11::ParseLibFunc(SettingsMgr->szCurlSetOpt);
	MK11::sLFS.CurlPerform		= MK11::ParseLibFunc(SettingsMgr->szCurlPerform);

	printfCyan("Parsed Settings\n");
}



void SpawnError(const char* msg)
{
	MessageBoxA(NULL, msg, "ASIMK11", MB_ICONEXCLAMATION);
}

bool HandleWindowsVersion()
{
	if (IsWindows10OrGreater())
	{
		return true;
	}

	if (IsWindows7SP1OrGreater())
	{
		AsyncError("ASIMK11 doesn't officially support Windows 8 or 7 SP1. It may misbehave.");
		return true;
	}

	AsyncError("ASIMK11 doesn't support Windows 7 or Earlier. Might not work.");
	return true;

	
}

bool HandleDXVersion()
{
	std::string expected_process("MK11.exe");
	std::string expected_process_dx12("MK11_DX12.exe");
	std::string process_name = GetProcessName();
	if (process_name != expected_process)
	{
		if (process_name == expected_process_dx12)
			AsyncError("ASIMK11 doesn't officially support DX12 version of the game. It may misbehave.");
		else
			return false;

	}
	return true;
}

bool OnInitializeHook()
{

	if (!HandleWindowsVersion())
		return false;

	if (!HandleDXVersion())
		return false;

	SettingsMgr->Init();

	if (SettingsMgr->bEnableConsoleWindow)
	{
		CreateConsole(SettingsMgr->bPauseOnStart);
	}

	ProcessSettings(); // Parse Settings
	PreGameHooks(); // Queue Blocker
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HooksMain, NULL, NULL, NULL);

	return true;
}

bool APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpRes)
{
	HHOOK hook_ = nullptr;
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, MenuGuiMainThread, hModule, 0, nullptr);
		if (!OnInitializeHook())
			break;
		if (!(hook_ = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, hModule, GetCurrentThreadId())))
		{
			char x[100];
			sprintf(x, "Failed To Hook Keyboard FN: 0x%X", GetLastError());
			MessageBox(NULL, x, "Error", MB_ICONERROR);
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		if (hook_)
			UnhookWindowsHookEx(hook_);
		break;
	}
	return true;
}