#include "mk11menu.h"
#include <Windows.h>
#include "mk11utils.h"
#include <iostream>
#include "..\imgui\imgui.h"
#include <sstream>

MK11Menu* GuiMenu = new MK11Menu();

//typedef ImDrawData* (__stdcall ermaccer_hkPresent)(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
//typedef void(__fastcall SetGlobWindow)(HWND window);
//
//ermaccer_hkPresent* ermaccer_function;
//SetGlobWindow* ermaccer_setWindow;

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

void MK11Menu::Color()
{

}

void MK11Menu::Draw()
{
	//Color();
	SharedStyle();

	ImGui::GetIO().MouseDrawCursor = true;
	//ImGui::Begin("ASIMK11 by thethiny");

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
		}
		else
		{
			ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Disabled");
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.f), "(Settings)");
		}

		ImGui::Separator();
		ImGui::Checkbox("Timestop", &sCamStruct.bTimestopActive);

	}
	else if (iCurrentTab == eTabs::UNLOCKER)
	{
		ImGui::TextWrapped("Unlocker Is not working until the game reaches the end of its life cycle.");
		ImGui::Spacing();
	}
	else if (iCurrentTab == eTabs::SWAPS)
	{
		ImGui::Text("Status:");
		ImGui::SameLine();
		if (!sActiveMods.bIntroSwap)
		{
			if (SettingsMgr->bEnableIntroSwap)
			{
				ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Disabled");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.f), "(Error)");
			}
			else
			{
				ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Disabled");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.f), "(Settings)");
			}
		}
		else
		{
			//ImGui::TextColored(ImVec4(1.f, .6f, 0.f, 1.f), "InActive");
			ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Enabled");
			ImGui::Text("P1 -");
			ImGui::SameLine();
			if (sIntroStruct.bEnabled)
			{
				if (sIntroStruct.PName[0] && sIntroStruct.PChar[0] && sIntroStruct.PName2[0] && sIntroStruct.PChar2[0])
				{
					ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Active");
				}
				else
				{
					ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Missing");
				}
			}
			else
			{
				ImGui::TextColored(ImVec4(1.f, .6f, 0.f, 1.f), "InActive");
			}
			ImGui::SameLine();
			ImGui::Text("- P2 -");
			ImGui::SameLine();
			if (sIntroStruct2.bEnabled)
			{
				if (sIntroStruct2.PName[0] && sIntroStruct2.PChar[0] && sIntroStruct2.PName2[0] && sIntroStruct2.PChar2[0])
				{
					ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Active");
				}
				else
				{
					ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "Missing");
				}
			}
			else
			{
				ImGui::TextColored(ImVec4(1.f, .6f, 0.f, 1.f), "InActive");
			}
			ImGui::Separator();

		}
		if (sActiveMods.bIntroSwap)
		{
			ImGui::Checkbox("Swap P1", &sIntroStruct.bEnabled);

			ImGui::PushItemWidth(200);
			if (ImGui::BeginCombo("Char1", sIntroStruct.PName))
			{
				for (int i = 0; i < 5; i++)
				{
					bool is_selected = (sIntroStruct.PName == MK11::szCharacters[i]);
					if (ImGui::Selectable(MK11::szCharacters[i], is_selected))
					{
						sprintf(sIntroStruct.PName, toUpper(MK11::szCharacters[i]).c_str());
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PushItemWidth(40);
			if (ImGui::BeginCombo("Ltr1", sIntroStruct.PChar))
			{

				for (int j = 0; j < 5; j++)
				{
					char t[2] = { 'A' + j };
					bool is_selected = (sIntroStruct.PChar == t);
					if (ImGui::Selectable(t, is_selected))
					{
						sprintf(sIntroStruct.PChar, t);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::PushItemWidth(200);
			if (ImGui::BeginCombo("sChar1", sIntroStruct.PName2))
			{
				for (int i = 0; i < 5; i++)
				{
					bool is_selected = (sIntroStruct.PName2 == MK11::szCharacters[i]);
					if (ImGui::Selectable(MK11::szCharacters[i], is_selected))
					{
						sprintf(sIntroStruct.PName2, toUpper(MK11::szCharacters[i]).c_str());
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PushItemWidth(40);
			if (ImGui::BeginCombo("sLtr1", sIntroStruct.PChar2))
			{

				for (int j = 0; j < 5; j++)
				{
					char t[2] = { 'A' + j };
					bool is_selected = (sIntroStruct.PChar2 == t);
					if (ImGui::Selectable(t, is_selected))
					{
						sprintf(sIntroStruct.PChar2, t);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			///////////////////////
			ImGui::Checkbox("Swap P2", &sIntroStruct2.bEnabled);
			ImGui::SameLine();
			if (ImGui::Button("Mirror Swap"))
			{
				sprintf(sIntroStruct2.PName, sIntroStruct.PName2);
				sprintf(sIntroStruct2.PName2, sIntroStruct.PName);
				sprintf(sIntroStruct2.PChar, sIntroStruct.PChar);
				sprintf(sIntroStruct2.PChar2, sIntroStruct.PChar2);

			}

			ImGui::PushItemWidth(200);
			if (ImGui::BeginCombo("Char2", sIntroStruct2.PName))
			{
				for (int i = 0; i < 5; i++)
				{
					bool is_selected = (sIntroStruct2.PName == MK11::szCharacters[i]);
					if (ImGui::Selectable(MK11::szCharacters[i], is_selected))
					{
						sprintf(sIntroStruct2.PName, toUpper(MK11::szCharacters[i]).c_str());
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PushItemWidth(40);
			if (ImGui::BeginCombo("Ltr2", sIntroStruct2.PChar))
			{

				for (int j = 0; j < 5; j++)
				{
					char t[2] = { 'A' + j };
					bool is_selected = (sIntroStruct2.PChar == t);
					if (ImGui::Selectable(t, is_selected))
					{
						sprintf(sIntroStruct2.PChar, t);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::PushItemWidth(200);
			if (ImGui::BeginCombo("sChar2", sIntroStruct2.PName2))
			{
				for (int i = 0; i < 5; i++)
				{
					bool is_selected = (sIntroStruct2.PName2 == MK11::szCharacters[i]);
					if (ImGui::Selectable(MK11::szCharacters[i], is_selected))
					{
						sprintf(sIntroStruct2.PName2, toUpper(MK11::szCharacters[i]).c_str());
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PushItemWidth(40);
			if (ImGui::BeginCombo("sLtr2", sIntroStruct2.PChar2))
			{

				for (int j = 0; j < 5; j++)
				{
					char t[2] = { 'A' + j };
					bool is_selected = (sIntroStruct2.PChar2 == t);
					if (ImGui::Selectable(t, is_selected))
					{
						sprintf(sIntroStruct2.PChar2, t);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
		}
		
	}
	else if (iCurrentTab == eTabs::ANTICHEAT)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::Checkbox("Cheat Engine Detection", &sActiveMods.bAntiCheatEngine);
		ImGui::Checkbox("Content Validation Check 1 (exe)", &sActiveMods.bAntiCVD1);
		ImGui::Checkbox("Content Validation Check 2 (ContentValidationData.txt)", &sActiveMods.bAntiCVD2);
		ImGui::PopItemFlag();
	}
	else
	{
		ImGui::TextWrapped((szTabName[iCurrentTab] + " not yet implemented.").c_str());
		ImGui::Spacing();
	}

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

ImGuiContext* SharedStyle(ImGuiContext* ctx)
{
	if (ctx)
		ImGui::SetCurrentContext(ctx);

	//Style
	ImGuiStyle* style = &ImGui::GetStyle();
	ImVec4* colors = style->Colors;

	colors[ImGuiCol_Text] = ImVec4(0.92f, 0.92f, 0.92f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(0.8f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.9f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.7f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.9f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.7f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.53f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.47f, 0.47f, 0.47f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.81f, 0.83f, 0.81f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.7f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.91f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.7f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.8f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.7f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.9f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.8f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.9f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.7f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.4f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.7f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.8f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.4f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.7f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.9f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_Tab] = ImVec4(0.8f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.9f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_TabActive] = ImVec4(0.7f, 0.f, 0.f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);


	return ImGui::GetCurrentContext();
}

ImGuiContext * __stdcall SharedPresent(ImGuiContext* ctx)
{
	ImGui::SetCurrentContext(ctx);

	GuiMenu->Draw();

	return ImGui::GetCurrentContext();

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
			//ermaccer_setWindow(hKieroWindow);
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
	//auto ermaccer_DrawData = ermaccer_function(pSwapChain, SyncInterval, Flags);

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	ImGui_ImplDX11_RenderDrawData(my_DrawData);
	/*if (ermaccer_DrawData != NULL)
		ImGui_ImplDX11_RenderDrawData(ermaccer_DrawData);*/

	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MenuGuiMainThread(LPVOID lpReserved)
{
	HMODULE ermaccer_hook = AwaitHModule("MK11Hook.asi", 1000); // Change this to be automatic to retreive all ASIs except myself and wait for them.
	if (ermaccer_hook != NULL) // Hook Exists
	{
		/*ermaccer_function = (ermaccer_hkPresent*)GetProcAddress(ermaccer_hook, "SharedPresent");
		ermaccer_setWindow = (SetGlobWindow*)GetProcAddress(ermaccer_hook, "SetGlobWindow");*/
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