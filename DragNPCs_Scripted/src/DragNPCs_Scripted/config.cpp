#define MINI_CASE_SENSITIVE
#include "ini.h"
#include "config.h"
#include "keyboard.h"

const char* FILE_NAME = "DragPeds_Scripted_Config.ini";
mINI::INIFile file(FILE_NAME);
mINI::INIStructure ini;

int Config::iShoveKey = VK_NUMPAD6;
int Config::iDragKey = VK_NUMPAD3;

void Config::Read()
{
	if (std::ifstream(FILE_NAME).good()) {
		file.read(ini);

		int _isk = std::stoi(ini.get("DragNPCsConfiguration").get("iShoveKey"));
		iShoveKey = _isk;

		int _idk = std::stoi(ini.get("DragNPCsConfiguration").get("iDragKey"));
		iDragKey = _idk;
	}
}
