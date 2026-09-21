#include "mtueih/run_hidden.h"


int main(int argc, char *argv[]) {
	if (argc < 2) { return 0; }

#ifdef RUN_HIDDEN_WAIT_MODE
	return run_hidden(argv[1], true);
#else
	return run_hidden(argv[1], false);
#endif
}
