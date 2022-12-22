#pragma once
#include "mk11.h"

namespace MK11Hooks {
	void			IntroSwap(char* dest, char* source, uint64_t length);
	HANDLE			__stdcall	CreateFileProxy(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
	__int64			__fastcall	curl_easy_setoptProxy(__int64 arg, int arg2, __int64* arg3);
	__int64			__fastcall	curl_easy_performProxy(__int64 arg);

	typedef			__int64(__fastcall LibCurlEasySetOpt)				(__int64, int, __int64*);
	typedef			__int64(__fastcall LibCurlEasyPerform)				(__int64);
	
	// Libcurl
	extern			LibCurlEasySetOpt*								curl_easy_setopt;
	extern			LibCurlEasyPerform*								curl_easy_perform;
}