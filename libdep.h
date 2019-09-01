#pragma once
#ifndef LIBDEP
#define LIBDEP

#include <windows.h>
#include <winuser.h>
#include <wincon.h>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cctype>
#include <string>
#include <vector>
#include <map>

using namespace std;

typedef vector < string > CM_INPUT;
typedef void(*FnCallPtr)(CM_INPUT, int);
#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)

#include "commands.h"
#include "navigation_system.h"

#endif // LIBDEP
