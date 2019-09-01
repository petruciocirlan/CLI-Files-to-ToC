#include "commands.h"

map < string, FnCallPtr > CmdCall;

void CM_Init()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	PCOORD garbage{ 0 };
	SetConsoleDisplayMode(hConsole, CONSOLE_FULLSCREEN_MODE, garbage);
	SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);

	CmdCall["clear"] = cmd_clear;
	CmdCall["cl"] = cmd_change_lesson;
	CmdCall["help"] = cmd_help;
	CmdCall["read"] = cmd_read_node;
	CmdCall["content"] = cmd_show_lessons;
	CmdCall["exit"] = cmd_end_navsys;

	CmdCall["clear"]({}, 0);

	string dataFolder = PROGRAM_PATH.substr(0, 1 + PROGRAM_PATH.rfind("\\", string::npos));

	if (PathFileExistsA((dataFolder + "data").c_str()) == FALSE)
	{
		cout << "\"" + (dataFolder + "data") + "\" is missing!\n";
		CM_Running = false;
		return;
	}

	starting_node = new nav_node;
	starting_node->IsFolder = true;
	starting_node->fileName = "data";
	starting_node->path = dataFolder;
	starting_node->parent = NULL;

	trie_start = new trie;
	build_navigation_tree(dataFolder + "data\\", starting_node);

	if (starting_node->children.empty() != FALSE)
	{
		cout << "\"" + (dataFolder + "data") + "\" is missing!\n";
	}

	current_location = NULL;

	CmdCall["clear"]({}, 0);
	cout << "Write \"HELP\" to see a list of available commands!\n";

	cout << '\n';
	SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "CHAPTER ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "SECTION ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << "SELECTED\n";
	SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);
	cout << '\n';

	print_navigation_tree(starting_node);
}

void cmd_clear(CM_INPUT ARGV = {}, int ARGC = 0)
{
	system("cls");
}

void cmd_change_lesson(CM_INPUT ARGV, int ARGC)
{
	if (ARGC != 2 || (ARGC == 2 && ARGV[1] == "/?"))
	{
		cout <<
			"Usage:\n"
			"\nCL <section name>\n"
			"Use quotes for section names that cotain spaces!\n"
			"Ex: CL \"quotes example\"\n";
		cout << L'\n';
		return;
	}

	trie_search((char*)ARGV[1].c_str());
}

void cmd_help(CM_INPUT ARGV, int ARGC)
{
	cout <<
		"CLEAR\tEmpty the console screen.\n"
		"CL\tChange current selected section.\n"
		"HELP\tShow info about available commands.\n"
		"READ\tRead the contents of selected section.\n"
		"CONTENT\tShow table of contents.\n"
		"EXIT\tExit the program.\n"
		"\n";
}

void cmd_read_node(CM_INPUT ARGV, int ARGC)
{
	SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);

	if (current_location == NULL)
	{
		throw_error(
			"No section is selected!\n"
			"Use \"CL\" to select one.\n"
			"To see other available commands use \"HELP\"!\n"
		);
		return;
	}
	else if (current_location->IsFolder == TRUE)
	{
		throw_error(
			"Something went wrong (tried to read contents of CHAPTER - is used only as group name currently)!"
			/*
			"Nodul ce contine numele capitolului nu contine informatii.\n"
			"Pentru a folosi comanda \"READ\" selectati un nod lectie!\n"*/
		);
		return;
	}

	ifstream node_content(current_location->path);
	char lineBuffer[256];
	bool firstTime = true;

	do
	{
		node_content.getline(lineBuffer, 256);
		if (firstTime == TRUE && node_content.gcount() == 0)
		{
			throw_error("Selected section is empty!");
			break;
		}
		else
		{
			cout << lineBuffer << '\n';
			firstTime = false;
		}
	} while (!node_content.eof());

	node_content.close();
}

void cmd_show_lessons(CM_INPUT ARGV, int ARGC)
{
	print_navigation_tree(starting_node);
}

void cmd_end_navsys(CM_INPUT ARGV, int ARGC)
{
	CM_Running = false;
}

void throw_error(string err)
{
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "Err: ";
	SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);
	cout << err;
	cout << '\n';
}

int GetUserInput(CM_INPUT& ARGV, int& ARGC)
{
	SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	cout << ">>>";
	SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);
	string input;
	getline(cin, input);
	if (iscntrl(input[0]) == TRUE)
		return -1;

	int last = 0, len = 0, pos = 0;
	string::iterator it;
	for (it = input.begin(); it != input.end(); ++it, ++pos)
	{
		if (!isspace(*it))
		{
			len++;

			if (isupper(*it))
				* it = tolower(*it);
		}
		else
		{
			if (len)
			{
				ARGV.push_back(input.substr(last, len));
				ARGC++;
			}

			while (it != input.end() && isspace(*it))
				it++, pos++;
			if (it == input.end())
				break;

			if (*it)
			{
				last = pos;
				len = 1;

				if (*it == '\"')
				{
					last = pos;
					++pos, ++it;

					while (it != input.end() && *it != '\"')
						++it, ++pos;

					if (it == input.end() || *it != '\"')
					{
						throw_error("Quote left open! (Missing ending \")");
						return -1;
					}

					len = pos - last;
					ARGV.push_back(input.substr(last + 1, len - 1));
					ARGC++;

					len = 0;
					continue;
				}
			}
			else
				len = 0;
		}
	}

	if (len)
	{
		ARGV.push_back(input.substr(last, len));
		ARGC++;
	}

	return 0;
}