#include "eItemsManager.h"
#include "prettyprint.h"

MK11Unlocker::lpwsThisFunction* MK11Unlocker::ProcGetItemName;
MK11Unlocker::lpwsThisFunction* MK11Unlocker::ProcGetItemPriceString;
MK11Unlocker::ullThisFunction* MK11Unlocker::ProcGetItemPrice;
MK11Unlocker::VR2Function* MK11Unlocker::ProcVR2Function;
MK11Unlocker::UnlockItem* MK11Unlocker::ProcUnlockItem;

MK11Unlocker::VR2Struct* MK11Unlocker::sVR2;
MK11Unlocker::VR2xPtrStaticStruct* MK11Unlocker::VR2xHash;
std::vector<MK11Unlocker::eInventoryItem>	MK11Unlocker::vItems;
std::vector<MK11Unlocker::UnlockObjectClass*> MK11Unlocker::HashQueue;
MK11Unlocker::UnlockObjectClass* MK11Unlocker::LastHashedItem = nullptr;

void MK11Unlocker::VR2xHashFunc()
{
	Sleep(31);
	printf("VR2xHashFunc::VR2x::%llX->%llX->%llX->%lX (%s) (May be Wrong Due to Threading Behavior)\n", VR2xHash, VR2xHash->data, VR2xHash->data->HashData1, VR2xHash->data->HashData1->HashValue, HexToString(VR2xHash->data->HashData1->HashValue).c_str());
	printf("VR2xHashFunc::VR2x->Value::%llX\n", VR2xHash->data->HashData1->HashValue);
}

MK11Unlocker::VR2ResultStruct* __fastcall MK11Unlocker::VR2Proxy(VR2Struct* VR2Struct, char*& lpVR2HashResult, char* szToHash)
{
	//MK11::sGameState.bHash = VR2; // Game will call it first time therefore I will be able to call it afterwards
	SetColorYellow();
	printf("VR2Hash::Struct::%llX{%lX, %lX}\n", VR2Struct, VR2Struct->bIsInit, VR2Struct->VRCounter);
	printf("VR2Hash::&HashResult::%llX\n", lpVR2HashResult);
	printf("VR2HASH::String::%llX -> %s\n", szToHash, szToHash);

	VR2ResultStruct* lpVR2Hash = ProcVR2Function(VR2Struct, lpVR2HashResult, szToHash);
	printf("VR2Hash::Result::%llX -> %llX -> %s\n", lpVR2Hash, lpVR2Hash->Hash, lpVR2Hash->Hash);

	VR2xHashFunc();

	ResetColors();
	return lpVR2Hash;
}

MK11Unlocker::VR2xStruct* MK11Unlocker::GetVR2xStruct()
{
	return VR2xHash->data->HashData1; // Should update function to be based queue counter (current or passed?)
}


MK11Unlocker::VR2xStruct* MK11Unlocker::GetVR2xHash(const char* ToHash, char* Hashed)
{
	SetColorYellow();
	VR2Struct* InputStruct = sVR2;
	printf("GetVR2xHash::Struct::%llX{%lX, %lX}\n", InputStruct, InputStruct->bIsInit, InputStruct->VRCounter);
	printf("GetVR2xHash::String::%llX -> %s\n", ToHash, ToHash);

	char* JobID = new char[128]; // To Prevent Overflow

	VR2xHash->QueueCount = 0; // Reset
	ProcVR2Function(InputStruct, JobID, (char*)ToHash);
	VR2xHash->QueueCount = 0; // Will execute once it reaches 0
	//VR2xHash->QueueCount--;
	VR2xStruct* myStruct = GetVR2xStruct();

	printf("GetVR2xHash::VR2x::%llX->%llX->%llX\n", VR2xHash, VR2xHash->data, VR2xHash->data->HashData1);
	strcpy(Hashed, HexToStringBE(myStruct->HashValue).c_str());

	ResetColors();
	return myStruct;
}

wchar_t* __fastcall MK11Unlocker::GetItemName(uint64_t thisptr)
{
	wchar_t* name = ProcGetItemName(thisptr);
	printfInfo("Item[%llX]\tName: %ls\n", thisptr, name);
	//name = L"Free Item";
	return name;
};

wchar_t* __fastcall MK11Unlocker::GetItemPriceString(uint64_t thisptr)
{
	wchar_t* price = ProcGetItemPriceString(thisptr);
	printfInfo("Item[%llX]\tItem Display Price: %ls\n", thisptr, price);
	//price = L"0";
	return price;
};

