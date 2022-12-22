#pragma once
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
#include <map>

#define			RVAtoLP( base, offset )		((PBYTE)base + offset)
#define			FuncMap						std::map<std::string, ULONGLONG>
#define			LibMap						std::map<std::string, FuncMap>
typedef			__int64						int64;

int64			GetGameEntryPoint();
int64			GetUser32EntryPoint();
int64			GetModuleEntryPoint(const char* name);
int64			GetGameAddr(__int64 addr);
int64			GetUser32Addr(__int64 addr);
int64			GetModuleAddr(__int64 addr, const char* name);
std::string		GetProcessName();
std::string		GetDirName();
std::string		toLower(std::string s);
std::string		toUpper(std::string s);
std::string		GetFileName(std::string filename);
HMODULE			AwaitHModule(const char* name, uint64_t timeout = 0);
uint64_t		stoui64h(std::string szString);
uint64_t*		FindPattern(void* handle, std::string_view bytes);
uint64_t		HookPattern(std::string Pattern, const char* PatternName, void* HookProc, int64_t PatternOffset = 0, PatchTypeEnum PatchType = PatchTypeEnum::PATCH_CALL, uint64_t PrePat = NULL, uint64_t* Entry = nullptr);
uint64_t		GetAbsoluteDestination(uint64_t Caller, uint64_t Offset = 1, uint64_t FuncLen = 5, uint16_t size = 4);
int32_t			RelativeToAbsoluteAddress(uint64_t Caller, uint64_t Offset, uint16_t size);
void			SetCheatPattern(std::string pattern, std::string name, uint64_t** lpPattern);
LibMap			ParsePEHeader();
int				StringToVK(std::string);
void			RaiseException(const char*, int64_t = 1);
bool			IsHex(char);
bool			IsBase(char c, int = 16);


//Templates

template<typename T>
std::string HexToString(T Value)
{
	uint64_t Pointer = (uint64_t)Value;
	uint8_t size = sizeof(Value);
	std::string szHex = "";
	for (int i = 0; i < size; i++)
	{
		char string[4];
		sprintf(string, "%02X ", uint8_t(Pointer >> (i * 8)));
		szHex += string;

	}
	return szHex;
}

template<typename T>
std::string HexToStringBE(T Value)
{
	uint64_t Pointer = (uint64_t)Value;
	uint8_t size = sizeof(T);
	std::string szHex = "";
	for (int i = size -1; i >= 0; i--)
	{
		char string[4];
		sprintf(string, "%02x", uint8_t(Pointer >> (i * 8)));
		szHex += string;

	}
	return szHex;
}


template <class T> T PopQueue(std::vector<T>& v)
{
	T front = v.front();
	v.erase(v.begin());
	return front;
}