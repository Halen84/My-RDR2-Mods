#include "script.h"
#include "keyboard.h"
#include "config.h"
#include <string>


// todo: Ped gets damaged when dismissed. Use SET_ENTITY_CAN_BE_DAMAGED w FALSE while dismissing.
// todo: Maybe convert g_bIsPedIntimidated to use the value of PED::_IS_PED_INTIMIDATED
// todo: For IsPedValidTarget, add a check to make sure ped is not getting up from ragdoll

// bug: Pressing "Threaten" will drag ped instead of actually threaten. (Only happens if you have already dragged ped)


// Global Script Variables

bool g_bIsPedIntimidated = false;
bool g_bDoesPlayerHaveTarget = false;
bool g_bClearTasks = false;

const float MAX_LEAVE_DIST = 300.0f;

Ped playerPed;


void resetScriptVariables()
{
	g_bIsPedIntimidated = false;
	g_bDoesPlayerHaveTarget = false;
}

Ped getClosestPed()
{
	const float MAX_DISTANCE = 2.0f; // Max distance a ped has to be for them be a target

	Ped closestPed{};
	Ped pedsArray[1024];
	Vector3 plrPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), false, false);

	int count = worldGetAllPeds(pedsArray, 1024);
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

float getPedDistanceFromPlayer(Ped ped)
{
	Vector3 pedPos = ENTITY::GET_ENTITY_COORDS(ped, false, false);
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(playerPed, false, false);
	return BUILTIN::VDIST2(playerPos.x, playerPos.y, playerPos.z, pedPos.x, pedPos.y, pedPos.z);
}


namespace Input
{
	bool IsShoveKeyPressed()
	{
		bool bUsingKeyboard = PAD::_IS_USING_KEYBOARD(0);
		if (bUsingKeyboard) {
			return IsKeyJustUp(Config::iShoveKey);
		}
		else {
			Entity ent;
			if (PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &ent)) {
				if (ENTITY::IS_ENTITY_A_PED(ent) && PED::IS_PED_HUMAN(ent)) {
					return PAD::IS_CONTROL_JUST_PRESSED(2, MISC::GET_HASH_KEY("INPUT_CONTEXT_A")) ||
					PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, MISC::GET_HASH_KEY("INPUT_CONTEXT_A"));
				}
			}
		}

		return false;
	}

	bool IsDragKeyPressed()
	{
		bool bUsingKeyboard = PAD::_IS_USING_KEYBOARD(0);
		if (bUsingKeyboard) {
			return IsKeyJustUp(Config::iDragKey);
		}
		else {
			return PAD::IS_CONTROL_JUST_PRESSED(2, MISC::GET_HASH_KEY("INPUT_INTERACT_ANIMAL")) ||
			PAD::IS_DISABLED_CONTROL_JUST_PRESSED(2, MISC::GET_HASH_KEY("INPUT_INTERACT_ANIMAL"));
		}
	}

	bool IsDismissJustPressed()
	{
		bool bUsingKeyboard = PAD::_IS_USING_KEYBOARD(0);
		return PAD::IS_CONTROL_JUST_PRESSED(bUsingKeyboard == TRUE ? 0 : 2, MISC::GET_HASH_KEY("INPUT_MELEE_GRAPPLE_BREAKOUT")) ||
		PAD::IS_DISABLED_CONTROL_JUST_PRESSED(bUsingKeyboard == TRUE ? 0 : 2, MISC::GET_HASH_KEY("INPUT_MELEE_GRAPPLE_BREAKOUT"));
	}

	bool IsThreatenJustPressed()
	{
		bool bUsingKeyboard = PAD::_IS_USING_KEYBOARD(0);
		return PAD::IS_CONTROL_JUST_PRESSED(bUsingKeyboard == TRUE ? 0 : 2, MISC::GET_HASH_KEY("INPUT_MELEE_GRAPPLE")) ||
		PAD::IS_DISABLED_CONTROL_JUST_PRESSED(bUsingKeyboard == TRUE ? 0 : 2, MISC::GET_HASH_KEY("INPUT_MELEE_GRAPPLE"));
	}
}

