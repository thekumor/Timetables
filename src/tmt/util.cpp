// ==================================================
//
//	Project: Timetables
//
//	Module: Application
//	Component: Utility
//	File: util.cpp
//
//  Purpose:
//  Implements the utility functions declared in
//	util.h.
//
//  Notes:
//  Provides error handling via MessageBox and
//	console text formatting using Windows console 
//  colors.
//
//	Author(s): The Kumor
//
// ==================================================

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