#include "..\pch.h"
#include "mk11.h"
#include <filesystem>

MK11::CamStruct sCamStruct;
MK11::IntroStruct sIntroStruct, sIntroStruct2;
MK11::ActiveMods sActiveMods;
MK11::CheatsStruct sCheatsStruct;
std::vector<std::wstring> MK11::vSwappedFiles;
uint64_t* MK11::lpGameVersionFull = nullptr;
uint64_t* MK11::lpGameVersion = nullptr;

bool MK11::operator==(const MK11::CharacterStruct& s1, std::string s2)
{
	return (s1.name == s2);
}

std::vector<MK11::CharacterStruct> MK11::sCharacters;
uint8_t MK11::ulCharactersCount;

void MK11::PopulateCharList()
{
	std::cout << "Creating Character List" << std::endl;
	for (const auto& file : std::filesystem::directory_iterator(GetDirName() + "\\..\\..\\Asset"))
	{
		if (file.is_directory())
			continue;
		if (!file.path().has_extension() || strcmp(toLower(file.path().extension().string()).c_str(), ".xxx"))
			continue;
		
		std::string basename = GetFileName(file.path().string());
		std::string filename = toLower(basename);
		if (filename.length() < 28) // No Char
			continue;

		if (strcmp("charintro_scriptassets.xxx", filename.substr(filename.length() - 26).c_str())) // Not Equal
			continue;

		std::string asset_name = filename.substr(0, filename.length() - 27);
		if (asset_name[asset_name.length()-2] != '_')
			continue; // Something Fishy

		uint8_t IntroLetter = asset_name[asset_name.length()-1] - 'a';
		std::string CharName = toUpper(basename.substr(0, basename.length() - 29)); // Maintain Casing

		auto found = std::find(sCharacters.begin(), sCharacters.end(), CharName);
		if (found != sCharacters.end())
		{
			if (found->intros < IntroLetter)
			{
				found->intros = IntroLetter;
			}
				
		}
		else
		{
			CharacterStruct sChar;
			sChar.intros = IntroLetter;
			sChar.name = CharName;
			sCharacters.push_back(sChar);
		}
		
	}
}

void MK11Hooks::IntroSwap(char* dest, char* source, uint64_t length)
{
	char* p1_string;
	char* p1_string_replace;
	bool found = false;
	if (sIntroStruct.bEnabled && sIntroStruct.PName[0] && sIntroStruct.PChar[0] && sIntroStruct.PName2[0] && sIntroStruct.PChar2[0]) // Populated
	{
		p1_string = new char[strlen(sIntroStruct.PName) + 25 + 1]; // 1 Null Terminator
		sprintf(p1_string, "%s_%c_CHARINTRO_SCRIPTASSETS", sIntroStruct.PName, sIntroStruct.PChar[0]);
		if (sIntroStruct.PName == "*")
		{
			// Get Name Here
		}
		if (sIntroStruct.PChar[0] == '*')
		{
			// Get Letter Here
		}

		if (!strcmp(p1_string, source)) // Found the string I want to replace
		{
			p1_string_replace = new char[strlen(sIntroStruct.PName2) + 25 + 1]; // 1 Null Terminator
			sprintf(p1_string_replace, "%s_%c_CHARINTRO_SCRIPTASSETS", sIntroStruct.PName2, sIntroStruct.PChar2[0]);
			std::cout << "IntroSwap::Swapping " << source << " <-> " << p1_string_replace << std::endl;
			source = p1_string_replace;
			found = true;
		}
	}

	char* p2_string;
	char* p2_string_replace;
	if (!found && sIntroStruct2.bEnabled && sIntroStruct2.PName[0] && sIntroStruct2.PChar[0] && sIntroStruct2.PName2[0] && sIntroStruct2.PChar2[0]) // Populated
	{
		p2_string = new char[strlen(sIntroStruct2.PName) + 25 + 1]; // 1 Null Terminator
		sprintf(p2_string, "%s_%c_CHARINTRO_SCRIPTASSETS", sIntroStruct2.PName, sIntroStruct2.PChar[0]);
		if (sIntroStruct2.PName == "*")
		{
			// Get Name Here
		}
		if (sIntroStruct2.PChar[0] == '*')
		{
			// Get Letter Here
		}

		if (!strcmp(p2_string, source)) // Found the string I want to replace
		{
			p2_string_replace = new char[strlen(sIntroStruct2.PName2) + 25 + 1]; // 1 Null Terminator
			sprintf(p2_string_replace, "%s_%c_CHARINTRO_SCRIPTASSETS", sIntroStruct2.PName2, sIntroStruct2.PChar2[0]);
			std::cout << "IntroSwap::Swapping " << source << " <-> " << p2_string_replace << std::endl;
			source = p2_string_replace;
		}
	}

	length = strlen(source) +1; // Auto Get Length + Null Terminator
	memcpy(dest, source, length);
	//std::cout << "IntroSwap::Loaded " << source << std::endl;
}

