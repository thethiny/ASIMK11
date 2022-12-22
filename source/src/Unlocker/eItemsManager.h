#pragma once
#include "../mk11utils.h"
#include "../mk11.h"
#include <cppython.h>

namespace MK11Unlocker
{
	// Classes

	enum class HashState {
		NONE = -1,
		SUCCESS,
		HASHING,
		SUBMITTED,
		FAILED,
	};

	class eInventory {
	public:
		int64 field0;
		int64 field8;
		char* source;
		char  pad[16];
		char* field32;
		int64 field40;
	};

	class eInventoryManager {
	public:

		void UnlockItem(char* name, eInventory* src);

	};

	class UnlockObjectClass {
	public:
		UnlockObjectClass(PyString Hash) { toHash = Hash; }
		UnlockObjectClass(PyString Hash, bool ImGui) : UnlockObjectClass(Hash) { bIsImGuiObject = ImGui; }
	public:
		HashState HashCurrentState = HashState::NONE;
		PyString szHash = "";
		bool bIsImGuiObject = false; // If True then update ImGui Display
		PyString toHash = "";
	public:
		bool IsHashed() { return (HashCurrentState == HashState::SUCCESS || HashCurrentState == HashState::FAILED); }
		bool AwaitHash();
		bool ValidateHash();
	};

	// Structs

	struct eInventoryItem {
		char name[512];
	};

	struct VR2Struct { // Static
		uint32_t bIsInit = 0; // 1 When init 
		uint32_t VRCounter = 0; // D5 when init
	};

	struct VR2ResultStruct { // Hash Result (Job ID)
		char* Hash;
		// Unused below. Kept for code compatibility
		int32_t UNK;
		int32_t UNK2;
		uint64_t* FunctionsTable;
		uint32_t UNK3[4];
		uint64_t* FunctionsTable2;
		uint64_t UNK4;
		int64_t UNK5;
		void* Function;
		char* ItemSlug;
		int32_t UNK6;
		int32_t UNK7;
		int64_t UNK8;
	};

	struct VR2xStruct { // Hash Result (Results)
		uint64_t* ParentPtr;
		VR2xStruct* ChildPtr;
		uint64_t* ParentPtrCpy;
		uint64_t PAD[2];
		uint32_t VAR[2];
		uint64_t PAD2;
		uint32_t HashValue; // Hash is first 32 bits, if it starts with a leading 0, then hash removes it.
		uint32_t HashExtra; // Not Required
	};

	struct VR2xPtrStruct { // Hash Pointer (Dynamic->Results)
		VR2xStruct* HashData1;
		VR2xStruct* HashData2;
		VR2xStruct* HashData3;
	};

	struct VR2xPtrStaticStruct { // Hash Pointer (Static->Dynamic->Results)
		VR2xPtrStruct* data;
		int64_t	QueueCount; // Not Queue Count, it's batch calculator. This basically selects from VR2xPtrStruct to know where is your return hash.
	};

	// Vars

	typedef	VR2ResultStruct* (__fastcall VR2Function)(VR2Struct* VR2Struct, char*& lpVR2HashResult, char* szToHash);
	typedef	uint64_t(__fastcall UnlockItem)(eInventoryManager*, char*, eInventory*);
	typedef	wchar_t* (__fastcall lpwsThisFunction)(uint64_t thisptr);
	typedef	uint64_t(__fastcall ullThisFunction)(uint64_t thisptr);
	typedef	uint64_t* (__fastcall lpullThisFunction)(uint64_t thisptr);

	extern VR2Struct* sVR2;
	extern VR2xPtrStaticStruct* VR2xHash;
	extern std::vector<eInventoryItem> vItems;
	extern VR2Function* ProcVR2Function;
	extern UnlockItem* ProcUnlockItem;
	extern ullThisFunction* ProcGetItemPrice;
	extern lpwsThisFunction* ProcGetItemName;
	extern lpwsThisFunction* ProcGetItemPriceString;
	extern std::vector<UnlockObjectClass*> HashQueue;
	extern UnlockObjectClass* LastHashedItem;
	// Procs
	VR2ResultStruct*	__fastcall	VR2Proxy(VR2Struct* seedPtr, char*& lpVR2HashResult, char* szToHash);
	wchar_t*			__fastcall	GetItemName(uint64_t thisptr);
	wchar_t*			__fastcall	GetItemPriceString(uint64_t thisptr);
	uint64_t			__fastcall	GetItemPrice(uint64_t thisptr);
	

	// Functions

	void PushUnlockQueue(const char* name);
	void VR2xHashFunc();
	void HashQueueHandler();
	std::string SyncHashItem(PyString Hash, bool bGui = false);
	VR2xStruct* GetVR2xStruct();
	VR2xStruct* GetVR2xHash(const char* ToHash, char* Hashed);
	UnlockObjectClass* PushHashQueue(PyString Hash, bool bGui = false);


}


// Unlocker IPC
#include <zmq.hpp>
/* Unlocker IPC Defines */
#define IPC_PROTOCOL "tcp"
#define IPC_ADDR "127.0.0.1"
#define IPC_PORT "12011"
#define IPC_SOCK IPC_PROTOCOL "://" IPC_ADDR ":" IPC_PORT
#define IPC_NAME "MK11Unlocker"

namespace MK11IPC
{
	extern bool bIsIPCActive;
	void Server();
	enum class eIPCRequest {
		INVALID_REQUEST = 0x0, // Other
		UNKNOWN_COMMAND,
		UNUSED,
		HEARTBEAT,
		GET_AUTH_TOKEN	= 0x10, // Auth
		GET_ACCESS_TOKEN,
		GET_STEAM_TOKEN,
		GET_STEAM_ID	= 0x20, // Game
		GET_USER_NAME,
		GET_GAME_VERSION,
		GET_USER_STRUCT,
		HASH_ITEM		= 0x30, // Functions
		UNLOCK_ITEM,
	};

	eIPCRequest ParseRequest(PyString Request, char* DstData);

	extern "C"	__declspec(dllexport)	PyString GetAuthToken();
	extern "C"	__declspec(dllexport)	PyString GetAccessToken();
	extern "C"	__declspec(dllexport)	PyString GetSteamToken();
	extern "C"	__declspec(dllexport)	PyString GetSteamID();
	extern "C"	__declspec(dllexport)	PyString GetMKUserName();
	extern "C"	__declspec(dllexport)	PyString GetGameVersion();
	extern "C"	__declspec(dllexport)	PyString GetItemHash(PyString HashString);
	extern "C"	__declspec(dllexport)	bool UnlockItem(PyString ItemID);

}