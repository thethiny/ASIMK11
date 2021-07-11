#include "includes.h"
#include "src/mk11.h"
#include <tlhelp32.h> 

#define MKPIPE TEXT("\\\\.\\pipe\\MK11Unlocker")

using namespace Memory::VP;
using namespace hook;


Trampoline* GameTramp, * User32Tramp;

void CreateConsole(bool = false);
void HooksMain();
void SyncAwait(std::string(*)(void), const char*, bool = false);
void BlockerEvents();
void PreGameHooks();
void UnlockerPipe();
void ProcessSettings();
void OnInitializeHook();

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

			if (MK11::sCamStruct.bTimestopEnabled && wParam == (SettingsMgr->iVKtimestop))
			{
				MK11::sCamStruct.bTimestopActive = !MK11::sCamStruct.bTimestopActive;
				if (MK11::sCamStruct.bTimestopActive)
				{
					std::cout << "TimeStop::On" << std::endl;
				}
				else
				{
					std::cout << "TimeStop::Off" << std::endl;
				}

			}

			if (SettingsMgr->bPatchUnlocker && wParam == (VK_F10))
			{
				uint64_t res;
				MK11Hooks::VR2Proxy(0, &res, "thethiny");
				char* hash = (char*)res;
				printf("Output: %I64X (%s)\n", res, hash);
			}

			// Cheats
			if (SettingsMgr->bEnableCheats)
			{
				if (GetAsyncKeyState(SettingsMgr->iVKCheats)) // Button Down
				{
					if (wParam == '1' && MK11::sCheatsStruct.lpMercy)
					{
						std::cout << "pMercyAnyTime ";
						if (MK11::sCheatsStruct.bMercy)
						{
							std::cout << "Disabled";
							Patch<uint16_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpMercy), 0x5340); // push rbx // Change this to read it automatically from memory, or verify against a hash or something
							Patch<uint8_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpMercy) + 2), 0x48);
						}
						else
						{
							std::cout << "Enabled";
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
							std::cout << "Disabled";
							Patch<uint16_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpGround), 0x5340); // mov al, 1
							Patch<uint8_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpGround) + 2), 0x48);
						}
						else
						{
							std::cout << "Enabled";
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
							std::cout << "Disabled";
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
							std::cout << "Enabled";
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
							std::cout << "Disabled";
							Patch<uint16_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpMeteor), 0x1C75); // jne +1E
						}
						else
						{
							std::cout << "Enabled";
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
							std::cout << "Disabled";
							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpDizzy), 0x24748948); // mov [rsp+10],rsi
							Patch<uint64_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpDizzy) + 4), 0x48571024); // push rdi / sub rsp, 20

							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpDizzy), 0x83485340);
							Patch<uint64_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpDizzy) + 4), 0x8B4820EC);
						}
						else
						{
							std::cout << "Enabled";
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
							std::cout << "Disabled";
							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpFatCombo), 0x28EC8348);
						}
						else
						{
							std::cout << "Enabled";
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
							std::cout << "Disabled";
							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpNoBlock), 0x245C8948);
						}
						else
						{
							std::cout << "Enabled";
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
							std::cout << "Disabled";
							Patch<uint64_t>(GetGameAddr((uint64_t)MK11::sCheatsStruct.lpFatalBlow), 0xA9058B48);
							Patch<uint64_t>(GetGameAddr(((uint64_t)MK11::sCheatsStruct.lpFatalBlow) + 4), 0x8302ED78);
						}
						else
						{
							std::cout << "Enabled";
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
	}
		
	std::cout.clear();
	std::cerr.clear();
	std::clog.clear();
	std::string consoleName = "thethiny's MK11 Mod Console";
	SetConsoleTitleA(consoleName.c_str());
	printf("ESettingsManager::bEnableConsoleWindow = true\n");
}

