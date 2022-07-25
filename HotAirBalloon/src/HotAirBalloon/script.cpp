#include "script.h"
#include "keyboard.h"
#include "config.h"
#include <string>

/*
* Name: Realistic Hot Air Balloon
* Description: Reintroduces hot air balloon controls into the game with a mostly realistic feeling.
* Features: Controller support, automatic despawning, user-friendly controls, configuration file (optional)
* 
* Installation: Put HotAirBalloon.asi (and BalloonConfig.ini if you want it) in your game directory.
* 
* To spawn the balloon, press the semicolon ( ; ) key. This can be changed in the config file.
* Note: The config file is optional and is NOT required.
* 
* - Mod is a WIP.
* - I tried to make the balloon feel as smooth and realistic as possible. If you want to mess around with some settings, then you'll need the configuration file. There are no limits to the settings.
* - I am trying to get animations working along with correct seat position, but it's kind of difficult at the moment.
*/


Ped playerPed;
Vector3 playerPos;


 #pragma region Prompt Stuff
Prompt ascendPrompt;
Prompt descendPrompt;
Prompt forwardPrompt;
Prompt rotateLeftPrompt;
Prompt rotateRightPrompt;

void register_prompt(Prompt &prompt, Hash control, const char* promptText)
{
	prompt = HUD::_UI_PROMPT_REGISTER_BEGIN();
	HUD::_UI_PROMPT_SET_CONTROL_ACTION(prompt, control);
	HUD::_UI_PROMPT_SET_TEXT(prompt, MISC::VAR_STRING(10, "LITERAL_STRING", promptText));
	HUD::_UI_PROMPT_SET_PRIORITY(prompt, 1);
	HUD::_UI_PROMPT_SET_TRANSPORT_MODE(prompt, 0);
	HUD::_UI_PROMPT_SET_HOLD_INDEFINITELY_MODE(prompt);
	HUD::_UI_PROMPT_SET_ATTRIBUTE(prompt, 14, true); // Allows multiple prompts to be held at once
	HUD::_UI_PROMPT_SET_ATTRIBUTE(prompt, 34, true); // Allow multiple prompts of the same type? Idk. It just works.
	HUD::_UI_PROMPT_REGISTER_END(prompt);
	HUD::_UI_PROMPT_SET_VISIBLE(prompt, false);
	HUD::_UI_PROMPT_SET_ENABLED(prompt, false);
}

void toggle_prompts(bool enabled)
{
	// If ascendPrompt is valid, then all of them are valid
	if (HUD::_UI_PROMPT_IS_VALID(ascendPrompt)) {
		HUD::_UI_PROMPT_SET_VISIBLE(ascendPrompt, enabled);
		HUD::_UI_PROMPT_SET_ENABLED(ascendPrompt, enabled);
		HUD::_UI_PROMPT_SET_VISIBLE(descendPrompt, enabled);
		HUD::_UI_PROMPT_SET_ENABLED(descendPrompt, enabled);
		HUD::_UI_PROMPT_SET_VISIBLE(forwardPrompt, enabled);
		HUD::_UI_PROMPT_SET_ENABLED(forwardPrompt, enabled);
		HUD::_UI_PROMPT_SET_VISIBLE(rotateLeftPrompt, enabled);
		HUD::_UI_PROMPT_SET_ENABLED(rotateLeftPrompt, enabled);
		HUD::_UI_PROMPT_SET_VISIBLE(rotateRightPrompt, enabled);
		HUD::_UI_PROMPT_SET_ENABLED(rotateRightPrompt, enabled);
	}
}
 #pragma endregion


Vehicle balloon;
Blip blip;

float RESISTANCE = 65.0f; // Higher = Slower you will go forward
//float ASCEND_SPEED = 0.0f; // Higher = Faster ascend speed (DEFAULT MAX ASCEND SPEED = ~2.5f)
float DESCEND_SPEED = 0.015f; // Higher = Faster descend speed (0.015f = ~-5.0f z velocity)
float ROTATION_SPEED = 0.45f; // Higher = Faster rotation speed

Hash KEY_ASCEND; // Shift
Hash KEY_DESCEND; // Ctrl
Hash KEY_FORWARD; // W
Hash KEY_ROT_L; // A
Hash KEY_ROT_R; // D

