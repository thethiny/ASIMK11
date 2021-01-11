#include "mk11menu.h"
#include <Windows.h>
#include "mk11utils.h"
#include <iostream>
#include "..\imgui\imgui.h"
#include <sstream>

MK11Menu* GuiMenu = new MK11Menu();

typedef ImDrawData* (__stdcall ermaccer_hkPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
typedef void(__fastcall SetGlobWindow)(HWND window);

ermaccer_hkPresent* ermaccer_function;
SetGlobWindow* ermaccer_setWindow;

extern std::string szTabName[] = {
	"Cheats",
	"Swaps",
	"Camera",
	"Unlocker",
	"AntiCheat"
};

static void ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

void MK11Menu::Initialize()
{

}

void MK11Menu::Draw()
{
	ImGui::GetIO().MouseDrawCursor = true;
	ImGui::Begin("ASIMK11 by thethiny");

	// Tabs

	if (ImGui::Button("Camera"))
		iCurrentTab = eTabs::CAMERA;
	ImGui::SameLine();
	if (ImGui::Button("Swaps"))
		iCurrentTab = eTabs::SWAPS;
	ImGui::SameLine();
	if (ImGui::Button("Cheats"))
		iCurrentTab = eTabs::CHEATS;
	ImGui::SameLine();
	if (ImGui::Button("Anti Cheat"))
		iCurrentTab = eTabs::ANTICHEAT;
	ImGui::SameLine();
	if (ImGui::Button("Unlocker"))
		iCurrentTab = eTabs::UNLOCKER;
	ImGui::Separator();

	if (iCurrentTab == eTabs::CAMERA)
	{
		
		if (sCamStruct.bCamEnabled)
		{
			if (sCamStruct.bCamActive)
			{
				ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Active");
			}
			else
			{
				ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "InActive");
			}
		}
		else
		{
			ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Disabled");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.f), "(Settings)");
		}
		
		
		ImGui::Separator();
		ImGui::Checkbox("Freeze XYZ", &sCamStruct.bEnableXYZ);
		ImGui::InputFloat("X", &sCamStruct.fX, SettingsMgr->fSpeed);
		ImGui::InputFloat("Y", &sCamStruct.fY, SettingsMgr->fSpeed);
		ImGui::InputFloat("Z", &sCamStruct.fZ, SettingsMgr->fSpeed);

		ImGui::Checkbox("Freeze POV", &sCamStruct.bEnablePOV);
		ImGui::InputFloat("POV", &sCamStruct.fPOV, SettingsMgr->fSpeed / 10);

		ImGui::Checkbox("Freeze Pitch/Yaw/Rot", &sCamStruct.bEnablePiYaRot);
		ImGui::InputInt("Pitch", &sCamStruct.iPitch, SettingsMgr->fSpeed);
		ImGui::InputInt("Yaw", &sCamStruct.iYaw, SettingsMgr->fSpeed);
		ImGui::InputInt("Rotation", &sCamStruct.iRot, SettingsMgr->fSpeed);

		ImGui::Checkbox("Timestop", &sCamStruct.bTimestopActive);
		
	}
	else if (iCurrentTab == eTabs::UNLOCKER)
	{
		ImGui::TextWrapped("Unlocker Is not working until the game reaches the end of its life cycle.");
		ImGui::Spacing();
	}
	else
	{
		ImGui::TextWrapped((szTabName[iCurrentTab] + " not yet implemented.").c_str());
		ImGui::Spacing();
	}


	/*
	if (ImGui::Button("Speed Modifier")) iCurrentTab = TAB_SPEED;
	ImGui::SameLine();
	if (ImGui::Button("Player Control")) iCurrentTab = TAB_PLAYER_CONTROL;
	ImGui::SameLine();
	if (ImGui::Button("Camera Control")) iCurrentTab = TAB_CAMERA;
	ImGui::Separator();

	if (iCurrentTab == TAB_SPEED)
	{
		ImGui::Text("Gamespeed Control");
		ImGui::InputFloat("", &fSlowMotionSpeed, 0.1);

		if (fSlowMotionSpeed > 2.0f) fSlowMotionSpeed = 2.0f;
		if (fSlowMotionSpeed < 0.0f) fSlowMotionSpeed = 0.0f;
		ImGui::Checkbox("Enable", &bSlowMotionEnabled);
	}
	if (iCurrentTab == TAB_CAMERA)
	{
		ImGui::Checkbox("Custom Camera Position", &bCustomCamera);
		ImGui::InputFloat3("X | Y | Z", &camPos.X);
		ImGui::Checkbox("Custom Camera Rotation", &bCustomCameraRot);
		ImGui::InputInt3("Pitch | Yaw | Roll", &camRot.Pitch);
		ImGui::Checkbox("Enable Freecam", &bFreeCameraMovement);
		ImGui::SameLine(); ShowHelpMarker("Requires both toggles enabled!\n You can configure keys in .ini file.");
		ImGui::InputFloat("Freecam Speed", &fFreeCameraSpeed);
		ImGui::InputInt("Freecam Rotation Speed", &iFreeCameraRotSpeed);


		ImGui::Separator();

		ImGui::Checkbox("Custom Cameras", &bEnableCustomCameras);

		if (ImGui::BeginCombo("Mode", szCurrentCameraOption))
		{
			for (int n = 0; n < IM_ARRAYSIZE(szCameraModes); n++)
			{
				bool is_selected = (szCurrentCameraOption == szCameraModes[n]);
				if (ImGui::Selectable(szCameraModes[n], is_selected))
					sprintf(szCurrentCameraOption, szCameraModes[n]);
				if (is_selected)
					ImGui::SetItemDefaultFocus();

			}
			ImGui::EndCombo();
		}
		iCurrentCustomCamera = GetCamMode(szCurrentCameraOption);
		ImGui::InputFloat("FPS Camera Offset", &fAdjustCam);
		ImGui::InputFloat("FPS Up/Down Offset", &fAdjustCamZ);
		ImGui::InputFloat("FPS Left/Right Offset", &fAdjustCamX);
	}
	if (iCurrentTab == TAB_PLAYER_CONTROL)
	{
		ImGui::Text("Position");
		ImGui::SameLine(); ShowHelpMarker("Preview only!");
		if (MK11::GetCharacterObject(PLAYER1))
		{
			MK11::GetCharacterPosition(&plrPos, PLAYER1);
			ImGui::InputFloat3("X | Y | Z", &plrPos.X);
		}
		if (MK11::GetCharacterObject(PLAYER2))
		{
			MK11::GetCharacterPosition(&plrPos2, PLAYER2);
			ImGui::InputFloat3("X | Y | Z", &plrPos2.X);
		}

	}*/
}