void HooksMain()
{
	
	if (SettingsMgr->bPatchUnlocker)
	{
		std::cout << "==bPatchUnlocker==" << std::endl;
		std::cout << "Injecting VR2 Proxy Function" << std::endl;

		InjectHook(GetGameAddr(0x40BE75E), GameTramp->Jump(MK11Hooks::VR2Proxy), PATCH_CALL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)UnlockerPipe, NULL, NULL, NULL);

	}

	if (SettingsMgr->bEnableTimeStop)
	{
		uint64_t* lpTimestopPattern = FindPattern(GetModuleHandleA(NULL), SettingsMgr->pTimestop);
		if (lpTimestopPattern != nullptr)
		{
			std::cout << "Timestop Pattern found at: " << std::hex << lpTimestopPattern << std::endl;
			InjectHook(GetGameAddr((uint64_t)lpTimestopPattern), GameTramp->Jump(MK11Hooks::TimestopFunction), PATCH_JUMP);
			std::cout << "Patched TimeStop" << std::dec << std::endl;
			MK11::sCamStruct.bTimestopEnabled = true;
		}
		else
		{
			std::cout << "Couldn't Patch Timestop Function. Timestop Mod Disabled" << std::endl;
		}
	}

	if (SettingsMgr->bEnableIntroSwap)
	{
		std::cout << "==bEnableIntroSwap==" << std::endl;
		uint64_t* lpIntroSwap = FindPattern(GetModuleHandleA(NULL), SettingsMgr->pIntroSwap);
		if (lpIntroSwap != NULL)
		{
			std::cout << "Intro Swap Pattern found at: " << std::hex << lpIntroSwap << std::endl;
			uint64_t fix_len_add = (uint64_t)lpIntroSwap;
			Patch<uint16_t>(fix_len_add, 0x428d); // lea eax, [rdx+
			Patch<uint8_t>(fix_len_add + 2, 0x2A); // 0x2A]
			uint64_t hook_address = fix_len_add + 0x35;
			InjectHook(hook_address, GameTramp->Jump(MK11Hooks::IntroSwap), PATCH_CALL);
			MK11::sActiveMods.bIntroSwap = true;
			std::cout << "Patched Anims Loader" << std::endl;
			MK11::PopulateCharList();
		}
		else
		{
			std::cout << "Couldn't find pattern. Intro Swap Mod Disabled" << std::endl;
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

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)BlockerEvents, NULL, NULL, NULL);
}

void SyncAwait(std::string(*function)(void), const char* string, bool not)
{
	while (function().empty() ^ not);
	std::cout << string << ": " << function() << std::endl;
}

void BlockerEvents()
{
	SyncAwait(&MK11::GetGameVersion, "Game Version");
	SyncAwait(&MK11::GetGameVersionFull, "Game FullVersion");
}

