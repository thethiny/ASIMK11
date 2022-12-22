#include "mk11hook.h"


MK11Hooks::LibCurlEasySetOpt*	MK11Hooks::curl_easy_setopt;
MK11Hooks::LibCurlEasyPerform*	MK11Hooks::curl_easy_perform;

void MK11Hooks::IntroSwap(char* dest, char* source, uint64_t length)
{
	char* p1_string;
	char* p1_string_replace;
	bool found = false;
	if (MK11::sIntroStruct.bEnabled && MK11::sIntroStruct.PName[0] && MK11::sIntroStruct.PChar[0] && MK11::sIntroStruct.PName2[0] && MK11::sIntroStruct.PChar2[0]) // Populated
	{
		p1_string = new char[strlen(MK11::sIntroStruct.PName) + 25 + 1]; // 1 Null Terminator
		sprintf(p1_string, "%s_%c_CHARINTRO_SCRIPTASSETS", MK11::sIntroStruct.PName, MK11::sIntroStruct.PChar[0]);
		if (MK11::sIntroStruct.PName == "*")
		{
			// Get Name Here
		}
		if (MK11::sIntroStruct.PChar[0] == '*')
		{
			// Get Letter Here
		}

		if (!strcmp(p1_string, source)) // Found the string I want to replace
		{
			p1_string_replace = new char[strlen(MK11::sIntroStruct.PName2) + 25 + 1]; // 1 Null Terminator
			sprintf(p1_string_replace, "%s_%c_CHARINTRO_SCRIPTASSETS", MK11::sIntroStruct.PName2, MK11::sIntroStruct.PChar2[0]);
			std::cout << "IntroSwap::Swapping " << source << " <-> " << p1_string_replace << std::endl;
			source = p1_string_replace;
			found = true;
		}
	}

	char* p2_string;
	char* p2_string_replace;
	if (!found && MK11::sIntroStruct2.bEnabled && MK11::sIntroStruct2.PName[0] && MK11::sIntroStruct2.PChar[0] && MK11::sIntroStruct2.PName2[0] && MK11::sIntroStruct2.PChar2[0]) // Populated
	{
		p2_string = new char[strlen(MK11::sIntroStruct2.PName) + 25 + 1]; // 1 Null Terminator
		sprintf(p2_string, "%s_%c_CHARINTRO_SCRIPTASSETS", MK11::sIntroStruct2.PName, MK11::sIntroStruct2.PChar[0]);
		if (MK11::sIntroStruct2.PName == "*")
		{
			// Get Name Here
		}
		if (MK11::sIntroStruct2.PChar[0] == '*')
		{
			// Get Letter Here
		}

		if (!strcmp(p2_string, source)) // Found the string I want to replace
		{
			p2_string_replace = new char[strlen(MK11::sIntroStruct2.PName2) + 25 + 1]; // 1 Null Terminator
			sprintf(p2_string_replace, "%s_%c_CHARINTRO_SCRIPTASSETS", MK11::sIntroStruct2.PName2, MK11::sIntroStruct2.PChar2[0]);
			std::cout << "IntroSwap::Swapping " << source << " <-> " << p2_string_replace << std::endl;
			source = p2_string_replace;
		}
	}

	length = strlen(source) + 1; // Auto Get Length + Null Terminator
	memcpy(dest, source, length);
	//std::cout << "IntroSwap::Loaded " << source << std::endl;
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
				SetColorInfo();
				wprintf(L"Loading %s from %s\n", wcFileName, wsSwapFolder.c_str());
				ResetColors();
				MK11::vSwappedFiles.push_back(wcFileName);
				return CreateFileW(newFileName.c_str(), dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
			}
		}

	}

	return CreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
}

