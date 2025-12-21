// ================================================
// 
//	Project: Timetables
// 
//	File: timetables.cpp
//	Desc: Entry point for a tool that tells you
//	whether your vegetable garden is valid or not.
// 
//	Date: 2025/12/21 2:07 PM
//	Authors: The Kumor
// 
// ================================================

// STL
#include <iostream>
#include <unordered_map>
#include <string>

// Timetables
#include <tmt/data.h>
#include <tmt/util.h>

#define TMT_PROMPT() SetConsoleText(TMT_COLOR_DEFAULT); std::cout << "(Timetables) > "
#define TMT_VERSION "1.0"

int main()
{
	using namespace tmt;

	struct Task
	{
		std::string Name, Hour, Day;
	};
	std::vector<Task> tasks = { };

	std::string startDate = "7/12/2025";
	std::string endDate = "14/12/2025";
	std::string description = "A regular week.";

	// ------------------------------------
	// echo command
	// ------------------------------------
	Command echo("echo", -1);
	echo.SetCallback([=](File* f, const std::vector<std::string>& params)
	{
		for (std::int32_t i = 0; i < params.size(); i++)
			std::cout << params[i] << " ";

		std::cout << std::endl;
	});

	// ------------------------------------
	// time command
	// ------------------------------------
	std::uint8_t startHour = 7, endHour = 20;
	Command time("time", 2);
	time.SetCallback([&startHour, &endHour, &tasks, &startDate, &endDate, &description](File* f, const std::vector<std::string>& params)
	{
		if (params.size() < 2) return;

		startHour = atoi(params[0].c_str());
		endHour = atoi(params[1].c_str());

		f->Reset();

		std::string defaultLine = "\\textbf{} & - & - & - & - & - & - & - \\\\ \\hline";

		std::int32_t j = 0;
		for (std::int32_t i = startHour; i <= endHour; i++, j++)
		{
			std::string line = defaultLine;
			line.insert(8, (i < 10 ? "0" : "") + std::to_string(i) + ":00");

			for (auto& k : tasks)
			{
				if (k.Hour == std::to_string(i))
				{
					std::int32_t limit = 0;

					if (k.Day == "Monday")
						limit = 1;
					else if (k.Day == "Tuesday")
						limit = 2;
					else if (k.Day == "Wednesday")
						limit = 3;
					else if (k.Day == "Thursday")
						limit = 4;
					else if (k.Day == "Friday")
						limit = 5;
					else if (k.Day == "Saturday")
						limit = 6;
					else if (k.Day == "Sunday")
						limit = 7;

					size_t pos = 0;
					std::int32_t l = 0;
					while (l < limit)
					{
						pos = line.find('&', pos + 1);
						l++;
					}

					line.erase(pos + 2, 1);
					line.insert(pos + 2, k.Name);
				}
			}

			f->PushLine(line, 16 + j);
		}

		std::unordered_map<std::string, bool> existingTasks;
		for (auto& k : tasks)
			existingTasks[k.Name] = true;

		std::string allTasks = "Tasks: ";
		for (auto& k : existingTasks)
			allTasks += k.first + ", ";

		allTasks.erase(allTasks.size() - 2, 2); // Remove the , at the end.

		f->PushLine("\\LARGE", 16 + j + 2);
		f->PushLine("\\ \\\\\\\\", 16 + j + 3);
		f->PushLine(allTasks + "\\\\", 16 + j + 4);
		f->PushLine(startDate + " - " + endDate + "\\\\", 16 + j + 5);
		f->PushLine("Description: " + description, 16 + j + 6);
	});

	// ------------------------------------
	// gen command
	// ------------------------------------
	Command gen("gen", 2);
	gen.SetCallback([](File* f, const std::vector<std::string>& params)
	{
		std::string param = TMT_OUTPUT_FILE;
		if (params.size() != 0)
			param = params[0];

		f->Output(param);

		std::fstream path("data/path.txt", std::ios::in);
		if (!path)
		{
			Error("Unable to find path file!");
			__debugbreak();
			return;
		}

		std::string latexPath;
		std::getline(path, latexPath);

		size_t slashPos = param.rfind('/', param.size());
		std::string directory = "./";
		std::string fileName = "";
		bool relative = false;

		if (slashPos != std::string::npos)
		{
			directory = param.substr(0, slashPos);
			fileName = param.substr(slashPos + 1, param.size() - slashPos);
			relative = true;
		}

		std::string cmdCd(relative ? "cd " + directory : "");
		std::string cmdPdf(
			latexPath +
			std::string("\\pdflatex ") +
			(relative ? fileName : param)
		);

		// Generate a .pdf file.
		std::string cmd(std::string((relative ? (cmdCd + " && ") : "") + cmdPdf + " > nul").c_str());

		SetConsoleText(TMT_COLOR_WARNING);
		std::cout << cmd << std::endl;

		std::system(cmd.c_str());

		path.close();
	});

	// ------------------------------------
	// exit command
	// ------------------------------------
	Command exitCmd("exit", 2);
	exitCmd.SetCallback([](File* f, const std::vector<std::string>& params)
	{
		exit(0);
	});

	// ------------------------------------
	// add command
	// ------------------------------------
	Command add("add", 2);
	add.SetCallback([&tasks](File* f, const std::vector<std::string>& params)
	{
		if (params.size() < 3) return;

		std::string name = params[0];
		std::string day = params[1];
		std::string hour = params[2];

		std::string hourTo = hour;
		if (params.size() > 3)
			hourTo = params[3];

		Task task;
		task.Name = name;
		task.Day = day;
		// task.Hour = hour;

		bool fail = false;

		for (std::int32_t i = atoi(hour.c_str()); i <= atoi(hourTo.c_str()); i++)
		{
			bool taken = false;
			task.Hour = std::to_string(i);

			for (std::vector<Task>::iterator it = tasks.begin(); it != tasks.end(); it++)
				if (it->Day == day && it->Hour == task.Hour)
				{ // Means it's taken.
					it->Name = name; // Just overwrite the name.
					taken = true;
					break;
				}

			if (!taken)
				tasks.push_back(task);
		}
	});

	// ------------------------------------
	// rem command
	// ------------------------------------
	Command rem("rem", 2);
	rem.SetCallback([&tasks](File* f, const std::vector<std::string>& params)
	{
		if (params.size() < 2) return;

		std::string day = params[0];
		std::string hour = params[1];

		std::string hourTo = params[1];
		if (params.size() > 2)
			hourTo = params[2];

		for (std::int32_t i = atoi(hour.c_str()); i <= atoi(hourTo.c_str()); i++)
		{
			for (std::vector<Task>::iterator it = tasks.begin(); it != tasks.end(); it++)
				if (it->Day == day && it->Hour == std::to_string(i))
				{ // Means it's taken.
					tasks.erase(it);
					break;
				}
		}
	});

	// ------------------------------------
	// date command
	// ------------------------------------
	Command date("date", 2);
	date.SetCallback([&startDate, &endDate](File* f, const std::vector<std::string>& params)
	{
		if (params.size() < 3) return;

		std::string day = params[0];
		std::string month = params[1];
		std::string year = params[2];

		std::int32_t numDay = atoi(day.c_str());
		std::int32_t numMonth = atoi(month.c_str());
		std::int32_t numYear = atoi(year.c_str());

		startDate =
			(numDay < 10 ? "0" : "") + day + "/" +
			(numMonth < 10 ? "0" : "") + month + "/" +
			year;

		for (std::int32_t i = 0; i < 7; i++)
		{
			bool isLongMonth = (numMonth == 1 || numMonth == 3 || numMonth == 5 || numMonth == 7 || numMonth == 8 || numMonth == 10 || numMonth == 12);
			bool isShortMonth = numMonth == 2;

			bool isLeapYear = numYear % 4 == 0;

			numDay++;

			if ((numDay == 31 && !isLongMonth) || (numDay == 32) ||
				(numMonth == 2 && numDay == 29 && !isLeapYear) || (numMonth == 2 && numDay == 30))
			{
				numMonth++;
				numDay = 1;

				if (numMonth == 13)
				{
					numMonth = 1;
					numYear++;
				}
			}
		}

		endDate =
			(numDay < 10 ? "0" : "") + std::to_string(numDay) + "/" +
			(numMonth < 10 ? "0" : "") + std::to_string(numMonth) + "/" +
			std::to_string(numYear);

		SetConsoleText(TMT_COLOR_WARNING);
		std::cout << "Date range set to: " << startDate << " - " << endDate << std::endl;
	});

	// ------------------------------------
	// desc command
	// ------------------------------------
	Command desc("desc", 2);
	desc.SetCallback([&description](File* f, const std::vector<std::string>& params)
	{
		if (params.size() < 1) return;

		description = "";
		for (auto& k : params)
			description += k + " ";

		description.erase(description.size() - 1, 1);
	});

	// ------------------------------------
	// erase command
	// ------------------------------------
	Command erase("erase", 2);
	erase.SetCallback([&tasks](File* f, const std::vector<std::string>& params)
	{
		if (params.size() < 1) return;

		std::string name = params[0];

		for (std::int32_t i = 0; i < tasks.size(); i++)
		{
			if (tasks[i].Name == name)
			{
				tasks.erase(tasks.begin() + i);
				i--;
			}
		}
	});

	// ------------------------------------
	// clear command
	// ------------------------------------
	Command clear("clear", 2);
	clear.SetCallback([&tasks](File* f, const std::vector<std::string>& params)
	{
		tasks.clear();
	});

	// ------------------------------------

	std::vector<Command> commands = { echo, time, gen, exitCmd, add, rem, date, desc, erase, clear };

	File file;
	file.Load(TMT_INPUT_FILE);

	std::string input;

	std::cout << "Timetables (c) 2025-2026 by The Kumor\n" << "Version " << TMT_VERSION << "\n" << std::endl;

	TMT_PROMPT();
	while (std::getline(std::cin, input))
	{
		RunCommands(&file, input, commands);
		TMT_PROMPT();
	}

	return 0;
}