// TODO: If rotating AND pressing forward, the balloon will want to go down if at certain angles. Find better Z..?
// TODO: Hover mode
// TODO: Should I enable veh controls while in balloon so I dont have to call PAD::IS_DISABLED_CONTROL_PRESSED?
// TODO: Animations with player under burner

void create_balloon()
{
	if (!ENTITY::DOES_ENTITY_EXIST(balloon)) {
		if (!STREAMING::HAS_MODEL_LOADED(MISC::GET_HASH_KEY("HOTAIRBALLOON01"))) {
			while (!STREAMING::HAS_MODEL_LOADED(MISC::GET_HASH_KEY("HOTAIRBALLOON01"))) {
				STREAMING::REQUEST_MODEL(MISC::GET_HASH_KEY("HOTAIRBALLOON01"), false);
				WAIT(0);
			}
		}

		if (!Config::bSpawnInBalloon) {
			Vector3 forward = ENTITY::GET_ENTITY_FORWARD_VECTOR(playerPed);
			playerPos.x += forward.x * 3.0f;
			playerPos.y += forward.y * 3.0f;
		}

		balloon = VEHICLE::CREATE_VEHICLE(MISC::GET_HASH_KEY("HOTAIRBALLOON01"), playerPos.x, playerPos.y, playerPos.z, ENTITY::GET_ENTITY_HEADING(playerPed), true, true, false, false);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(balloon, true);
		CAM::_DISABLE_CAM_COLLISION_FOR_ENTITY(balloon);
		ENTITY::SET_ENTITY_DYNAMIC(balloon, true);
		blip = MAP::BLIP_ADD_FOR_ENTITY(MISC::GET_HASH_KEY("BLIP_STYLE_COMPANION"), balloon);

		if (Config::bSpawnInBalloon) {
			PED::SET_PED_INTO_VEHICLE(playerPed, balloon, -1);
		}
		
		//ENTITY::ATTACH_ENTITY_TO_ENTITY(playerPed, balloon, 0, 0.3f, -0.4f, 1.1f, 0.0f, 0.0f, 0.0f, false, false, false, false, 2, true, false, false);

		// mfw when no enum eInputType
		KEY_ASCEND = MISC::GET_HASH_KEY("INPUT_VEH_FLY_THROTTLE_UP"); // Shift
		KEY_DESCEND = MISC::GET_HASH_KEY("INPUT_VEH_HANDBRAKE"); // Ctrl
		KEY_FORWARD = MISC::GET_HASH_KEY("INPUT_VEH_MOVE_UP_ONLY"); // W
		KEY_ROT_L = MISC::GET_HASH_KEY("INPUT_VEH_MOVE_LEFT_ONLY"); // A
		KEY_ROT_R = MISC::GET_HASH_KEY("INPUT_VEH_MOVE_RIGHT_ONLY"); // D

		if (Config::bShowControls) {
			register_prompt(ascendPrompt, KEY_ASCEND, "Ascend");
			register_prompt(descendPrompt, KEY_DESCEND, "Descend");
			register_prompt(forwardPrompt, KEY_FORWARD, "Forward");
			register_prompt(rotateLeftPrompt, KEY_ROT_L, "Rotate Left");
			register_prompt(rotateRightPrompt, KEY_ROT_R, "Rotate Right");
		}
	}	
}


// So the balloon can't be moved or rotated while just sitting on the ground
bool shouldBalloonMove()
{
	return ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(balloon) > 0.125f;
}


void subtitle(const std::string& str)
{
	UILOG::_UILOG_SET_CACHED_OBJECTIVE(str.c_str());
	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_HAS_DISPLAYED_CACHED_OBJECTIVE();
	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
}


