#include "..\pch.h"
#include "mk10utils.h"

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