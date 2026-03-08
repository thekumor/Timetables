// ==================================================
//
//	Project: Timetables
//
//	Module: Application
//	Component: I/O
//	File: data.h
//
//  Purpose:
//  Defines the file-processing and command execution
//  infrastructure used by the application.
//
//  Notes:
//  Provides a File abstraction for loading and
//  modifying text content. Includes a Command
//  system that applies parameterized operations
//  to a File instance.
//
//	Author(s): The Kumor
//
// ==================================================

#pragma once

// STL
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <functional>
#include <iostream>

// WinAPI
#include <windows.h>

// Timetables
#include <tmt/util.h>

namespace tmt
{

	class File
	{
	public:
		File(const std::string& path);
		File() = default;
		~File() = default;

		void Load(const std::string& path);
		void Output(const std::string& path);
		void PushLine(const std::string& line, std::int32_t i);
		void SetLine(std::uint32_t line, const std::string& str);
		void SetLine(const std::string& where, const std::string& str);
		void Reset();

	private:
		std::vector<std::string> m_OriginalContent;
		std::vector<std::string> m_Content;
		std::fstream m_Input, m_Output;
	};

	using CmdCallback = std::function<void(File*, const std::vector<std::string>& params)>;

	class Command
	{
	public:
		Command(const std::string& name, std::int32_t params);
		Command() = default;
		~Command() = default;

		std::string GetName() const { return m_Name; }
		std::uint32_t GetParams() const { return m_Params; }

		void Run(File* file, const std::vector<std::string>& params);
		void SetCallback(CmdCallback callback);

	private:
		std::string m_Name;
		std::int32_t m_Params;
		CmdCallback m_Callback;
	};

	void RunCommands(File* file, const std::string& input, std::vector<Command>& commands);

}