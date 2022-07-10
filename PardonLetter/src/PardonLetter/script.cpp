// Pardon Letters Script By TuffyTown

#include "script.h"
#include "keyboard.h"
#include "util.hpp"
#include "conversion.hpp"

#define IS_DEBUG 0

const int g_HudDataUnsaved = 1935496;
const int g_shopsData = 1914319;
const int g_savedGlobals = 40;

// Pardon Letter Related

Hash getCurrentSelectedItem()
{
	// g_HudDataUnsaved.???
	return reinterpret_cast<unsigned int&>(*getGlobalPtr(g_HudDataUnsaved + 9633));
}
bool isPlayerInPostOffice()
{
	// g_shopsData.sActiveShop.eShopType
	return reinterpret_cast<int&>(*getGlobalPtr(g_shopsData + 16855)) == ST_POST_OFFICE;
}
bool isSendMailKeyPressed()
{
	if (PAD::_IS_USING_KEYBOARD(0)) {
		// Enter or mouse input
		return IsKeyJustUp(VK_RETURN) || PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_CURSOR_ACCEPT) || PAD::IS_CONTROL_JUST_PRESSED(0, INPUT_CURSOR_ACCEPT_DOUBLE_CLICK);
	}

	return PAD::IS_CONTROL_JUST_RELEASED(2, INPUT_CONTEXT_A);
}
bool isPlayerArthur()
{
	return reinterpret_cast<Hash&>(*getGlobalPtr(40 + 39)) == joaat("PLAYER_ZERO");
}

// Location Related

bool isDistrictOKToUse(int district)
{
	if (isPlayerArthur()) {
		if (district >= DISTRICT_TALL_TREES) return false;
	}

	return true;
}
bool isRegionOKToUse(int region)
{
	if (region == (int)eRegionIndex::REGION_INVALID) { return false; }
	// Prison and Fort Wallace always stay red
	if (region == (int)eRegionIndex::REGION_BLU_SISIKA || region == (int)eRegionIndex::REGION_CML_OLDFORTWALLACE) { return false; }

	if (isPlayerArthur()) {
		if (region >= (int)eRegionIndex::REGION_TAL_FIRST) {
			return false;
		}
		if (region == (int)eRegionIndex::REGION_GRT_BEECHERS || region == (int)eRegionIndex::REGION_GRT_BLACKWATER || region == (int)eRegionIndex::REGION_GRT_QUAKERS_COVE) {
			return false;
		}
	}

	return true;
}
bool isRegionVolumeGood(Volume volume)
{
	if (volume > 0 && volume < 1000000) {
		return VOLUME::DOES_VOLUME_EXIST(volume);
	}
	return false;
}

// unk function
void func_3413(int region, int iParam1)
{
	*getGlobalPtr(23118 + 1 + (region * 11) + 2) = iParam1;
}

// unk function
void func_3420(int region, int iParam1)
{
	*getGlobalPtr(23118 + 1 + (region * 11) + 1) = iParam1;
}


namespace ShopMgr
{
	//=======================
	// SHOP FLAGS
	//=======================
	// g_shopsData.f_15614[shop]

	bool IsShopFlagSet(int shop, int flag)
	{
		return (*getGlobalPtr(g_shopsData + 15614 + 1 + shop) & flag) != 0;
	}

	void SetShopFlag(int shop, int flag)
	{
		*getGlobalPtr(g_shopsData + 15614 + 1 + shop) = (*getGlobalPtr(g_shopsData + 15614 + 1 + shop) | flag);
	}

	void ClearShopFlag(int shop, int flag)
	{
		*getGlobalPtr(g_shopsData + 15614 + 1 + shop) = (*getGlobalPtr(g_shopsData + 15614 + 1 + shop) - (*getGlobalPtr(g_shopsData + 15614 + 1 + shop) & flag));
	}

	//=======================
	// SHOP TYPE FLAGS
	//=======================
	// g_shopsData.f_3[shopType * 446].f_7

	bool IsShopTypeFlagSet(int shopType, int flag)
	{
		return (*getGlobalPtr(g_shopsData + 3 + 1 + (shopType * 446) + 7) & flag) != 0;
	}

	void ClearShopTypeFlag(int shopType, int flag)
	{
		*getGlobalPtr(g_shopsData + 3 + 1 + (shopType * 446) + 7) = (*getGlobalPtr(g_shopsData + 3 + 1 + (shopType * 446) + 7) - (*getGlobalPtr(g_shopsData + 3 + 1 + (shopType * 446) + 7) & flag));
	}

	void SetShopTypeFlag(int shopType, int flag)
	{
		*getGlobalPtr(g_shopsData + 3 + 1 + (shopType * 446) + 7) = (*getGlobalPtr(g_shopsData + 3 + 1 + (shopType * 446) + 7) | flag);
	}

