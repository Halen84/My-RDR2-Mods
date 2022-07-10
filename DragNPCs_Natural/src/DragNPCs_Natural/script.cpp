#include "script.h"
#include "keyboard.h"
#include <string>

// Global Script Variables

bool g_bDoesPlayerHaveTarget = false;
bool g_bIsPromptCreated = false;
bool g_bIsCamCreated = false;
bool g_bStartIntimidationTask = false;

const float MAX_DIST = 300.0f; // Max distance you have to be from a ped for him to be "dismissed" automatically
const float MAX_DRAG_DIST = 3.0f; // Max distance you have to be to initiate dragging on a ped
const int DISMISS_TIMER = 5000;
const int INTIMIDATION_TIMER = 8500;
const int IntimidatedOnAssOrOnKnees = 27; // Flag name

Ped playerPed;
Prompt dragPrompt;
Cam customCam;

void resetScriptVariables()
{
	g_bDoesPlayerHaveTarget = false;
	g_bIsPromptCreated = false;
	g_bIsCamCreated = false;
	g_bStartIntimidationTask = false;
	if (HUD::_UI_PROMPT_IS_VALID(dragPrompt)) {
		HUD::_UI_PROMPT_DELETE(dragPrompt);
	}
	if (CAM::DOES_CAM_EXIST(customCam)) {
		CAM::DESTROY_CAM(customCam, true);
	}
}

float getPedDistanceFromPlayer(Ped ped)
{
	Vector3 pedPos = ENTITY::GET_ENTITY_COORDS(ped, false, false);
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, false, false);
	return BUILTIN::VDIST2(playerPos.x, playerPos.y, playerPos.z, pedPos.x, pedPos.y, pedPos.z);
}

void createDragPrompt(Ped ped)
{
	if (!g_bIsPromptCreated) {
		dragPrompt = HUD::_UI_PROMPT_REGISTER_BEGIN();
		HUD::_UI_PROMPT_SET_CONTROL_ACTION(dragPrompt, MISC::GET_HASH_KEY("INPUT_INTERACT_ANIMAL")); // G Key, or Left Stick
		HUD::_UI_PROMPT_SET_TEXT(dragPrompt, MISC::VAR_STRING(10, "LITERAL_STRING", "Drag"));
		HUD::_UI_PROMPT_SET_STANDARD_MODE(dragPrompt, true);
		HUD::_UI_PROMPT_SET_PRIORITY(dragPrompt, 2);
		HUD::_UI_PROMPT_SET_TRANSPORT_MODE(dragPrompt, 1);
		HUD::_UI_PROMPT_REGISTER_END(dragPrompt);

		HUD::_UI_PROMPT_SET_GROUP(dragPrompt, HUD::_UI_PROMPT_GET_GROUP_ID_FOR_TARGET_ENTITY(ped), 0);
		HUD::_UI_PROMPT_SET_ENABLED(dragPrompt, true);
		HUD::_UI_PROMPT_SET_VISIBLE(dragPrompt, true);

		g_bIsPromptCreated = true;
	}
}

// Used to replace the dragging camera that's used in-game
void createFakeGameplayCam()
{
	if (!g_bIsCamCreated) {
		Vector3 gpCamCoords = CAM::GET_GAMEPLAY_CAM_COORD();
		Vector3 gpCamRot = CAM::GET_GAMEPLAY_CAM_ROT(2);
		float gpCamFov = CAM::GET_GAMEPLAY_CAM_FOV();

		customCam = CAM::CREATE_CAMERA_WITH_PARAMS(MISC::GET_HASH_KEY("DEFAULT_SCRIPTED_CAMERA"), gpCamCoords.x, gpCamCoords.y, gpCamCoords.z, gpCamRot.x, gpCamRot.y, gpCamRot.z, gpCamFov, true, 2);
		g_bIsCamCreated = true;
	}
}

void toggleDragPrompt(bool bVisible)
{
	HUD::_UI_PROMPT_SET_ENABLED(dragPrompt, bVisible);
	HUD::_UI_PROMPT_SET_VISIBLE(dragPrompt, bVisible);
}

bool isDismissJustPressed()
{
	bool bUsingKeyboard = PAD::_IS_USING_KEYBOARD(0);
	return PAD::IS_CONTROL_JUST_PRESSED(bUsingKeyboard == TRUE ? 0 : 2, MISC::GET_HASH_KEY("INPUT_MELEE_GRAPPLE_BREAKOUT")) ||
	PAD::IS_DISABLED_CONTROL_JUST_PRESSED(bUsingKeyboard == TRUE ? 0 : 2, MISC::GET_HASH_KEY("INPUT_MELEE_GRAPPLE_BREAKOUT"));
}
bool isGrappleAttackJustPressed()
{
	bool bUsingKeyboard = PAD::_IS_USING_KEYBOARD(0);
	return PAD::IS_CONTROL_JUST_PRESSED(bUsingKeyboard == TRUE ? 0 : 2, MISC::GET_HASH_KEY("INPUT_MELEE_GRAPPLE_ATTACK")) ||
	PAD::IS_DISABLED_CONTROL_JUST_PRESSED(bUsingKeyboard == TRUE ? 0 : 2, MISC::GET_HASH_KEY("INPUT_MELEE_GRAPPLE_ATTACK"));
}


