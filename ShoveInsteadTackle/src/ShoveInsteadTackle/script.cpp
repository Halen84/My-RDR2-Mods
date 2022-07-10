#include "script.h"
#include "keyboard.h"


void main()
{	
	while (true)
	{
		// Some missions require grapple/tackles iirc
		if (!MISC::GET_MISSION_FLAG())
		{
			PED::_SET_PED_ACTION_DISABLE_FLAG(PLAYER::PLAYER_PED_ID(), 1); // ADF_GRAPPLE
			PED::_SET_PED_ACTION_DISABLE_FLAG(PLAYER::PLAYER_PED_ID(), 33); // ADF_TACKLE
		}

		WAIT(0);
	}
}


void ScriptMain()
{		
	srand(GetTickCount());
	main();
}
