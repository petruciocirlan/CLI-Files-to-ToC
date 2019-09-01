#pragma once
#ifndef NAVIGATION_SYSTEM
#define NAVIGATION_SYSTEM

#include "libdep.h"
#pragma comment(lib, "Shlwapi.lib")
#include <shlwapi.h>

extern string PROGRAM_PATH;
extern HANDLE hConsole;
extern bool CM_Running;

struct node_data
{
	int order;
	string name;

	int stringToNum(string str)
	{
		int x = 0;
		unsigned int i = 0;
		while (i < str.length() && '0' <= str[i] && str[i] <= '9')
		{
			x = x * 10 + str[i] - '0';
			i++;
		}

		return x;
	}
	node_data(string fullName)
	{
		order = stringToNum(fullName.substr(0, 3));
		fullName.erase(0, 3);
		size_t eraseExt = fullName.find('.');
		if (eraseExt != string::npos)
			name = fullName.erase(eraseExt);
		else
			name = fullName;
	}
};

struct nav_node
{
	bool IsFolder;
	string fileName;
	string path;

	nav_node *parent;
	vector < pair < int, nav_node* > > children;
};

struct trie
{
	bool Exists;
	int Children;
	nav_node *LessonAddress;

	trie *prev;
	vector < pair < char, trie* > > next;

	trie()
	{
		Exists = false;
		Children = 0;
		LessonAddress = NULL;
		prev = NULL;
	}
};

extern nav_node *starting_node, *current_location;
extern trie *trie_start;

void build_navigation_tree(string, nav_node*);
bool childrenSort(pair < int, nav_node* >, pair < int, nav_node* >);
void print_navigation_tree(nav_node*, int = 0);

void trie_insert(char*, nav_node*);
void trie_search(char*);

#endif // NAVIGATION_SYSTEM
