#include "pch.h"
#include "RLPresetStealer.h"
#include "bakkesmod/utilities/LoadoutUtilities.h"
#include "bakkesmod/core/loadout_structs.h"





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

	/*
	* Hook the actual fetching of pris and loadouts to happen during game,
	* but the swaps can only be made in the lobby
	*/
	//gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.EventMatchEnded", std::bind(&RLPresetStealer::loadAllPresetsInLobby, this));
	//for easier testing
	gameWrapper->HookEvent("Function GameEvent_Soccar_TA.Active.StartRound", std::bind(&RLPresetStealer::loadAllPresetsInLobby, this));


	gameWrapper->HookEvent("Function TAGame.GameEvent_Soccar_TA.Destroyed", std::bind(&RLPresetStealer::displayAllSwapButtons, this));

	
}

void RLPresetStealer::callCodeInConsole(std::string code)

{
	//have to have itemmod enable so call that first
	cvarManager->executeCommand("cl_itemmod_enabled 1; cl_itemmod_code \"" + code + "\"");
}


/*
* MAKE SURE THIS IS CALLED IN MAIN MENU
* 
*/
void RLPresetStealer::displayAllSwapButtons() {

	/*
	* should happen in main lobby. null check more?
	*/

	LOG("Displaying all swap buttons");

	
	if (gameWrapper->IsInFreeplay()){

		LOG("in free play");

		

	}

	if (gameWrapper->IsInCustomTraining()) {

		LOG("in custom training");


	}

	if (gameWrapper->IsInOnlineGame()) {

		LOG("in online game");

	}

	if (this->RLPRESESTSTEALER_map.empty() ){

		LOG("Loadouts are empty");

		return;
	}


	std::map<std::string, BMLoadout::BMLoadout> name_with_loadout = this->RLPRESESTSTEALER_map;

	

	std::map<std::string, BMLoadout::BMLoadout>::iterator mapIt;



	for (mapIt = name_with_loadout.begin(); mapIt != name_with_loadout.end(); ++mapIt) {

		const char* name = mapIt->first.c_str();

		BMLoadout::BMLoadout loadout = mapIt->second;


		if (ImGui::Button(name)) {

			LOG("created button for " + *name);

			std::string presetCode = BMLoadout::save(loadout);

			//callCodeInConsole(presetCode);


		}



	}


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

	std::map<std::string, BMLoadout::BMLoadout> loadout_map;

	//loop through each PRI and get the loadout
	for (PriWrapper pri : array_pris) {


		std::optional<pluginsdk::Loadout> loadout_promise = LoadoutUtilities::GetLoadoutFromPri(pri, pri.GetTeamNum2());


		if (!loadout_promise) { continue; }



		pluginsdk::ItemDataMap& items = loadout_promise->items;
		pluginsdk::CarColors& colors = loadout_promise->paint_finish;

		std::map<pluginsdk::Equipslot, pluginsdk::ItemData>::iterator it;

		
		

		LOG("items logging");


		//for testing the print out
		/*
		for (it = items.begin(); it != items.end(); it++) {

			LOG("equipslot = {}, item data = {}", it->first, it->second.product_id);

		}
		*/ 

		//make sure names are correct
		LOG("got a loadout From PRI: " + pri.GetPlayerName().ToString());


		//save the loadout
		BMLoadout::BMLoadout newLoadout;


		newLoadout.body.blueColor.should_override = true;
		newLoadout.body.orangeColor.should_override = true;


		
		for (it = items.begin(); it != items.end(); ++it) {

			//TODO:  null check any of this?

			BMLoadout::Item item;

			item.slot_index = (uint8_t)it->first;

			item.product_id = (uint16_t)it->second.product_id;

			
			//How to get paint index from items?
			//item.paint_index = (uint8_t)it->second.attributes.at(pluginsdk::ItemAttribute::AttributeType::PAINT);
			
			//pluginsdk::ItemAttribute::AttributeType::PAINT

			

			newLoadout.body.blue_loadout[item.slot_index] = item;
			newLoadout.body.orange_loadout[item.slot_index] = item;




		}

		

		//add singular loadout to map of all loadouts

		std::pair<std::string, BMLoadout::BMLoadout> name_loadout_pair;
		name_loadout_pair.first = pri.GetPlayerName().ToString();
		name_loadout_pair.second = newLoadout;


		loadout_map.insert(name_loadout_pair);




	}

	
	this->RLPRESESTSTEALER_map = loadout_map;

	
}


void RLPresetStealer::registerCvars() {

	cvarManager->registerCvar("presetStealer_enabled", "1", "disable/enable preset stealer", true, true, 0, true, 1);
	cvarManager->registerCvar("presetStealer_autoSwitchLoadout", "1", "disable/enable auto switch", true, true, 0, true, 1);

	//max presets saved is 7 because largest online-gamemode is chaos(7 ppl besides current user)
	cvarManager->registerCvar("presetStealer_numPresetSaves", "1", "number of presets that can be saved per game", true, true, 1, true, 7);




}


