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
#include <map>

#define RVAtoLP( base, offset ) ((PBYTE)base + offset)

int64 GetGameEntryPoint();
int64 GetUser32EntryPoint();
int64 GetModuleEntryPoint(const char* name);
int64 GetGameAddr(__int64 addr);
int64 GetUser32Addr(__int64 addr);
int64 GetModuleAddr(__int64 addr, const char* name);
std::string toLower(std::string s);
std::string toUpper(std::string s);
std::string GetFileName(std::string filename);
std::string GetProcessName();
std::string GetDirName();
HMODULE AwaitHModule(const char* name, uint64_t timeout=0);
uint64_t stoui64h(std::string szString);
uint64_t* FindPattern(void* handle, std::string_view bytes);
void SetCheatPattern(std::string pattern, std::string name, uint64_t** lpPattern);
void ParsePEHeader();
typedef std::map<std::string, ULONGLONG> FuncMap;
extern std::map<std::string, FuncMap> IAT;

template<typename T>
std::string HexToString(T Value)
{
	uint8_t size = sizeof(T);
	std::string szHex = "";
	for (int i = 0; i < size; i++)
	{
		char string[4];
		sprintf(string, "%02X ", uint8_t(Value >> (i * 8)));
		szHex += string;

	}
	return szHex;
}