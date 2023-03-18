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
Prompt prompt_ascend;
Prompt prompt_descend;
Prompt prompt_forward;
Prompt prompt_rotateLeft;
Prompt prompt_rotateRight;

void register_prompt(Prompt &prompt, Hash control, const char* promptText)
{
	if (Config::bShowControls == false) return;

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
	if (Config::bShowControls == false) return;

	// If ascendPrompt is valid, then all of them are valid
	if (HUD::_UI_PROMPT_IS_VALID(prompt_ascend)) {
		HUD::_UI_PROMPT_SET_VISIBLE(prompt_ascend, enabled);
		HUD::_UI_PROMPT_SET_ENABLED(prompt_ascend, enabled);
		HUD::_UI_PROMPT_SET_VISIBLE(prompt_descend, enabled);
		HUD::_UI_PROMPT_SET_ENABLED(prompt_descend, enabled);
		HUD::_UI_PROMPT_SET_VISIBLE(prompt_forward, enabled);
		HUD::_UI_PROMPT_SET_ENABLED(prompt_forward, enabled);
		HUD::_UI_PROMPT_SET_VISIBLE(prompt_rotateLeft, enabled);
		HUD::_UI_PROMPT_SET_ENABLED(prompt_rotateLeft, enabled);
		HUD::_UI_PROMPT_SET_VISIBLE(prompt_rotateRight, enabled);
		HUD::_UI_PROMPT_SET_ENABLED(prompt_rotateRight, enabled);
	}
}
 #pragma endregion


float RESISTANCE = 65.0f; // Higher = Slower you will go forward
//float ASCEND_SPEED = 0.0f; // Higher = Faster ascend speed (DEFAULT MAX ASCEND SPEED = ~2.5f)
float DESCEND_SPEED = 0.015f; // Higher = Faster descend speed (0.015f = ~-5.0f z velocity)
float ROTATION_SPEED = 0.45f; // Higher = Faster rotation speed

const Hash KEY_ASCEND = INPUT_VEH_FLY_THROTTLE_UP; // Shift
const Hash KEY_DESCEND = INPUT_VEH_HANDBRAKE; // Ctrl
const Hash KEY_FORWARD = INPUT_VEH_MOVE_UP_ONLY; // W
const Hash KEY_ROT_L = INPUT_VEH_MOVE_LEFT_ONLY; // A
const Hash KEY_ROT_R = INPUT_VEH_MOVE_RIGHT_ONLY; // D

const Hash BALLOON_MODEL = 0x5EB0BAE0; // HOTAIRBALLOON01
Vehicle balloonVehicle;
Blip balloonBlip;

void delete_balloon(bool releaseModel)
{
	if (ENTITY::DOES_ENTITY_EXIST(balloonVehicle)) {
		ENTITY::DELETE_ENTITY(&balloonVehicle);
		if (releaseModel) {
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(BALLOON_MODEL);
		}
	}
}

void create_balloon()
{
	if (ENTITY::DOES_ENTITY_EXIST(balloonVehicle)) {
		delete_balloon(false);
	}

	if (!ENTITY::DOES_ENTITY_EXIST(balloonVehicle)) {
		if (!STREAMING::HAS_MODEL_LOADED(BALLOON_MODEL)) {
			while (!STREAMING::HAS_MODEL_LOADED(BALLOON_MODEL)) {
				STREAMING::REQUEST_MODEL(BALLOON_MODEL, false);
				WAIT(0);
			}
		}

		if (!Config::bSpawnInBalloon) {
			Vector3 forward = ENTITY::GET_ENTITY_FORWARD_VECTOR(playerPed);
			playerPos.x += forward.x * 3.0f;
			playerPos.y += forward.y * 3.0f;
		}

		balloonVehicle = VEHICLE::CREATE_VEHICLE(BALLOON_MODEL, playerPos.x, playerPos.y, playerPos.z, ENTITY::GET_ENTITY_HEADING(playerPed), true, true, false, false);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(balloonVehicle, true);
		ENTITY::SET_ENTITY_DYNAMIC(balloonVehicle, true);
		balloonBlip = MAP::BLIP_ADD_FOR_ENTITY(MISC::GET_HASH_KEY("BLIP_STYLE_COMPANION"), balloonVehicle);

		if (Config::bSpawnInBalloon) {
			PED::SET_PED_INTO_VEHICLE(playerPed, balloonVehicle, -1);
		}

		register_prompt(prompt_ascend, KEY_ASCEND, "Ascend");
		register_prompt(prompt_descend, KEY_DESCEND, "Descend");
		register_prompt(prompt_forward, KEY_FORWARD, "Forward");
		register_prompt(prompt_rotateLeft, KEY_ROT_L, "Rotate Left");
		register_prompt(prompt_rotateRight, KEY_ROT_R, "Rotate Right");
	}	
}


