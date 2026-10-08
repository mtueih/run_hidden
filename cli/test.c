#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[]) {
	Sleep(3000);
	if (argc < 2) {
		return 1;
	}

	FILE *test_file = fopen("test.txt", "w");
	if (test_file == NULL) {
		return 1;
	}

	for (int i = 0; i < argc; ++i) {
		Sleep(3000);

		if (fprintf(test_file, "%s\n", argv[i]) < 0) {
			perror("fprintf");

			if (fclose(test_file) == EOF) {
				return 1;
			}
			return 1;
		}
	}

	if (fclose(test_file) == EOF) {
		return 1;
	}
	return 0;
}
