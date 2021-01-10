#pragma once
typedef __int64 int64;
#include <string>
#include "windows.h"
#include "../utils/MemoryMgr.h"
#include "../utils/Trampoline.h"
#include "../utils/Patterns.h"
#include "eSettingsManager.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <chrono>

int64 GetGameEntryPoint();
int64 GetUser32EntryPoint();
int64 GetModuleEntryPoint(const char* name);
int64 GetGameAddr(__int64 addr);
int64 GetUser32Addr(__int64 addr);
int64 GetModuleAddr(__int64 addr, const char* name);
std::string toLower(std::string s);
std::string GetProcessName();
std::string GetDirName();
HMODULE AwaitHModule(const char* name, uint64_t timeout=0);
uint64_t stoui64h(std::string szString);
uint64_t* find_pattern(void* handle, std::string_view bytes);
void SetCheatPattern(std::string pattern, std::string name, uint64_t** lpPattern);
template <class CamType>
void ListenCamHotkey(int VK_btn, CamType* addr, bool inc, double speed = SettingsMgr->fSpeed, float WaitDuration = SettingsMgr->fCamHold) // Eventually Change this Function to allow for Holding Shift by returning char as state. 0 = No, 1 = yes, -1 = shift.
{
	if (GetAsyncKeyState(VK_btn))
	{
		if (!inc)
			speed = -speed;
		*addr += speed;
		auto start = std::chrono::system_clock::now();
		while (GetAsyncKeyState(VK_btn))
		{
			std::chrono::duration<double> now = std::chrono::system_clock::now() - start;
			if (now.count() > WaitDuration)
			{
				*addr += speed;
				start = std::chrono::system_clock::now();
			}
		}
	}
}