#include "script.h"
#include "keyboard.h"

// Global Script Variables

bool g_bDoesPlayerHaveTarget = false;
bool g_bIsPromptCreated = false;
bool g_bIsCamCreated = false;
Ped g_iTargetPed = NULL;

const float MAX_DIST = 250.0f; // Max distance you have to be from a ped for him to be "dismissed" automatically
const float MAX_DRAG_DIST = 3.0f; // Max distance you have to be to initiate dragging on a ped
const int IntimidatedOnAssOrOnKnees = 27; // Flag name


Ped playerPed;
Prompt dragPrompt;
Cam customCam;


// Reset variables to default state after a drag interaction
void resetScriptVariables()
{
	g_bDoesPlayerHaveTarget = false;
	g_bIsPromptCreated = false;
	g_bIsCamCreated = false;
	g_iTargetPed = NULL;
	if (HUD::_UI_PROMPT_IS_VALID(dragPrompt)) {
		HUD::_UI_PROMPT_DELETE(dragPrompt);
	}
	if (CAM::DOES_CAM_EXIST(customCam)) {
		CAM::DESTROY_CAM(customCam, true);
	}
}

// Get the distance (in RAGE units) the passed ped is from the player
float getPedDistanceFromPlayer(Ped ped)
{
	Vector3 pedPos = ENTITY::GET_ENTITY_COORDS(ped, false, false);
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, false, false);
	return BUILTIN::VDIST2(playerPos.x, playerPos.y, playerPos.z, pedPos.x, pedPos.y, pedPos.z);
}

bool isDismissJustPressed()
{
	bool bUsingKeyboard = PAD::IS_USING_KEYBOARD_AND_MOUSE(0);
	return PAD::IS_CONTROL_JUST_PRESSED(bUsingKeyboard == TRUE ? 0 : 2, MISC::GET_HASH_KEY("INPUT_MELEE_GRAPPLE_BREAKOUT")) ||
	PAD::IS_DISABLED_CONTROL_JUST_PRESSED(bUsingKeyboard == TRUE ? 0 : 2, MISC::GET_HASH_KEY("INPUT_MELEE_GRAPPLE_BREAKOUT"));
}

bool isGrappleAttackJustPressed()
{
	bool bUsingKeyboard = PAD::IS_USING_KEYBOARD_AND_MOUSE(0);
	return PAD::IS_CONTROL_JUST_PRESSED(bUsingKeyboard == TRUE ? 0 : 2, MISC::GET_HASH_KEY("INPUT_MELEE_GRAPPLE_ATTACK")) ||
	PAD::IS_DISABLED_CONTROL_JUST_PRESSED(bUsingKeyboard == TRUE ? 0 : 2, MISC::GET_HASH_KEY("INPUT_MELEE_GRAPPLE_ATTACK"));
}


namespace Validation
{
	// Checks if the ped is valid target to start dragging
	bool IsPedValidTarget(Ped ped)
	{
		return
			ENTITY::DOES_ENTITY_EXIST(ped)
			&& ENTITY::IS_ENTITY_ON_SCREEN(ped)
			&& PED::IS_PED_HUMAN(ped)
			&& !ENTITY::IS_ENTITY_DEAD(ped)
			&& !PED::IS_PED_FATALLY_INJURED(ped)
			&& !PED::IS_PED_HOGTIED(ped)
			&& !PED::IS_PED_ON_MOUNT(ped)
			&& !PED::IS_PED_IN_ANY_VEHICLE(ped, false)
			&& !ENTITY::IS_ENTITY_IN_WATER(ped)
			&& !PED::IS_PED_INCAPACITATED(ped)
			&& !PED::IS_PED_RAGDOLL(ped);
	}

	// Checks if the player is in a valid state to start dragging a ped
	bool IsPlayerStateValid()
	{
		return
			!PED::IS_PED_ON_MOUNT(playerPed)
			&& !PED::IS_PED_IN_ANY_VEHICLE(playerPed, false)
			&& !PED::IS_PED_RAGDOLL(playerPed)
			&& !PED::IS_PED_HOGTYING(playerPed)
			&& !PED::GET_PED_IS_GRAPPLING(playerPed);
	}

