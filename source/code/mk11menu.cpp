#include "mk11menu.h"
#include <Windows.h>
#include "mk11utils.h"
#include <iostream>
#include "..\imgui\imgui.h"

MK11Menu* GuiMenu = new MK11Menu();

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

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MenuGuiMainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	printf("ASIMK11::Kiero::Binding\n");
	do
	{
		kiero::Status::Enum status = kiero::init(kiero::RenderType::D3D11);
		//MessageBoxA(nullptr, "Pause", "Kiero", 0);
		if (status == kiero::Status::Success)
		{
			// Wait for Ermaccer's Hook to finish ImGui Patching
			HMODULE ermaccer_hook = AwaitHModule("MK11Hook.asi", 1000); // Change this to be automatic to retreive all ASIs except myself and wait for them.
			if (ermaccer_hook != NULL) // Hook Exists
			{
				printf("MK11Hook Present! Waiting 1 Second for the hook to finish before init ImGui\n");
				Sleep(1000); // Sleep 1 second, assuming ermaccer's hook is done
			}

			kiero::Status::Enum status = kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	printf("ASIMK11::Kiero::Bound\n");
	return TRUE;
}