namespace Validation
{
	bool AreScriptVariablesDefault()
	{
		// We can ignore bCanPedBeBeaten
		return g_bIsPedIntimidated == false && g_bDoesPlayerHaveTarget == false;
	}

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
		return getPedDistanceFromPlayer(ped) <= 2.5f && !PED::IS_PED_BEING_DRAGGED(ped) && !PED::GET_PED_IS_BEING_GRAPPLED(ped) && IsPedValidTarget(ped);
	}

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
	void ResetFlags(Ped ped)
	{
		PED::SET_PED_CONFIG_FLAG(ped, PCF_DisableTalkTo, false);
		PED::SET_PED_CONFIG_FLAG(ped, PCF_AllowNonTempExceptionEvents, false);
		//PED::SET_PED_CONFIG_FLAG(ped, 222, false);
		PED::SET_PED_CONFIG_FLAG(ped, PCF_ForceInteractionLockonOnTargetPed, false);
		PED::SET_PED_CONFIG_FLAG(ped, PCF_DisableInteractionLockonOnTargetPed, false);
		PED::SET_PED_CONFIG_FLAG(ped, 431, false);
		PED::_CLEAR_PED_GRAPPLE_FLAG(ped, 2);
		PED::_CLEAR_PED_TARGET_ACTION_DISABLE_FLAG(ped, 31);

		// I have to do all this bullshit or else the ped wont flee for some fucking reason
		PED::SET_PED_CONFIG_FLAG(playerPed, 352, false);
		PED::SET_PED_CONFIG_FLAG(playerPed, 485, false);
		PED::SET_PED_CONFIG_FLAG(playerPed, 486, false);
		PED::_CLEAR_PED_GRAPPLE_FLAG(playerPed, 1027);
		PED::_CLEAR_PED_GRAPPLE_FLAG(playerPed, 32768);
		PED::_CLEAR_PED_GRAPPLE_FLAG(playerPed, 32800);
		PED::_SET_PED_GRAPPLE_STYLE(playerPed, 0);
		PED::_SET_PED_GRAPPLE_ANIMATION(playerPed, 0);
		
		TASK::CLEAR_PED_TASKS(playerPed, true, true);
		TASK::CLEAR_PED_TASKS(ped, true, true);
	}

	void Intimidate(Ped ped)
	{
		TASK::TASK_PUT_PED_DIRECTLY_INTO_MELEE(playerPed, ped, MISC::GET_HASH_KEY("AR_SHOVE_TO_INTIMIDATION_FRONT"), 0.0f, 0.2f, true, 0);

		PED::_SET_PED_INTERACTION_PERSONALITY(ped, MISC::GET_HASH_KEY("SCRIPTEDINTIMIDATION"));
		TASK::_TASK_INTIMIDATED_2(ped, playerPed, 0, false, false, false, true, false, 0);

		PED::FORCE_PED_AI_AND_ANIMATION_UPDATE(ped, false, false);
		PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);
		PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, false);
		PED::SET_PED_CAN_BE_TARGETTED(ped, true);
		PED::_CLEAR_PED_DESIRED_LOCO_MOTION_TYPE(ped);
		PED::_SET_PED_TARGET_ACTION_DISABLE_FLAG(ped, 31); // 31 = ADF_GRAPPLE_TRANSITION
		TASK::_0x41D1331AFAD5A091(ped, 1, 0);
		TASK::_0x41D1331AFAD5A091(ped, 2, 0);

		PED::SET_PED_CONFIG_FLAG(ped, PCF_ForceInteractionLockonOnTargetPed, true);
		PED::SET_PED_CONFIG_FLAG(ped, PCF_DisableInteractionLockonOnTargetPed, false);
		// Replaces common prompts (rob, greet, etc) with grapple prompts, and make prompts visible without locking on
		PED::SET_PED_CONFIG_FLAG(ped, 431, true);

		g_bIsPedIntimidated = true;
		g_bDoesPlayerHaveTarget = true;
	}

	void Drag(Ped ped)
	{
		PED::_CLEAR_PED_GRAPPLE_FLAG(playerPed, 32768);
		PED::_CLEAR_PED_GRAPPLE_FLAG(ped, 32768);
		PED::SET_PED_CONFIG_FLAG(ped, 222, true);
		TASK::TASK_GRAPPLE(playerPed, ped, MISC::GET_HASH_KEY("AR_GRAPPLE_DRAG_FRONT_ON_ASS"), 0, 1.0f, 0, 0);
		PED::SET_PED_CONFIG_FLAG(ped, PCF_DisableInteractionLockonOnTargetPed, true);
	}

	void Flee(Ped ped)
	{
		TASK::CLEAR_PED_TASKS(ped, true, true);
		TASK::TASK_FLEE_PED(ped, playerPed, MAJORTHREAT, 0, -1.0f, -1, 0);
		//TASK::TASK_SMART_FLEE_PED(ped, playerPed, -1.0f, -1, 0, 3.0f, 0); // TODO: TEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		PED::SET_PED_KEEP_TASK(ped, true);
		PED::SET_PED_CONFIG_FLAG(ped, 230, true); // TODO: TEST!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 5, false);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 17, true);
		PED::SET_PED_COMBAT_ATTRIBUTES(ped, 58, false);
		// To be honest, I don't know what these do
		PED::SET_PED_FLEE_ATTRIBUTES(ped, 512, false);
		PED::SET_PED_FLEE_ATTRIBUTES(ped, 1024, true);
		PED::SET_PED_FLEE_ATTRIBUTES(ped, 2048, false);
		PED::SET_PED_FLEE_ATTRIBUTES(ped, 32768, false);
		PED::SET_PED_FLEE_ATTRIBUTES(ped, 131072, true);

		resetScriptVariables();
	}

	void Dismiss(Ped ped)
	{
		if (g_bIsPedIntimidated) {
			PED::SET_PED_CAN_RAGDOLL_FROM_PLAYER_IMPACT(ped, true);
			PED::SET_PED_CAN_BE_TARGETTED(ped, true);
			TASK::CLEAR_PED_TASKS(playerPed, true, true);
			TASK::CLEAR_PED_TASKS(ped, true, true);
		}

		//PED::SET_PED_CAN_USE_AUTO_CONVERSATION_LOOKAT(playerPed, true);
		//ENTITY::CLEAR_ENTITY_LAST_DAMAGE_ENTITY(ped);
		PED::SET_PED_RESET_FLAG(ped, 177, false);
		PED::SET_PED_CONFIG_FLAG(ped, PCF_DisableInteractionLockonOnTargetPed, false);

		ResetFlags(ped);
		resetScriptVariables();
		Flee(ped);

		BUILTIN::SETTIMERA(0);
		g_bClearTasks = true;
	}
}