	// Checks if the ped can be properly dragged
	bool CanPedBeDragged(Ped ped)
	{
		return
			getPedDistanceFromPlayer(ped) <= MAX_DRAG_DIST
			&& !PED::IS_PED_BEING_DRAGGED(ped) 
			&& !PED::GET_PED_IS_BEING_GRAPPLED(ped) 
			&& IsPedValidTarget(ped);
	}

	// Checks the validity of the drag interaction and target ped
	void ValidateScriptVariables()
	{
		if (g_bDoesPlayerHaveTarget) {
			if (ENTITY::DOES_ENTITY_EXIST(g_iTargetPed)) {
				if (ENTITY::IS_ENTITY_DEAD(g_iTargetPed) || PED::GET_PED_CONFIG_FLAG(g_iTargetPed, PCF_Knockedout, true)) {
					resetScriptVariables();
				}
			}
			else {
				resetScriptVariables();
			}
		}
	}
}


namespace Camera
{
	// Used to replace the dragging camera that's used in-game
	void Create()
	{
		if (!g_bIsCamCreated) {
			Vector3 c = CAM::GET_GAMEPLAY_CAM_COORD();
			Vector3 r = CAM::GET_GAMEPLAY_CAM_ROT(2);
			float fov = CAM::GET_GAMEPLAY_CAM_FOV();

			customCam = CAM::CREATE_CAMERA_WITH_PARAMS(MISC::GET_HASH_KEY("SMOOTHED_SPLINE_CAMERA"), c.x, c.y, c.z, r.x, r.y, r.z, fov, true, 2);
			g_bIsCamCreated = true;
		}
	}

	// Constantly updates the fake cameras rotation and position
	void Update()
	{
		if (CAM::DOES_CAM_EXIST(customCam) && !CAM::_IS_IN_FULL_FIRST_PERSON_MODE() && !TASK::_IS_PED_DUELLING(playerPed)) {
			CAM::RENDER_SCRIPT_CAMS(true, false, 0, true, false, 0);
			CAM::SET_CAM_ACTIVE(customCam, true);

			Vector3 c = CAM::GET_GAMEPLAY_CAM_COORD();
			Vector3 r = CAM::GET_GAMEPLAY_CAM_ROT(2);
			CAM::SET_CAM_COORD(customCam, c.x, c.y, c.z);
			CAM::SET_CAM_ROT(customCam, r.x, r.y, r.z, 2);
		}
		else {
			CAM::SET_CAM_ACTIVE(customCam, false);
		}
	}
}


namespace Target
{
	// Initiate the dragging of the target ped
	void Drag()
	{
		if (Validation::IsPlayerStateValid() && Validation::IsPedValidTarget(g_iTargetPed)) {
			PED::_CLEAR_PED_GRAPPLE_FLAG(playerPed, 1 << 15);
			PED::_CLEAR_PED_GRAPPLE_FLAG(g_iTargetPed, 1 << 15);
			PED::SET_PED_CONFIG_FLAG(g_iTargetPed, PCF_EnableIntimidationDragging, true);
			PED::SET_PED_CONFIG_FLAG(g_iTargetPed, PCF_DisableIntimidationBackingAway, false);
			PED::SET_PED_CONFIG_FLAG(g_iTargetPed, PCF_BlockRobberyInteractionEscape, true);
			PED::SET_PED_CONFIG_FLAG(g_iTargetPed, 225 /*PCF_0x0C5FB46A*/, true);
			PED::SET_PED_CONFIG_FLAG(g_iTargetPed, 159 /*PCF_0xF95CE6DA*/, true);
			PED::SET_PED_COMBAT_ATTRIBUTES(g_iTargetPed, ALWAYS_FLEE, false);
			PED::SET_PED_FLEE_ATTRIBUTES(g_iTargetPed, FA_NEVER_FLEE, true);
			PLAYER::_0xC67A4910425F11F1(PLAYER::PLAYER_ID(), 0);
			TASK::TASK_GRAPPLE(playerPed, g_iTargetPed, MISC::GET_HASH_KEY("AR_GRAPPLE_DRAG_FRONT_ON_ASS"), 0, 1.0f, 0, 0);
			PED::SET_PED_CONFIG_FLAG(playerPed, 334 /*PCF_0x23029D96*/, false);
			PED::SET_PED_CONFIG_FLAG(playerPed, 328 /*PCF_0x5E9A5F16*/, false);

			// Enabling this enables the "Beat" prompt, but disables the "Threaten" prompt
			// Pressing the threaten prompt just throws the ped exactly like a dismiss..most of the time
			PED::SET_PED_CONFIG_FLAG(g_iTargetPed, PCF_DisableInteractionLockonOnTargetPed, true);
		}
	}

