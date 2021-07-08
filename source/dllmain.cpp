#include "includes.h"
#include "pch.h"
#include "code/mk11.h"
#include <tlhelp32.h> 

#define MKPIPE TEXT("\\\\.\\pipe\\MK11Unlocker")

using namespace Memory::VP;
using namespace hook;

LibMap IAT{};
Trampoline* GameTramp, * User32Tramp;

void UnlockerPipe();
void BlockerEvents();
void SetCheatPattern(std::string pattern, std::string name, uint64_t** lpPattern);

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

			if (sCamStruct.bTimestopEnabled && wParam == (SettingsMgr->iVKtimestop))
			{
				sCamStruct.bTimestopActive = !sCamStruct.bTimestopActive;
				if (sCamStruct.bTimestopActive)
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
				printf("Output: %X (%s)\n", res, hash);
			}

			// Cheats
			if (SettingsMgr->bEnableCheats)
			{
				if (GetAsyncKeyState(SettingsMgr->iVKCheats)) // Button Down
				{
					if (wParam == '1' && sCheatsStruct.lpMercy)
					{
						std::cout << "pMercyAnyTime ";
						if (sCheatsStruct.bMercy)
						{
							std::cout << "Disabled";
							Patch<uint16_t>(GetGameAddr((uint64_t)sCheatsStruct.lpMercy), 0x5340); // push rbx // Change this to read it automatically from memory, or verify against a hash or something
							Patch<uint8_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpMercy) + 2), 0x48);
						}
						else
						{
							std::cout << "Enabled";
							Patch<uint16_t>(GetGameAddr((uint64_t)sCheatsStruct.lpMercy), 0x01B0); // mov al, 1
							Patch<uint8_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpMercy) + 2), 0xC3);
						}
						std::cout << std::endl;
						sCheatsStruct.bMercy = !sCheatsStruct.bMercy;
					}
					if (wParam == '2' && sCheatsStruct.lpGround)
					{
						std::cout << "pNoGroundReq ";
						if (sCheatsStruct.bGround)
						{
							std::cout << "Disabled";
							Patch<uint16_t>(GetGameAddr((uint64_t)sCheatsStruct.lpGround), 0x5340); // mov al, 1
							Patch<uint8_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpGround) + 2), 0x48);
						}
						else
						{
							std::cout << "Enabled";
							Patch<uint16_t>(GetGameAddr((uint64_t)sCheatsStruct.lpGround), 0x01B0); // mov al, 1
							Patch<uint8_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpGround) + 2), 0xC3);
						}
						std::cout << std::endl;
						sCheatsStruct.bGround = !sCheatsStruct.bGround;
					}
					if (wParam == '3' && sCheatsStruct.lpBrut)
					{
						std::cout << "pNoBrutalityReq ";
						if (sCheatsStruct.bBrut)
						{
							std::cout << "Disabled";
							Patch<uint16_t>(GetGameAddr((uint64_t)sCheatsStruct.lpBrut), 0x8948);
							Patch<uint8_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpBrut) + 2), 0x6C);
							if (sCheatsStruct.lpBrutB)
							{
								Patch<uint16_t>(GetGameAddr((uint64_t)sCheatsStruct.lpBrutB), 0x5340);
								Patch<uint8_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpBrutB) + 2), 0x48);
							}
						}
						else
						{
							std::cout << "Enabled";
							Patch<uint16_t>(GetGameAddr((uint64_t)sCheatsStruct.lpBrut), 0x01B0); // mov al, 1
							Patch<uint8_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpBrut) + 2), 0xC3);
							if (sCheatsStruct.lpBrutB)
							{
								Patch<uint16_t>(GetGameAddr((uint64_t)sCheatsStruct.lpBrutB), 0x01B0); // mov al, 1
								Patch<uint8_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpBrutB) + 2), 0xC3);
							}
						}
						std::cout << std::endl;
						sCheatsStruct.bBrut = !sCheatsStruct.bBrut;
					}
					if (wParam == '4' && sCheatsStruct.lpMeteor)
					{
						std::cout << "pMeteorAlwaysSpawns ";
						if (sCheatsStruct.bMeteor)
						{
							std::cout << "Disabled";
							Patch<uint16_t>(GetGameAddr((uint64_t)sCheatsStruct.lpMeteor), 0x1C75); // jne +1E
						}
						else
						{
							std::cout << "Enabled";
							Nop(GetGameAddr((uint64_t)sCheatsStruct.lpMeteor), 2);
						}
						std::cout << std::endl;
						sCheatsStruct.bMeteor = !sCheatsStruct.bMeteor;
					}
					if (wParam == '5' && sCheatsStruct.lpDizzy && sCheatsStruct.lpFatality)
					{
						std::cout << "pFatalityAlwaysAvailable ";
						if (sCheatsStruct.bDizzy)
						{
							std::cout << "Disabled";
							Patch<uint64_t>(GetGameAddr((uint64_t)sCheatsStruct.lpDizzy), 0x24748948); // mov [rsp+10],rsi
							Patch<uint64_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpDizzy) + 4), 0x48571024); // push rdi / sub rsp, 20

							Patch<uint64_t>(GetGameAddr((uint64_t)sCheatsStruct.lpDizzy), 0x83485340);
							Patch<uint64_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpDizzy) + 4), 0x8B4820EC);
						}
						else
						{
							std::cout << "Enabled";
							Patch<uint64_t>(GetGameAddr((uint64_t)sCheatsStruct.lpDizzy), 0x1C0C748); // mov rax, 1
							Patch<uint64_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpDizzy) + 4), 0xC3000000); // ret

							Patch<uint64_t>(GetGameAddr((uint64_t)sCheatsStruct.lpFatality), 0x1C0C748);
							Patch<uint64_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpFatality) + 4), 0xC3000000);
						}
						std::cout << std::endl;
						sCheatsStruct.bFatality = sCheatsStruct.bDizzy = !sCheatsStruct.bDizzy;
					}
					if (wParam == '6' && sCheatsStruct.lpFatCombo)
					{
						std::cout << "pComboInFatality ";
						if (sCheatsStruct.bFatCombo)
						{
							std::cout << "Disabled";
							Patch<uint64_t>(GetGameAddr((uint64_t)sCheatsStruct.lpFatCombo), 0x28EC8348);
						}
						else
						{
							std::cout << "Enabled";
							Patch<uint64_t>(GetGameAddr((uint64_t)sCheatsStruct.lpFatCombo), 0xC3C03148); // xor rax, rax / ret
						}
						std::cout << std::endl;
						sCheatsStruct.bFatCombo = !sCheatsStruct.bFatCombo;
					}
					if (wParam == '7' && sCheatsStruct.lpNoBlock)
					{
						std::cout << "pBlockingDisabled ";
						if (sCheatsStruct.bNoBlock)
						{
							std::cout << "Disabled";
							Patch<uint64_t>(GetGameAddr((uint64_t)sCheatsStruct.lpNoBlock), 0x245C8948);
						}
						else
						{
							std::cout << "Enabled";
							Patch<uint64_t>(GetGameAddr((uint64_t)sCheatsStruct.lpNoBlock), 0xC3C03148); // xor rax, rax / ret
						}
						std::cout << std::endl;
						sCheatsStruct.bNoBlock = !sCheatsStruct.bNoBlock;
					}
					if (wParam == '8' && sCheatsStruct.lpFatalBlow)
					{
						std::cout << "pFatalBlowAtMaxHealth ";
						if (sCheatsStruct.bFatalBlow)
						{
							std::cout << "Disabled";
							Patch<uint64_t>(GetGameAddr((uint64_t)sCheatsStruct.lpFatalBlow), 0xA9058B48);
							Patch<uint64_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpFatalBlow) + 4), 0x8302ED78);
						}
						else
						{
							std::cout << "Enabled";
							Patch<uint64_t>(GetGameAddr((uint64_t)sCheatsStruct.lpFatalBlow), 0x1C0C748); // mov rax, 1
							Patch<uint64_t>(GetGameAddr(((uint64_t)sCheatsStruct.lpFatalBlow) + 4), 0xC3000000); // ret
						}
						std::cout << std::endl;
						sCheatsStruct.bFatalBlow = !sCheatsStruct.bFatalBlow;
					}
				}
			}
		}
	}
	return CallNextHookEx(0, code, wParam, lParam);
}

