#include "mk11menu.h"
#include <Windows.h>
#include "mk11utils.h"
#include <iostream>
#include "..\imgui\imgui.h"
#include <sstream>

MK11Menu* GuiMenu = new MK11Menu();

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

void MK11Menu::Draw()
{
	SharedStyle();

	ImGui::GetIO().MouseDrawCursor = true;
	//if (!bSharedInit)
	ImGui::Begin("ASIMK11 by thethiny");
	// Tabs

	if (ImGui::BeginTabBar("ASIMK11Tabs"))
	{
		if (ImGui::BeginTabItem("Camera"))
		{
			iCurrentTab = eTabs::CAMERA;
			ImGui::EndTabItem();
		}
			
		if (ImGui::BeginTabItem("Swaps"))
		{
			iCurrentTab = eTabs::SWAPS;
			ImGui::EndTabItem();
		}
			
		if (ImGui::BeginTabItem("Cheats"))
		{
			iCurrentTab = eTabs::CHEATS;
			ImGui::EndTabItem();
		}
			
		if (ImGui::BeginTabItem("Anti Cheat"))
		{
			iCurrentTab = eTabs::ANTICHEAT;
			ImGui::EndTabItem();
		}
		
		if (ImGui::BeginTabItem("Unlocker"))
		{
			iCurrentTab = eTabs::UNLOCKER;
			ImGui::EndTabItem();
		}
		
		ImGui::EndTabBar();
	}

	if (iCurrentTab == eTabs::CAMERA)
	{
		ImGui::Separator();
		ImGui::Checkbox("Timestop", &MK11::sCamStruct.bTimestopActive);
	}
	else if (iCurrentTab == eTabs::UNLOCKER)
	{
		ImGui::TextWrapped("Unlocker Is not working until the game reaches the end of its life cycle.");
		ImGui::Spacing();
	}
	else if (iCurrentTab == eTabs::SWAPS)
	{
		ImGui::Text("Intro Swap Status:");
		ImGui::SameLine();
		if (!MK11::sActiveMods.bIntroSwap)
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
			ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Enabled");
			ImGui::Text("P1 -");
			ImGui::SameLine();
			if (MK11::sIntroStruct.bEnabled)
			{
				if (MK11::sIntroStruct.PName[0] && MK11::sIntroStruct.PChar[0] && MK11::sIntroStruct.PName2[0] && MK11::sIntroStruct.PChar2[0])
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
			if (MK11::sIntroStruct2.bEnabled)
			{
				if (MK11::sIntroStruct2.PName[0] && MK11::sIntroStruct2.PChar[0] && MK11::sIntroStruct2.PName2[0] && MK11::sIntroStruct2.PChar2[0])
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
		if (MK11::sActiveMods.bIntroSwap)
		{
			ImGui::Checkbox("Swap P1", &MK11::sIntroStruct.bEnabled);

			ImGui::PushItemWidth(200);
			if (ImGui::BeginCombo("Char1", MK11::sIntroStruct.PName))
			{
				for (auto i = MK11::sCharacters.cbegin(); i != MK11::sCharacters.cend(); i++)
				{
					bool is_selected = (MK11::sIntroStruct.PName == i->name.c_str());
					if (ImGui::Selectable(i->name.c_str(), is_selected))
					{
						//sprintf(MK11::sIntroStruct.PName, toUpper(i->name).c_str());
						sprintf(MK11::sIntroStruct.PName, i->name.c_str());
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PushItemWidth(40);
			if (ImGui::BeginCombo("Ltr1", MK11::sIntroStruct.PChar))
			{
				auto Chara = std::find(MK11::sCharacters.begin(), MK11::sCharacters.end(), MK11::sIntroStruct.PName);
				for (int j = 0; j <= Chara->intros; j++)
				{
					char t[2] = { 'A' + j };
					bool is_selected = (MK11::sIntroStruct.PChar == t);
					if (ImGui::Selectable(t, is_selected))
					{
						sprintf(MK11::sIntroStruct.PChar, t);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::PushItemWidth(200);
			if (ImGui::BeginCombo("sChar1", MK11::sIntroStruct.PName2))
			{
				for (auto i = MK11::sCharacters.cbegin(); i != MK11::sCharacters.cend(); i++)
				{
					bool is_selected = (MK11::sIntroStruct.PName2 == i->name.c_str()); // From Above
					if (ImGui::Selectable(i->name.c_str(), is_selected))
					{
						sprintf(MK11::sIntroStruct.PName2, i->name.c_str());
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PushItemWidth(40);
			if (ImGui::BeginCombo("sLtr1", MK11::sIntroStruct.PChar2))
			{
				auto Chara = std::find(MK11::sCharacters.begin(), MK11::sCharacters.end(), MK11::sIntroStruct.PName2);
				for (int j = 0; j <= Chara->intros; j++)
				{
					char t[2] = { 'A' + j };
					bool is_selected = (MK11::sIntroStruct.PChar2 == t);
					if (ImGui::Selectable(t, is_selected))
					{
						sprintf(MK11::sIntroStruct.PChar2, t);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			///////////////////////
			ImGui::Checkbox("Swap P2", &MK11::sIntroStruct2.bEnabled);
			ImGui::SameLine();
			if (ImGui::Button("Mirror Swap"))
			{
				sprintf(MK11::sIntroStruct2.PName, MK11::sIntroStruct.PName2);
				sprintf(MK11::sIntroStruct2.PName2, MK11::sIntroStruct.PName);
				sprintf(MK11::sIntroStruct2.PChar, MK11::sIntroStruct.PChar);
				sprintf(MK11::sIntroStruct2.PChar2, MK11::sIntroStruct.PChar2);

			}

			ImGui::PushItemWidth(200);
			if (ImGui::BeginCombo("Char2", MK11::sIntroStruct2.PName))
			{
				for (auto i = MK11::sCharacters.cbegin(); i != MK11::sCharacters.cend(); i++)
				{
					bool is_selected = (MK11::sIntroStruct2.PName == i->name.c_str());
					if (ImGui::Selectable(i->name.c_str(), is_selected))
					{
						//sprintf(MK11::sIntroStruct.PName, toUpper(i->name).c_str());
						sprintf(MK11::sIntroStruct2.PName, i->name.c_str());
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PushItemWidth(40);
			if (ImGui::BeginCombo("Ltr2", MK11::sIntroStruct2.PChar))
			{

				auto Chara = std::find(MK11::sCharacters.begin(), MK11::sCharacters.end(), MK11::sIntroStruct2.PName);
				for (int j = 0; j <= Chara->intros; j++)
				{
					char t[2] = { 'A' + j };
					bool is_selected = (MK11::sIntroStruct2.PChar == t);
					if (ImGui::Selectable(t, is_selected))
					{
						sprintf(MK11::sIntroStruct2.PChar, t);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();

			ImGui::PushItemWidth(200);
			if (ImGui::BeginCombo("sChar2", MK11::sIntroStruct2.PName2))
			{
				for (auto i = MK11::sCharacters.cbegin(); i != MK11::sCharacters.cend(); i++)
				{
					bool is_selected = (MK11::sIntroStruct2.PName2 == i->name.c_str());
					if (ImGui::Selectable(i->name.c_str(), is_selected))
					{
						sprintf(MK11::sIntroStruct2.PName2, i->name.c_str());
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine();
			ImGui::PushItemWidth(40);
			if (ImGui::BeginCombo("sLtr2", MK11::sIntroStruct2.PChar2))
			{

				auto Chara = std::find(MK11::sCharacters.begin(), MK11::sCharacters.end(), MK11::sIntroStruct2.PName2);
				for (int j = 0; j <= Chara->intros; j++)
				{
					char t[2] = { 'A' + j };
					bool is_selected = (MK11::sIntroStruct2.PChar2 == t);
					if (ImGui::Selectable(t, is_selected))
					{
						sprintf(MK11::sIntroStruct2.PChar2, t);
					}
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
		}

		// Mod Loader Section
		ImGui::Separator();
		ImGui::Text("File Swaps Status:");
		ImGui::SameLine();
		if (MK11::sActiveMods.bModLoader)
		{
			while (MK11::vSwappedFiles.size() > SettingsMgr->iLogSize)
			{
				MK11::vSwappedFiles.erase(MK11::vSwappedFiles.begin());
			}
			ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Active");
			char buffer[32];
			sprintf(buffer, "Last (%u) Swaps:", SettingsMgr->iLogSize);
			buffer[31] = '\0';
			ImGui::Text(buffer);
			for (auto i = MK11::vSwappedFiles.rbegin(); i != MK11::vSwappedFiles.rend(); i++)
			{			
				char* dest;
				uint32_t len = wcslen(i->c_str());
				dest = new char[len+1];
				wcstombs(dest, i->c_str(), len);
				dest[len] = '\0'; // Terminate String to avoid glitches in text
				ImGui::TextColored(ImVec4(0.8f, 0.95f, 0.8f, 1.f), dest);
			}
		}
		else
		{
			if (SettingsMgr->bModLoader)
			{
				ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Error");
			}
			else
			{
				ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Disabled");
			}
		}
		
	}
	else if (iCurrentTab == eTabs::ANTICHEAT)
	{
		ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
		ImGui::Checkbox("Cheat Engine Detection", &MK11::sActiveMods.bAntiCheatEngine);
		ImGui::Checkbox("Content Validation Check 1 (exe)", &MK11::sActiveMods.bAntiCVD1);
		ImGui::Checkbox("Content Validation Check 2 (ContentValidationData.txt)", &MK11::sActiveMods.bAntiCVD2);
		ImGui::PopItemFlag();
	}
	else if (iCurrentTab == eTabs::CHEATS)
	{
		if (SettingsMgr->bEnableCheats)
			{
				ImGui::TextColored(ImVec4(0.7f, 0.3f, 0.f, 1.f), "Not Yet Implemented");
				ImGui::TextWrapped(std::string("Available Cheats are as seen below. To use the cheats hold the "
					+ SettingsMgr->hkCheats +
					" Button, then press the number of the cheat you want to enable").c_str());
				ImGui::Spacing();

				ImVec4 v4Enabled = ImVec4(0.f, 1.f, 0.f, 1.f);
				ImVec4 v4Disabled = ImVec4(1.f, 0.f, 0.f, 1.f);
				ImVec4* vPtr = &v4Disabled;

				ImGui::Bullet();
				if (MK11::sCheatsStruct.lpMercy)
					vPtr = &v4Enabled;
				ImGui::TextColored(*vPtr, "1.\tPerform Mercy Any Time.");
				vPtr = &v4Disabled;
				ImGui::SameLine();
				if (MK11::sCheatsStruct.bMercy)
					ImGui::TextColored(v4Enabled, "On");
				else
				{
					ImGui::TextColored(v4Disabled, "Off");
				}

				ImGui::Bullet();
				if (MK11::sCheatsStruct.lpGround)
					vPtr = &v4Enabled;
				ImGui::TextColored(*vPtr, "2.\tPerform Ground Moves Any Time.");
				vPtr = &v4Disabled;
				ImGui::SameLine();
				if (MK11::sCheatsStruct.bGround)
					ImGui::TextColored(v4Enabled, "On");
				else
				{
					ImGui::TextColored(v4Disabled, "Off");
				}
				
				ImGui::Bullet();
				if (MK11::sCheatsStruct.lpBrut)
					vPtr = &v4Enabled;
				ImGui::TextColored(*vPtr, "3.\tNo Brutality Requirements.\n\tPerform Brutality Anytime.");
				vPtr = &v4Disabled;
				ImGui::SameLine();
				if (MK11::sCheatsStruct.bBrut)
					ImGui::TextColored(v4Enabled, "On");
				else
				{
					ImGui::TextColored(v4Disabled, "Off");
				}

				ImGui::Bullet();
				if (MK11::sCheatsStruct.lpMeteor)
					vPtr = &v4Enabled;
				ImGui::TextColored(*vPtr, "4.\tTowers of Time Meteor Always Spawns.");
				vPtr = &v4Disabled;
				ImGui::SameLine();
				if (MK11::sCheatsStruct.bMeteor)
					ImGui::TextColored(v4Enabled, "On");
				else
				{
					ImGui::TextColored(v4Disabled, "Off");
				}

				ImGui::Bullet();
				if (MK11::sCheatsStruct.lpDizzy)
					vPtr = &v4Enabled;
				ImGui::TextColored(*vPtr, "5.\tAlways Allow FinishHim.");
				vPtr = &v4Disabled;
				ImGui::SameLine();
				if (MK11::sCheatsStruct.bDizzy)
					ImGui::TextColored(v4Enabled, "On");
				else
				{
					ImGui::TextColored(v4Disabled, "Off");
				}

				ImGui::Bullet();
				if (MK11::sCheatsStruct.lpFatCombo)
					vPtr = &v4Enabled;
				ImGui::TextColored(*vPtr, "6.\tAllow Non-Chain combos in FinishHim.");
				vPtr = &v4Disabled;
				ImGui::SameLine();
				if (MK11::sCheatsStruct.bFatCombo)
					ImGui::TextColored(v4Enabled, "On");
				else
				{
					ImGui::TextColored(v4Disabled, "Off");
				}

				ImGui::Bullet();
				if (MK11::sCheatsStruct.lpNoBlock)
					vPtr = &v4Enabled;
				ImGui::TextColored(*vPtr, "7.\tBlocking Disabled.");
				vPtr = &v4Disabled;
				ImGui::SameLine();
				if (MK11::sCheatsStruct.bNoBlock)
					ImGui::TextColored(v4Enabled, "On");
				else
				{
					ImGui::TextColored(v4Disabled, "Off");
				}

				ImGui::Bullet();
				if (MK11::sCheatsStruct.lpFatalBlow)
					vPtr = &v4Enabled;
				ImGui::TextColored(*vPtr, "8.\tAllow Fatal Blow at Full Health.");
				vPtr = &v4Disabled;
				ImGui::SameLine();
				if (MK11::sCheatsStruct.bFatalBlow)
					ImGui::TextColored(v4Enabled, "On");
				else
				{
					ImGui::TextColored(v4Disabled, "Off");
				}
			}
		else
		{
			ImGui::TextWrapped("Cheats Are Disabled in the Settings.");
			ImGui::Spacing();
		}
		
	}
	else
	{
		ImGui::TextWrapped((szTabName[iCurrentTab] + " not yet implemented.").c_str());
		ImGui::Spacing();
	}

	if (!InitImGui)
		ImGui::End();

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


bool bSharedInit = false;
void InitShared()
{
	bSharedInit = true;
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
	if (GuiMenu->GetActiveState())
	{
		ImGui::SetCurrentContext(ctx);

		GuiMenu->Draw();

		return ImGui::GetCurrentContext();
	}
	else
		return ctx;
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
	auto my_DrawData = ImGui::GetDrawData();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);

	ImGui_ImplDX11_RenderDrawData(my_DrawData);

	return oPresent(pSwapChain, SyncInterval, Flags);
}

inline bool ShouldHookDX(float timeout = 2.5f)
{
	auto start = std::chrono::system_clock::now();
	while (!bSharedInit)
	{
		std::chrono::duration<double> now = std::chrono::system_clock::now() - start;
		if (now.count() > timeout) // Seconds
		{
			return false; // Exit Loop cuz no loader
		}
	}
	return true;
	
}

DWORD WINAPI MenuGuiMainThread(LPVOID lpReserved)
{
	if (ShouldHookDX(2.5f))
	{
		return true;
	}

	bool init_hook = false;
	printf("ASIMK11::Kiero::Binding\n");
	do
	{
		kiero::Status::Enum status = kiero::init(kiero::RenderType::D3D11);
		if (status == kiero::Status::Success)
		{
			kiero::Status::Enum status = kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);

	printf("ASIMK11::Kiero::Bound\n");
	return TRUE;
}