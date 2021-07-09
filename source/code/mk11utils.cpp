#include "..\pch.h"
#include "mk11utils.h"
#include <map>

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
	int64 Entry = GetGameEntryPoint();
	if (addr > Entry)
		return addr;
	return Entry + addr;
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
	return new_string;
}

std::string toUpper(std::string s)
{
	std::string new_string("");
	for (int i = 0; i < s.length(); i++)
	{
		new_string += std::toupper(s[i]);
	}
	return new_string;
}

std::string GetFileName(std::string filename)
{
	std::string basename;
	size_t pos = filename.find_last_of("/\\"); // Or
	if (pos != -1)
	{
		basename = filename.substr(pos + 1);
		return basename;
	}
	return filename;
}

std::string GetProcessName()
{
	CHAR fileName[MAX_PATH + 1];
	DWORD chars = GetModuleFileNameA(NULL, fileName, MAX_PATH + 1);
	if (chars)
	{
		return GetFileName(std::string(fileName));
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


uint64_t* FindPattern(void* handle, std::string_view bytes)
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
		*lpPattern = FindPattern(GetModuleHandleA(NULL), pattern);
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

LibMap ParsePEHeader()
{
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)GetModuleHandleA(NULL);
	PIMAGE_NT_HEADERS pNTHeader = (PIMAGE_NT_HEADERS)RVAtoLP((PBYTE)pDosHeader, pDosHeader->e_lfanew);
	if (pNTHeader->Signature != IMAGE_NT_SIGNATURE)
		throw(-1); // Not an EXE

	LibMap IAT{};

	PIMAGE_IMPORT_DESCRIPTOR pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)RVAtoLP(pDosHeader, pNTHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	for (int i = 0; pImportDesc[i].Characteristics != 0; i++)
	{
		std::string szLibrary = (char*)RVAtoLP(pDosHeader, pImportDesc[i].Name);
		if (!pImportDesc[i].FirstThunk || !pImportDesc[i].OriginalFirstThunk)
			continue;

		PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)RVAtoLP(pDosHeader, pImportDesc[i].FirstThunk);
		PIMAGE_THUNK_DATA pOrigThunk = (PIMAGE_THUNK_DATA)RVAtoLP(pDosHeader, pImportDesc[i].OriginalFirstThunk);

		FuncMap FunctionsMap{};

		for (; pOrigThunk->u1.Function != NULL; pOrigThunk++, pThunk++)
		{
			if (pOrigThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG)
				continue;

			PIMAGE_IMPORT_BY_NAME import = (PIMAGE_IMPORT_BY_NAME)RVAtoLP(pDosHeader, pOrigThunk->u1.AddressOfData);

			std::string FuncName = (char*)import->Name;
			FunctionsMap[FuncName] = pThunk->u1.Function;
		}
		IAT[szLibrary] = FunctionsMap;
	}

	return IAT;

}

int StringToVK(std::string sKey)
{
	for (auto& c : sKey) c = toupper(c); // To Upper sKey

	if (sKey == "SHIFT")
	{
		return VK_SHIFT;
	}

	if (sKey == "TAB")
	{
		return VK_TAB;
	}

	if (sKey == "SPACE")
	{
		return VK_SPACE;
	}

	if (sKey == "ALT")
	{
		return VK_MENU;
	}

	if (sKey.substr(0, 6) == "NUMPAD") // Numpad
	{
		char cLast = sKey[6];
		switch (cLast)
		{
		case '+':
			return VK_ADD;
		case '-':
			return VK_SUBTRACT;
		case 'E': //NUMPAD Enter
			return VK_RETURN;
		case '*':
			return VK_MULTIPLY;
		case '/':
			return VK_DIVIDE;
		default:
			return cLast + 0x30; // NUMPAD Number
		}

	}
	if (sKey.length() == 1)
	{
		if (sKey[0] >= '0' && sKey[0] <= 'Z' && sKey[0] != 0x40) // Letter or Number. 0x40 is not available which is @
			return sKey[0]; // Letters and Numbers' VK are their ASCII Repr
		if (sKey[0] == '/' || sKey[0] == '?')
			return VK_OEM_2;
		if (sKey[0] == '-')
			return VK_OEM_MINUS;
		if (sKey[0] == '+')
			return VK_OEM_PLUS;
		if (sKey[0] == '`' || sKey[0] == '~')
			return VK_OEM_3;
	}

	if (sKey[0] == 'F' && sKey.length() > 1 && sKey.length() <= 3) // F1-F12 buttons
	{
		std::string digits = sKey.substr(1, sKey.length() - 1).c_str();
		return std::stoi(digits.c_str()) + 0x70 - 1;
	}

	// Arrows
	if (sKey == "DOWN")
		return VK_DOWN;
	if (sKey == "UP")
		return VK_UP;
	if (sKey == "LEFT")
		return VK_LEFT;
	if (sKey == "RIGHT")
		return VK_RIGHT;
}