std::map<int, const char*> CURL_MAP
{
	{46,	"CURLOPT_UPLOAD"},
	{47,	"CURLOPT_POST"},
	{10001,	"CURLOPT_WRITEDATA"},
	{10029,	"CURLOPT_HEADERDATA"},
	{10002,	"CURLOPT_URL"},
	{10004,	"CURLOPT_PROXY"},
	{10173,	"CURLOPT_USERNAME"},
	{10005,	"CURLOPT_USERPWD"},
	{10023,	"CURLOPT_HTTPHEADER"},
	{60,	"CURLOPT_POSTFIELDSIZE"},
	{20012,	"CURLOPT_READFUNCTION"},
	{10009,	"CURLOPT_READDATA"},
	{10010,	"CURLOPT_ERRORBUFFER"},
	{8,		"CURLPROTO_FTPS"},
	{10015,	"CURLOPT_POSTFIELDS"},
	{20011,	"CURLOPT_WRITEFUNCTION"},
	{10018,	"CURLOPT_USERAGENT"},
	{80,	"CURLOPT_HTTPGET"},
	{81,	"CURLOPT_SSL_VERIFYHOST"},
	{14,	"CURLOPT_INFILESIZE"},
	{64,	"CURLOPT_SSL_VERIFYPEER"},
	{99,	"CURLOPT_NOSIGNAL"},
	{41,	"CURLOPT_VERBOSE"},
	{42,	"CURLOPT_HEADER"},
	{43,	"CURLOPT_NOPROGRESS"},
	{10086,	"CURLOPT_SSLCERTTYPE"},
	{20079, "CURLOPT_HEADERFUNCTION"},
	{20108, "CURLOPT_SSL_CTX_FUNCTION"},
	{10065, "CURLOPT_CAINFO"},
	{10097, "CURLOPT_CAPATH"},

};

bool bFirstPost = true;

enum class ArgTypes {
	ARGTYPE_NONE = 0, // UNK
	ARGTYPE_STRING, // String Pointer
	ARGTYPE_AGBINARY, // Data
	ARGTYPE_FUNCTION, // Callback
	ARGTYPE_CANCEL, // Return 0
	ARGTYPE_SETZERO, // Arg3 becomes 0
	ARGTYPE_INT, // Integer
	ARGTYPE_STRUCT, // Struct Pointer
};

ArgTypes GetArgType(const char* arg_type)
{
	if (arg_type == "CURLOPT_URL")
		return ArgTypes::ARGTYPE_STRING;
	if (arg_type == "CURLOPT_USERAGENT")
		return ArgTypes::ARGTYPE_STRING;
	if (arg_type == "CURLOPT_WRITEDATA")
		return ArgTypes::ARGTYPE_STRUCT;
	if (arg_type == "CURLOPT_USERNAME")
		return ArgTypes::ARGTYPE_STRUCT;
	if (arg_type == "CURLOPT_USERPWD")
		return ArgTypes::ARGTYPE_STRUCT;
	if (arg_type == "CURLOPT_HEADERDATA")
		return ArgTypes::ARGTYPE_AGBINARY;
	if (arg_type == "CURLOPT_READDATA")
		return ArgTypes::ARGTYPE_AGBINARY;
	if (arg_type == "CURLOPT_WRITEFUNCTION")
		return ArgTypes::ARGTYPE_FUNCTION;
	if (arg_type == "CURLOPT_READFUNCTION")
		return ArgTypes::ARGTYPE_FUNCTION;
	if (arg_type == "CURLOPT_SSL_VERIFYPEER")
		return ArgTypes::ARGTYPE_SETZERO;
	if (arg_type == "CURLOPT_SSL_VERIFYHOST")
		return ArgTypes::ARGTYPE_SETZERO;
	if (arg_type == "CURLOPT_INFILESIZE")
		return ArgTypes::ARGTYPE_INT;
	if (arg_type == "CURLOPT_POSTFIELDSIZE")
		return ArgTypes::ARGTYPE_INT;
	if (arg_type == "CURLOPT_CAPATH")
		return ArgTypes::ARGTYPE_STRING;
	if (arg_type == "CURLOPT_CAINFO")
		return ArgTypes::ARGTYPE_STRING;
	if (arg_type == "CURLOPT_HEADERFUNCTION")
		return ArgTypes::ARGTYPE_FUNCTION;
	if (arg_type == "CURLOPT_SSL_CTX_FUNCTION")
		return ArgTypes::ARGTYPE_FUNCTION;
	if (arg_type == "CURLOPT_SSLCERTTYPE")
		return ArgTypes::ARGTYPE_STRING;
	return ArgTypes::ARGTYPE_NONE;
}

#define shortFunc(func, str) func(arg, arg3, str)

std::vector<__int64> vBreakPerforms;
void PushPerformBreakpoint(__int64 arg, __int64* arg3, const char *c)
{
	auto str = CPPython::string((char*)arg3);
	if (str.endswith(c))
	{
		vBreakPerforms.push_back(arg);
		printfCyan("LibCurl::%llX::Pushed %llX\n", MK11Hooks::curl_easy_perform, arg);
	}
	
}

std::map<uint64_t, MK11::HTTPPostStruct*> MK11::CurlObjectMap;