void PreGameHooks()
{
	GameTramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
	std::cout << "Generated Trampolines" << std::endl;
	MK11::IAT = ParsePEHeader();

	if (SettingsMgr->FORCE_CHECK_VER)
	{
		std::cout << "==bGetGameVersionShort==" << std::endl;
		if (SettingsMgr->pGameVer.empty())
		{
			std::cout << "GameVer Not Specified. Please Add Pattern to ini file!" << std::endl;
		}
		else
		{
			uint64_t* lpPattern = FindPattern(GetModuleHandleA(NULL), SettingsMgr->pGameVer);
			if (!lpPattern)
			{
				std::cout << "Couldn't find Game Version Pattern" << std::endl;
			}
			else
			{
				std::cout << "Game Version Pattern found at: " << std::hex << lpPattern << std::dec << std::endl;
				lpPattern = (uint64_t*)((uint64_t)(lpPattern)+0xF);
				uint64_t offset = 0;
				memcpy(&offset, lpPattern, 4);
				offset += ((uint64_t)lpPattern) + 4;
				std::cout << std::hex << "Game Version Offset: " << offset << std::endl;
				MK11::lpGameVersion = (uint64_t*)offset;
			}
		}

		std::cout << "==bGetGameVersion==" << std::endl;
		if (SettingsMgr->pGameVerFull.empty())
		{
			std::cout << "GameVerFull Not Specified. Please Add Pattern to ini file!" << std::endl;
		}
		else
		{
			uint64_t* lpPattern = FindPattern(GetModuleHandleA(NULL), SettingsMgr->pGameVerFull);
			if (!lpPattern)
			{
				std::cout << "Couldn't find Game FullVersion Pattern" << std::endl;
			}
			else
			{
				std::cout << "Game FullVersion Pattern found at: " << std::hex << lpPattern << std::dec << std::endl;
				lpPattern = (uint64_t*)((uint64_t)lpPattern + 0x1C);
				uint64_t offset = 0;
				memcpy(&offset, lpPattern, 4);
				offset += (uint64_t)lpPattern + 4;
				std::cout << std::hex << "Game FullVersion Offset: " << offset << std::endl;
				MK11::lpGameVersionFull = (uint64_t*)offset;
			}

		}

		if (SettingsMgr->bModLoader)
		{
			std::cout << "==bModLoader==" << std::endl;
			uint64_t CreateFileW = (uint64_t)MK11::GetLibProcFromNT(MK11::sLFS.ModLoader);
			if (!CreateFileW)
			{
				std::cout << "Couldn't find CreateFileW in kernel32.dll!" << std::endl;
			}
			else
			{
				const char* CFWPattern = HexToString(CreateFileW).c_str();
				MK11::sActiveMods.bModLoader = true;
				int32_t patched = 0;

				std::cout << "Patching " << MK11::sLFS.ModLoader.LibName << "[" << MK11::sLFS.ModLoader.ProcName << "] (" << std::hex << CreateFileW << ")" << std::endl;

				uint64_t lpPattern = (uint64_t)FindPattern(GetModuleHandleA(NULL), CFWPattern);
				while (lpPattern)
				{
					Patch<uint64_t>(GetGameAddr(lpPattern), (uint64_t)MK11Hooks::CreateFileProxy);
					std::cout << "Patched at " << std::hex << lpPattern << std::endl;
					patched++;
					lpPattern = (uint64_t)FindPattern(GetModuleHandleA(NULL), CFWPattern);
				}
				if (!patched)
				{
					std::cout << "Couldn't patch CreateFileW!" << std::endl;
				}
			}
			
		}
	}

	if (1) // CURL_TEST
	{
		std::cout << "==CURLLLLLLL==" << std::endl;
		uint64_t Func = (uint64_t)MK11::GetLibProcFromNT(MK11::sLFS.CurlSetOpt);
		uint64_t Func2 = (uint64_t)MK11::GetLibProcFromNT(MK11::sLFS.CurlPerform);
		if (Func && Func2)
		{
			std::cout << "Found " << MK11::sLFS.CurlSetOpt.LibName << "[" << MK11::sLFS.CurlSetOpt.ProcName << "] (" << std::hex << Func << ")" << std::endl;
			std::cout << "Found " << MK11::sLFS.CurlPerform.LibName << "[" << MK11::sLFS.CurlPerform.ProcName << "] (" << std::hex << Func << ")" << std::endl;
			// Do Stuff
		}
	}


	if (SettingsMgr->bDisableAntiCheatEngine)
	{
		std::cout << "==bDisableAntiCheatEngine==" << std::endl;
		uint64_t Func = (uint64_t)MK11::GetLibProcFromNT(MK11::sLFS.AntiCheatEngine);
		if (Func)
		{
			std::cout << "Patching " << MK11::sLFS.AntiCheatEngine.LibName << "[" << MK11::sLFS.AntiCheatEngine.ProcName << "] (" << std::hex << Func << ")" << std::endl;
			Patch(Func, 0xC3);
			Patch(Func + 1, 0x90909090);
			MK11::sActiveMods.bAntiCheatEngine = true;
			std::cout << "Anti Cheat Engine Patched" << std::endl;
		}
		else
		{
			std::cout << "Disabling Anti Cheat Engine" << std::endl;
		}
	}

	if (SettingsMgr->bDisableAntiCVD)
	{
		std::cout << "==bDisableAntiCVD==" << std::endl;
		if (SettingsMgr->pCVD1.empty())
		{
			std::cout << "CVD1 Not Specified. Please Add Pattern to ini file!" << std::endl;
		}
		else if (SettingsMgr->pCVD2.empty())
		{
			std::cout << "CVD2 Not Specified. Please Add Pattern to ini file!" << std::endl;
		}
		else
		{
			uint64_t* lpCVDPattern = FindPattern(GetModuleHandleA(NULL), SettingsMgr->pCVD1);
			uint64_t* lpCVD2Pattern = FindPattern(GetModuleHandleA(NULL), SettingsMgr->pCVD2);
			if (!lpCVDPattern)
			{
				std::cout << "Couldn't find CVD1 Pattern" << std::endl;
			}
			else if (!lpCVD2Pattern)
			{
				std::cout << "Couldn't find CVD2 Pattern" << std::endl;
			}
			else
			{
				uint64_t hook_address = (uint64_t)lpCVDPattern;
				std::cout << "CVD1 Pattern found at: " << std::hex << lpCVDPattern << std::dec << std::endl;
				Patch(GetGameAddr(hook_address), (uint16_t)0xC039); // cmp eax, eax
				Patch(GetGameAddr(hook_address) + 2, (uint16_t)0x9090); // Nop
				MK11::sActiveMods.bAntiCVD1 = true;
				std::cout << "Anti CVD1 Patched" << std::endl;

				hook_address = (uint64_t)lpCVD2Pattern;
				std::cout << "CVD2 Pattern found at: " << std::hex << lpCVD2Pattern << std::dec << std::endl;
				Patch(GetGameAddr(hook_address), (uint8_t)0xC3); // ret
				MK11::sActiveMods.bAntiCVD2 = true;
				std::cout << "Anti CVD2 Patched" << std::endl;
			}
		}

	}

	printf("TESTTTTTTTTTTTTTTTTTTTTT\n");
	InjectHook(GetGameAddr(0x140D35FC2), GameTramp->Jump(MK11Hooks::GetItemName), PATCH_CALL);
	InjectHook(GetGameAddr(0x140D35D9E), GameTramp->Jump(MK11Hooks::GetItemPriceString), PATCH_CALL);
	InjectHook(GetGameAddr(0x140D35D20), GameTramp->Jump(MK11Hooks::GetItemPrice), PATCH_CALL);
}

