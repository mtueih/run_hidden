#include "run_hidden/run_hidden.h"

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		return 0;
	}

	return run_hidden(argv[1], false);
}
