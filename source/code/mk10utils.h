#pragma once
typedef __int64 int64;


int64 GetGameEntryPoint();
int64 GetUser32EntryPoint();
int64 GetGameAddr(__int64 addr);
int64 GetUser32Addr(__int64 addr);