bool MK11Menu::GetActiveState()
{
	return bIsActive;
}

void MK11Menu::ToggleActive()
{
	bIsActive = !bIsActive;
}


// Kiero ImGui Stuff

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND hKieroWindow = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;
bool bKieroInit = false;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(hKieroWindow);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!bKieroInit)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			hKieroWindow = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(hKieroWindow, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			ermaccer_setWindow(hKieroWindow);
			bKieroInit = true;
		}
		else
		{
			return oPresent(pSwapChain, SyncInterval, Flags);
		}
			
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::GetIO().MouseDrawCursor = false;
	if (GuiMenu->GetActiveState())
	{
		GuiMenu->Draw();
	}

	ImGui::EndFrame();

	ImGui::Render();
	auto my_DrawData = ImGui::GetDrawData();
	auto ermaccer_DrawData = ermaccer_function(pSwapChain, SyncInterval, Flags);

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	ImGui_ImplDX11_RenderDrawData(my_DrawData);
	if (ermaccer_DrawData != NULL)
		ImGui_ImplDX11_RenderDrawData(ermaccer_DrawData);

	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MenuGuiMainThread(LPVOID lpReserved)
{
	HMODULE ermaccer_hook = AwaitHModule("MK11Hook.asi", 1000); // Change this to be automatic to retreive all ASIs except myself and wait for them.
	if (ermaccer_hook != NULL) // Hook Exists
	{
		ermaccer_function = (ermaccer_hkPresent*)GetProcAddress(ermaccer_hook, "SharedPresent");
		ermaccer_setWindow = (SetGlobWindow*)GetProcAddress(ermaccer_hook, "SetGlobWindow");
	}

	bool init_hook = false;
	printf("ASIMK11::Kiero::Binding\n");
	do
	{
		kiero::Status::Enum status = kiero::init(kiero::RenderType::D3D11);
		//MessageBoxA(nullptr, "Pause", "Kiero", 0);
		if (status == kiero::Status::Success)
		{
			// Wait for Ermaccer's Hook to finish ImGui Patching
			//HMODULE ermaccer_hook = AwaitHModule("MK11Hook.asi", 1000); // Change this to be automatic to retreive all ASIs except myself and wait for them.
			//if (ermaccer_hook != NULL) // Hook Exists
			//{
			//	printf("MK11Hook Present! Waiting 1 Second for the hook to finish before init ImGui\n");
			//	Sleep(1000); // Sleep 1 second, assuming ermaccer's hook is done
			//}

			kiero::Status::Enum status = kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);

	printf("ASIMK11::Kiero::Bound\n");
	return TRUE;
}