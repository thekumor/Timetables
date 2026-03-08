// ==================================================
//
//	Project: Timetables
//
//	Module: Application
//	Component: I/O
//	File: data.cpp
//
//  Purpose:
//  Implements the file-processing and command
//  execution infrastructure declared in data.h.
//
//  Notes:
//  Contains the File implementation and the
//  Command execution system used by the
//  application.
//
//	Author(s): The Kumor
//
// ==================================================

#include "data.h"

namespace tmt
{

	File::File(const std::string& path)
	{
		Load(path);
	}

	void File::Load(const std::string& path)
	{
		m_Input.open(path, std::ios::in);

		if (!m_Input)
		{
			Error("Unable to load input (default) file.");
			__debugbreak();
			return;
		}

		std::string line;

		while (std::getline(m_Input, line))
			m_Content.push_back(line);

		m_OriginalContent = m_Content;

		m_Input.close();
	}

	void File::Output(const std::string& path)
	{
		m_Output.open(path, std::ios::out);

		if (!m_Output)
		{
			Error("Unable to load output file.");
			__debugbreak();
			return;
		}

		for (std::vector<std::string>::iterator it = m_Content.begin(); it != m_Content.end(); it++)
			m_Output << *it << "\n";

		m_Output.close();
	}

	void File::PushLine(const std::string& line, std::int32_t i)
	{
		m_Content.insert(m_Content.begin() + i, line);
	}

	void File::SetLine(std::uint32_t line, const std::string& str)
	{
		line--;
		if (line >= m_Content.size() || line < 0) return;

		m_Content[line] = str;
	}

	void File::SetLine(const std::string& where, const std::string& str)
	{
		for (std::vector<std::string>::iterator it = m_Content.begin(); it != m_Content.end(); it++)
			if (*it == where)
			{
				*it = str;
				break;
			}
	}

	void File::Reset()
	{
		m_Content = m_OriginalContent;
	}

	Command::Command(const std::string& name, std::int32_t params)
		: m_Name(name), m_Params(params)
	{
	}

	void Command::Run(File* file, const std::vector<std::string>& params)
	{
		m_Callback(file, params);
	}

	void Command::SetCallback(CmdCallback callback)
	{
		m_Callback = callback;
	}

	void RunCommands(File* file, const std::string& input, std::vector<Command>& commands)
	{
		size_t pos = input.find(' ');
		std::string cmdName = input.substr(0, pos);

		bool correctCommand = false;

		for (std::vector<Command>::iterator it = commands.begin(); it != commands.end(); it++)
		{
			if (it->GetName() == cmdName)
			{
				correctCommand = true;

				std::vector<std::string> params = { };

				size_t start = pos + 1;
				while ((pos = input.find(' ', start)) != std::string::npos)
				{
					std::string param = input.substr(start, pos - start);
					params.push_back(param);

					start = pos + 1;
				}
				if (start < input.size() && input.size() != cmdName.size())
					params.push_back(input.substr(start));

				it->Run(file, params);

				break;
			}
		}
		if (!correctCommand)
		{
			SetConsoleText(TMT_COLOR_BAD);
			std::cout << cmdName << " is not a correct command." << std::endl;
		}
	}

}