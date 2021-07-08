#pragma once
#include "mk11utils.h"
#include "mk11menu.h"

enum  PLAYER_NUM
{
	INVALID_PLAYER_NUM = 0xFFFFFFFF,
	PLAYER1 = 0x0,
	PLAYER2,
	PLAYER3,
	PLAYER4,
	MAX_PLAYERS,
	CPU_PLAYER = 0x5,
	NOT_CPU_PLAYER = 0x6,
	BACKGROUND_PLAYER = 0x7,
};

enum MOD_HOOK_STATUS
{
	SUCCESS = 0,
	DISABLED,
	NOT_SPECIFIED,
	NOT_FOUND
};

namespace MK11 {

	struct CamStruct {
		bool bLogCam = false; //Make it a flag to control prints
		bool bTimestopActive = false;
		bool bTimestopEnabled = false;
	};

	struct IntroStruct {
		char PName[100] = { 0 }, PName2[100] = { 0 };
		char PChar[2] = { 0 }, PChar2[2]{ 0 };
		bool bEnabled = false;
	};

	struct CharacterStruct {
		std::string name;
		uint8_t intros=0;
	};

	bool operator==(const CharacterStruct& s1, std::string s2);

	extern std::vector<CharacterStruct> sCharacters;
	extern uint8_t ulCharactersCount;
	extern uint64_t* lpGameVersion;
	extern uint64_t* lpGameVersionFull;
	void PopulateCharList();

	struct ActiveMods {
		bool bIntroSwap = false;
		bool bAntiCheatEngine = false;
		bool bAntiCVD1 = false;
		bool bAntiCVD2 = false;
		bool bModLoader = false;
	};

	struct CheatsStruct {
		uint64_t* lpMercy, * lpGround, * lpBrut, * lpBrutB, * lpMeteor, * lpDizzy, *lpFatality, *lpFatCombo, *lpNoBlock, *lpFatalBlow;
		bool bMercy = false, bGround = false, bBrut = false, bBrutB = false, bMeteor = false, bDizzy = false, bFatality = false, bFatCombo = false, bNoBlock = false, bFatalBlow = false;
	};

	extern std::vector<std::wstring> vSwappedFiles;

	std::string GetGameVersion();
	std::string GetGameVersionFull();

}

extern MK11::CamStruct sCamStruct;
extern MK11::IntroStruct sIntroStruct, sIntroStruct2;
extern MK11::ActiveMods sActiveMods;
extern MK11::CheatsStruct sCheatsStruct;


namespace MK11Hooks {
	void IntroSwap(char* dest, char* source, uint64_t length);
	void TimestopFunction(uint64_t somePtr, uint32_t rvalue);
	HANDLE __stdcall CreateFileProxy(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
	uint64_t* __fastcall VR2Proxy(uint64_t seed, uint64_t* result_hash, const char* to_hash);

	typedef uint64_t* (__fastcall VR2Function)(uint64_t seed, uint64_t* result_hash, const char* to_hash);
	extern VR2Function* procVR2Function;
}