	// Flee the target ped
	void Flee()
	{
		PED::SET_PED_FLEE_ATTRIBUTES(g_iTargetPed, FA_NEVER_FLEE, false);
		PED::SET_PED_FLEE_ATTRIBUTES(g_iTargetPed, FA_DISABLE_COWER, true);
		PED::SET_PED_FLEE_ATTRIBUTES(g_iTargetPed, FA_DISABLE_EXIT_VEHICLE, false);
		PED::SET_PED_FLEE_ATTRIBUTES(g_iTargetPed, FA_COWER_INSTEAD_OF_FLEE, false);
		PED::SET_PED_FLEE_ATTRIBUTES(g_iTargetPed, FA_DISABLE_HESITATE_IN_VEHICLE, true);

		PED::SET_PED_COMBAT_ATTRIBUTES(g_iTargetPed, ALWAYS_FIGHT, false);
		PED::SET_PED_COMBAT_ATTRIBUTES(g_iTargetPed, ALWAYS_FLEE, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(g_iTargetPed, DISABLE_FLEE_FROM_COMBAT, false);

		TASK::TASK_FLEE_PED(g_iTargetPed, playerPed, MAJORTHREAT, 0, -1.0f, -1, 0);

		resetScriptVariables();
	}

	// Dismiss the target ped
	void Dismiss()
	{
		if (HUD::_UI_PROMPT_IS_VALID(dragPrompt)) {
			HUD::_UI_PROMPT_SET_ENABLED(dragPrompt, false);
			HUD::_UI_PROMPT_SET_VISIBLE(dragPrompt, false);
			HUD::_UI_PROMPT_DELETE(dragPrompt);
		}

		if (CAM::DOES_CAM_EXIST(customCam)) {
			CAM::DESTROY_CAM(customCam, false);
		}

		TASK::CLEAR_PED_TASKS(g_iTargetPed, true, false);
		TASK::CLEAR_PED_TASKS(playerPed, true, false);
		PED::SET_PED_CONFIG_FLAG(g_iTargetPed, PCF_DisableInteractionLockonOnTargetPed, false);
		PED::SET_PED_CONFIG_FLAG(g_iTargetPed, PCF_DisableTalkTo, false);

		resetScriptVariables();
		Flee();
	}

	// Handles when the target should be dismissed
	void HandleDismissal(bool beingDragged)
	{
		if (getPedDistanceFromPlayer(g_iTargetPed) >= MAX_DIST) {
			Target::Dismiss();
		}

		if (isDismissJustPressed()) {
			Target::Dismiss();
		}
	}
}


namespace DragPrompt
{
	// Creates the drag prompt
	void Create(Ped ped)
	{
		if (!g_bIsPromptCreated) {
			dragPrompt = HUD::_UI_PROMPT_REGISTER_BEGIN();
			HUD::_UI_PROMPT_SET_CONTROL_ACTION(dragPrompt, MISC::GET_HASH_KEY("INPUT_INTERACT_ANIMAL")); // G Key, or Left Stick
			HUD::_UI_PROMPT_SET_TEXT(dragPrompt, MISC::VAR_STRING(10, "LITERAL_STRING", "Drag"));
			HUD::_UI_PROMPT_SET_STANDARD_MODE(dragPrompt, true);
			HUD::_UI_PROMPT_SET_PRIORITY(dragPrompt, 2);
			HUD::_UI_PROMPT_SET_TRANSPORT_MODE(dragPrompt, 1);
			HUD::_UI_PROMPT_SET_GROUP(dragPrompt, HUD::_UI_PROMPT_GET_GROUP_ID_FOR_TARGET_ENTITY(ped), 0);
			HUD::_UI_PROMPT_REGISTER_END(dragPrompt);

			HUD::_UI_PROMPT_SET_ENABLED(dragPrompt, true);
			HUD::_UI_PROMPT_SET_VISIBLE(dragPrompt, true);

			g_bIsPromptCreated = true;
		}
	}