namespace Validation
{
	bool IsPedValidTarget(Ped ped)
	{
		return
		ENTITY::DOES_ENTITY_EXIST(ped) && ENTITY::IS_ENTITY_ON_SCREEN(ped) && PED::IS_PED_HUMAN(ped) && !ENTITY::IS_ENTITY_DEAD(ped) && !PED::IS_PED_FATALLY_INJURED(ped) && !PED::IS_PED_HOGTIED(ped)
		&& !PED::IS_PED_ON_MOUNT(ped) && !PED::IS_PED_IN_ANY_VEHICLE(ped, false) && !ENTITY::IS_ENTITY_IN_WATER(ped) && !PED::IS_PED_INCAPACITATED(ped) && !PED::IS_PED_RAGDOLL(ped);
	}

	bool IsPlayerStateValid()
	{
		return
		!PED::IS_PED_ON_MOUNT(playerPed) && !PED::IS_PED_IN_ANY_VEHICLE(playerPed, false) && !PED::IS_PED_RAGDOLL(playerPed) && !PED::IS_PED_HOGTYING(playerPed)
		&& !PED::GET_PED_IS_GRAPPLING(playerPed);
	}

	bool CanPedBeDragged(Ped ped)
	{
		return getPedDistanceFromPlayer(ped) <= MAX_DRAG_DIST && !PED::IS_PED_BEING_DRAGGED(ped) && !PED::GET_PED_IS_BEING_GRAPPLED(ped) && IsPedValidTarget(ped);
		// PED::_0x947E43F544B6AB34(ped, PLAYER::PLAYER_ID(), IntimidatedOnAssOrOnKnees, 400);
	}

	// Needed because when dismissing, the player is considered to still be in melee combat,
	// or we happened to lock onto the ped in the short period after dismissing
	// and will make g_bDoesPlayerHaveTarget automatically revert to true and whatnot.
	// 
	// I actually don't necessarily need this, but it doesn't hurt having it
	bool WasPedDismissedRecently()
	{
		return BUILTIN::TIMERA() < DISMISS_TIMER;
	}

	// A script variable resetter basically
	void ValidateScriptVariables(Ped ped)
	{
		if (g_bDoesPlayerHaveTarget) {
			if (ENTITY::DOES_ENTITY_EXIST(ped)) {
				if (ENTITY::IS_ENTITY_DEAD(ped) || PED::GET_PED_CONFIG_FLAG(ped, PCF_Knockedout, true)) {
					resetScriptVariables();
				}


			} else {
				resetScriptVariables();
			}
		}
	}
}

namespace Target
{
	void Drag(Ped ped)
	{
		if (Validation::IsPlayerStateValid() && Validation::IsPedValidTarget(ped)) {
			g_bStartIntimidationTask = false;
			PED::_CLEAR_PED_GRAPPLE_FLAG(playerPed, 32768);
			PED::_CLEAR_PED_GRAPPLE_FLAG(ped, 32768);
			PED::SET_PED_CONFIG_FLAG(ped, 222, true); // No idea what this does
			TASK::TASK_GRAPPLE(playerPed, ped, MISC::GET_HASH_KEY("AR_GRAPPLE_DRAG_FRONT_ON_ASS"), 0, 1.0f, 0, 0);

			// Enabling this enables the "Beat" prompt, but disables the "Threaten" prompt
			// Pressing the threaten prompt just throws the ped exactly like a dismiss..most of the time
			PED::SET_PED_CONFIG_FLAG(ped, PCF_DisableInteractionLockonOnTargetPed, true);
		}
	}

