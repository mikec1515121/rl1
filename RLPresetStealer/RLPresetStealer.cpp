#include "pch.h"
#include "RLPresetStealer.h"
#include "bakkesmod/utilities/LoadoutUtilities.h"





BAKKESMOD_PLUGIN(RLPresetStealer, "Copy a player's preset in your game for you to use", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RLPresetStealer::onLoad()
{
	_globalCvarManager = cvarManager;
	LOG("hello world!");
	loadHooks();

	//cvarManager->registerNotifier("my_aweseome_notifier", [&](std::vector<std::string> args) {
	//	cvarManager->log("Hello notifier!");
	//}, "", 0);

	//auto cvar = cvarManager->registerCvar("template_cvar", "hello-cvar", "just a example of a cvar");
	//auto cvar2 = cvarManager->registerCvar("template_cvar2", "0", "just a example of a cvar with more settings", true, true, -10, true, 10 );

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

		auto loadout_promise = LoadoutUtilities::GetLoadoutFromPri(pri, pri.GetTeamNum2());

		if (!loadout_promise) { continue; }

		auto& [items, paint_finish] = *loadout_promise;

		LOG("got a loadout From PRI");


	}

	
}


