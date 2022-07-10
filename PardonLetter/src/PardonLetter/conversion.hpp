// Pardon Letters Script By TuffyTown

#pragma once
#include "script.h"

Hash joaat(const char* key) { return MISC::GET_HASH_KEY(key); }

namespace Conversion
{
	Hash GetStateHashFromIndex(int eLBS)
	{
		switch (eLBS)
		{
		case LAW_BOUNTY_STATE_LEMOYNE:
			return STATE_LEMOYNE;
		case LAW_BOUNTY_STATE_WEST_ELIZABETH:
			return STATE_WEST_ELIZABETH;
		case LAW_BOUNTY_STATE_NEW_HANOVER:
			return STATE_NEW_HANOVER;
		case LAW_BOUNTY_STATE_FIRST:
			return STATE_AMBARINO;
		case LAW_BOUNTY_STATE_NEW_AUSTIN:
			return STATE_NEW_AUSTIN;
		case LAW_BOUNTY_STATE_GUARMA:
			return STATE_GUARMA;
		default:
			break;
		}
		return 0;
	}


	Hash GetDistrictHashFromIndex(int district)
	{
		switch (district)
		{
		case 0:
			return joaat("DISTRICT_BAYOU_NWA");
		case 1:
			return joaat("DISTRICT_BIG_VALLEY");
		case 2:
			return joaat("DISTRICT_BLUEGILL_MARSH");
		case 3:
			return joaat("DISTRICT_CUMBERLAND_FOREST");
		case 4:
			return joaat("DISTRICT_GREAT_PLAINS");
		case 5:
			return joaat("DISTRICT_GRIZZLIES");
		case 6:
			return joaat("DISTRICT_GRIZZLIES_EAST");
		case 7:
			return joaat("DISTRICT_GRIZZLIES_WEST");
		case 8:
			return joaat("DISTRICT_GUAMA");
		case 9:
			return joaat("DISTRICT_HEARTLAND");
		case 10:
			return joaat("DISTRICT_ROANOKE_RIDGE");
		case 11:
			return joaat("DISTRICT_SCARLETT_MEADOWS");
		case 12:
			return joaat("DISTRICT_TALL_TREES");
		case 15:
			return joaat("DISTRICT_CHOLLA_SPRINGS");
		case 13:
			return joaat("DISTRICT_GAPTOOTH_RIDGE");
		case 16:
			return joaat("DISTRICT_HENNIGANS_STEAD");
		case 14:
			return joaat("DISTRICT_RIO_BRAVO");
		default:
			break;
		}
		return 1776960747;
	}


