#pragma once
#include "mk11utils.h"
#include "mk11menu.h"
#include "../utils/CPPython/cppython.h"

enum PLAYER_NUM
{
	INVALID_PLAYER_NUM			= 0xFFFFFFFF,
	PLAYER1						= 0x0,
	PLAYER2,
	PLAYER3,
	PLAYER4,
	MAX_PLAYERS,
	CPU_PLAYER					= 0x5,
	NOT_CPU_PLAYER				= 0x6,
	BACKGROUND_PLAYER			= 0x7,
};

enum MOD_HOOK_STATUS
{
	DISABLED					= -1,
	SUCCESS						,
	NOT_SPECIFIED				,
	NOT_FOUND					,
};

namespace MK11 {

	// Structs
	struct CamStruct {
		bool bLogCam				= false;
		bool bTimestopActive		= false;
		bool bTimestopEnabled		= false;
	};

	struct IntroStruct {
		char PName[100]				{ 0 };
		char PName2[100]			{ 0 };
		char PChar[2]				{ 0 };
		char PChar2[2]				{ 0 };
		bool bEnabled				= false;
	};

	struct CharacterStruct {
		std::string name;
		uint8_t intros=0;
	};

	struct ActiveMods {
		bool bIntroSwap				= false;
		bool bAntiCheatEngine		= false;
		bool bAntiCVD1				= false;
		bool bAntiCVD2				= false;
		bool bModLoader				= false;
	};

	struct CheatsStruct {
		uint64_t* lpMercy, * lpGround, * lpBrut, * lpBrutB, * lpMeteor, * lpDizzy, *lpFatality, *lpFatCombo, *lpNoBlock, *lpFatalBlow;
		bool bMercy = false, bGround = false, bBrut = false, bBrutB = false, bMeteor = false, bDizzy = false, bFatality = false, bFatCombo = false, bNoBlock = false, bFatalBlow = false;
	};

	struct LibFuncStruct {
		std::string FullName;
		std::string LibName;
		std::string ProcName;
	};

	struct LibMapsStruct {
		LibFuncStruct ModLoader;
		LibFuncStruct AntiCheatEngine;
		LibFuncStruct CurlSetOpt;
		LibFuncStruct CurlPerform;
	};
	
	// Vars
	extern std::vector<CharacterStruct>			sCharacters;
	extern uint8_t								ulCharactersCount;
	extern uint64_t*							lpGameVersion;
	extern uint64_t*							lpGameVersionFull;
	extern std::vector<std::wstring>			vSwappedFiles;
	extern LibMap								IAT;
	// StructVars
	extern CamStruct							sCamStruct;
	extern IntroStruct							sIntroStruct;
	extern IntroStruct							sIntroStruct2;
	extern ActiveMods							sActiveMods;
	extern CheatsStruct							sCheatsStruct;
	extern LibMapsStruct						sLFS;
	

	// Functions
	std::string GetGameVersion();
	std::string GetGameVersionFull();
	void PopulateCharList();
	bool operator==(const CharacterStruct& s1, std::string s2);
	//LFS
	LibFuncStruct ParseLibFunc(CPPython::string);
	void ParseLibFunc(LibFuncStruct&);
	uint64_t* GetLibProcFromNT(const LibFuncStruct&);
}


namespace MK11Hooks {
	void			IntroSwap(char* dest, char* source, uint64_t length);
	void			TimestopFunction(uint64_t somePtr, uint32_t rvalue);
	HANDLE			__stdcall CreateFileProxy(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
	uint64_t*		__fastcall VR2Proxy(uint64_t seed, uint64_t* result_hash, const char* to_hash);
	wchar_t*		__fastcall GetItemName(uint64_t thisptr);
	wchar_t*		__fastcall GetItemPriceString(uint64_t thisptr);
	uint64_t		__fastcall GetItemPrice(uint64_t thisptr);
			
	typedef			uint64_t*			(__fastcall VR2Function)			(uint64_t seed, uint64_t* result_hash, const char* to_hash);
	typedef			wchar_t*			(__fastcall lpwsThisFunction)		(uint64_t thisptr);
	typedef			uint64_t			(__fastcall ullThisFunction)		(uint64_t thisptr);
	typedef			uint64_t*			(__fastcall lpullThisFunction)		(uint64_t thisptr);

	// Unlocker
	extern		VR2Function*			ProcVR2Function;
	// Premium Shop
	extern		ullThisFunction*		ProcGetItemPrice;
	extern		lpwsThisFunction*		ProcGetItemName;
	extern		lpwsThisFunction*		ProcGetItemPriceString;
}



