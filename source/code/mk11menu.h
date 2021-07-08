#pragma once
#include "../includes.h"
#include "mk11.h"

enum eTabs {
	ERR = -1,
	NONE,
	CHEATS,
	SWAPS,
	CAMERA,
	UNLOCKER,
	ANTICHEAT,
	SECRET,
};

extern std::string szTabName[];

class MK11Menu {
private:
	bool bIsActive = false;
	eTabs iCurrentTab = eTabs::NONE;
public:
	void Draw();
	bool GetActiveState();
	void ToggleActive();
	bool operator!() { return !bIsActive; }
};

extern MK11Menu* GuiMenu;

extern "C" __declspec(dllexport) ImGuiContext * __stdcall SharedPresent(ImGuiContext * ctx);
extern "C" __declspec(dllexport) ImGuiContext * SharedStyle(ImGuiContext * ctx = 0);
extern "C" __declspec(dllexport) void InitShared();
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