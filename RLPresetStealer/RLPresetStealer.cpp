#include "pch.h"
#include "RLPresetStealer.h"
#include "bakkesmod/utilities/LoadoutUtilities.h"





BAKKESMOD_PLUGIN(RLPresetStealer, "Copy a player's preset in your game for you to use", plugin_version, PLUGINTYPE_FREEPLAY)


std::shared_ptr<CVarManagerWrapper> _globalCvarManager;



void RLPresetStealer::onLoad()
{
	_globalCvarManager = cvarManager;



	loadHooks();
	registerCvars();

	

	//cvarManager->registerNotifier("my_aweseome_notifier", [&](std::vector<std::string> args) {
	//	cvarManager->log("Hello notifier!");
	//}, "", 0);


	//cvar.addOnValueChanged([this](std::string cvarName, CVarWrapper newCvar) {
	//	cvarManager->log("the cvar with name: " + cvarName + " changed");
	//	cvarManager->log("the new value is:" + newCvar.getStringValue());
	//});

	//cvar2.addOnValueChanged(std::bind(&RLPresetStealer::YourPluginMethod, this, _1, _2));

	// enabled decleared in the header
	//enabled = std::make_shared<bool>(false);
	//cvarManager->registerCvar("TEMPLATE_Enabled", "0", "Enable the TEMPLATE plugin", true, true, 0, true, 1).bindTo(enabled);

	//cvarManager->registerNotifier("NOTIFIER", [this](std::vector<std::string> params){FUNCTION();}, "DESCRIPTION", PERMISSION_ALL);
	//cvarManager->registerCvar("CVAR", "DEFAULTVALUE", "DESCRIPTION", true, true, MINVAL, true, MAXVAL);//.bindTo(CVARVARIABLE);
	//gameWrapper->HookEvent("FUNCTIONNAME", std::bind(&TEMPLATE::FUNCTION, this));
	//gameWrapper->HookEventWithCallerPost<ActorWrapper>("FUNCTIONNAME", std::bind(&RLPresetStealer::FUNCTION, this, _1, _2, _3));
	//gameWrapper->RegisterDrawable(bind(&TEMPLATE::Render, this, std::placeholders::_1));


	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", [this](std::string eventName) {
	//	cvarManager->log("Your hook got called and the ball went POOF");
	//});
	// You could also use std::bind here
	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", std::bind(&RLPresetStealer::YourPluginMethod, this);

	//
}



void RLPresetStealer::onUnload()
{
	LOG("goodbye world");
	
}





void RLPresetStealer::loadHooks() {

	LOG("loading all hooks");

	//need to think of where to hook the pri calls
	//if player leaves game early this will not call
	//gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded", std::bind(&RLPresetStealer::loadAllPresetsInLobby, this));

	//for easier testing
	gameWrapper->HookEvent("Function GameEvent_Soccar_TA.Active.StartRound", std::bind(&RLPresetStealer::loadAllPresetsInLobby, this));
	
}



void RLPresetStealer::loadAllPresetsInLobby() {


	//null check being in game
	if (!gameWrapper->IsInOnlineGame()) {
		return;
	}

	ServerWrapper server = gameWrapper->GetCurrentGameState();


	//null check server
	if (!server) { return; }


	//if this is reached we have user is in game and server exists
	//get all PRIs from game event object
	auto array_pris = server.GetPRIs();

	if (array_pris.IsNull()) {
		LOG("PRIS ARE NULL");
		return;
	}

	//loop through each PRI and get the loadout
	for (PriWrapper pri : array_pris) {


		std::optional<pluginsdk::Loadout> loadout_promise = LoadoutUtilities::GetLoadoutFromPri(pri, pri.GetTeamNum2());


		if (!loadout_promise) { continue; }



		pluginsdk::ItemDataMap& items = loadout_promise->items;
		pluginsdk::CarColors& colors = loadout_promise->paint_finish;

		std::map<pluginsdk::Equipslot, pluginsdk::ItemData>::iterator it;

		LOG("items logging");

		for (it = items.begin(); it != items.end(); it++) {

			LOG("equipslot = {}, item data = {}", it->first, it->second.product_id);

		}


		LOG(colors);



	



		//make sure names are correct
		LOG("got a loadout From PRI: " + pri.GetPlayerName().ToString());


	}

	
}


void RLPresetStealer::registerCvars() {

	cvarManager->registerCvar("presetStealer_enabled", "1", "disable/enable preset", true, true, 0, true, 1);
	cvarManager->registerCvar("presetStealer_autoSwitchLoadout", "1", "disable/enable auto switch", true, true, 0, true, 1);

	//max presets saved is 7 because largest online-gamemode is chaos(7 ppl besides current user)
	cvarManager->registerCvar("presetStealer_numPresetSaves", "1", "number of presets that can be saved per game", true, true, 1, true, 7);


}