	void Flee(Ped ped)
	{
		TASK::TASK_FLEE_PED(ped, playerPed, MAJORTHREAT, 0, -1.0f, -1, 0);
		PED::SET_PED_KEEP_TASK(ped, true); // ?

		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, false); // CA_ALWAYS_FIGHT
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 17, true); // CA_ALWAYS_FLEE
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 58, false); // CA_DISABLE_FLEE_FROM_COMBAT
		// To be honest, I don't know what any of these do
		PED::SET_PED_FLEE_ATTRIBUTES(ped, 512, false);
		PED::SET_PED_FLEE_ATTRIBUTES(ped, 1024, true);
		PED::SET_PED_FLEE_ATTRIBUTES(ped, 2048, false);
		PED::SET_PED_FLEE_ATTRIBUTES(ped, 32768, false);
		PED::SET_PED_FLEE_ATTRIBUTES(ped, 131072, true);

		resetScriptVariables();
	}

	void Dismiss(Ped ped)
	{
		if (HUD::_UI_PROMPT_IS_VALID(dragPrompt)) {
			HUD::_UI_PROMPT_SET_ENABLED(dragPrompt, false);
			HUD::_UI_PROMPT_SET_VISIBLE(dragPrompt, false);
			HUD::_UI_PROMPT_DELETE(dragPrompt);
		}
		if (CAM::DOES_CAM_EXIST(customCam)) {
			CAM::DESTROY_CAM(customCam, true);
		}

		TASK::CLEAR_PED_TASKS(ped, true, false);
		TASK::CLEAR_PED_TASKS(playerPed, true, false);

		PED::SET_PED_CONFIG_FLAG(ped, PCF_DisableInteractionLockonOnTargetPed, false);

		resetScriptVariables();
		Flee(ped);

		BUILTIN::SETTIMERA(0); // see comment at Validation::WasPedDismissedRecently()
	}
}
//void subtitle(const std::string& str)
//{
//	UILOG::_UILOG_SET_CACHED_OBJECTIVE(str.c_str());
//	UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
//	UILOG::_UILOG_CLEAR_HAS_DISPLAYED_CACHED_OBJECTIVE();
//	UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
//}
Ped GetClosestPed()
{
	const int ARRAY_SIZE = 1024;
	const float MAX_DISTANCE = 10.0f; // Max distance a ped has to be for them be a target

	Ped closestPed{};
	Ped pedsArray[ARRAY_SIZE];
	Vector3 plrPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false, false);

	int count = worldGetAllPeds(pedsArray, ARRAY_SIZE);
	float closestDist = MAX_DISTANCE;

	for (int i = 0; i < count; i++)
	{
		if (pedsArray[i] == PLAYER::PLAYER_PED_ID()) { continue; }

		Vector3 pedPos = ENTITY::GET_ENTITY_COORDS(pedsArray[i], false, false);
		float distance = BUILTIN::VDIST2(plrPos.x, plrPos.y, plrPos.z, pedPos.x, pedPos.y, pedPos.z);
		if (distance <= closestDist) {
			closestPed = pedsArray[i];
			closestDist = distance;
		}
	}

	return closestPed;
}