uint64_t __fastcall MK11Unlocker::GetItemPrice(uint64_t thisptr)
{
	uint64_t price = ProcGetItemPrice(thisptr);
	printfInfo("Item[%llX]\tItem Buy Price: %llu\n", thisptr, price);
	//price = 0; // causes bans
	return price;
};

void MK11Unlocker::eInventoryManager::UnlockItem(char* name, eInventory* src)
{
	if (!src)
		return;

	src->source = "PREMIUM_SHOP";

	auto items_count = vItems.size();
	if (!items_count)
	{
		printfRed("No Items Selected!\n");
		return;
	}
		

	while (!vItems.empty())
	{
		auto item = vItems[vItems.size() - 1];
		ProcUnlockItem(this, item.name, src);
		printfGreen("Unlocker::UnlockItem(%s) [%d/%d]\n", item.name, items_count - vItems.size() + 1, items_count);
		vItems.pop_back();
	}
}

void MK11Unlocker::PushUnlockQueue(const char* name)
{
	if (!name)
		return;
	eInventoryItem eItem;
	sprintf(eItem.name, name);
	vItems.push_back(eItem);
}

MK11Unlocker::UnlockObjectClass* MK11Unlocker::PushHashQueue(PyString HashString, bool bGui)
{
	UnlockObjectClass* HashQueueItem = new UnlockObjectClass(HashString, bGui);
	HashQueueItem->HashCurrentState = HashState::SUBMITTED;

	HashQueue.push_back(HashQueueItem);

	printfCyan("HashQueueHandler::PushHashQueue(%llX) -> %s\n", HashQueueItem, HashString.c_str());

	if (bGui)
		LastHashedItem = HashQueueItem;

	return HashQueueItem;
}

void MK11Unlocker::HashQueueHandler()
{
	printf("HashQueueHandler::Started()\n");
	while (1)
	{
		if (HashQueue.empty())
		{
			Sleep(10); // Allow other functions to work
			continue;
		}
			

		printfCyan("HashQueueHandler::GetQueueFront()\n");
		UnlockObjectClass* HashQueueItem = PopQueue(HashQueue);
		if (HashQueueItem->bIsImGuiObject)
			LastHashedItem = HashQueueItem;

		HashQueueItem->HashCurrentState = HashState::HASHING;
		char* HashResult = new char[10];
		printfCyan("HashQueueHandler::GetVR2xHash(%s, %llX)\n", HashQueueItem->toHash.c_str(), HashResult);
		VR2xStruct* ResultStructPtr = GetVR2xHash(HashQueueItem->toHash.c_str(), HashResult);

		int ctr = 0;
		while ((uint32_t)ResultStructPtr->HashValue == 0)
		{
			if (ctr > 10)
				break;
			Sleep(31);
			ctr++;
		}

		if ((uint32_t)ResultStructPtr->HashValue == 0)
		{
			printfError("HashQueueHandler::%s::FAILED\n", HashQueueItem->toHash.c_str());
			HashQueueItem->HashCurrentState = HashState::FAILED;
		}
		else
		{
			HashQueueItem->szHash = HexToStringBE(ResultStructPtr->HashValue);
			printfSuccess("HashQueueHandler::%s::Success [%llX] [%llX -> %s]\n", HashQueueItem->toHash.c_str(), ResultStructPtr, HashResult, HashQueueItem->szHash.c_str());
			HashQueueItem->HashCurrentState = HashState::SUCCESS;
		}

	}
}

bool MK11Unlocker::UnlockObjectClass::ValidateHash()
{
	auto CmpStr = this->szHash.upper();
	if (this->HashCurrentState != HashState::SUCCESS)
		return false;
	if (CmpStr.empty())
		return false;
	for (int i = 0; i < CmpStr.length(); i++)
	{
		if (!IsHex(CmpStr[i]))
			return false;
	}

	return true;
}

bool MK11Unlocker::UnlockObjectClass::AwaitHash()
{
	int ctr = 0;

	while (!this->IsHashed())
	{
		if (ctr > 10)
			return false;
		Sleep(31);
		ctr++;
	}
	return this->ValidateHash();
}

std::string MK11Unlocker::SyncHashItem(PyString Hash, bool bGui)
{
	UnlockObjectClass* Item = PushHashQueue(Hash, bGui);
	if (!Item->AwaitHash())
		return "Failed";

	return Item->szHash;

}

// Unlocker IPC

bool MK11IPC::bIsIPCActive = false;

