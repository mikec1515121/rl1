#include "pch.h"
#include "RLPresetStealer.h"


std::string RLPresetStealer::GetPluginName() {
	return "RLPresetStealer";
}

void RLPresetStealer::SetImGuiContext(uintptr_t ctx) {
	ImGui::SetCurrentContext(reinterpret_cast<ImGuiContext*>(ctx));
}



// Render the plugin settings here
// This will show up in bakkesmod when the plugin is loaded at
//  f2 -> plugins -> RLPresetStealer


void RLPresetStealer::RenderSettings() {

    ImGui::TextUnformatted("After each game, you will have the ability to save and equip someones preset from the previous game");

    //enable_disable plugin toggle
    CVarWrapper enableCvarPlug = cvarManager->getCvar("presetStealer_enabled");

    if (!enableCvarPlug) { return; }

    bool enabledPlug = enableCvarPlug.getBoolValue();

    if (ImGui::Checkbox("Disable plugin", &enabledPlug)) {

        enableCvarPlug.setValue(enabledPlug);
    }


    //enable_disable auto swap toggle
    CVarWrapper enableCvarSwap = cvarManager->getCvar("presetStealer_autoSwitchLoadout");

    if (!enableCvarSwap) { return; }

    bool enabledSwap = enableCvarSwap.getBoolValue();

    if (ImGui::Checkbox("Disable Auto Swap", &enabledSwap)) {

        enableCvarSwap.setValue(enabledSwap);
    }



    //slider for the max number of players' presets that user can save

    /*
    CVarWrapper numSavesCvar = cvarManager->getCvar("presetStealer_numPresetSaves");

    if (!numSavesCvar) { return; }

    int numSaves = numSavesCvar.getIntValue();

    if (ImGui::SliderInt("Number of presets that can be saved", &numSaves, 1, 7)) {

        numSavesCvar.setValue(numSaves);

    }
    if (ImGui::IsItemHovered()) {

        std::string hoverText = "current =  " + std::to_string(numSaves);

        ImGui::SetTooltip(hoverText.c_str());

    }


    */



}



// Do ImGui rendering here
void RLPresetStealer::Render()
{
	if (!ImGui::Begin(menuTitle_.c_str(), &isWindowOpen_, ImGuiWindowFlags_None))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::End();

	if (!isWindowOpen_)
	{
		cvarManager->executeCommand("togglemenu " + GetMenuName());
	}
}

// Name of the menu that is used to toggle the window.
std::string RLPresetStealer::GetMenuName()
{
	return "rlpresetstealer";
}

// Title to give the menu
std::string RLPresetStealer::GetMenuTitle()
{
	return menuTitle_;
}



// Should events such as mouse clicks/key inputs be blocked so they won't reach the game
bool RLPresetStealer::ShouldBlockInput()
{
	return ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard;
}

// Return true if window should be interactive
bool RLPresetStealer::IsActiveOverlay()
{
	return true;
}

// Called when window is opened
void RLPresetStealer::OnOpen()
{
	isWindowOpen_ = true;
}

// Called when window is closed
void RLPresetStealer::OnClose()
{
	isWindowOpen_ = false;
}