void main()
{	
	Ped targetPed{};
	bool bIsPedBeingDragged = false;

	while (true)
	{
		playerPed = PLAYER::PLAYER_PED_ID(); // Because I'm lazy. Fuck you

		if (g_bDoesPlayerHaveTarget == false && !Validation::WasPedDismissedRecently()) {
			// Condition: Player has intimidated a ped while having ILO
			if (PLAYER::IS_PLAYER_TARGETTING_ANYTHING(PLAYER::PLAYER_ID())) {
				if (PLAYER::GET_PLAYER_INTERACTION_TARGET_ENTITY(PLAYER::PLAYER_ID(), &targetPed, false, false)) {
					if (ENTITY::IS_ENTITY_A_PED(targetPed)) {
						if (PED::_0x947E43F544B6AB34(targetPed, PLAYER::PLAYER_ID(), IntimidatedOnAssOrOnKnees, 400)) {
							// TODO: Player must un-ILO for the prompt to appear, need to fix somehow (most likely because of Draw Weapon prompt)
							g_bDoesPlayerHaveTarget = true;
							createDragPrompt(targetPed);
						}
					}
				}
			}

			// Condition: Player has intimidated a ped without having ILO
			if (PED::IS_PED_IN_MELEE_COMBAT(playerPed)) {
				Ped meleeTarget = PED::GET_MELEE_TARGET_FOR_PED(playerPed);
				if (PED::_0x947E43F544B6AB34(meleeTarget, PLAYER::PLAYER_ID(), IntimidatedOnAssOrOnKnees, 400)) {
					g_bDoesPlayerHaveTarget = true;
					targetPed = meleeTarget;
					createDragPrompt(targetPed);
				}
			}
		}
		if (IsKeyJustUp(VK_OEM_2)) {
			Vector3 plrPos = ENTITY::GET_ENTITY_COORDS(playerPed, true, true);
			plrPos.x *= 2.0f;
			plrPos.y *= 2.0f;
			plrPos.z *= 2.0f;
			TASK::TASK_DUEL(playerPed, MISC::GET_HASH_KEY("DUELING_CONFIG_PLAYER"), MISC::GET_HASH_KEY("WEAPON_REVOLVER_DOUBLEACTION"), GetClosestPed(), 0.25f, 0, plrPos.x, plrPos.y, plrPos.z, 0.0f, 1);
		}


		if (HUD::_UI_PROMPT_IS_VALID(dragPrompt)) {
			if (HUD::_UI_PROMPT_HAS_STANDARD_MODE_COMPLETED(dragPrompt, 0)) {
				if (Validation::CanPedBeDragged(targetPed)) {
					// I have to create a new camera on the player ped because
					// the drag cam has this retarded effect where it will always
					// want to face the direction of the dragged ped no matter what.

					// The scripted version of this mod doesn't have this drag cam effect (idk how/why!).
					// This is all a new thing to me. Don't call createFakeGameplayCam() if you want to see what I mean.
					
					// Unfortunately, this new cam has like a grainy effect and idk how to fix it. Only happens if cam is moving.
					createFakeGameplayCam();
					Target::Drag(targetPed);
				}
			}
		}


		if (g_bDoesPlayerHaveTarget) {
			Validation::ValidateScriptVariables(targetPed);
			if (ENTITY::DOES_ENTITY_EXIST(targetPed)) {
				// Cam Update
				// Had some reports of people saying camera gets frozen when dueling - TODO TEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				if (bIsPedBeingDragged && !CAM::_IS_IN_FULL_FIRST_PERSON_MODE() && !TASK::_IS_PED_DUELLING(playerPed)) {
					CAM::RENDER_SCRIPT_CAMS(true, false, 0, true, false, 0);
					CAM::SET_CAM_ACTIVE(customCam, true);

					Vector3 gpCamCoords = CAM::GET_GAMEPLAY_CAM_COORD();
					Vector3 gpCamRot = CAM::GET_GAMEPLAY_CAM_ROT(2);
					CAM::SET_CAM_COORD(customCam, gpCamCoords.x, gpCamCoords.y, gpCamCoords.z);
					CAM::SET_CAM_ROT(customCam, gpCamRot.x, gpCamRot.y, gpCamRot.z, 2);
				}
				else {
					CAM::SET_CAM_ACTIVE(customCam, false);
				}

				// Dismissing Ped
				if (getPedDistanceFromPlayer(targetPed) >= MAX_DIST) {
					Target::Dismiss(targetPed);
				}
				if (isDismissJustPressed() && bIsPedBeingDragged) {
					Target::Dismiss(targetPed);
				}

				// Put ped into intimidation task after "Beat" prompt has been pressed
				if (isGrappleAttackJustPressed() && bIsPedBeingDragged) {
					g_bStartIntimidationTask = true;
					// Needed because the ped won't automatically go into the intimidation task after beat prompt is pressed
					// so we just need to keep calling _TASK_INTIMIDATED_2 until it puts the ped into it
					BUILTIN::SETTIMERB(0);
				}
				if (g_bStartIntimidationTask && BUILTIN::TIMERB() < INTIMIDATION_TIMER) {
					// TODO: Sometimes ped will stand up and then go back down to intimidated on ass state
					TASK::_TASK_INTIMIDATED_2(targetPed, playerPed, 0, false, false, false, true /*TRY THIS WITH FALSE*/, false, 83968);
					PED::SET_PED_CONFIG_FLAG(targetPed, PCF_DisableInteractionLockonOnTargetPed, false);
					PED::SET_PED_CONFIG_FLAG(targetPed, 222, false);
				}
				if (g_bStartIntimidationTask) {
					// PRF_PreventFleeFromIntimidatedTask
					PED::SET_PED_RESET_FLAG(targetPed, 177, true);
				}

				// Toggle Prompts
				/* TODO: Fix this
				if (!bIsPedBeingDragged && !g_bStartIntimidationTask) {
					toggleDragPrompt(true);
					//createDragPrompt(targetPed);
				}
				else if (!bIsPedBeingDragged && g_bStartIntimidationTask) {
					toggleDragPrompt(true);
					//createDragPrompt(targetPed);
				}
				else {
					toggleDragPrompt(false);
				}
				*/
				HUD::_UI_PROMPT_SET_ENABLED(dragPrompt, !bIsPedBeingDragged);
				HUD::_UI_PROMPT_SET_VISIBLE(dragPrompt, !bIsPedBeingDragged);
			}
		}

		// This value needs to be "1 frame behind" so I can properly handle key presses
		bIsPedBeingDragged = PED::IS_PED_BEING_DRAGGED(targetPed);

		WAIT(0);
	}
}


void ScriptMain()
{	
	srand(GetTickCount());
	main();
}

// PED::_SET_PED_GRAPPLE_FLAG(ped, 4096, true); // breaking out of intimidate
