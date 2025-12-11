#include "util.h"

namespace tmt
{

	void Error(const char* message)
	{
		MessageBox(nullptr, message, "Error", IDOK);
	}

	void SetConsoleText(std::int32_t color)
	{
		HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(handle, color);
	}

}