void main()
{	
	Ped targetPed{};
	bool bIsPedBeingDragged = false;
	Config::Read();
	
	while (true)
	{
		playerPed = PLAYER::PLAYER_PED_ID();
		Validation::ValidateScriptVariables(targetPed);

		if (Input::IsShoveKeyPressed()) {
			if (Validation::AreScriptVariablesDefault() && !g_bDoesPlayerHaveTarget) {
				targetPed = getClosestPed();
				if (Validation::IsPedValidTarget(targetPed) && Validation::IsPlayerStateValid()) {
					//PED::SET_PED_CAN_USE_AUTO_CONVERSATION_LOOKAT(playerPed, false);					
					Target::Intimidate(targetPed);
				}
			}
		}

		if (Input::IsDismissJustPressed() && g_bDoesPlayerHaveTarget) {
			Target::Dismiss(targetPed);
		}
		else if (Input::IsThreatenJustPressed() && g_bIsPedIntimidated) {
			// true = Dismiss prompt?
			// false = Threaten prompt?
			PED::SET_PED_CONFIG_FLAG(targetPed, PCF_DisableTalkTo, true);
		}


		if (g_bDoesPlayerHaveTarget) {
			if (g_bIsPedIntimidated) {
				// PRF_PreventFleeFromIntimidatedTask
				PED::SET_PED_RESET_FLAG(targetPed, 177, true);

				if (getPedDistanceFromPlayer(targetPed) > MAX_LEAVE_DIST) {
					Target::Dismiss(targetPed);
				}

				if (Input::IsDragKeyPressed() && Validation::CanPedBeDragged(targetPed)) {
					Target::Drag(targetPed);
				}
			}
		}

		// I literally didnt fucking change a god damn thing,
		// and now the ped just wont fucking flee at all so i have to this retarded bullshit
		if (g_bClearTasks) {
			// I HATE PROGRAMMING I HATE PROGRAMMING I HATE PROGRAMMING I HATE PROGRAMMING I HATE PROGRAMMING I HATE PROGRAMMING 
			if (BUILTIN::TIMERA() < 6000) {
				TASK::CLEAR_PED_TASKS(targetPed, true, false);
			}
			else {
				g_bClearTasks = false;
				Target::Flee(targetPed);
			}
		}

		bIsPedBeingDragged = PED::IS_PED_BEING_DRAGGED(targetPed);
		WAIT(0);
	}
}


void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