void MK11IPC::Server()
{
	using namespace MK11Unlocker;

	printfBlue("%s::Creating Server\n", IPC_NAME);
	zmq::context_t context(1); // Single Thread
	zmq::socket_t scServer(context, zmq::socket_type::rep);

	if (!scServer)
	{
		printfError("%s::Failed To Create Server!\n", IPC_NAME);
		return;
	}

	scServer.bind(IPC_SOCK);
	const std::string last_endpoint = scServer.get(zmq::sockopt::last_endpoint);
	printfBlue("%s::Accepting Connections at %s\n", IPC_NAME, last_endpoint.c_str());
	bIsIPCActive = true;

	while (1)
	{
		zmq::message_t mRecv;
		std::string Message;

		scServer.recv(mRecv);
		PyString ReceivedString = mRecv.to_string();

		const char* ClientName = mRecv.gets("Identity");

		printfCyan("%s::Received From %s::[%s]\n", IPC_NAME, ClientName, ReceivedString.c_str());

		char* CommandData = new char[ReceivedString.length()+1];
		auto Mode = ParseRequest(ReceivedString, CommandData);

		switch (Mode)
		{
		case eIPCRequest::HASH_ITEM:
			Message = GetItemHash(CommandData);
			break;
		case eIPCRequest::GET_ACCESS_TOKEN:
			Message = GetAccessToken();
			break;
		case eIPCRequest::GET_AUTH_TOKEN:
			Message = GetAuthToken();
			break;
		case eIPCRequest::GET_STEAM_TOKEN:
			Message = GetSteamToken();
			break;
		case eIPCRequest::UNLOCK_ITEM:
			Message = UnlockItem(CommandData) ? "1" : "0";
			break;
		case eIPCRequest::GET_GAME_VERSION:
			Message = GetGameVersion();
			break;
		case eIPCRequest::HEARTBEAT:
			Message = "OK";
			break;
		case eIPCRequest::INVALID_REQUEST:
			Message = "Invalid Request!";
			break;
		case eIPCRequest::UNKNOWN_COMMAND:
		default:	
			Message = "Unknown Command!";
		}

		printfCyan("Pipe::Sending::%s\n", Message.c_str());
		scServer.send(zmq::buffer(Message));

	}

	scServer.close();
	printfRed("%s::Disconnected\n", IPC_NAME);
	bIsIPCActive = false;
}

MK11IPC::eIPCRequest MK11IPC::ParseRequest(PyString Request, char* Data)
{
	auto x = Request.split(':', 1);
	PyString Command = x[0].strip().upper();
	PyString CommandData = x.size() > 1 ? x[1].strip() : "";
	strcpy(Data, CommandData.c_str());

	if (Command == "HASH")
	{
		if (x.size() < 2)
			return eIPCRequest::INVALID_REQUEST;
		return eIPCRequest::HASH_ITEM;
	}
	if (Command == "AUTH")
	{
		return eIPCRequest::GET_AUTH_TOKEN;
	}
	if (Command == "KEY")
	{
		return eIPCRequest::GET_STEAM_TOKEN;
	}
	if (Command == "ACCESS")
	{
		return eIPCRequest::GET_ACCESS_TOKEN;
	}
	if (Command == "USER")
	{
		return eIPCRequest::GET_USER_NAME;
	}
	if (Command == "VER")
	{
		return eIPCRequest::GET_GAME_VERSION;
	}
	if (Command == "ITEM")
	{
		return eIPCRequest::UNLOCK_ITEM;
	}
	if (Command == "ID")
	{
		return eIPCRequest::GET_STEAM_ID;
	}
	if (Command == "HBT")
	{
		return eIPCRequest::HEARTBEAT;
	}
	
	return eIPCRequest::UNKNOWN_COMMAND;
}

bool MK11IPC::UnlockItem(PyString ItemID)
{
	if (!MK11::sGameState.bUnlock)
		return false;
	MK11Unlocker::PushUnlockQueue(ItemID.c_str());
	return true;
}
PyString MK11IPC::GetItemHash(PyString HashString)
{
	return (MK11::sGameState.bAccessToken && MK11Unlocker::sVR2->bIsInit) ? MK11Unlocker::SyncHashItem(HashString) : ""; // Must be authed for the IV value to change, and must be Init.
}
PyString MK11IPC::GetAuthToken()
{
	return MK11::sGameState.bAuthToken ? MK11::sUserKeys.AuthTokenBody : "";
}
PyString MK11IPC::GetAccessToken()
{
	return MK11::sGameState.bAccessToken ? MK11::sUserKeys.AuthTokenResponse : "";
}
PyString MK11IPC::GetSteamToken()
{
	return MK11::sGameState.bSteamKey ? MK11::sUserKeys.SteamKeyBody : "";
}
PyString MK11IPC::GetGameVersion()
{
	return MK11::sGameState.bGameVersion ? MK11::GetGameVersionFull() : "";
}