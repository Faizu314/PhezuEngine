#include <iostream>

int main(int argc, const char* argv[]) {
	std::cout << "Hello World" << std::endl;
	printf("Argument count: %i\n", argc);

	for (int i = 1; i < argc; i++) {
		printf("Argument %i: %s\n", i, argv[i]);
	}

	return 0;
}