	Hash GetRegionHashFromIndex(int region, bool isWilderness)
	{
		switch (region)
		{
		case 0:
			return joaat("REGION_BAY_CRAWDADWILLIES");
		case 1:
			return joaat("REGION_BAY_MACOMBS_END");
		case 2:
			return joaat("REGION_BAY_MERKINSWALLER");
		case 3:
			return joaat("REGION_BAY_LAGRAS");
		case 4:
			return joaat("REGION_BAY_LAKAY");
		case 5:
			return joaat("REGION_BAY_SAINT_DENIS");
		case 6:
			return joaat("REGION_BAY_ORANGE_PLANTATION");
		case 7:
			return joaat("REGION_BAY_SERIAL_KILLER");
		case 8:
			return joaat("REGION_BAY_SERENDIPITY");
		case 9:
			return joaat("REGION_BAY_SHADYBELLE");
		case 10:
			return joaat("REGION_BAY_SILTWATERSTRAND");
		case 11:
			return joaat("REGION_BGV_APPLESEEDTIMBER");
		case 12:
			return joaat("REGION_BGV_BERYLS_DREAM");
		case 13:
			return joaat("REGION_BGV_BLACKBONEFOREST_TRAPPER");
		case 14:
			return joaat("REGION_BGV_DAKOTARIVER_TRAPPER");
		case 15:
			return joaat("REGION_BGV_FORTRIGGS");
		case 16:
			return joaat("REGION_BGV_HANGINGDOG");
		case 17:
			return joaat("REGION_BGV_LONEMULESTEAD");
		case 18:
			return joaat("REGION_BGV_MISSING_HUSBAND");
		case 19:
			return joaat("REGION_BGV_MONTO_REST");
		case 20:
			return joaat("REGION_BGV_OWANJILA_DAM");
		case 21:
			return joaat("REGION_BGV_PAINTEDSKY");
		case 22:
			return joaat("REGION_BGV_PRONGHORN");
		case 24:
			return joaat("REGION_BGV_SHACK");
		case 23:
			return joaat("REGION_BGV_RIGGS_STATION");
		case 25:
			return joaat("REGION_BGV_SHEPHERDS_RISE");
		case 26:
			return joaat("REGION_BGV_STRAWBERRY");
		case 27:
			return joaat("REGION_BGV_VALLEY_VIEW");
		case 28:
			return joaat("REGION_BGV_WALLACE_STATION");
		case 30:
			return joaat("REGION_BGV_WATSONSCABIN");
		case 29:
			return joaat("REGION_BGV_OLD_MAN_JONES");
		case 31:
			return joaat("REGION_BLU_CANEBREAK_MANOR");
		case 32:
			return joaat("REGION_BLU_COPPERHEAD");
		case 33:
			return joaat("REGION_BLU_SISIKA");
		case 34:
			return joaat("REGION_BLU_TRAVELLING_SALESMAN");
		case 35:
			return joaat("REGION_CML_BACCHUSBRIDGE");
		case 36:
			return joaat("REGION_CML_DINO_LADY");
		case 37:
			return joaat("REGION_CML_OLDFORTWALLACE");
		case 38:
			return joaat("REGION_CML_SIXPOINTCABIN");
		case 39:
			return joaat("REGION_GRT_BEECHERS");
		case 40:
			return joaat("REGION_GRT_BLACKWATER");
		case 41:
			return joaat("REGION_GRT_QUAKERS_COVE");
		case 42:
			return joaat("REGION_GRZ_ADLERRANCH");
		case 43:
			return joaat("REGION_GRZ_DEAD_RIVAL");
		case 53:
			return joaat("REGION_GRZ_CALUMETRAVINE");
		case 54:
			return joaat("REGION_GRE_CIVIL_WAR_BRIDE");
		case 44:
			return joaat("REGION_GRZ_CHEZPORTER");
		case 55:
			return joaat("REGION_GRZ_COHUTTA");
		case 45:
			return joaat("REGION_GRZ_COLTER");
		case 56:
			return joaat("REGION_GRZ_COTORRA_SPRINGS");
		case 46:
			return joaat("REGION_GRZ_FROZEN_EXPLORER");
		case 57:
			return joaat("REGION_GRZ_GUNFIGHT");
		case 47:
			return joaat("REGION_GRZ_MILLESANI_CLAIM");
		case 48:
			return joaat("REGION_GRZ_MOUNTAIN_MAN");
		case 49:
			return joaat("REGION_GRZ_MOUNT_HAGEN_PEAK");
		case 50:
			return joaat("REGION_GRZ_STARVING_CHILDREN");
		case 51:
			return joaat("REGION_GRZ_TEMPEST_RIM");
		case 58:
			return joaat("REGION_GRZ_THELOFT");
		case 59:
			return joaat("REGION_GRE_VETERAN");
		case 60:
			return joaat("REGION_GRZ_WAPITI");
		case 52:
			return joaat("REGION_GRZ_WINTERMINING_TOWN");
		case 61:
			return joaat("REGION_GRZ_TRAVELLING_SALESMAN");
		case 62:
			return joaat("REGION_GUA_AGUASDULCES");
		case 63:
			return joaat("REGION_GUA_CAMP");
		case 64:
			return joaat("REGION_GUA_CINCOTORRES");
		case 65:
			return joaat("REGION_GUA_LACAPILLA");
		case 66:
			return joaat("REGION_GUA_MANICATO");
		case 67:
			return joaat("REGION_HRT_ABANDONED_MILL");
		case 69:
			return joaat("REGION_HRT_CARMODYDELL");
		case 70:
			return joaat("REGION_HRT_CORNWALLKEROSENE");
		case 71:
			return joaat("REGION_HRT_CROP_FARM");
		case 72:
			return joaat("REGION_HRT_CUMBERLANDFALLS");
		case 73:
			return joaat("REGION_HRT_DOWNSRANCH");
		case 74:
			return joaat("REGION_HRT_EMERALDRANCH");
		case 75:
			return joaat("REGION_HRT_GRANGERS_HOGGERY");
		case 76:
			return joaat("REGION_HRT_HORSESHOEOVERLOOK");
		case 77:
			return joaat("REGION_HRT_LARNEDSOD");
		case 78:
			return joaat("REGION_HRT_LOONY_CULT");
		case 79:
			return joaat("REGION_HRT_LUCKYSCABIN");
		case 80:
			return joaat("REGION_HRT_SWANSONS_STATION");
		case 81:
			return joaat("REGION_HRT_VALENTINE");
		case 82:
			return joaat("REGION_ROA_ABERDEENPIGFARM");
		case 83:
			return joaat("REGION_ROA_ANNESBURG");
		case 84:
			return joaat("REGION_ROA_BEAVERHOLLOW");
		case 68:
			return joaat("REGION_ROA_BEECHERS_C");
		case 85:
			return joaat("REGION_ROA_BLACK_BALSAM_RISE");
		case 86:
			return joaat("REGION_ROA_BRANDYWINE_DROP");
		case 87:
			return joaat("REGION_ROA_BUTCHERCREEK");
		case 88:
			return joaat("REGION_ROA_DOVERHILL");
		case 89:
			return joaat("REGION_ROA_HAPPY_FAMILY");
		case 90:
			return joaat("REGION_ROA_ISOLATIONIST");
		case 91:
			return joaat("REGION_ROA_MACLEANSHOUSE");
		case 92:
			return joaat("REGION_ROA_MOSSY_FLATS");
		case 93:
			return joaat("REGION_ROA_ROANOKE_VALLEY");
		case 94:
			return joaat("REGION_ROA_ROCKYSEVEN");
		case 95:
			return joaat("REGION_ROA_TRAPPER");
		case 97:
			return joaat("REGION_ROA_VANHORNMANSION");
		case 98:
			return joaat("REGION_ROA_VANHORNPOST");
		case 96:
			return joaat("REGION_ROA_OLD_MAN_JONES");
		case 99:
			return joaat("REGION_SCM_BRAITHWAITEMANOR");
		case 100:
			return joaat("REGION_SCM_BULGERGLADE");
		case 101:
			return joaat("REGION_SCM_CALIGAHALL");
		case 102:
			return joaat("REGION_SCM_CATFISHJACKSONS");
		case 103:
			return joaat("REGION_SCM_CLEMENSCOVE");
		case 104:
			return joaat("REGION_SCM_CLEMENSPOINT");
		case 105:
			return joaat("REGION_SCM_COMPSONS_STEAD");
		case 106:
			return joaat("REGION_SCM_DAIRY_FARM");
		case 107:
			return joaat("REGION_SCM_HORSE_SHOP");
		case 108:
			return joaat("REGION_SCM_LONNIESSHACK");
		case 109:
			return joaat("REGION_SCM_LOVE_TRIANGLE");
		case 110:
			return joaat("REGION_SCM_RADLEYS_PASTURE");
		case 111:
			return joaat("REGION_SCM_RHODES");
		case 112:
			return joaat("REGION_SCM_SLAVE_PEN");
		case 113:
			return joaat("REGION_TAL_AURORA_BASIN");
		case 115:
			return joaat("REGION_TAL_COCHINAY");
		case 116:
			return joaat("REGION_TAL_MANZANITAPOST");
		case 117:
			return joaat("REGION_TAL_PACIFICUNIONRR");
		case 118:
			return joaat("REGION_TAL_TANNERSREACH");
		case 114:
			return joaat("REGION_TAL_DEAD_SETTLER");
		case 119:
			return joaat("REGION_TAL_TRAPPER");
		case 135:
			return joaat("REGION_HEN_MACFARLANES_RANCH");
		case 136:
			return joaat("REGION_HEN_THIEVES_LANDING");
		case 137:
			return joaat("REGION_HEN_TRAVELLING_SALESMAN");
		case 138:
			return -1573562784;
		case 127:
			return joaat("REGION_CHO_ARMADILLO");
		case 128:
			return joaat("REGION_CHO_COOTS_CHAPEL");
		case 129:
			return joaat("REGION_CHO_DON_JULIO_HOUSE");
		case 131:
			return joaat("REGION_CHO_RIDGEWOOD_FARM");
		case 130:
			return joaat("REGION_CHO_RILEYS_CHARGE");
		case 132:
			return joaat("REGION_CHO_TWIN_ROCKS");
		case 133:
			return joaat("REGION_CHO_TRAVELLING_SALESMAN");
		case 134:
			return 1869665995;
		case 120:
			return joaat("REGION_GAP_GAPTOOTH_BREACH");
		case 121:
			return joaat("REGION_GAP_TUMBLEWEED");
		case 122:
			return joaat("REGION_GAP_RATHSKELLER_FORK");
		case 123:
			return joaat("REGION_RIO_BENEDICT_POINT");
		case 124:
			return joaat("REGION_RIO_FORT_MERCER");
		case 125:
			return joaat("REGION_RIO_PLAIN_VIEW");
		case 126:
			return joaat("REGION_RIO_TRAVELLING_SALESMAN");
		case 139:
			return joaat("REGION_CENTRALUNIONRR");
		default:
			break;
		}
		if (isWilderness) {
			return joaat("WILDERNESS");
		}
		return 0;
	}