__int64 __fastcall MK11Hooks::curl_easy_setoptProxy(__int64 arg, int arg2, __int64 *arg3)
{
	std::string Format = "libcurl::curl_easy_setopt::%llX(";

	if (arg2 == 10029)
	{
		MK11::CurlObjectMap[arg] = (MK11::HTTPPostStruct*)arg3;
	}
	

	const char* arg_type = CURL_MAP[arg2];
	bool bCall = true;
	char c[256];
	char cData[4096]; // Data
	if (arg_type)
	{
		Format += arg_type;
	}
	else
	{
		Format += std::to_string(arg2);
	}
	Format += ", ";
	switch (GetArgType(arg_type))
	{
	case ArgTypes::ARGTYPE_SETZERO:
		arg3 = 0;
		Format += "%llX";
		break;
	case ArgTypes::ARGTYPE_INT:
		Format += "%lli";
		break;
	case ArgTypes::ARGTYPE_CANCEL:
		bCall = false;
		break;
	case ArgTypes::ARGTYPE_STRING:
		if (arg3)
		{
			PushPerformBreakpoint(arg, arg3, "auth");
			PushPerformBreakpoint(arg, arg3, "access");
			Format += "%s";
		}
		else
		{
			Format += "%i";
		}
		
		break;
	/*case ArgTypes::ARGTYPE_AGBINARY:
		Format += HexToString(*arg3);
		break;*/
	case ArgTypes::ARGTYPE_STRUCT:
		Format += "%llX";
		break;
	case ArgTypes::ARGTYPE_FUNCTION:
		sprintf(c, "MK11.exe+%llX() [%llX]", ((__int64(arg3)) - GetGameEntryPoint()), arg3);
		Format += c;
		break;
	default:
		Format += "%llX";
	}
	Format += ")\n";

	if (bCall)
	{
		int iVerbose = SettingsMgr->iVerbose;
		if (iVerbose > 1)
			printfCyan(Format.c_str(), arg, arg3);
		return MK11Hooks::curl_easy_setopt(arg, arg2, arg3);
	}
	return 0;
	
}

void curl_redirect(__int64 arg, char* redirect_path)
{
	MK11Hooks::curl_easy_setopt(arg, 10002, (__int64*)redirect_path);
}

__int64 __fastcall MK11Hooks::curl_easy_performProxy(__int64 arg)
{
	MK11::HTTPPostStruct* s = *(MK11::HTTPPostStruct**)(arg + 0x688);
	PyString Endpoint = "";
	if (s->UnknownSize >= 0)
	{
		Endpoint = s->Endpoint;
	}
	
	int iVerbose = SettingsMgr->iVerbose;

	if (SettingsMgr->bMITM && !Endpoint.empty() && !SettingsMgr->szMITMUrl.empty())
	{
		std::string NewUrl = SettingsMgr->szMITMUrl + "/" + Endpoint;
		MK11Hooks::curl_easy_setopt(arg, 10002, (__int64*)NewUrl.c_str());
		if (iVerbose)
			printfYellow("libcurl::Redirect::curl_easy_perform(%llX) -> %s\n", arg, NewUrl.c_str());
		//return MK11Hooks::curl_easy_perform(arg, arg2, arg3);
	}

	if (iVerbose)
		printfYellow("libcurl::Capture::curl_easy_perform(%llX) -> %llX(%s)\n", arg, s, Endpoint.c_str());
	if (Endpoint.endswith("auth"))
	{
		MK11::sUserKeys.SteamKeyBody = std::string(s->Body, s->BodySize);
		MK11::sGameState.bSteamKey = true;
		if (iVerbose > 1)
			MessageBoxA(0, MK11::sUserKeys.SteamKeyBody.c_str(), "LibCurl::curl_easy_perform(AUTH)", 0);
	}
	else if (Endpoint.endswith("access"))
	{
		MK11::sUserKeys.AuthTokenBody = std::string(s->Body, s->BodySize);
		MK11::sGameState.bAuthToken = true;
		if (iVerbose > 1)
			MessageBoxA(0, MK11::sUserKeys.AuthTokenBody.c_str(), "LibCurl::curl_easy_perform(ACCESS)", 0);
		auto response = MK11Hooks::curl_easy_perform(arg);
		MK11::sUserKeys.AuthTokenResponse = std::string(s->ResponseBody, s->ResponseBodySize);
		MK11::sGameState.bAccessToken = true;
		if (iVerbose > 1)
			MessageBoxA(0, s->ResponseBody, "Action Performed", 0);
		return response;

	}
	
	return MK11Hooks::curl_easy_perform(arg);
}