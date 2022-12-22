#pragma once
#include "../includes.h"
#include "mk11.h"


#define TextSuccess(text) ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), text);
#define TextWarning(text) ImGui::TextColored(ImVec4(1.f, 0.5f, 0.f, 1.f), text);
#define TextError(text) ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), text);

enum eTabs {
	ERR = -1,
	ARGTYPE_NONE,
	CHEATS,
	SWAPS,
	CAMERA,
	ANTICHEAT,
	UNLOCKER,
	PRIVATE_SERVER,
};

extern std::map<eTabs, std::string> szTabName;

class MK11Menu {
private:
	bool bIsActive = false;
	char szHashString[256] = {};
public:
	void Draw();
	bool GetActiveState();
	void ToggleActive();
	bool operator!() { return !bIsActive; }
	operator bool() { return bIsActive; }
	void SetUnlockerHash();
};

extern MK11Menu* GuiMenu;

extern "C" __declspec(dllexport) void SharedPresent();
extern "C" __declspec(dllexport) void SharedStyle();
extern "C" __declspec(dllexport) void InitShared(ImGuiContext*);
extern bool bSharedInit;

// Kiero stuff
extern Present oPresent;
extern HWND hKieroWindow;
extern WNDPROC oWndProc;
extern ID3D11Device* pDevice;
extern ID3D11DeviceContext* pContext;
extern ID3D11RenderTargetView* mainRenderTargetView;
extern bool bKieroInit;

void InitImGui();
LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
DWORD WINAPI MenuGuiMainThread(LPVOID lpReserved);