#include "libdep.h"

int main(int argc, char** argv)
{
	PROGRAM_PATH = argv[0];
	
	CM_Running = true;
	CM_Init();

	while (CM_Running)
	{
		CM_INPUT UserInput;
		int InputSize = 0;
		while (GetUserInput(UserInput, InputSize) != 0 || !InputSize)
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

	return 0;
}