void main()
{	
	bool bHoverMode = false;

	Config::Read();
	RESISTANCE = Config::fVelocityResistance;
	DESCEND_SPEED = Config::fDescendSpeed;
	ROTATION_SPEED = Config::fRotationSpeed;

	while (true)
	{
		if (IsKeyJustUp(Config::iSpawnKey)) {
			create_balloon();
		}

		if (ENTITY::DOES_ENTITY_EXIST(balloon)) {
			if (PED::IS_PED_IN_VEHICLE(playerPed, balloon, false)) {
				Vector3 rot = ENTITY::GET_ENTITY_ROTATION(balloon, 2);
				Vector3 velocity = ENTITY::GET_ENTITY_VELOCITY(balloon, -1);
				Vector3 forward = ENTITY::GET_ENTITY_FORWARD_VECTOR(playerPed);

				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, KEY_ROT_L) && !PAD::IS_CONTROL_PRESSED(0, KEY_ROT_R)) {
					// Rotate Left
					rot.z += ROTATION_SPEED;
					if (rot.z > 180.0f) {
						rot.z = -179.9f;
					}
				}
				else if (PAD::IS_CONTROL_PRESSED(0, KEY_ROT_R) && !PAD::IS_DISABLED_CONTROL_PRESSED(0, KEY_ROT_L)) {
					// Rotate Right
					rot.z -= ROTATION_SPEED;
					if (rot.z < -180.0f) {
						rot.z = 179.9f;
					}
				}

				if (shouldBalloonMove()) {
					ENTITY::SET_ENTITY_ROTATION(balloon, rot.x, rot.y, rot.z, 2, false);
				}

				if (PAD::IS_CONTROL_PRESSED(0, KEY_FORWARD)) {
					if (shouldBalloonMove()) {
						// This probably could be done better
						// TODO: If rotating AND pressing forward, the balloon will want to go down if at certain angles. Find better Z..?
						ENTITY::SET_ENTITY_VELOCITY(balloon, velocity.x + (forward.x / RESISTANCE), velocity.y + (forward.y / RESISTANCE), velocity.z);
					}
				}

				if (PAD::IS_CONTROL_PRESSED(0, KEY_DESCEND) && !PAD::IS_CONTROL_PRESSED(0, KEY_ASCEND)) {
					if (shouldBalloonMove()) {
						ENTITY::SET_ENTITY_VELOCITY(balloon, velocity.x, velocity.y, velocity.z - DESCEND_SPEED);
					}
				}
				//else if (!PAD::IS_CONTROL_PRESSED(0, KEY_DESCEND) && !PAD::IS_CONTROL_PRESSED(0, KEY_ASCEND) && !PAD::IS_CONTROL_PRESSED(0, KEY_FORWARD) && velocity.z <= 0.0f) {
				//	if (shouldBalloonMove()) {
				//		ENTITY::SET_ENTITY_VELOCITY(balloon, velocity.x, velocity.y, 0);
				//	}
				//}
				//else if (PAD::IS_CONTROL_PRESSED(0, KEY_ASCEND) && !PAD::IS_CONTROL_PRESSED(0, KEY_DESCEND)) {
				//	if (shouldBalloonMove()) {
				//		ENTITY::SET_ENTITY_VELOCITY(balloon, velocity.x, velocity.y, velocity.z + ASCEND_SPEED);
				//	}
				//}


				if (!VEHICLE::IS_VEHICLE_DRIVEABLE(balloon, false, false)) {
					MAP::REMOVE_BLIP(&blip);
				}

				if (Config::bShowControls) {
					toggle_prompts(true);
				}
			}
			else {
				if (Config::bShowControls) {
					toggle_prompts(false);
				}
			}


			Vector3 coords = ENTITY::GET_ENTITY_COORDS(balloon, false, false);
			if (BUILTIN::VDIST2(playerPos.x, playerPos.y, playerPos.z, coords.x, coords.y, coords.z) > 50000.0f) {
				// No need to keep the balloon around if we're not near it
				ENTITY::DELETE_ENTITY(&balloon);
			}
			

			// VEHICLE::_0x7C9E45A4CED2E8DA() seems to make the balloon hover, but theres
			// no way to make it stop havering it seems
			
			/*if (PAD::IS_CONTROL_JUST_PRESSED(0, MISC::GET_HASH_KEY("INPUT_HUD_SPECIAL"))) {
				bHoverMode = !bHoverMode;
				if (bHoverMode) {
					subtitle("Hover mode activated. Press ~INPUT_HUD_SPECIAL~ to cancel.");
					VEHICLE::_0x7C9E45A4CED2E8DA(balloon, 1.0f);
				} else {
					subtitle("Hover mode deactivated. Press ~INPUT_HUD_SPECIAL~ to activate hover mode.");
					VEHICLE::_0x7C9E45A4CED2E8DA(balloon, 0.0f); // This doesn't cancel hover?
				}
			}*/
		}


		playerPed = PLAYER::PLAYER_PED_ID();
		playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, false);
		WAIT(0);
	}
}


void ScriptMain()
{		
	srand(GetTickCount());
	main();
}