	//=======================
	// UnlockShopsInRegion 
	//=======================

	void UnlockShopsInRegion(int region)
	{
		if (!isRegionOKToUse(region)) return;

		for (int shopType = ST_FIRST; shopType < NUM_SHOP_TYPES; shopType++) {
			int shop = Conversion::GetShopIndexFromRegion(region, shopType);
			if (shop != -1) {
				if (IsShopFlagSet(shop, SHOP_FLAG_FORCE_LOCK)) { ClearShopFlag(shop, SHOP_FLAG_FORCE_LOCK); }
				if (IsShopFlagSet(shop, SHOP_FLAG_DISABLED)) { ClearShopFlag(shop, SHOP_FLAG_DISABLED); }
				if (IsShopFlagSet(shop, SHOP_FLAG_FULLY_CLOSED)) { ClearShopFlag(shop, SHOP_FLAG_FULLY_CLOSED); }
				if (IsShopTypeFlagSet(shopType, STF_SHOP_TYPE_DISABLED)) { ClearShopTypeFlag(shopType, STF_SHOP_TYPE_DISABLED); }
			}
		}
	}
}


namespace FlagMgr
{
	//==========
	// DISTRICT 
	//==========
	// g_savedGlobals.f_431[district]

	void ClearDistrictFlag(int district, eDistrictFlag flag)
	{
		*getGlobalPtr(g_savedGlobals + 431 + 1 + district) = (*getGlobalPtr(g_savedGlobals + 431 + 1 + district) - (*getGlobalPtr(g_savedGlobals + 431 + 1 + district) & (int)flag));
	}

	void SetDistrictFlag(int district, eDistrictFlag flag)
	{
		*getGlobalPtr(g_savedGlobals + 431 + 1 + district) = (*getGlobalPtr(g_savedGlobals + 431 + 1 + district) | (int)flag);
	}

	bool IsDistrictFlagSet(int district, eDistrictFlag flag)
	{
		return (*getGlobalPtr(g_savedGlobals + 431 + 1 + district) & (int)flag) != 0;
	}

	//==========
	// REGION 
	//==========
	// Global_23118[region * 11]

	void SetRegionFlag(int region, eRegionFlag flag)
	{
		*getGlobalPtr(23118 + 1 + (region * 11)) = (*getGlobalPtr(23118 + 1 + (region * 11)) | (int)flag);
	}

	void ClearRegionFlag(int region, eRegionFlag flag)
	{
		*getGlobalPtr(23118 + 1 + (region * 11)) = (*getGlobalPtr(23118 + 1 + (region * 11)) - (*getGlobalPtr(23118 + 1 + (region * 11)) & (int)flag));
	}

	bool IsRegionFlagSet(int region, eRegionFlag flag)
	{
		return (*getGlobalPtr(23118 + 1 + (region * 11)) & (int)flag) != 0;
	}

	//==========
	// STATE
	//==========
	// g_savedGlobals.f_358[eLBS * 12].f_5

	void SetStateFlag(int eLBS, eStateFlag flag)
	{
		*getGlobalPtr(g_savedGlobals + 358 + 1 + (eLBS * 12) + 5) = (*getGlobalPtr(g_savedGlobals + 358 + 1 + (eLBS * 12) + 5) | (int)flag);
	}

	void ClearStateFlag(int eLBS, eStateFlag flag)
	{
		*getGlobalPtr(g_savedGlobals + 358 + 1 + (eLBS * 12) + 5) = (*getGlobalPtr(g_savedGlobals + 358 + 1 + (eLBS * 12) + 5) - (*getGlobalPtr(g_savedGlobals + 358 + 1 + (eLBS * 12) + 5) & (int)flag));
	}

	bool IsStateFlagSet(int eLBS, eStateFlag flag)
	{
		return (*getGlobalPtr(g_savedGlobals + 358 + 1 + (eLBS * 12) + 5) & (int)flag) != 0;
	}
}


namespace BlipMgr
{
	Hash GetStateBountyBlipName(int eLBS)
	{
		char buffer[64];
		strcpy_s(buffer, sizeof buffer, Conversion::GetStateBlipNameFromState(eLBS));
		const char* name = HUD::_GET_TEXT_SUBSTRING_2(buffer, HUD::GET_LENGTH_OF_LITERAL_STRING(buffer));
		return MISC::GET_HASH_KEY(name);
	}

	void DisableRegionBlip(Hash hash)
	{
		if (MAP::_MAP_IS_REGION_HIGHLIGHTED_WITH_STYLE(hash, -1305340593) || MAP::_MAP_IS_REGION_HIGHLIGHTED_WITH_STYLE(hash, joaat("BLIP_STYLE_WANTED_REGION"))) {
			MAP::_MAP_DISABLE_REGION_BLIP(hash);
		}
	}

