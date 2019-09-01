#pragma once
#ifndef COMMANDS
#define COMMANDS

#include "libdep.h"
extern map < string, FnCallPtr > CmdCall;

void CM_Init();
void cmd_clear(CM_INPUT, int);
void cmd_change_lesson(CM_INPUT, int);
void cmd_help(CM_INPUT, int);
void cmd_read_node(CM_INPUT, int);
void cmd_show_lessons(CM_INPUT, int);
void cmd_end_navsys(CM_INPUT, int);

inline void throw_error(string);
int GetUserInput(CM_INPUT&, int&);

#endif // COMMANDS