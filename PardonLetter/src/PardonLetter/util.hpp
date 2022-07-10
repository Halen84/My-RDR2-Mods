#pragma once

#include <string>
#include "script.h"

namespace Utility
{
	void PrintSubtitle(const char* msg)
	{
		UILOG::_UILOG_SET_CACHED_OBJECTIVE(msg);
		UILOG::_UILOG_PRINT_CACHED_OBJECTIVE();
		UILOG::_UILOG_CLEAR_HAS_DISPLAYED_CACHED_OBJECTIVE();
		UILOG::_UILOG_CLEAR_CACHED_OBJECTIVE();
	}

	void PrintSubtitle(const std::string& msg)
	{
		PrintSubtitle(msg.c_str());
	}

	void PrintSubtitle(int msg)
	{
		PrintSubtitle(std::to_string(msg).c_str());
	}
}
