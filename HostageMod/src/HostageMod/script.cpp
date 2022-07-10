#include "script.h"
#include "keyboard.h"


Ped g_pedPlayer; // Global_35, our player ped
bool g_bIsPedHostage = false; // If we have a hostage currently
bool g_bDoesPlayerHaveTarget = false; // If we have any target at all. Remnant of old code
bool g_bCopsAttack = true; // Should the cops attack us right now
int g_iShitFlag = -1; // Variable to make sure we don't loop over world peds every frame
const float MAX_LEAVE_DIST = 150.0f;


void resetScriptVariables()
{
	g_bIsPedHostage = false;
	g_bDoesPlayerHaveTarget = false;
	g_bCopsAttack = true;
	g_iShitFlag = -1;
}

Ped getClosestPed()
{
	const int ARRAY_SIZE = 1024;
	const float MAX_DISTANCE = 2.0f; // Max distance a ped has to be for them be a target

	Ped closestPed = NULL;
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

float getPedDistanceFromPlayer(Ped ped)
{
	Vector3 pedPos = ENTITY::GET_ENTITY_COORDS(ped, false, false);
	Vector3 playerPos = ENTITY::GET_ENTITY_COORDS(g_pedPlayer, false, false);
	return BUILTIN::VDIST2(playerPos.x, playerPos.y, playerPos.z, pedPos.x, pedPos.y, pedPos.z);
}

void setCopsAttack(bool bCopsAttack)
{
	g_bCopsAttack = bCopsAttack;

	if (!g_bCopsAttack)
		g_iShitFlag = -1;
	else
		g_iShitFlag = 2;
}

bool isActivationKeyPressed()
{
	if (PAD::_IS_USING_KEYBOARD(0)) {
		return IsKeyJustUp(0x54); // T Key
	}
	return PAD::IS_CONTROL_JUST_PRESSED(2, INPUT_FRONTEND_RS);
}


namespace Target
{
	void Reset(Ped ped)
	{
		TASK::CLEAR_PED_TASKS(ped, true, true);
		TASK::CLEAR_PED_TASKS(g_pedPlayer, true, true);

		PED::SET_PED_CONFIG_FLAG(ped, PCF_DisableInteractionLockonOnTargetPed, false);
		PED::SET_PED_CONFIG_FLAG(g_pedPlayer, 192, false);
		PED::SET_PED_CONFIG_FLAG(g_pedPlayer, 352, false);
		PED::SET_PED_CONFIG_FLAG(g_pedPlayer, 485, false);
		PED::SET_PED_CONFIG_FLAG(g_pedPlayer, 486, false);

		PED::_CLEAR_PED_GRAPPLE_FLAG(ped, 2);
		PED::_CLEAR_PED_GRAPPLE_FLAG(g_pedPlayer, 1027);
		PED::_CLEAR_PED_GRAPPLE_FLAG(g_pedPlayer, 32768);
		PED::_CLEAR_PED_GRAPPLE_FLAG(g_pedPlayer, 32800);
		PED::_CLEAR_PED_ACTION_DISABLE_FLAG(ped, 23); // ADF_BREAKOUT

		PED::_SET_PED_GRAPPLE_STYLE(g_pedPlayer, 0);
		PED::_SET_PED_GRAPPLE_ANIMATION(g_pedPlayer, 0);
		setCopsAttack(true);
	}

	void Flee(Ped ped)
	{
		TASK::TASK_FLEE_PED(ped, g_pedPlayer, MAJORTHREAT, 0, -1.0f, -1, 0);
		PED::SET_PED_KEEP_TASK(ped, true);

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
		if (g_bIsPedHostage) {
			PED::FORCE_PED_AI_AND_ANIMATION_UPDATE(g_pedPlayer, false, false);

			Hash grappleAction = MISC::GET_HASH_KEY("ACT_GRAPPLE_THROW_FRONT"); // ACT_GRAPPLE_RELEASE
			/*int random = MISC::GET_RANDOM_INT_IN_RANGE(0, 3);
			grappleAction = (random == 0 ? MISC::GET_HASH_KEY("ACT_GRAPPLE_RELEASE") : MISC::GET_HASH_KEY("ACT_GRAPPLE_THROW_FRONT"));*/
			PED::_SET_PED_GRAPPLE_ACTION(g_pedPlayer, grappleAction);

			PED::SET_PED_CONFIG_FLAG(ped, PCF_DisableInteractionLockonOnTargetPed, false);
			PED::SET_PED_CONFIG_FLAG(g_pedPlayer, 192, false);
			PED::SET_PED_CONFIG_FLAG(g_pedPlayer, 352, false);
			PED::SET_PED_CONFIG_FLAG(g_pedPlayer, 485, false);
			PED::SET_PED_CONFIG_FLAG(g_pedPlayer, 486, false);

			PED::_CLEAR_PED_GRAPPLE_FLAG(ped, 2);
			PED::_CLEAR_PED_GRAPPLE_FLAG(g_pedPlayer, 1027);
			PED::_CLEAR_PED_GRAPPLE_FLAG(g_pedPlayer, 32768);
			PED::_CLEAR_PED_GRAPPLE_FLAG(g_pedPlayer, 32800);
			PED::_CLEAR_PED_ACTION_DISABLE_FLAG(ped, 23); // ADF_BREAKOUT

			PED::_SET_PED_GRAPPLE_STYLE(g_pedPlayer, 0);
			PED::_SET_PED_GRAPPLE_ANIMATION(g_pedPlayer, 0);

			TASK::CLEAR_PED_TASKS(g_pedPlayer, true, true);
			TASK::CLEAR_PED_TASKS(ped, true, true);

			setCopsAttack(true);
		}

		resetScriptVariables();
		Flee(ped);
	}

	void TakeHostage(Ped ped)
	{
		WEAPON::MAKE_PED_DROP_WEAPON(ped, true, 0, true, false);

		PED::_SET_PED_GRAPPLE_ANIMATION(g_pedPlayer, MISC::GET_HASH_KEY("GRAPPLE_1H_PISTOL_HOSTAGE"));
		PED::_0x57F35552E771BE9D(g_pedPlayer, 14);
		PED::_SET_PED_GRAPPLE_FLAG(g_pedPlayer, 1027, false);
		PED::_SET_PED_GRAPPLE_STYLE(g_pedPlayer, MISC::GET_HASH_KEY("GS_FACE_TO_BACK"));

		PED::_SET_PED_GRAPPLE_ANIMATION(ped, MISC::GET_HASH_KEY("GRAPPLE_1H_PISTOL_HOSTAGE"));
		PED::_0x57F35552E771BE9D(ped, 14);
		PED::_SET_PED_GRAPPLE_FLAG(ped, 2, false);
		PED::_SET_PED_GRAPPLE_STYLE(ped, MISC::GET_HASH_KEY("GS_FACE_TO_BACK"));
		PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(ped, true);

		TASK::TASK_PUT_PED_DIRECTLY_INTO_GRAPPLE(g_pedPlayer, ped, MISC::GET_HASH_KEY("AR_GRAPPLE_BACK_FROM_BACK"), 0, 0, TRUE, 0);
		PED::FORCE_PED_MOTION_STATE(g_pedPlayer, 1926482157, false, 0, false);
		PED::FORCE_PED_MOTION_STATE(ped, 1926482157, false, 0, false);
		PED::SET_PED_CONFIG_FLAG(g_pedPlayer, 352, true);
		PED::SET_PED_CONFIG_FLAG(g_pedPlayer, 485, true);
		PED::SET_PED_CONFIG_FLAG(g_pedPlayer, 486, true);
		PED::_SET_PED_ACTION_DISABLE_FLAG(ped, 23); // ADF_BREAKOUT
		setCopsAttack(false);

		g_bIsPedHostage = true;
		g_bDoesPlayerHaveTarget = true;
	}
}


namespace Validation
{
	bool AreScriptVariablesDefault()
	{
		// older function...
		return g_bIsPedHostage == false && g_bDoesPlayerHaveTarget == false;
	}

	bool IsPistolEquipped()
	{
		// there might be anims for a knife too. didnt bother to test tbh
		Hash weapon = WEAPON::_GET_PED_CURRENT_HELD_WEAPON(g_pedPlayer);
		if (WEAPON::_IS_WEAPON_ONE_HANDED(weapon) && (WEAPON::IS_WEAPON_REVOLVER(weapon) || WEAPON::IS_WEAPON_PISTOL(weapon))) return true;
		return false;
	}

	bool IsPedValidTarget(Ped ped)
	{
		return
		ENTITY::DOES_ENTITY_EXIST(ped) && ENTITY::IS_ENTITY_ON_SCREEN(ped) && PED::IS_PED_HUMAN(ped) && !ENTITY::IS_ENTITY_DEAD(ped) && !PED::IS_PED_FATALLY_INJURED(ped) && !PED::IS_PED_HOGTIED(ped)
		&& !PED::IS_PED_ON_MOUNT(ped) && !PED::IS_PED_IN_ANY_VEHICLE(ped, false) && !ENTITY::IS_ENTITY_IN_WATER(ped) && !PED::IS_PED_INCAPACITATED(ped) && !PED::IS_PED_RAGDOLL(ped) && !PED::IS_PED_SITTING(ped);
	}

	bool IsPlayerStateValid()
	{
		return
		!PED::IS_PED_ON_MOUNT(g_pedPlayer) && !PED::IS_PED_IN_ANY_VEHICLE(g_pedPlayer, false) && !PED::IS_PED_RAGDOLL(g_pedPlayer) && !PED::IS_PED_HOGTYING(g_pedPlayer)
		&& !PED::GET_PED_IS_GRAPPLING(g_pedPlayer) && !PLAYER::_IS_SPECIAL_ABILITY_ACTIVE(PLAYER::PLAYER_ID()); /*Deadeye*/
	}

	void ValidateScriptVariables(Ped ped)
	{
		if (!PED::GET_PED_IS_GRAPPLING(g_pedPlayer) && g_bIsPedHostage) {
			// we're not grappling but g_bIsPedHostage is true.
			// the hostage anim is technically a grapple.
			resetScriptVariables();
			Target::Reset(ped);
		}

		if (g_bDoesPlayerHaveTarget) {
			if (ENTITY::DOES_ENTITY_EXIST(ped)) {
				if (ENTITY::IS_ENTITY_DEAD(ped) || PED::GET_PED_CONFIG_FLAG(ped, PCF_Knockedout, true)) {
					// dead or knocked out
					resetScriptVariables();
					Target::Reset(ped);
				}
			} else {
				// entity doesnt exist somehow AND we have a target still
				resetScriptVariables();
				Target::Reset(ped);
			}
		}
	}
}


void main()
{	
	Ped targetPed = NULL;
	bool bIsDismissJustPressed = false;
	
	while (true)
	{
		g_pedPlayer = PLAYER::PLAYER_PED_ID();
		bIsDismissJustPressed = PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_MELEE_GRAPPLE_BREAKOUT);

		Validation::ValidateScriptVariables(targetPed);

		if (isActivationKeyPressed()) {
			if (Validation::AreScriptVariablesDefault()) {
				targetPed = getClosestPed();
				if (Validation::IsPedValidTarget(targetPed) && Validation::IsPlayerStateValid()) {
					if (Validation::IsPistolEquipped()) {
						Target::TakeHostage(targetPed);
					}
				}
			}
		}

		if (g_bDoesPlayerHaveTarget) {
			if (ENTITY::DOES_ENTITY_EXIST(targetPed)) {
				if (bIsDismissJustPressed && g_bIsPedHostage) {
					Target::Dismiss(targetPed);
				}

				if (getPedDistanceFromPlayer(targetPed) > MAX_LEAVE_DIST) {
					// how the hell did we reach this?
					Target::Dismiss(targetPed);
				}
			}
		}


		//================================================
		// A retarded way to make cops not shoot at you
		//================================================


		if (!g_bCopsAttack) {
			Ped pedsArray[1024];
			int count = worldGetAllPeds(pedsArray, 1024);

			for (int i = 0; i < count; i++) {
				if (pedsArray[i] == PLAYER::PLAYER_PED_ID()) { continue; }
				if (!PED::IS_PED_HUMAN(pedsArray[i])) { continue; }
				if (ENTITY::IS_ENTITY_DEAD(pedsArray[i])) { continue; }

				if (PED::GET_PED_RELATIONSHIP_GROUP_HASH(pedsArray[i]) == MISC::GET_HASH_KEY("REL_COP")) {
					TASK::TASK_FOLLOW_ENTITY_WHILE_AIMING_AT_ENTITY(pedsArray[i], g_pedPlayer, g_pedPlayer, 0, -1, 1.0f, 3.0f, 0);
				}
			}
		}

		if (g_iShitFlag == 2) {
			g_iShitFlag = 1;
			Ped pedsArray[1024];
			int count = worldGetAllPeds(pedsArray, 1024);

			for (int i = 0; i < count; i++) {
				if (pedsArray[i] == PLAYER::PLAYER_PED_ID()) { continue; }
				if (!PED::IS_PED_HUMAN(pedsArray[i])) { continue; }
				if (ENTITY::IS_ENTITY_DEAD(pedsArray[i])) { continue; }

				if (PED::GET_PED_RELATIONSHIP_GROUP_HASH(pedsArray[i]) == MISC::GET_HASH_KEY("REL_COP")) {
					TASK::CLEAR_PED_TASKS(pedsArray[i], true, true); // clear TASK_FOLLOW_ENTITY_WHILE_AIMING_AT_ENTITY
				}
			}
		}

		WAIT(0);
	}
}


void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