	// Enables/Disabled prompt
	void Toggle(bool bEnabled)
	{
		HUD::_UI_PROMPT_SET_ENABLED(dragPrompt, bEnabled);
		HUD::_UI_PROMPT_SET_VISIBLE(dragPrompt, bEnabled);
	}

	// Handles the creation of the "Drag" prompt
	void HandleCreation()
	{
		if (g_bDoesPlayerHaveTarget == false) {

			// Condition: Player has intimidated a ped while having ILO
			if (PLAYER::IS_PLAYER_TARGETTING_ANYTHING(PLAYER::PLAYER_ID())) {
				if (PLAYER::GET_PLAYER_INTERACTION_TARGET_ENTITY(PLAYER::PLAYER_ID(), &g_iTargetPed, false, false)) {
					if (ENTITY::IS_ENTITY_A_PED(g_iTargetPed)) {
						if (PED::_0x947E43F544B6AB34(g_iTargetPed, PLAYER::PLAYER_ID(), IntimidatedOnAssOrOnKnees, 400)) {
							// TODO: Player must un-ILO for the prompt to appear, need to fix somehow (most likely because of Draw Weapon prompt)
							g_bDoesPlayerHaveTarget = true;
							DragPrompt::Create(g_iTargetPed);
						}
					}
				}
			}

			// Condition: Player has intimidated a ped without having ILO
			if (PED::IS_PED_IN_MELEE_COMBAT(playerPed)) {
				Ped meleeTarget = PED::GET_MELEE_TARGET_FOR_PED(playerPed);
				if (PED::_0x947E43F544B6AB34(meleeTarget, PLAYER::PLAYER_ID(), IntimidatedOnAssOrOnKnees, 400)) {
					g_bDoesPlayerHaveTarget = true;
					g_iTargetPed = meleeTarget;
					DragPrompt::Create(g_iTargetPed);
				}
			}
		}
	}

	// Starts dragging the ped if "Drag" prompt is pressed
	void Update()
	{
		if (HUD::_UI_PROMPT_IS_VALID(dragPrompt)) {
			if (HUD::_UI_PROMPT_HAS_STANDARD_MODE_COMPLETED(dragPrompt, 0)) {
				if (Validation::CanPedBeDragged(g_iTargetPed)) {
					// I have to create a new camera on the player ped because
					// the drag cam has this retarded effect where it will always
					// want to face the direction of the dragged ped no matter what.
					// The scripted version of this mod doesn't have this drag cam effect (idk how/why!)
					Camera::Create();
					Target::Drag();
				}
			}
		}
	}
}


void main()
{	
	bool bIsTargetBeingDragged = false;

	while (true)
	{
		playerPed = PLAYER::PLAYER_PED_ID(); // Because I'm lazy

		DragPrompt::HandleCreation();
		DragPrompt::Update();
		
		if (g_bDoesPlayerHaveTarget) {
			Validation::ValidateScriptVariables();
			if (ENTITY::DOES_ENTITY_EXIST(g_iTargetPed)) {
				PED::SET_PED_RESET_FLAG(g_iTargetPed, PRF_PreventFleeFromIntimidatedTask, true);

				Camera::Update();
				Target::HandleDismissal(bIsTargetBeingDragged);

				if (isGrappleAttackJustPressed() && bIsTargetBeingDragged) {
					PED::SET_PED_COMBAT_ATTRIBUTES(g_iTargetPed, ALWAYS_FLEE, false);
					PED::SET_PED_FLEE_ATTRIBUTES(g_iTargetPed, FA_NEVER_FLEE, true);
					// Put the ped back into the intimidation state
					TASK::_TASK_INTIMIDATED_2(g_iTargetPed, playerPed, 0, true, true, true, false, true, 0);
				}

				HUD::_UI_PROMPT_SET_ENABLED(dragPrompt, !bIsTargetBeingDragged);
				HUD::_UI_PROMPT_SET_VISIBLE(dragPrompt, !bIsTargetBeingDragged);
			}
		}

		bIsTargetBeingDragged = PED::IS_PED_BEING_DRAGGED(g_iTargetPed);

		WAIT(0);
	}
}


void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
