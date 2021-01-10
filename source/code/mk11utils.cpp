#include "..\pch.h"
#include "mk11utils.h"

using namespace Memory::VP;
using namespace hook;


// Address Utilities

int64 GetGameEntryPoint()
{
	static __int64 addr = reinterpret_cast<__int64>(GetModuleHandle(nullptr));
	return addr;
}

int64 GetUser32EntryPoint()
{
	static __int64 addr = reinterpret_cast<__int64>(GetModuleHandle("user32.dll"));
	return addr;
}

int64 GetModuleEntryPoint(const char* name)
{
	static __int64 addr = reinterpret_cast<__int64>(GetModuleHandle(name));
	return addr;
}

int64 GetGameAddr(__int64 addr)
{
	if (addr > GetGameEntryPoint())
		return addr;
	return GetGameEntryPoint() + addr;
	//int64 new_addr = GetGameEntryPoint() + addr;
	/*if (new_addr - 0x140000000 <= 0)
	{
		return new_addr;
	}
	return new_addr - 0x140000000;*/
	//return GetGameEntryPoint() - 0x140000000 + addr;
	//return GetGameEntryPoint() + addr;
}

int64 GetUser32Addr(__int64 addr)
{
	return GetUser32EntryPoint() + addr;
}

int64 GetModuleAddr(__int64 addr, const char* name)
{
	return GetModuleEntryPoint(name) + addr;
}



// Functional Utilities

std::string toLower(std::string s)
{
	std::string new_string("");
	for (int i = 0; i < s.length(); i++)
	{
		new_string += std::tolower(s[i]);
	}
}

std::string GetProcessName()
{
	CHAR fileName[MAX_PATH + 1];
	DWORD chars = GetModuleFileNameA(NULL, fileName, MAX_PATH + 1);
	if (chars)
	{
		std::string basename;
		std::string filename = std::string(fileName);
		size_t pos = filename.find_last_of('\\');
		if (pos != -1)
		{
			basename = filename.substr(pos + 1);
			return basename;
		}
		return filename;
	}
	return "";
}

std::string GetDirName()
{
	CHAR fileName[MAX_PATH + 1];
	DWORD chars = GetModuleFileNameA(NULL, fileName, MAX_PATH + 1);
	if (chars)
	{
		std::string basename;
		std::string filename = std::string(fileName);
		size_t pos = filename.find_last_of('\\');
		if (pos != -1)
		{
			basename = filename.substr(0, pos);
			return basename;
		}
		return filename;
	}
	return "";
}

HMODULE AwaitHModule(const char* name, uint64_t timeout)
{
	HMODULE toAwait = NULL;
	auto start = std::chrono::system_clock::now();
	while (!toAwait)
	{
		if (timeout > 0)
		{
			std::chrono::duration<double> now = std::chrono::system_clock::now() - start;
			if (now.count()*1000 > timeout)
			{
				std::cout << "No Handle " << name << std::endl;
				return NULL;
			}
		}
		toAwait = GetModuleHandle(name);	
	}
	std::cout << "Obtained Handle for " << name << std::endl;
	return toAwait;
}

uint64_t stoui64h(std::string szString)
{
	return strtoull(szString.c_str(), nullptr, 16);
}


uint64_t* find_pattern(void* handle, std::string_view bytes)
{
	hook::pattern pCamPattern = hook::make_module_pattern(handle, bytes); // Make pattern external
	if (!pCamPattern.count_hint(1).empty())
	{
		return pCamPattern.get(0).get<uint64_t>(0);
	}
	return nullptr;
}

void SetCheatPattern(std::string pattern, std::string name, uint64_t** lpPattern)
{
	if (!pattern.empty())
	{
		std::cout << "Searching for " << name << ": " << pattern << std::endl;
		*lpPattern = find_pattern(GetModuleHandleA(NULL), pattern);
		if (*lpPattern != nullptr)
		{
			std::cout << "Found at: " << std::hex << *lpPattern << std::dec << std::endl;
		}
		else
		{
			std::cout << name << "Not found >> Disabled." << std::endl;
		}
	}
}

//template <class CamType>
//void ListenCamHotkey<CamType>(int VK_btn, CamType* addr, bool inc, double speed, float WaitDuration) // Eventually Change this Function to allow for Holding Shift by returning char as state. 0 = No, 1 = yes, -1 = shift.
//{
//	if (GetAsyncKeyState(VK_btn))
//	{
//		if (!inc)
//			speed = -speed;
//		*addr += speed;
//		auto start = std::chrono::system_clock::now();
//		while (GetAsyncKeyState(VK_btn))
//		{
//			std::chrono::duration<double> now = std::chrono::system_clock::now() - start;
//			if (now.count() > WaitDuration)
//			{
//				*addr += speed;
//				start = std::chrono::system_clock::now();
//			}
//		}
//	}
//}