	Hash GetLawRegionFromRegionIndex(int region)
	{
		switch (region)
		{
		case 81:
			return joaat("LAW_REGION_VALENTINE");
		case 111:
			return joaat("LAW_REGION_RHODES");
		case 5:
			return joaat("LAW_REGION_SAINT_DENIS");
		case 66:
			return joaat("LAW_REGION_MANICATO");
		case 83:
			return joaat("LAW_REGION_ANNESBURG");
		case 26:
			return joaat("LAW_REGION_STRAWBERRY");
		case 40:
			return joaat("LAW_REGION_BLACKWATER");
			//DEBUG::_0x4DC69742196F818A(8, 111, "Have NOT passed Finale 1, using MAINGAME dispatch!");
			//return joaat("LAW_REGION_BLACKWATER_MAINGAME");
			break;
		case 98:
			return joaat("LAW_REGION_VAN_HORN");
		case 70:
			return joaat("LAW_REGION_CORNWALL");
		case 74:
			return joaat("LAW_REGION_EMERALD_RANCH");
		case 99:
			return joaat("LAW_REGION_BRAITHWAITE_MANOR");
		case 101:
			return joaat("LAW_REGION_CALIGA_HALL");
		case 62:
			return joaat("LAW_REGION_AGUASDULCES");
		case 3:
			return joaat("LAW_REGION_LAGRAS");
		case 33:
			return joaat("LAW_REGION_SISIKA");
		case 87:
			return joaat("LAW_REGION_BUTCHER_CREEK");
		case 37:
			return joaat("LAW_REGION_FORT_WALLACE");
		case 60:
			return joaat("LAW_REGION_WAPITI");
		case 135:
			return joaat("LAW_REGION_MACFARLANES_RANCH");
		case 127:
			return joaat("LAW_REGION_ARMADILLO");
		case 131:
			return joaat("LAW_REGION_RIDGEWOOD_FARM");
		case 121:
			return joaat("LAW_REGION_TUMBLEWEED");
		case 136:
			return joaat("LAW_REGION_THIEVES_LANDING");
			//DEBUG::_0x4DC69742196F818A(8, 111, "Have NOT passed Finale 1, using MAINGAME dispatch!");
			//return joaat("LAW_REGION_THIEVES_LANDING_MAINGAME");
		case 22:
			return joaat("LAW_REGION_PRONGHORN_RANCH");
		case 39:
			return joaat("LAW_REGION_BEECHERS_HOPE");
		case 4:
		case 9:
		case 45:
		case 63:
		case 76:
		case 84:
		case 104:
			break;
		case 116:
			return joaat("LAW_REGION_MANZANITA_POST");
		}
		return 0;
	}