void MK11Hooks::TimestopFunction(uint64_t somePtr, uint32_t rvalue)
{
	uint64_t rax = *(uint64_t*)(somePtr);

	if (!sCamStruct.bTimestopActive)
		rvalue = *(uint32_t*)(somePtr + 0x18);

	if (*(uint32_t*)(somePtr + 0x14) == rvalue)
		return;
	uint64_t function_addr = *(uint64_t*)(rax + 0xD8);
	*(uint32_t*)(somePtr + 0x14) = rvalue;
	((void (*)(uint64_t, uint32_t))function_addr)(somePtr, rvalue);
}

HANDLE __stdcall MK11Hooks::CreateFileProxy(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	if (lpFileName)
	{
		//std::wstring fileName = lpFileName;
		wchar_t* wcFileName = (wchar_t*)lpFileName;
		std::wstring fileName(wcFileName, wcslen(wcFileName));
		if (!wcsncmp(wcFileName, L"..", 2))
		{
			std::wstring wsSwapFolder = L"MKSwap";
			std::wstring newFileName = L"..\\" + wsSwapFolder + fileName.substr(2, fileName.length() - 2);
			if (std::filesystem::exists(newFileName.c_str()))
			{
				wprintf(L"Loading %s from %s\n", wcFileName, wsSwapFolder.c_str());
				MK11::vSwappedFiles.push_back(wcFileName);
				return CreateFileW(newFileName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
			}
		}
		
	}
	
	return CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

// Unlocker Part
MK11Hooks::VR2Function* MK11Hooks::procVR2Function = (MK11Hooks::VR2Function*)(GetGameAddr(0x3D9F760));

uint64_t* __fastcall MK11Hooks::VR2Proxy(uint64_t seed, uint64_t* result_hash, const char* to_hash)
{
	printf("seed: %X\n", seed);
	printf("result_hash: %X\n", result_hash);
	printf("to_hash: %X (%s)\n", to_hash, to_hash);

	uint64_t* return_value = procVR2Function(0, result_hash, to_hash);
	printf("hash: %X (%s)\n", return_value, (char*)*return_value);

	return return_value;
}

std::string MK11::GetGameVersion()
{
	if (!lpGameVersion)
	{
		return "";
	}
	if (!*lpGameVersion)
	{
		return "";
	}
	return reinterpret_cast<char*>(*lpGameVersion);
}

std::string MK11::GetGameVersionFull()
{
	if (lpGameVersionFull == NULL)
	{
		return "";
	}
	if (!*lpGameVersionFull)
	{
		return "";
	}
	uint64_t* ptr = (uint64_t*)(uint64_t(*lpGameVersionFull) + 0x30);
	if (!*ptr)
	{
		return "";
	}
	return reinterpret_cast<char*>(*ptr);
};