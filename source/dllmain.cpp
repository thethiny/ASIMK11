#include "includes.h"
#include "pch.h"
#include "code/mk11.h"

using namespace Memory::VP;
using namespace hook;

#define MKPIPE TEXT("\\\\.\\pipe\\MK11Unlocker")

void UnlockerPipe();
void CamListener();
void SetCheatPattern(std::string pattern, std::string name, uint64_t** lpPattern);


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

typedef uint64_t* (__fastcall vr2_function)(uint64_t seed, uint64_t* result_hash, const char* to_hash);
vr2_function* vr2_game_function = (vr2_function*)(GetGameAddr(0x3D9F760));
uint64_t vr_thisptr = NULL;

uint64_t* __fastcall vr2_proxy_function(uint64_t seed, uint64_t* result_hash, const char* to_hash)
{
	printf("seed: %X\n", seed);
	printf("result_hash: %X\n", result_hash);
	printf("to_hash: %X (%s)\n", to_hash, to_hash);

	uint64_t* return_value = vr2_game_function(0, result_hash, to_hash);
	printf("hash: %X (%s)\n", return_value, (char*)*return_value);

	return return_value;
}


HANDLE __stdcall CreateFileProxy(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	wchar_t* wcFileName = (wchar_t*)lpFileName;
	std::wstring fileName(wcFileName, wcslen(wcFileName));
	if (!wcsncmp(wcFileName, L"..", 2))
	{
		std::wstring wsSwapFolder = L"MKSwap";
		std::wstring newFileName = L"..\\" + wsSwapFolder + fileName.substr(2, fileName.length() - 2);
		if (std::filesystem::exists(newFileName.c_str()))
		{
			wprintf(L"Loading %s from %s\n", wcFileName, wsSwapFolder.c_str());
			return CreateFileW(newFileName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);;
		}
	}
	return CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

struct CamStruct {
	float fX=0, fY=0, fZ=0; // Change to pointers
	float fPOV=0;
	int32_t iYaw=0, iPitch=0, iRot=0;
	bool bLogCam = false; //Make it a flag to control prints
	bool bCamActive = false;
	uint32_t uCamSpeed = 5;
} sCamStruct;

uint64_t lpCamFunction;
uint64_t lpCaughtCamPtr = NULL;
uint32_t CamOffset = NULL;

struct CheatsStruct {
	uint64_t *lpMercy, *lpGround, *lpBrut, *lpBrutB, *lpMeteor;
	bool bMercy = false, bGround = false, bBrut = false, bBrutB = false, bMeteor = false;
} sCheatsStruct;

uint64_t CamFunction(uint64_t lpCamPtr, uint64_t fValue)
{
	if (lpCamPtr != lpCaughtCamPtr)
	{
		lpCaughtCamPtr = lpCamPtr;
		std::cout << "Intercepted Cam Ptr: " << std::hex << lpCamPtr << std::endl;
	}

	float* fX = (float*)(lpCamPtr + CamOffset), *fY = (float*)(lpCamPtr + CamOffset + 4), *fZ = (float*)(lpCamPtr + CamOffset + 8);
	float* fPOV = (float*)(lpCamPtr + CamOffset + 24);
	int32_t* iYaw = (int32_t*)(lpCamPtr + CamOffset + 16), *iPitch = (int32_t*)(lpCamPtr + CamOffset + 12), *iRot = (int32_t*)(lpCamPtr + CamOffset + 20);

	if (!sCamStruct.bCamActive)
	{
		sCamStruct.fX = *fX;
		sCamStruct.fY = *fY;
		sCamStruct.fZ = *fZ;
		sCamStruct.fPOV = *fPOV;
		sCamStruct.iYaw = *iYaw;
		sCamStruct.iPitch = *iPitch;
		sCamStruct.iRot = *iRot;
	}
	else
	{
		*fX = sCamStruct.fX;
		*fY = sCamStruct.fY;
		*fZ = sCamStruct.fZ;
		*fPOV = sCamStruct.fPOV;
		*iYaw = sCamStruct.iYaw;
		*iPitch = sCamStruct.iPitch;
		*iRot = sCamStruct.iRot;
	}

	if (sCamStruct.bLogCam)
	{
		printf("Cam::XYZ: %f, %f, %f\n", *fX, *fY, *fZ);
		printf("Cam::POV: %f\n", *fPOV);
		printf("Cam::Yaw Pitch Rotation: %f, %f, %f\n", *iYaw, *iPitch, *iRot);
	}
	

	return ((uint64_t(__fastcall*)(uint64_t, uint64_t))GetGameAddr(lpCamFunction))(lpCamPtr, fValue);
}


void HooksMain()
{
	HMODULE hUser32 = NULL;
	HMODULE hKernelBase = NULL;
	HMODULE hKernel32 = NULL;

	hUser32 = AwaitHModule("user32.dll");
	hKernelBase = AwaitHModule("kernelbase.dll");
	hKernel32 = AwaitHModule("kernel32.dll");

	printf("Initializing ImGui Menu\n");
	GuiMenu->Initialize();


	Trampoline* game_tramp = Trampoline::MakeTrampoline(GetModuleHandle(nullptr));
	Trampoline* user32_tramp = Trampoline::MakeTrampoline(hUser32);
	std::cout << "Generated Trampolines" << std::endl;

	if (SettingsMgr->bDisableAntiCheatEngine)
	{
		std::cout << "==bDisableAntiCheatEngine==" << std::endl;
		char* antiCEFunctionName = "EnumChildWindows";
		std::cout << "Patching " << antiCEFunctionName << std::endl;
		uint64_t hook_address = (uint64_t)GetProcAddress(hUser32, antiCEFunctionName);
		std::cout << "Found at " << std::hex << hook_address << std::dec << std::endl;
		Patch(hook_address, 0xC3);
		Patch(hook_address + 1, 0x90909090);
		std::cout << "Anti Cheat Engine Patched" << std::endl;
	}

	if (SettingsMgr->bDisableAntiCVD1)
	{
		std::cout << "==bDisableAntiCVD1==" << std::endl;
		if (!SettingsMgr->iCVD1.empty())
		{
			uint64_t hook_address = stoui64h(SettingsMgr->iCVD1);
			std::cout << "Patching at " << std::hex << hook_address << std::dec << std::endl;
			Patch(GetGameAddr(hook_address), (uint16_t)0xC039); // cmp eax, eax
			Patch(GetGameAddr(hook_address) + 2, (uint16_t)0x9090); // Nop
			std::cout << "Anti CVD1 Patched" << std::endl;
		}
		else std::cout << "Address Not Specified. Please Add Address to ini file!" << std::endl;
		
	}

	if (SettingsMgr->bDisableAntiCVD2)
	{
		std::cout << "==bDisableAntiCVD2==" << std::endl;
		if (!SettingsMgr->iCVD2.empty())
		{
			uint64_t hook_address = stoui64h(SettingsMgr->iCVD2);
			std::cout << "Patching at " << std::hex << hook_address << std::dec << std::endl;
			Patch(GetGameAddr(hook_address), (uint8_t)0xC3); // ret
			std::cout << "Anti CVD2 Patched" << std::endl;
		}
		else std::cout << "Address Not Specified. Please Add Address to ini file!" << std::endl;
		
	}

	if (SettingsMgr->bPatchUnlocker)
	{
		std::cout << "==bPatchUnlocker==" << std::endl;
		std::cout << "Injecting VR2 Proxy Function" << std::endl;

		InjectHook(GetGameAddr(0x40BE75E), game_tramp->Jump(vr2_proxy_function), PATCH_CALL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)UnlockerPipe, NULL, NULL, NULL);

	}

	if (SettingsMgr->bModLoader)
	{
		std::cout << "==bModLoader==" << std::endl;
		if (!SettingsMgr->iModLoader.empty())
		{
			uint64_t hook_address = stoui64h(SettingsMgr->iModLoader);
			std::cout << "Patching at " << std::hex << hook_address << std::dec << std::endl;
			Patch<uint64_t>(GetGameAddr(hook_address), (uint64_t)CreateFileProxy);
		}
		else std::cout << "Address Not Specified. Please Add Address to ini file!" << std::endl;
		
	}

	if (SettingsMgr->bEnableCameraMod)
	{
		std::cout << "==bEnableCameraMod==" << std::endl;
		std::cout << "Searching for Pattern: " << SettingsMgr->pCam << std::endl;
		uint64_t* lpCamPattern = find_pattern(GetModuleHandleA(NULL), SettingsMgr->pCam);
		if (lpCamPattern != nullptr)
		{
			CamOffset = stoui64h(SettingsMgr->iCamOffset); // Set it once to reduce workload on cam function
			uint64_t lpHookfunction = ((uint64_t)lpCamPattern) + 0x35;
			std::cout << "Camera Pattern found at: " << std::hex << lpCamPattern << std::endl;
			std::cout << "Hooking function at: " << lpHookfunction << std::endl;
			uint32_t call_distance;
			memcpy(&call_distance, (uint32_t*)GetGameAddr(lpHookfunction + 1), 4);
			//std::cout << "Function Call Distance: " << call_distance << std::endl;
			lpCamFunction = call_distance + lpHookfunction + 0x5; // 0x5 is the size of cur opcodes
			std::cout << "Function to be called: " << lpCamFunction << std::endl;

			InjectHook(GetGameAddr(lpHookfunction), game_tramp->Jump(CamFunction), PATCH_CALL);
			std::cout << "Patched Camera Function" << std::dec << std::endl;

			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)CamListener, NULL, NULL, NULL);
			std::cout << "Launched Cam Thread" << std::endl;

		}
		else
		{
			std::cout << "Couldn't Patch Camera Function. Camera Mod Disabled" << std::endl;
		}
	}

	if (SettingsMgr->bEnableCheats)
	{
		std::cout << "==bEnableCheats==" << std::endl;
		SetCheatPattern(SettingsMgr->pMercyAnyTime, "pMercyAnyTime", &(sCheatsStruct.lpMercy));
		SetCheatPattern(SettingsMgr->pNoGroundReq, "pNoGroundReq", &(sCheatsStruct.lpGround));
		SetCheatPattern(SettingsMgr->pNoBrutalityReq, "pNoBrutalityReq", &(sCheatsStruct.lpBrut));
		SetCheatPattern(SettingsMgr->pBrutalityAlwaysCorrect, "pBrutalityAlwaysCorrect", &(sCheatsStruct.lpBrutB));
		SetCheatPattern(SettingsMgr->pMeteorAlwaysSpawns, "pMeteorAlwaysSpawns", &(sCheatsStruct.lpMeteor));
	}


	while (1)
	{
		if (GetAsyncKeyState(VK_F10) && SettingsMgr->bPatchUnlocker)
		{
			while (GetAsyncKeyState(VK_F10)); // wait

			uint64_t res;
			vr2_proxy_function(0, &res, "thethiny");
			char* hash = (char*)res;
			printf("Output: %X (%s)\n", res, hash);
		}

		// Cheats
		if (SettingsMgr->bEnableCheats)
		{
			while (GetAsyncKeyState(SettingsMgr->iVKCheats))
			{
				if (GetAsyncKeyState('1') && sCheatsStruct.lpMercy)
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
					while (GetAsyncKeyState('1')); // Wait
				}
				if (GetAsyncKeyState('2') && sCheatsStruct.lpGround)
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
					while (GetAsyncKeyState('2')); // Wait
				}
				if (GetAsyncKeyState('3') && sCheatsStruct.lpBrut)
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
					while (GetAsyncKeyState('3')); // Wait
				}
				if (GetAsyncKeyState('4') && sCheatsStruct.lpMeteor)
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
						Patch<uint16_t>(GetGameAddr((uint64_t)sCheatsStruct.lpMeteor), 0x9090); // nop, nop
					}
					std::cout << std::endl;
					sCheatsStruct.bMeteor = !sCheatsStruct.bMeteor;
					while (GetAsyncKeyState('4')); // Wait
				}
			}
		}
	}

}