void CreateConsole(bool bFreeze = false)
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
			sCamStruct.bTimestopEnabled = true;
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
			uint64_t fix_len_add = (uint64_t)lpIntroSwap;
			Patch<uint16_t>(fix_len_add, 0x428d); // lea eax, [rdx+
			Patch<uint8_t>(fix_len_add + 2, 0x2A); // 0x2A]
			uint64_t hook_address = fix_len_add + 0x35;
			InjectHook(hook_address, GameTramp->Jump(MK11Hooks::IntroSwap), PATCH_CALL);
			sActiveMods.bIntroSwap = true;
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
		SetCheatPattern(SettingsMgr->pMercyAnyTime,				"pMercyAnyTime",			&(sCheatsStruct.lpMercy));
		SetCheatPattern(SettingsMgr->pNoGroundReq,				"pNoGroundReq",				&(sCheatsStruct.lpGround));
		SetCheatPattern(SettingsMgr->pNoBrutalityReq,			"pNoBrutalityReq",			&(sCheatsStruct.lpBrut));
		SetCheatPattern(SettingsMgr->pBrutalityAlwaysCorrect,	"pBrutalityAlwaysCorrect",	&(sCheatsStruct.lpBrutB));
		SetCheatPattern(SettingsMgr->pMeteorAlwaysSpawns,		"pMeteorAlwaysSpawns",		&(sCheatsStruct.lpMeteor));
		SetCheatPattern(SettingsMgr->pFatalityAlwaysAvailable,	"pFatalityAlwaysAvailable", &(sCheatsStruct.lpFatality));
		SetCheatPattern(SettingsMgr->pDizzyAlwaysAvailable,		"pDizzyAlwaysAvailable",	&(sCheatsStruct.lpDizzy));
		SetCheatPattern(SettingsMgr->pComboInFatality,			"pComboInFatality",			&(sCheatsStruct.lpFatCombo));
		SetCheatPattern(SettingsMgr->pBlockingDisabled,			"pBlockingDisabled",		&(sCheatsStruct.lpNoBlock));
		SetCheatPattern(SettingsMgr->pFatalBlowAtMaxHealth,		"pFatalBlowAtMaxHealth",	&(sCheatsStruct.lpFatalBlow));
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)BlockerEvents, NULL, NULL, NULL);
}