	int GetRegionIndexFromHash(Hash region)
	{
		switch (region)
		{
		case 2209803665:
			return 0;
		case (Hash)eRegionHash::REGION_BAY_CRAWDADWILLIES:
			return 0;
		case (Hash)eRegionHash::REGION_BAY_MACOMBS_END:
			return 1;
		case (Hash)eRegionHash::REGION_BAY_MERKINSWALLER:
			return 2;
		case (Hash)eRegionHash::REGION_BAY_LAGRAS:
			return 3;
		case (Hash)eRegionHash::REGION_BAY_LAKAY:
			return 4;
		case (Hash)eRegionHash::REGION_BAY_SAINT_DENIS:
			return 5;
		case (Hash)eRegionHash::REGION_BAY_ORANGE_PLANTATION:
			return 6;
		case (Hash)eRegionHash::REGION_BAY_SERIAL_KILLER:
			return 7;
		case (Hash)eRegionHash::REGION_BAY_SERENDIPITY:
			return 8;
		case (Hash)eRegionHash::REGION_BAY_SHADYBELLE:
			return 9;
		case (Hash)eRegionHash::REGION_BAY_SILTWATERSTRAND:
			return 10;
		case (Hash)eRegionHash::REGION_BGV_APPLESEEDTIMBER:
			return 11;
		case (Hash)eRegionHash::REGION_BGV_BERYLS_DREAM:
			return 12;
		case (Hash)eRegionHash::REGION_BGV_BLACKBONEFOREST_TRAPPER:
			return 13;
		case (Hash)eRegionHash::REGION_BGV_DAKOTARIVER_TRAPPER:
			return 14;
		case (Hash)eRegionHash::REGION_BGV_FORTRIGGS:
			return 15;
		case (Hash)eRegionHash::REGION_BGV_HANGINGDOG:
			return 16;
		case (Hash)eRegionHash::REGION_BGV_LONEMULESTEAD:
			return 17;
		case (Hash)eRegionHash::REGION_BGV_MISSING_HUSBAND:
			return 18;
		case (Hash)eRegionHash::REGION_BGV_MONTO_REST:
			return 19;
		case (Hash)eRegionHash::REGION_BGV_OWANJILA_DAM:
			return 20;
		case (Hash)eRegionHash::REGION_BGV_PAINTEDSKY:
			return 21;
		case (Hash)eRegionHash::REGION_BGV_PRONGHORN:
			return 22;
		case (Hash)eRegionHash::REGION_BGV_RIGGS_STATION:
			return 23;
		case (Hash)eRegionHash::REGION_BGV_SHACK:
			return 24;
		case (Hash)eRegionHash::REGION_BGV_SHEPHERDS_RISE:
			return 25;
		case (Hash)eRegionHash::REGION_BGV_STRAWBERRY:
			return 26;
		case (Hash)eRegionHash::REGION_BGV_VALLEY_VIEW:
			return 27;
		case (Hash)eRegionHash::REGION_BGV_WATSONSCABIN:
			return 30;
		case (Hash)eRegionHash::REGION_BGV_OLD_MAN_JONES:
			return 29;
		case (Hash)eRegionHash::REGION_BLU_CANEBREAK_MANOR:
			return 31;
		case (Hash)eRegionHash::REGION_BLU_COPPERHEAD:
			return 32;
		case (Hash)eRegionHash::REGION_BLU_SISIKA:
			return 33;
		case (Hash)eRegionHash::REGION_BLU_TRAVELLING_SALESMAN:
			return 34;
		case (Hash)eRegionHash::REGION_CML_BACCHUSBRIDGE:
			return 35;
		case (Hash)eRegionHash::REGION_CML_DINO_LADY:
			return 36;
		case (Hash)eRegionHash::REGION_CML_OLDFORTWALLACE:
			return 37;
		case (Hash)eRegionHash::REGION_CML_SIXPOINTCABIN:
			return 38;
		case (Hash)eRegionHash::REGION_GRT_BEECHERS:
			return 39;
		case (Hash)eRegionHash::REGION_GRT_BLACKWATER:
			return 40;
		case (Hash)eRegionHash::REGION_GRT_QUAKERS_COVE:
			return 41;
		case (Hash)eRegionHash::REGION_GRZ_ADLERRANCH:
			return 42;
		case (Hash)eRegionHash::REGION_GRZ_DEAD_RIVAL:
			return 43;
		case (Hash)eRegionHash::REGION_GRZ_CALUMETRAVINE:
			return 53;
		case (Hash)eRegionHash::REGION_GRE_CIVIL_WAR_BRIDE:
			return 54;
		case (Hash)eRegionHash::REGION_GRZ_CHEZPORTER:
			return 44;
		case (Hash)eRegionHash::REGION_GRZ_COHUTTA:
			return 55;
		case (Hash)eRegionHash::REGION_GRZ_COLTER:
			return 45;
		case (Hash)eRegionHash::REGION_GRZ_COTORRA_SPRINGS:
			return 56;
		case (Hash)eRegionHash::REGION_GRZ_FROZEN_EXPLORER:
			return 46;
		case (Hash)eRegionHash::REGION_GRZ_GUNFIGHT:
			return 57;
		case (Hash)eRegionHash::REGION_GRZ_MILLESANI_CLAIM:
			return 47;
		case (Hash)eRegionHash::REGION_GRZ_MOUNTAIN_MAN:
			return 48;
		case (Hash)eRegionHash::REGION_GRZ_MOUNT_HAGEN_PEAK:
			return 49;
		case (Hash)eRegionHash::REGION_GRZ_STARVING_CHILDREN:
			return 50;
		case (Hash)eRegionHash::REGION_GRZ_TEMPEST_RIM:
			return 51;
		case (Hash)eRegionHash::REGION_GRZ_THELOFT:
			return 58;
		case (Hash)eRegionHash::REGION_BGV_WALLACE_STATION:
			return 28;
		case (Hash)eRegionHash::REGION_GRE_VETERAN:
			return 59;
		case (Hash)eRegionHash::REGION_GRZ_WAPITI:
			return 60;
		case (Hash)eRegionHash::REGION_GRZ_WINTERMINING_TOWN:
			return 52;
		case (Hash)eRegionHash::REGION_GRZ_TRAVELLING_SALESMAN:
			return 61;
		case (Hash)eRegionHash::REGION_GUA_AGUASDULCES:
			return 62;
		case (Hash)eRegionHash::REGION_GUA_CAMP:
			return 63;
		case (Hash)eRegionHash::REGION_GUA_CINCOTORRES:
			return 64;
		case (Hash)eRegionHash::REGION_GUA_LACAPILLA:
			return 65;
		case (Hash)eRegionHash::REGION_GUA_MANICATO:
			return 66;
		case (Hash)eRegionHash::REGION_HRT_ABANDONED_MILL:
			return 67;
		case (Hash)eRegionHash::REGION_HRT_CARMODYDELL:
			return 69;
		case (Hash)eRegionHash::REGION_HRT_CORNWALLKEROSENE:
			return 70;
		case (Hash)eRegionHash::REGION_HRT_CROP_FARM:
			return 71;
		case (Hash)eRegionHash::REGION_HRT_CUMBERLANDFALLS:
			return 72;
		case (Hash)eRegionHash::REGION_HRT_DOWNSRANCH:
			return 73;
		case (Hash)eRegionHash::REGION_HRT_EMERALDRANCH:
			return 74;
		case (Hash)eRegionHash::REGION_HRT_GRANGERS_HOGGERY:
			return 75;
		case (Hash)eRegionHash::REGION_HRT_HORSESHOEOVERLOOK:
			return 76;
		case (Hash)eRegionHash::REGION_HRT_LARNEDSOD:
			return 77;
		case (Hash)eRegionHash::REGION_HRT_LOONY_CULT:
			return 78;
		case (Hash)eRegionHash::REGION_HRT_LUCKYSCABIN:
			return 79;
		case (Hash)eRegionHash::REGION_HRT_SWANSONS_STATION:
			return 80;
		case (Hash)eRegionHash::REGION_HRT_VALENTINE:
			return 81;
		case (Hash)eRegionHash::REGION_ROA_ABERDEENPIGFARM:
			return 82;
		case (Hash)eRegionHash::REGION_ROA_ANNESBURG:
			return 83;
		case (Hash)eRegionHash::REGION_ROA_BEECHERS_C:
			return 68;
		case (Hash)eRegionHash::REGION_ROA_BEAVERHOLLOW:
			return 84;
		case (Hash)eRegionHash::REGION_ROA_BLACK_BALSAM_RISE:
			return 85;
		case (Hash)eRegionHash::REGION_ROA_BRANDYWINE_DROP:
			return 86;
		case (Hash)eRegionHash::REGION_ROA_BUTCHERCREEK:
			return 87;
		case (Hash)eRegionHash::REGION_ROA_DOVERHILL:
			return 88;
		case (Hash)eRegionHash::REGION_ROA_HAPPY_FAMILY:
			return 89;
		case (Hash)eRegionHash::REGION_ROA_ISOLATIONIST:
			return 90;
		case (Hash)eRegionHash::REGION_ROA_MACLEANSHOUSE:
			return 91;
		case (Hash)eRegionHash::REGION_ROA_MOSSY_FLATS:
			return 92;
		case (Hash)eRegionHash::REGION_ROA_ROANOKE_VALLEY:
			return 93;
		case (Hash)eRegionHash::REGION_ROA_ROCKYSEVEN:
			return 94;
		case (Hash)eRegionHash::REGION_ROA_TRAPPER:
			return 95;
		case (Hash)eRegionHash::REGION_ROA_VANHORNMANSION:
			return 97;
		case (Hash)eRegionHash::REGION_ROA_VANHORNPOST:
			return 98;
		case (Hash)eRegionHash::REGION_ROA_OLD_MAN_JONES:
			return 96;
		case (Hash)eRegionHash::REGION_SCM_BRAITHWAITEMANOR:
			return 99;
		case (Hash)eRegionHash::REGION_SCM_BULGERGLADE:
			return 100;
		case (Hash)eRegionHash::REGION_SCM_CALIGAHALL:
			return 101;
		case (Hash)eRegionHash::REGION_SCM_CATFISHJACKSONS:
			return 102;
		case (Hash)eRegionHash::REGION_SCM_CLEMENSCOVE:
			return 103;
		case (Hash)eRegionHash::REGION_SCM_CLEMENSPOINT:
			return 104;
		case (Hash)eRegionHash::REGION_SCM_HORSE_SHOP:
			return 107;
		case (Hash)eRegionHash::REGION_SCM_LONNIESSHACK:
			return 108;
		case (Hash)eRegionHash::REGION_SCM_LOVE_TRIANGLE:
			return 109;
		case (Hash)eRegionHash::REGION_SCM_COMPSONS_STEAD:
			return 105;
		case (Hash)eRegionHash::REGION_SCM_DAIRY_FARM:
			return 106;
		case (Hash)eRegionHash::REGION_SCM_RADLEYS_PASTURE:
			return 110;
		case (Hash)eRegionHash::REGION_SCM_RHODES:
			return 111;
		case (Hash)eRegionHash::REGION_SCM_SLAVE_PEN:
			return 112;
		case (Hash)eRegionHash::REGION_TAL_AURORA_BASIN:
			return 113;
		case (Hash)eRegionHash::REGION_TAL_DEAD_SETTLER:
			return 114;
		case (Hash)eRegionHash::REGION_TAL_COCHINAY:
			return 115;
		case (Hash)eRegionHash::REGION_TAL_MANZANITAPOST:
			return 116;
		case (Hash)eRegionHash::REGION_TAL_PACIFICUNIONRR:
			return 117;
		case (Hash)eRegionHash::REGION_TAL_TANNERSREACH:
			return 118;
		case (Hash)eRegionHash::REGION_TAL_TRAPPER:
			return 119;
		case (Hash)eRegionHash::REGION_HEN_MACFARLANES_RANCH:
			return 135;
		case (Hash)eRegionHash::REGION_HEN_THIEVES_LANDING:
			return 136;
		case (Hash)eRegionHash::REGION_HEN_TRAVELLING_SALESMAN:
			return 137;
		case 2721404512:
			return 138;
		case (Hash)eRegionHash::REGION_CHO_ARMADILLO:
			return 127;
		case (Hash)eRegionHash::REGION_CHO_COOTS_CHAPEL:
			return 128;
		case (Hash)eRegionHash::REGION_CHO_DON_JULIO_HOUSE:
			return 129;
		case (Hash)eRegionHash::REGION_CHO_RIDGEWOOD_FARM:
			return 131;
		case (Hash)eRegionHash::REGION_CHO_RILEYS_CHARGE:
			return 130;
		case (Hash)eRegionHash::REGION_CHO_TWIN_ROCKS:
			return 132;
		case (Hash)eRegionHash::REGION_CHO_TRAVELLING_SALESMAN:
			return 133;
		case 1869665995:
			return 134;
		case (Hash)eRegionHash::REGION_GAP_GAPTOOTH_BREACH:
			return 120;
		case (Hash)eRegionHash::REGION_GAP_TUMBLEWEED:
			return 121;
		case (Hash)eRegionHash::REGION_GAP_RATHSKELLER_FORK:
			return 122;
		case (Hash)eRegionHash::REGION_RIO_BENEDICT_POINT:
			return 123;
		case (Hash)eRegionHash::REGION_RIO_FORT_MERCER:
			return 124;
		case (Hash)eRegionHash::REGION_RIO_PLAIN_VIEW:
			return 125;
		case (Hash)eRegionHash::REGION_RIO_TRAVELLING_SALESMAN:
			return 126;
		case (Hash)eRegionHash::REGION_CENTRALUNIONRR:
			return 139;
		default:
			break;
		}

		return -1;
	}


