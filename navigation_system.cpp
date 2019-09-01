#include "cm_libs.h"

string PROGRAM_PATH;
HANDLE hConsole;
bool CM_Running;
nav_node* starting_node, * current_location;
trie* trie_start;

void run_navigation_system(CM_INPUT ARGV, int ARGC)
{
	CM_Running = true;
	CM_Init(ARGV, ARGC);

	while (CM_Running)
	{
		CM_INPUT UserInput;
		int InputSize = 0;
		if (GetUserInput(UserInput, InputSize) != 0)
			continue;

		if (!InputSize)
			continue;

		if (CmdCall.find(UserInput[0]) == CmdCall.end())
		{
			cout << "\"" + UserInput[0] + "\" is not recognized as a command,\n"
				"to see a list of available commands use \"HELP\"\n"
				"\n";
		}
		else
			CmdCall[UserInput[0]](UserInput, InputSize);
	}

	return;
}

void print_navigation_tree(nav_node* node, int depth)
{
	if (node->children.empty() != FALSE)
	{
		for (int i = 0; i < depth; ++i)
			cout << '|';
		cout << "empty\n";
		return;
	}
	
	if (!CM_Running) return;

	vector < pair < int, nav_node* > > ::iterator it;
	for (it = node->children.begin(); it != node->children.end(); ++it)
	{
		nav_node* currentNode = it->second;
		for (int i = 0; i < depth; ++i)
			cout << ' ';

		if (currentNode == current_location)
			SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		else
		{
			if (currentNode->IsFolder == TRUE)
				SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			else
				SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY);
		}

		cout << currentNode->fileName << '\n';

		SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);

		if (currentNode->IsFolder == TRUE)
		{
			print_navigation_tree(currentNode, depth + 1);
			if (!depth)
				cout << '\n';
		}
	}
}

bool childrenSort(pair < int, nav_node* > x, pair < int, nav_node* > y)
{
	if (x.first == y.first)
	{
		throw_error("The contents of the 'data' folder does not follow the rules!"); // TODO: change "the rules", use better wording
		CmdCall["exit"];
	}
	return x.first < y.first;
}

void build_navigation_tree(string dirName, nav_node* parentNode)
{
	if (!CM_Running) return;

	WIN32_FIND_DATAA file;
	HANDLE fileHandle;

	fileHandle = FindFirstFileA((dirName + "*").c_str(), &file);

	do
	{
		if (isdigit(file.cFileName[0]) == FALSE)
			continue;

		nav_node* newFile = new nav_node;
		node_data content(file.cFileName);

		newFile->path = dirName + file.cFileName;
		newFile->fileName = content.name;
		newFile->parent = parentNode;

		trie_insert((char*)newFile->fileName.c_str(), newFile);

		parentNode->children.push_back(make_pair(content.order, newFile));

		if ((file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			&& file.cFileName[0] != '.')
		{
			newFile->IsFolder = true;

			build_navigation_tree(newFile->path + "\\", newFile);
		}
		else
		{
			newFile->IsFolder = false;
		}

	} while (FindNextFileA(fileHandle, &file));

	sort(parentNode->children.begin(), parentNode->children.end(), childrenSort);

	FindClose(fileHandle);
}

void trie_search(char* str)
{
	if (!CM_Running) return;

	trie* nod = trie_start;
	while (iscntrl(*str) == FALSE)
	{
		char toSearch = tolower(*str);

		vector < pair < char, trie* > > ::iterator it;
		for (it = nod->next.begin(); it != nod->next.end(); ++it)
			if (it->first == toSearch)
				break;

		if (it == nod->next.end())
		{
			break;
		}
		else
		{
			nod = it->second;
		}

		str++;
	}

	if (nod->LessonAddress != NULL)
	{
		if (current_location == nod->LessonAddress)
		{
			cout << "This section is already selected!\n\n";
		}
		else if (nod->LessonAddress->IsFolder == TRUE)
		{
			cout << "You cannot select a chapter!\n";
			// TODO: add read feature to chapter, implement that by having each chapter use a 'metadata' text file
		}
		else
		{
			current_location = nod->LessonAddress;
			print_navigation_tree(starting_node);
		}
	}
	else
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		if (iscntrl(*str) == FALSE)
			cout << "There is no section with this name!\n";
		else
			cout << "There are at least two sections with this prefix!\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE | FOREGROUND_INTENSITY);
	}
}

void trie_insert(char* str, nav_node* adr)
{
	if (!CM_Running) return;

	trie* prev = NULL;
	trie* nod = trie_start;

	while (iscntrl(*str) == FALSE)
	{
		char toSearch = tolower(*str);

		if (nod->Exists == FALSE)
			nod->LessonAddress = NULL;

		nod->prev = prev;
		prev = nod;

		vector < pair < char, trie* > > ::iterator it;
		for (it = nod->next.begin(); it != nod->next.end(); ++it)
			if (it->first == toSearch)
				break;

		if (it == nod->next.end())
		{
			nod->next.push_back(make_pair(toSearch, new trie));

			nod->Children++;

			nod = nod->next.back().second;
			nod->prev = prev;
		}
		else
		{
			nod = it->second;
		}

		str++;
	}

	/// Check if there are multiples files with the same name
	/// If there's a folder with this name override it with the file
	if (nod->Exists == TRUE &&
		nod->LessonAddress->IsFolder == FALSE)
	{
		if (adr->IsFolder == FALSE)
		{
			throw_error("The files in the 'data' folder are not in the original state!");
			CmdCall["exit"];
		}
		return;
	}

	nod->Exists = true;
	do
	{
		nod->LessonAddress = adr;
		nod = nod->prev;
	} while (nod != NULL && nod->Children < 2 && (nod->Exists == false || nod->LessonAddress->IsFolder == true));
}