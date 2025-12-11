#pragma once

// STL
#include <cstdint>

// WinAPI
#include <windows.h>

#define TMT_INPUT_FILE "data/base.tex"
#define TMT_OUTPUT_FILE "data/output.tex"

#define TMT_COLOR_DEFAULT 7
#define TMT_COLOR_BAD 4
#define TMT_COLOR_GOOD 6
#define TMT_COLOR_WARNING 14

namespace tmt
{

	void Error(const char* message);
	void SetConsoleText(std::int32_t color);

}