#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "BMLoadout.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class RLPresetStealer: public BakkesMod::Plugin::BakkesModPlugin, public BakkesMod::Plugin::PluginSettingsWindow, public BakkesMod::Plugin::PluginWindow
{

	std::shared_ptr<bool> enabled;
	std::map<std::string, BMLoadout::BMLoadout> RLPRESESTSTEALER_map;

	//Boilerplate
	virtual void onLoad();
	virtual void onUnload();


	void loadAllPresetsInLobby();
	void displayAllSwapButtons();
	void callCodeInConsole(std::string code);


	void loadHooks();
	void registerCvars();
	

	// Inherited via PluginSettingsWindow

	void RenderSettings() override;

	std::string GetPluginName() override;
	void SetImGuiContext(uintptr_t ctx) override;
	


	// Inherited via PluginWindow
	

	bool isWindowOpen_ = false;
	bool isMinimized_ = false;
	std::string menuTitle_ = "RLPresetStealer";

	
	virtual void Render() override;
	virtual std::string GetMenuName() override;
	virtual std::string GetMenuTitle() override;
	
	virtual bool ShouldBlockInput() override;
	virtual bool IsActiveOverlay() override;
	virtual void OnOpen() override;
	virtual void OnClose() override;

	
	
	
};