void UnlockerPipe()
{
	HANDLE hPipe;
	DWORD dwWritten;
	DWORD MAX_BUF_SIZE = 256;
	DWORD dwRead;

	hPipe = CreateNamedPipe(MKPIPE,
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
		2,
		MAX_BUF_SIZE * 5,
		0,
		-1, //NMPWAIT_USE_DEFAULT_WAIT,
		NULL
	);

	if (hPipe != INVALID_HANDLE_VALUE)
	{
		std::cout << "Pipe::Created" << std::endl;
	}
	else
	{
		std::cout << "Pipe::Failed To Create" << std::endl;
		return;
	}
	while (1)
	{
		std::cout << "Pipe::Awaiting Connection" << std::endl;
		std::cout << ConnectNamedPipe(hPipe, NULL) << std::endl;
		std::cout << "Pipe::Connection Received" << std::endl;

		while (1)
		{
			char buffer[256];
			ReadFile(hPipe, buffer, 256, &dwRead, NULL);
			if (!dwRead)
				break;
			FlushFileBuffers(hPipe);
			std::string received(buffer, dwRead);
			std::cout << "Pipe::Received (" << dwRead << "): " << received << "<-" << std::endl;

			uint64_t res;
			MK11Hooks::ProcVR2Function(0x1431B1FD0, &res, received.c_str());
			char* hash = (char*)res;

			std::string to_send(hash);
			to_send += "\n";
			printf("Pipe::Sending: %s\n", to_send.c_str());

			WriteFile(hPipe, to_send.c_str(), to_send.length(), &dwWritten, NULL);
			FlushFileBuffers(hPipe);
		
		}
	
		std::cout << "Pipe::Disconnected" << std::endl;
		DisconnectNamedPipe(hPipe);
	}
	

}

void ProcessSettings()
{
	// KeyBinds
	SettingsMgr->iVKCheats		= StringToVK(SettingsMgr->hkCheats);
	SettingsMgr->iVKtimestop	= StringToVK(SettingsMgr->hkTimestop);
	SettingsMgr->iVKMenuToggle	= StringToVK(SettingsMgr->hkMenu);
	SettingsMgr->iVKMenuInfo	= StringToVK(SettingsMgr->hkInfo);

	// DLL Procs
	MK11::sLFS.ModLoader		= MK11::ParseLibFunc(SettingsMgr->szModLoader);
	MK11::sLFS.AntiCheatEngine	= MK11::ParseLibFunc(SettingsMgr->szAntiCheatEngine);
	MK11::sLFS.CurlSetOpt		= MK11::ParseLibFunc("libcurl.curl_easy_setopt");
	MK11::sLFS.CurlPerform		= MK11::ParseLibFunc("libcurl.curl_easy_perform");

	std::cout << "Parsed Settings" << std::endl;
}

void DX12Error()
{
	MessageBoxA(NULL, "ASIMK11 doesn't officially support DX12 version of the game. It may misbehave.", "ASIMK11", MB_ICONEXCLAMATION);
}

void OnInitializeHook()
{
	std::string expected_process("MK11.exe");
	std::string expected_process_dx12("MK11_DX12.exe");
	std::string process_name = GetProcessName();

	if (process_name != expected_process)
	{
		if (process_name == expected_process_dx12)
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DX12Error, NULL, NULL, NULL);
	}

	SettingsMgr->Init();

	if (SettingsMgr->bEnableConsoleWindow)
	{
		CreateConsole(SettingsMgr->bPauseOnStart);
	}

	ProcessSettings(); // Parse Settings
	PreGameHooks(); // Queue Blocker
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HooksMain, NULL, NULL, NULL);
}

bool APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpRes)
{
	HHOOK hook_ = nullptr;
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, MenuGuiMainThread, hModule, 0, nullptr);
		OnInitializeHook();
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