	void DisableStateWantedZoneBlip(int eLBS)
	{
		if (isPlayerArthur() && eLBS == LAW_BOUNTY_STATE_NEW_AUSTIN) return;
		Hash stateHash = Conversion::GetStateHashFromIndex(eLBS);
		Hash stateBlip = GetStateBountyBlipName(eLBS);

		if (MAP::_MAP_IS_REGION_HIGHLIGHTED_WITH_STYLE(stateHash, joaat("BLIP_STYLE_WANTED_REGION")))
			MAP::_MAP_DISABLE_REGION_BLIP(stateHash);
		if (MAP::_MAP_IS_REGION_HIGHLIGHTED_WITH_STYLE(stateBlip, joaat("BLIP_STYLE_WANTED_REGION")))
			MAP::_MAP_DISABLE_REGION_BLIP(stateBlip);
	}

	void DisableDistrictWantedZoneBlip(int district)
	{
		if (district == DISTRICT_GUAMA) return; // original return
		if (!isDistrictOKToUse(district)) return;

		//DisableRegionBlip(Conversion::GetDistrictHashFromIndex(district));
	}

	void DisableRegionWantedZoneBlip(int region)
	{
		if (!isRegionOKToUse(region)) return;

		char buffer[64];
		strcpy_s(buffer, sizeof buffer, Conversion::GetRegionLiteralName(region));
		strcat_s(buffer, sizeof buffer, "_OUTLINE");

		const char* outline = HUD::_GET_TEXT_SUBSTRING_2(buffer, HUD::GET_LENGTH_OF_LITERAL_STRING(buffer));
		Hash wantedOutlineHash = MISC::GET_HASH_KEY(outline);

		if (MAP::_MAP_IS_REGION_HIGHLIGHTED_WITH_STYLE(wantedOutlineHash, -1305340593) || MAP::_MAP_IS_REGION_HIGHLIGHTED_WITH_STYLE(wantedOutlineHash, joaat("BLIP_STYLE_WANTED_REGION"))) {
			MAP::_MAP_DISABLE_REGION_BLIP(wantedOutlineHash);
		}
	}

	void DisableRegionWantedAndLockdownBlips(int region)
	{
		if (!isRegionOKToUse(region)) return;

		DisableRegionWantedZoneBlip(region); // original
		DisableRegionBlip(Conversion::GetRegionHashFromIndex(region, false));
	}
}


namespace LockdownMgr
{
	//==========
	// REGION 
	//==========

	void ResetRegionLockdownStartTime(int region)
	{
		if (!isRegionOKToUse(region)) return;
		*getGlobalPtr(23118 + (region * 11) + 3) = 0; // lockdown time?
		*getGlobalPtr(23118 + (region * 11) + 1) = 0; // ???
	}

	void DisableLockdownInRegion(int region)
	{
		if (!isRegionOKToUse(region)) return;

		// i guess these globals return a bullshit pointer sometimes (script crash)
		// so just check if what we got is a valid handle w/ isRegionVolumeGood
		Volume someVolume = reinterpret_cast<Volume&>(*getGlobalPtr(1888801 + 1 + region + 5));
		if (!isRegionVolumeGood(someVolume)) {
			someVolume = reinterpret_cast<Volume&>(*getGlobalPtr(1888801 + 1 + region + 3));
			if (!isRegionVolumeGood(someVolume)) {
				someVolume = NULL;
			}
		}

		bool inLockdown = FlagMgr::IsRegionFlagSet(region, eRegionFlag::RF_LOCKDOWN);
		if (inLockdown)
			ShopMgr::UnlockShopsInRegion(region);
		FlagMgr::ClearRegionFlag(region, eRegionFlag::RF_LOCKDOWN);

		ResetRegionLockdownStartTime(region);
		func_3420(region, -15);

		if (VOLUME::DOES_VOLUME_EXIST(someVolume)) {
			PATHFIND::_0xF2A2177AC848B3A8(someVolume, 1, 1); // no idea what this does
		}

		/*if (inLockdown) {
			FlagMgr::ClearRegionFlag(region, eRegionFlag::RF_LOCKDOWN);
			func_3420(region, -15);
			if (VOLUME::DOES_VOLUME_EXIST(someVolume)) {
				PATHFIND::_0xF2A2177AC848B3A8(someVolume, 1, 1);
			}
		}*/

		BlipMgr::DisableRegionWantedAndLockdownBlips(region);
	}

	void DisableWantedInRegion(int region)
	{
		if (!isRegionOKToUse(region)) return;

		//bool inWantedZone = FlagMgr::IsRegionFlagSet(region, eRegionFlag::RF_WANTED_ZONE);
		FlagMgr::ClearRegionFlag(region, eRegionFlag::RF_WANTED_ZONE);
		func_3413(region, -15);

		BlipMgr::DisableRegionWantedAndLockdownBlips(region);
	}