void CamListener()
{
	while (1) // Read Hotkeys and Speed from ini
	{
		// ImGui Menu
		if (GetAsyncKeyState(SettingsMgr->iVKMenuToggle))
		{
			GuiMenu->ToggleActive();
			while (GetAsyncKeyState(SettingsMgr->iVKMenuToggle)); // Wait
		}
		if (GetAsyncKeyState(SettingsMgr->iVKCamToggle))
		{
			sCamStruct.bCamActive = !sCamStruct.bCamActive;
			if (sCamStruct.bCamActive)
			{
				std::cout << "Cam::On" << std::endl;
			}
			else 
			{
				std::cout << "Cam::Off" << std::endl;
			}

			while (GetAsyncKeyState(SettingsMgr->iVKCamToggle)); // wait until negative edge
		}
		if (sCamStruct.bCamActive) // Check for Buttons to perform cam stuff
		{
			ListenCamHotkey(SettingsMgr->iVKxp, &(sCamStruct.fX), true);
			ListenCamHotkey(SettingsMgr->iVKxm, &(sCamStruct.fX), false);
			ListenCamHotkey(SettingsMgr->iVKyp, &(sCamStruct.fY), true);
			ListenCamHotkey(SettingsMgr->iVKym, &(sCamStruct.fY), false);
			ListenCamHotkey(SettingsMgr->iVKzp, &(sCamStruct.fZ), true);
			ListenCamHotkey(SettingsMgr->iVKzm, &(sCamStruct.fZ), false);
			ListenCamHotkey(SettingsMgr->iVKfovp, &(sCamStruct.fPOV), true, SettingsMgr->fSpeed / 10);
			ListenCamHotkey(SettingsMgr->iVKfovm, &(sCamStruct.fPOV), false, SettingsMgr->fSpeed / 10);
			ListenCamHotkey(SettingsMgr->iVKrotp, &(sCamStruct.iRot), true);
			ListenCamHotkey(SettingsMgr->iVKrotm, &(sCamStruct.iRot), false);
			ListenCamHotkey(SettingsMgr->iVKpitchp, &(sCamStruct.iPitch), true);
			ListenCamHotkey(SettingsMgr->iVKpitchm, &(sCamStruct.iPitch), false);
			ListenCamHotkey(SettingsMgr->iVKyawp, &(sCamStruct.iYaw), true);
			ListenCamHotkey(SettingsMgr->iVKyawm, &(sCamStruct.iYaw), false);
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
			vr2_game_function(0x1431B1FD0, &res, received.c_str());
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
			MessageBoxA(NULL, "ASIMK11 doesn't support DX12 version of the game", "ASIMK11", 0);
		return;
	}

	SettingsMgr->Init();
	if (SettingsMgr->bEnableConsoleWindow)
	{
		CreateConsole(SettingsMgr->bPauseOnStart);
		std::cout << "Console Created!" << std::endl;
	}

	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)HooksMain, NULL, NULL, NULL);
}

bool APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID lpRes)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(nullptr, 0, MenuGuiMainThread, hModule, 0, nullptr);
		OnInitializeHook();
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return true;
}