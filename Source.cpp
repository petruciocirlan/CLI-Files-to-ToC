#include "libdep.h"

int main(int argc, char** argv)
{
	PROGRAM_PATH = argv[0];
	run_navigation_system({}, 0);

	return 0;
}