	//==========
	// DISTRICT 
	//==========

	void DisableDistrictLockdown(int district)
	{
		if (!isDistrictOKToUse(district)) return;

		/*FlagMgr::ClearDistrictFlag(district, eDistrictFlag::DISTRICT_FLAG_IS_IN_LOCKDOWN);
		FlagMgr::ClearDistrictFlag(district, eDistrictFlag::DISTRICT_FLAG_IS_WANTED_ZONE);
		FlagMgr::ClearDistrictFlag(district, eDistrictFlag::DISTRICT_FLAG_NONE);
		FlagMgr::ClearDistrictFlag(district, eDistrictFlag::DISTRICT_FLAG_SUPPRESS_PASSIVE);
		FlagMgr::SetDistrictFlag(district, eDistrictFlag::DISTRICT_FLAG_NONE);
		
		BlipMgr::DisableDistrictWantedZoneBlip(district);*/
	}

	//==========
	// STATE
	//==========

	void SetStateBounty(int eLBS, int bounty)
	{
		if (eLBS == LAW_BOUNTY_STATE_NEW_AUSTIN) return;
		*getGlobalPtr(g_savedGlobals + 358 + 1 + (eLBS * 12)) = 0;
	}

	void DisableStateLockdown(int eLBS)
	{
		if (eLBS == LAW_BOUNTY_STATE_NEW_AUSTIN && isPlayerArthur()) return;

		if (FlagMgr::IsStateFlagSet(eLBS, eStateFlag::STATE_FLAG_IS_IN_LOCKDOWN)) {
			FlagMgr::ClearStateFlag(eLBS, eStateFlag::STATE_FLAG_IS_IN_LOCKDOWN);
		}
		if (FlagMgr::IsStateFlagSet(eLBS, eStateFlag::STATE_FLAG_IS_WANTED_ZONE)) {
			FlagMgr::ClearStateFlag(eLBS, eStateFlag::STATE_FLAG_IS_WANTED_ZONE);
		}
		
		FlagMgr::SetStateFlag(eLBS, eStateFlag::STATE_FLAG_NONE);
		SetStateBounty(eLBS, 0);

		BlipMgr::DisableStateWantedZoneBlip(eLBS);
	}

	void ClearAllLockdownAndWantedZonesInState(int eLBS)
	{
		VOLUME::_0x748C5F51A18CB8F0(FALSE); // nullsub

		DisableStateLockdown(eLBS);

		for (int region = (int)eRegionIndex::REGION_FIRST; region < (int)eRegionIndex::REGION_LAST; region++) {
			DisableWantedInRegion(region);
			DisableLockdownInRegion(region);
		}

		/*for (int district = DISTRICT_BAYOU_NWA; district < NUM_DISTRICTS; district++) {
			DisableDistrictLockdown(district);
		}*/

		VOLUME::_0x748C5F51A18CB8F0(TRUE); // nullsub
	}
}


void ClearBounties()
{
	LAW::CLEAR_BOUNTY(PLAYER::PLAYER_ID());
	LAW::CLEAR_PLAYER_PAST_CRIMES(PLAYER::PLAYER_ID());
	LAW::CLEAR_WANTED_SCORE(PLAYER::PLAYER_ID());
	LAW::SET_BOUNTY(PLAYER::PLAYER_ID(), 0);
	LAW::_SET_BOUNTY_HUNTER_PURSUIT_CLEARED();
	PED::SET_PED_CONFIG_FLAG(PLAYER::PLAYER_PED_ID(), PCF_HasBounty, false); // redundant?

	for (int i = LAW_BOUNTY_STATE_FIRST; i < LAW_BOUNTY_STATE_MAX; i++) {
		LockdownMgr::ClearAllLockdownAndWantedZonesInState(i);
	}
}


void main()
{	
	Hash selectionLastFrame = 0; // What we were selecting last frame
	while (true)
	{
	#if IS_DEBUG
		if (IsKeyJustUp(VK_OEM_3)) {
			ClearBounties();
			Utility::PrintSubtitle("[DEBUG]: ClearBounties");
		}
	#endif
		
		if (!MISC::GET_MISSION_FLAG()) { // wont have any effect in missions. if used, you just wasted it.
			if (isPlayerInPostOffice() && UIAPPS::IS_UIAPP_ACTIVE_BY_HASH(joaat("SATCHEL"))) {
				if (isSendMailKeyPressed() && selectionLastFrame == joaat("KIT_PARDON_LETTER")) {
					ClearBounties();
					Utility::PrintSubtitle("Pardon Letter used. Your bounties have been cleared.");
				}
			}
		}

		selectionLastFrame = getCurrentSelectedItem(); // This value will always be 1 frame behind
		WAIT(0);
	}
}


void ScriptMain()
{		
	srand(GetTickCount());
	main();
}