	int GetLBSFromStateHash(Hash state)
	{
		switch (state)
		{
		case STATE_LEMOYNE:
			return LAW_BOUNTY_STATE_LEMOYNE;
		case STATE_WEST_ELIZABETH:
			return LAW_BOUNTY_STATE_WEST_ELIZABETH;
		case STATE_NEW_HANOVER:
			return LAW_BOUNTY_STATE_NEW_HANOVER;
		case STATE_AMBARINO:
			return LAW_BOUNTY_STATE_FIRST;
		case STATE_NEW_AUSTIN:
			return LAW_BOUNTY_STATE_NEW_AUSTIN;
		case STATE_GUARMA:
			return LAW_BOUNTY_STATE_GUARMA;
		default:
			break;
		}

		//return LAW_BOUNTY_STATE_FIRST; // original
		return LAW_BOUNTY_STATE_INVALID;
	}


	int GetDistrictFromRegion(int region)
	{
		if (region >= (int)eRegionIndex::REGION_FIRST && region <= (int)eRegionIndex::REGION_BAY_SILTWATERSTRAND) {
			return DISTRICT_BAYOU_NWA;
		}
		else if (region >= (int)eRegionIndex::REGION_BGV_APPLESEEDTIMBER && region <= (int)eRegionIndex::REGION_BGV_LAST) {
			return DISTRICT_BIG_VALLEY;
		}
		else if (region >= (int)eRegionIndex::REGION_BLU_FIRST && region <= (int)eRegionIndex::REGION_BLU_LAST) {
			return DISTRICT_BLUEGILL_MARSH;
		}
		else if (region >= (int)eRegionIndex::REGION_CML_BACCHUSBRIDGE && region <= (int)eRegionIndex::REGION_CML_LAST) {
			return DISTRICT_CUMBERLAND_FOREST;
		}
		else if (region >= (int)eRegionIndex::REGION_GRT_BEECHERS && region <= (int)eRegionIndex::REGION_GRT_QUAKERS_COVE) {
			return DISTRICT_GREAT_PLAINS;
		}
		else if (region >= (int)eRegionIndex::REGION_GRW_FIRST && region <= (int)eRegionIndex::REGION_GRZ_TRAVELLING_SALESMAN) {
			return DISTRICT_GRIZZLIES; // includes grz west and east?
		}
		else if (region >= (int)eRegionIndex::REGION_GUA_FIRST && region <= (int)eRegionIndex::REGION_GUA_LAST) {
			return DISTRICT_GUAMA;
		}
		else if (region >= (int)eRegionIndex::REGION_HRT_ABANDONED_MILL && region <= (int)eRegionIndex::REGION_HRT_VALENTINE) {
			return DISTRICT_HEARTLAND;
		}
		else if (region >= (int)eRegionIndex::REGION_ROA_FIRST && region <= (int)eRegionIndex::REGION_ROA_LAST) {
			return DISTRICT_ROANOKE_RIDGE;
		}
		else if (region >= (int)eRegionIndex::REGION_SCM_FIRST && region <= (int)eRegionIndex::REGION_SCM_LAST) {
			return DISTRICT_SCARLETT_MEADOWS;
		}
		else if (region >= (int)eRegionIndex::REGION_TAL_FIRST && region <= (int)eRegionIndex::REGION_TAL_TRAPPER) {
			return DISTRICT_TALL_TREES;
		}
		else if (region >= (int)eRegionIndex::REGION_GAP_FIRST && region <= (int)eRegionIndex::REGION_GAP_LAST) {
			return DISTRICT_GAPTOOTH_RIDGE;
		}
		else if (region >= (int)eRegionIndex::REGION_RIO_BENEDICT_POINT && region <= (int)eRegionIndex::REGION_RIO_LAST) {
			return DISTRICT_RIO_BRAVO;
		}
		else if (region >= (int)eRegionIndex::REGION_CHO_FIRST && region <= (int)eRegionIndex::REGION_CHO_LAST) {
			return DISTRICT_CHOLLA_SPRINGS;
		}
		else if (region >= (int)eRegionIndex::REGION_HEN_FIRST && region <= (int)eRegionIndex::REGION_CENTRALUNIONRR) {
			return DISTRICT_HENNIGANS_STEAD;
		}

		return DISTRICT_HEARTLAND;
	}