// So the balloon can't be moved or rotated while just sitting on the ground
// Probably a better way to do this...
bool shouldBalloonMove()
{
	return ENTITY::GET_ENTITY_HEIGHT_ABOVE_GROUND(balloonVehicle) > 0.125f;
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

		if (ENTITY::DOES_ENTITY_EXIST(balloonVehicle))
		{
			if (PED::IS_PED_IN_VEHICLE(playerPed, balloonVehicle, false))
			{
				CAM::SET_GAMEPLAY_CAM_IGNORE_ENTITY_COLLISION_THIS_UPDATE(balloonVehicle);

				Vector3 rot = ENTITY::GET_ENTITY_ROTATION(balloonVehicle, 2);
				Vector3 velocity = ENTITY::GET_ENTITY_VELOCITY(balloonVehicle, -1);
				Vector3 forward = ENTITY::GET_ENTITY_FORWARD_VECTOR(playerPed);

				// Handle balloon rotation
				// TODO: If rotating AND pressing forward, the balloon will want to go down if at certain angles. Find better Z..?
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

				// Update rotation
				if (shouldBalloonMove()) {
					ENTITY::SET_ENTITY_ROTATION(balloonVehicle, rot.x, rot.y, rot.z, 2, false);
				}

				// Handle balloon XY velocity
				if (PAD::IS_CONTROL_PRESSED(0, KEY_FORWARD)) {
					if (shouldBalloonMove()) {
						// TODO: If rotating AND pressing forward, the balloon will want to go down if at certain angles. Find better Z..?
						ENTITY::SET_ENTITY_VELOCITY(balloonVehicle, velocity.x + (forward.x / RESISTANCE), velocity.y + (forward.y / RESISTANCE), velocity.z);
					}
				}

				// Handle balloon Z velocity
				if (PAD::IS_CONTROL_PRESSED(0, KEY_DESCEND) && !PAD::IS_CONTROL_PRESSED(0, KEY_ASCEND)) {
					if (shouldBalloonMove()) {
						ENTITY::SET_ENTITY_VELOCITY(balloonVehicle, velocity.x, velocity.y, velocity.z - DESCEND_SPEED);
					}
				}

				// Remove blip
				if (!VEHICLE::IS_VEHICLE_DRIVEABLE(balloonVehicle, false, false)) {
					MAP::REMOVE_BLIP(&balloonBlip);
				}	
			}

			// Handle prompt visibility
			if (ENTITY::DOES_ENTITY_EXIST(balloonVehicle) && PED::IS_PED_IN_MODEL(playerPed, BALLOON_MODEL) && PED::IS_PED_IN_VEHICLE(playerPed, balloonVehicle, false)) {
				toggle_prompts(true);
			}
			else {
				MAP::REMOVE_BLIP(&balloonBlip);
				toggle_prompts(false);
			}

			// Delete the balloon
			Vector3 balloonPos = ENTITY::GET_ENTITY_COORDS(balloonVehicle, false, false);
			if (BUILTIN::VDIST2(playerPos.x, playerPos.y, playerPos.z, balloonPos.x, balloonPos.y, balloonPos.z) > 30000.0f) {
				// No need to keep the balloon around if we're not near it
				delete_balloon(true);
			}
			
			/*if (PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_HUD_SPECIAL)) {
				bHoverMode = !bHoverMode;
				if (bHoverMode) {
					// Hover mode activated. Press ~INPUT_HUD_SPECIAL~ to cancel.
					VEHICLE::_SET_BALLOON_HOVER_STATE(balloonVehicle, 1.0f);
				} else {
					// Hover mode deactivated. Press ~INPUT_HUD_SPECIAL~ to activate hover mode.
					VEHICLE::_SET_BALLOON_HOVER_STATE(balloonVehicle, 0.0f); // This doesn't cancel hover?
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
