# Timetables
Tool that helps create timetables.

## Background
In today’s fast-paced environment, effective time management has become essential for
productivity and personal well-being. Many individuals struggle to plan
their daily and weekly activities efficiently, often relying on manual methods such as paper
schedules or scattered digital tools. These approaches can lead to missed deadlines,
overlapping commitments, and increased stress.

## Description
Timetables is a lightweight and efficient program designed to help users create, manage,
and organize their weekly schedules. It provides a simple yet powerful way to plan daily
tasks, classes, work activities, and personal events. By automating timetable creation and
offering clear visual overviews, the program ensures users can manage their time
effectively, avoid scheduling conflicts, and stay productive throughout the week.
Key features include:

- Creating weekly and daily timetables easily.
- Adding, editing, or removing tasks and events.
- Viewing schedules in a clear and organized format.
- Supporting time management for students, professionals, and individuals.
- Simplifying adjustments when plans change.

This project is ideal for anyone looking to stay organized and maintain a schedule without the
hassle of manual planning.

**Path to Pdflatex must be specified in data/path.txt, otherwise program will be unable to generate
an output. Example path is:\
C:\Users\theku\miniforge3\pkgs\miktex-24.4-hcfcfb64_1\Library\miktex\texmfs\install\miktex\bin\x64**

![Prompt interface](images/github/ui.png)
![Generator result](images/github/output.png)

## Target Audience
The program is intended for a wide range of users, including:

- Students,
- Working professionals,
- Small teams or organizations,
- Other individuals

## Features and Functionalities
Features include:
- Generating .pdf files (gen),
- Commands:
	- echo [text]
	- time [hour] [hourEnd]
	- gen [file=data/output.tex]
	- exit
	- add [task] [weekday] [hour] [hourEnd=hour]
	- rem [weekday] [hour] [hourEnd=hour]
	- date [day] [month] [year]
	- desc [text]
	- erase [task]
	- clear
	- out [hour] [hourEnd]

## Software & Hardware Requirements
Microsoft Windows 10/11. Possible to run on a potato.