	int GetShopIndexFromRegion(int region, int shopType)
	{
		if (shopType == ST_DYNAMIC) {
			return 101;
		}
		else if (shopType == ST_MUSICIAN) {
			return 126;
		}
		else if (shopType == ST_HANDHELD) {
			return 127;
		}

		switch (region)
		{
		case 79:
			switch (shopType)
			{
			case ST_FIRST:
				return 128;
			case 6:
				return 130;
			case 27:
				return 131;
			case 26:
				return 129;
			case 19:
				return 59;
			default:
				break;
			}
			break;
		case 38:
			switch (shopType)
			{
			case 15:
				return 6;
			case 33:
				return 11;
			case 29:
				return 106;
			case 30:
				return 107;
			case 3:
				return 7;
			case 8:
				return 9;
			case 10:
				return 10;
			case 9:
				return 13;
			case 2:
				return 14;
			case 1:
				return 15;
			case 31:
				return 108;
			case 22:
				return 16;
			case 7:
				return 17;
			case 21:
				return 12;
			default:
				break;
			}
			break;
		case 98:
			switch (shopType)
			{
			case ST_FIRST:
				return 132;
			case 6:
				return 134;
			case 27:
				return 135;
			case 26:
				return 133;
			case 19:
				return 57;
			default:
				break;
			}
			break;
		case 69:
			switch (shopType)
			{
			case 4:
				return 18;
			case 3:
				return 19;
			case 2:
				return 20;
			case 1:
				return 21;
			case 31:
				return 105;
			default:
				break;
			}
			break;
		case 71:
			switch (shopType)
			{
			case ST_FIRST:
				return 137;
			case 6:
				return 139;
			case 27:
				return 140;
			case 26:
				return 138;
			case 19:
				return 56;
			default:
				break;
			}
			break;
		case 4:
			switch (shopType)
			{
			case ST_FIRST:
				return 141;
			case 6:
				return 143;
			case 27:
				return 144;
			case 26:
				return 142;
			default:
				break;
			}
			break;
		case 3:
			switch (shopType)
			{
			case 17:
				return 22;
			case 31:
				return 118;
			default:
				break;
			}
			break;
		case 5:
			switch (shopType)
			{
			case 15:
				return 36;
			case 8:
				return 37;
			case 33:
				return 52;
			case 29:
				return 109;
			case 10:
				return 38;
			case 30:
				return 110;
			case ST_FIRST:
				return 39;
			case 4:
				return 41;
			case 3:
				return 42;
			case 6:
				return 43;
			case 9:
				return 44;
			case 2:
				return 45;
			case 1:
				return 46;
			case 31:
				return 111;
			case 22:
				return 51;
			case 7:
				return 47;
			case 12:
				return 48;
			case 13:
				return 49;
			case 14:
				return 50;
			case 18:
				return 55;
			case 21:
				return 54;
			default:
				break;
			}
			break;
		case 105:
			switch (shopType)
			{
			case 15:
				return 25;
			case 33:
				return 33;
			case 29:
				return 112;
			case 10:
				return 26;
			case 4:
				return 27;
			case 3:
				return 28;
			case 6:
				return 29;
			case 2:
				return 30;
			case 31:
				return 113;
			case 22:
				return 32;
			case 1:
				return 31;
			default:
				break;
			}
			break;
		case 101:
			switch (shopType)
			{
			case 9:
				return 35;
			default:
				break;
			}
			break;
		case 13:
			switch (shopType)
			{
			case 18:
				return 4;
			default:
				break;
			}
			break;
		case 23:
			switch (shopType)
			{
			case 2:
				return 23;
			case 1:
				return 24;
			default:
				break;
			}
			break;
		case 9:
			switch (shopType)
			{
			case ST_FIRST:
				return 145;
			case 26:
				return 146;
			case 6:
				return 147;
			case 27:
				return 148;
			case 19:
				return 58;
			default:
				break;
			}
			break;
		case 26:
			switch (shopType)
			{
			case 29:
				return 114;
			case 33:
				return 66;
			case 10:
				return 60;
			case 3:
				return 61;
			case 20:
				return 62;
			case 9:
				return 63;
			case 31:
				return 115;
			case 22:
				return 64;
			case 2:
				return 65;
			default:
				break;
			}
			break;
		case 14:
			switch (shopType)
			{
			case 18:
				return 5;
			default:
				break;
			}
			break;
		case 76:
			switch (shopType)
			{
			case 15:
				return 68;
			case 8:
				return 69;
			case 33:
				return 70;
			case 29:
				return 102;
			case 10:
				return 72;
			case 30:
				return 103;
			case ST_FIRST:
				return 73;
			case 3:
				return 74;
			case 6:
				return 75;
			case 9:
				return 76;
			case 2:
				return 77;
			case 31:
				return 104;
			case 1:
				return 78;
			case 22:
				return 80;
			case 20:
				return 79;
			default:
				break;
			}
			break;
		case 92:
			switch (shopType)
			{
			case 30:
				return 116;
			case 4:
				return 82;
			case 2:
				return 86;
			case 1:
				return 85;
			case 31:
				return 117;
			case 9:
				return 84;
			default:
				break;
			}
			break;
		case 78:
			switch (shopType)
			{
			case 29:
				return 120;
			case 6:
				return 0;
			case 2:
				return 2;
			case 31:
				return 121;
			case 22:
				return 1;
			case 1:
				return 3;
			default:
				break;
			}
			break;
		case 90:
		case 128:
			switch (shopType)
			{
			case 18:
				return 34;
			default:
				break;
			}
			break;
		case 28:
			switch (shopType)
			{
			case 2:
				return 89;
			case 1:
				return 88;
			case 3:
				return 87;
			default:
				break;
			}
			break;
		case 65:
			switch (shopType)
			{
			case 31:
				return 119;
			default:
				break;
			}
			break;
		case 82:
			switch (shopType)
			{
			case 31:
				return 122;
			default:
				break;
			}
			break;
		case 110:
			switch (shopType)
			{
			case 31:
				return 124;
			default:
				break;
			}
			break;
		case 115:
			switch (shopType)
			{
			case 33:
				return 93;
			case 3:
				return 90;
			case 6:
				return 91;
			case 9:
				return 94;
			case 10:
				return 92;
			default:
				break;
			}
			break;
		case 120:
			switch (shopType)
			{
			case 3:
				return 95;
			case 2:
				return 96;
			default:
				break;
			}
			break;
		case 123:
			switch (shopType)
			{
			case 4:
				return 98;
			default:
				break;
			}
			break;
		case 117:
			switch (shopType)
			{
			case 31:
				return 123;
			case 2:
				return 99;
			case 1:
				return 100;
			default:
				break;
			}
			break;
		case 113:
			switch (shopType)
			{
			case 18:
				return 67;
			default:
				break;
			}
			break;
		case 127:
			switch (shopType)
			{
			case 4:
				return 125;
			default:
				break;
			}
			break;
		}
		return -1;
	}