void SyncAwait(std::string(*function)(void), const char* string, bool not = false)
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

	if (SettingsMgr->FORCE_CHECK_VER)
	{
		std::cout << "==bGetGameVersionShort==" << std::endl;
		if (SettingsMgr->pGameVer.empty())
		{
			std::cout << "GameVer Not Specified. Please Add Pattern to ini file!" << std::endl;
		}
		else
		{
			uint64_t* lpPattern = FindPattern(GetModuleHandleA(NULL), "49 63 4C 24 08 85 C9 74 12");
			if (!lpPattern)
			{
				std::cout << "Couldn't find Game Version Pattern" << std::endl;
			}
			else
			{
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
			uint64_t* lpPattern = FindPattern(GetModuleHandleA(NULL), "48 89 84 24 ? ? ? ? 4c 8b f1 33 DB 89 5C 24");
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
			IAT = ParsePEHeader();
			std::cout << "==bModLoader==" << std::endl;
			uint64_t CreateFileW = IAT["kernel32.dll"]["CreateFileW"];
			if (!CreateFileW)
			{
				std::cout << "Couldn't find CreateFileW in kernel32.dll!";
			}
			else
			{
				const char* CFWPattern = HexToString(CreateFileW).c_str();
				sActiveMods.bModLoader = true;
				int32_t patched = 0;

				std::cout << "Patching Kernel32.CreateFileW (" << std::hex << CreateFileW << ")" << std::endl;

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


	if (SettingsMgr->bDisableAntiCheatEngine)
	{
		std::cout << "==bDisableAntiCheatEngine==" << std::endl;
		uint64_t Func = (uint64_t)IAT["user32.dll"]["EnumChildWindows"];
		if (Func)
		{
			std::cout << "Found at " << std::hex << Func << std::dec << std::endl;
			Patch(Func, 0xC3);
			Patch(Func + 1, 0x90909090);
			sActiveMods.bAntiCheatEngine = true;
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
				sActiveMods.bAntiCVD1 = true;
				std::cout << "Anti CVD1 Patched" << std::endl;

				hook_address = (uint64_t)lpCVD2Pattern;
				std::cout << "CVD2 Pattern found at: " << std::hex << lpCVD2Pattern << std::dec << std::endl;
				Patch(GetGameAddr(hook_address), (uint8_t)0xC3); // ret
				sActiveMods.bAntiCVD2 = true;
				std::cout << "Anti CVD2 Patched" << std::endl;
			}
		}

	}
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
			MK11Hooks::procVR2Function(0x1431B1FD0, &res, received.c_str());
			char* hash = (char*)res;

			std::string to_send(hash);
			to_send += "\n";
			printf("Pipe::Sending: %s\n", to_send);

			WriteFile(hPipe, to_send.c_str(), to_send.length(), &dwWritten, NULL);
			FlushFileBuffers(hPipe);
		
		}
	
		std::cout << "Pipe::Disconnected" << std::endl;
		DisconnectNamedPipe(hPipe);
	}
	

}

void OnInitializeHook()
{
	std::string expected_process("MK11.exe");
	std::string expected_process_dx12("MK11_DX12.exe");
	std::string process_name = GetProcessName();

	if (process_name != expected_process)
	{
		if (process_name == expected_process_dx12)
			MessageBoxA(NULL, "ASIMK11 doesn't officially support DX12 version of the game. It may misbehave.", "ASIMK11", MB_ICONEXCLAMATION);
	}

	SettingsMgr->Init();
	if (SettingsMgr->bEnableConsoleWindow)
	{
		CreateConsole(SettingsMgr->bPauseOnStart);
		std::cout << "Console Created!" << std::endl;
	}

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