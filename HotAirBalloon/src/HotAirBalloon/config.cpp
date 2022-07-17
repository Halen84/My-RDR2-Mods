#include "config.h"
#include "script.h"
#include <string>

#define MINI_CASE_SENSITIVE
#include "ini.h"

const char* FILE_NAME = "BalloonConfig.ini";
mINI::INIFile file(FILE_NAME);
mINI::INIStructure ini;

float Config::fVelocityResistance = 65.0f; // 80.0f
float Config::fDescendSpeed = 0.015f;
float Config::fRotationSpeed = 0.45f;
bool Config::bShowControls = true;
bool Config::bSpawnInBalloon = false;
int Config::iSpawnKey = 186; // Semicolon

void Config::Read()
{
	if (std::ifstream(FILE_NAME).good()) {
		file.read(ini);

		float _fvr = std::stof(ini.get("HotAirBalloonSettings").get("fVelocityResistance"));
		fVelocityResistance = _fvr;

		/*float _fds = std::stof(ini.get("HotAirBalloonSettings").get("fDescendSpeed"));
		fDescendSpeed = _fds;*/

		float _frs = std::stof(ini.get("HotAirBalloonSettings").get("fRotationSpeed"));
		fRotationSpeed = _frs;

		std::string _brs = (ini.get("HotAirBalloonSettings").get("bShowControls"));
		if (_brs.substr(0, 4) == "true") {
			bShowControls = true;
		} else {
			bShowControls = false;
		}

		std::string _bsib = (ini.get("HotAirBalloonSettings").get("bSpawnInBalloon"));
		if (_bsib.substr(0, 4) == "true") {
			bSpawnInBalloon = true;
		} else {
			bSpawnInBalloon = false;
		}
		
		int _isk = std::stoi(ini.get("HotAirBalloonSettings").get("iSpawnKey"));
		iSpawnKey = _isk;
	}
}