	const char* GetRegionLiteralName(int region)
	{
		switch (region)
		{
		case -1:
			return "REGION_INVALID";
		case 0:
			return "REGION_BAY_CRAWDADWILLIES";
		case 1:
			return "REGION_BAY_MACOMBS_END";
		case 2:
			return "REGION_BAY_MERKINSWALLER";
		case 3:
			return "REGION_BAY_LAGRAS";
		case 4:
			return "REGION_BAY_LAKAY";
		case 5:
			return "REGION_BAY_SAINT_DENIS";
		case 6:
			return "REGION_BAY_ORANGE_PLANTATION";
		case 7:
			return "REGION_BAY_SERIAL_KILLER";
		case 8:
			return "REGION_BAY_SERENDIPITY";
		case 9:
			return "REGION_BAY_SHADYBELLE";
		case 10:
			return "REGION_BAY_SILTWATERSTRAND";
		case 11:
			return "REGION_BGV_APPLESEEDTIMBER";
		case 12:
			return "REGION_BGV_BERYLS_DREAM";
		case 13:
			return "REGION_BGV_BLACKBONEFOREST_TRAPPER";
		case 14:
			return "REGION_BGV_DAKOTARIVER_TRAPPER";
		case 15:
			return "REGION_BGV_FORTRIGGS";
		case 16:
			return "REGION_BGV_HANGINGDOG";
		case 17:
			return "REGION_BGV_LONEMULESTEAD";
		case 18:
			return "REGION_BGV_MISSING_HUSBAND";
		case 19:
			return "REGION_BGV_MONTO_REST";
		case 20:
			return "REGION_BGV_OWANJILA_DAM";
		case 21:
			return "REGION_BGV_PAINTEDSKY";
		case 22:
			return "REGION_BGV_PRONGHORN";
		case 23:
			return "REGION_BGV_RIGGS_STATION";
		case 24:
			return "REGION_BGV_SHACK";
		case 25:
			return "REGION_BGV_SHEPHERDS_RISE";
		case 26:
			return "REGION_BGV_STRAWBERRY";
		case 27:
			return "REGION_BGV_VALLEY_VIEW";
		case 28:
			return "REGION_BGV_WALLACE_STATION";
		case 30:
			return "REGION_BGV_WATSONSCABIN";
		case 29:
			return "REGION_BGV_OLD_MAN_JONES";
		case 31:
			return "REGION_BLU_CANEBREAK_MANOR";
		case 32:
			return "REGION_BLU_COPPERHEAD";
		case 33:
			return "REGION_BLU_SISIKA";
		case 34:
			return "REGION_BLU_TRAVELLING_SALESMAN";
		case 35:
			return "REGION_CML_BACCHUSBRIDGE";
		case 36:
			return "REGION_CML_DINO_LADY";
		case 37:
			return "REGION_CML_OLDFORTWALLACE";
		case 38:
			return "REGION_CML_SIXPOINTCABIN";
		case 39:
			return "REGION_GRT_BEECHERS";
		case 40:
			return "REGION_GRT_BLACKWATER";
		case 41:
			return "REGION_GRT_QUAKERS_COVE";
		case 42:
			return "REGION_GRZ_ADLERRANCH";
		case 43:
			return "REGION_GRZ_DEAD_RIVAL";
		case 53:
			return "REGION_GRZ_CALUMETRAVINE";
		case 54:
			return "REGION_GRE_CIVIL_WAR_BRIDE";
		case 44:
			return "REGION_GRZ_CHEZPORTER";
		case 55:
			return "REGION_GRZ_COHUTTA";
		case 45:
			return "REGION_GRZ_COLTER";
		case 56:
			return "REGION_GRZ_COTORRA_SPRINGS";
		case 46:
			return "REGION_GRZ_FROZEN_EXPLORER";
		case 57:
			return "REGION_GRZ_GUNFIGHT";
		case 47:
			return "REGION_GRZ_MILLESANI_CLAIM";
		case 48:
			return "REGION_GRZ_MOUNTAIN_MAN";
		case 49:
			return "REGION_GRZ_MOUNT_HAGEN_PEAK";
		case 50:
			return "REGION_GRZ_STARVING_CHILDREN";
		case 51:
			return "REGION_GRZ_TEMPEST_RIM";
		case 58:
			return "REGION_GRZ_THELOFT";
		case 59:
			return "REGION_GRE_VETERAN";
		case 60:
			return "REGION_GRZ_WAPITI";
		case 52:
			return "REGION_GRZ_WINTERMINING_TOWN";
		case 61:
			return "REGION_GRZ_TRAVELLING_SALESMAN";
		case 62:
			return "REGION_GUA_AGUASDULCES";
		case 63:
			return "REGION_GUA_CAMP";
		case 64:
			return "REGION_GUA_CINCOTORRES";
		case 65:
			return "REGION_GUA_LACAPILLA";
		case 66:
			return "REGION_GUA_MANICATO";
		case 67:
			return "REGION_HRT_ABANDONED_MILL";
		case 69:
			return "REGION_HRT_CARMODYDELL";
		case 70:
			return "REGION_HRT_CORNWALLKEROSENE";
		case 71:
			return "REGION_HRT_CROP_FARM";
		case 72:
			return "REGION_HRT_CUMBERLANDFALLS";
		case 73:
			return "REGION_HRT_DOWNSRANCH";
		case 74:
			return "REGION_HRT_EMERALDRANCH";
		case 75:
			return "REGION_HRT_GRANGERS_HOGGERY";
		case 76:
			return "REGION_HRT_HORSESHOEOVERLOOK";
		case 77:
			return "REGION_HRT_LARNEDSOD";
		case 78:
			return "REGION_HRT_LOONY_CULT";
		case 79:
			return "REGION_HRT_LUCKYSCABIN";
		case 80:
			return "REGION_HRT_SWANSONS_STATION";
		case 81:
			return "REGION_HRT_VALENTINE";
		case 82:
			return "REGION_ROA_ABERDEENPIGFARM";
		case 83:
			return "REGION_ROA_ANNESBURG";
		case 84:
			return "REGION_ROA_BEAVERHOLLOW";
		case 68:
			return "REGION_ROA_BEECHERS_C";
		case 85:
			return "REGION_ROA_BLACK_BALSAM_RISE";
		case 86:
			return "REGION_ROA_BRANDYWINE_DROP";
		case 87:
			return "REGION_ROA_BUTCHERCREEK";
		case 88:
			return "REGION_ROA_DOVERHILL";
		case 89:
			return "REGION_ROA_HAPPY_FAMILY";
		case 90:
			return "REGION_ROA_ISOLATIONIST";
		case 91:
			return "REGION_ROA_MACLEANSHOUSE";
		case 92:
			return "REGION_ROA_MOSSY_FLATS";
		case 93:
			return "REGION_ROA_ROANOKE_VALLEY";
		case 94:
			return "REGION_ROA_ROCKYSEVEN";
		case 95:
			return "REGION_ROA_TRAPPER";
		case 97:
			return "REGION_ROA_VANHORNMANSION";
		case 98:
			return "REGION_ROA_VANHORNPOST";
		case 96:
			return "REGION_ROA_OLD_MAN_JONES";
		case 99:
			return "REGION_SCM_BRAITHWAITEMANOR";
		case 100:
			return "REGION_SCM_BULGERGLADE";
		case 101:
			return "REGION_SCM_CALIGAHALL";
		case 102:
			return "REGION_SCM_CATFISHJACKSONS";
		case 103:
			return "REGION_SCM_CLEMENSCOVE";
		case 104:
			return "REGION_SCM_CLEMENSPOINT";
		case 105:
			return "REGION_SCM_COMPSONS_STEAD";
		case 106:
			return "REGION_SCM_DAIRY_FARM";
		case 107:
			return "REGION_SCM_HORSE_SHOP";
		case 108:
			return "REGION_SCM_LONNIESSHACK";
		case 109:
			return "REGION_SCM_LOVE_TRIANGLE";
		case 110:
			return "REGION_SCM_RADLEYS_PASTURE";
		case 111:
			return "REGION_SCM_RHODES";
		case 112:
			return "REGION_SCM_SLAVE_PEN";
		case 113:
			return "REGION_TAL_AURORA_BASIN";
		case 114:
			return "REGION_TAL_DEAD_SETTLER";
		case 115:
			return "REGION_TAL_COCHINAY";
		case 116:
			return "REGION_TAL_MANZANITAPOST";
		case 117:
			return "REGION_TAL_PACIFICUNIONRR";
		case 118:
			return "REGION_TAL_TANNERSREACH";
		case 119:
			return "REGION_TAL_TRAPPER";
		case 135:
			return "REGION_HEN_MACFARLANES_RANCH";
		case 136:
			return "REGION_HEN_THIEVES_LANDING";
		case 137:
			return "REGION_HEN_TRAVELLING_SALESMAN";
		case 138:
			return "REGION_HEN_HARRIET";
		case 127:
			return "REGION_CHO_ARMADILLO";
		case 128:
			return "REGION_CHO_COOTS_CHAPEL";
		case 129:
			return "REGION_CHO_DON_JULIO_HOUSE";
		case 131:
			return "REGION_CHO_RIDGEWOOD_FARM";
		case 130:
			return "REGION_CHO_RILEYS_CHARGE";
		case 132:
			return "REGION_CHO_TWIN_ROCKS";
		case 133:
			return "REGION_CHO_TRAVELLING_SALESMAN";
		case 134:
			return "REGION_CHO_TRAPPER";
		case 120:
			return "REGION_GAP_GAPTOOTH_BREACH";
		case 121:
			return "REGION_GAP_TUMBLEWEED";
		case 122:
			return "REGION_GAP_RATHSKELLER_FORK";
		case 123:
			return "REGION_RIO_BENEDICT_POINT";
		case 124:
			return "REGION_RIO_FORT_MERCER";
		case 125:
			return "REGION_RIO_PLAIN_VIEW";
		case 126:
			return "REGION_RIO_TRAVELLING_SALESMAN";
		case 139:
			return "REGION_CENTRALUNIONRR";
		default:
			break;
		}
		return "REGION_INVALID";
	}

	
	const char* GetStateBlipNameFromState(int state)
	{
		switch (state)
		{
		case 0:
			return "LBS_AMBARINO_BOUNTY";
		case 1:
			return "LBS_NEW_HANOVER_BOUNTY";
		case 2:
			return "LBS_LEMOYNE_BOUNTY";
		case 3:
			return "LBS_W_ELIZABETH_BOUNTY";
		case 4:
			return "LBS_NEW_AUSTIN_BOUNTY";
		case 5:
			return "LBS_GUARMA_BOUNTY";
		default:
			break;
		}
		return "LBS_INVALID_BOUNTY";
	}
}
