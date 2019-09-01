#pragma once
#ifndef CM_COMMANDS
#define CM_COMMANDS

#include "cm_libs.h"
extern map < string, FnCallPtr > CmdCall;

void CM_Init(CM_INPUT, int);
void cmd_clear(CM_INPUT, int);
void cmd_change_lesson(CM_INPUT, int);
void cmd_help(CM_INPUT, int);
void cmd_read_node(CM_INPUT, int);
void cmd_show_lessons(CM_INPUT, int);
void cmd_end_navsys(CM_INPUT, int);

inline void throw_error(string, int);
int GetUserInput(CM_INPUT&, int&);


#endif // CM_COMMANDS
