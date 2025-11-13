#include <iostream>
#include <string>

#include "Command.hpp"
#include "EditorShell.hpp"
#include "Parser.hpp"

int main(int argc, const char* argv[]) {
	printf("Starting Editor Shell\n");

	std::string input;
	Phezu::Editor::Command command{};
	Phezu::Editor::EditorShell editorShell;

	while (true) {
		printf("> ");

		if (!std::getline(std::cin, input))
			break;

		ParseInput(input, command);

		if (command.Type == Phezu::Editor::CommandType::Exit)
			break;
		if (command.Type == Phezu::Editor::CommandType::Invalid) {
			printf("Invalid command\n");
			continue;
		}

		editorShell.ExecuteCommand(command);
	}

	editorShell.Destroy();

	return 0;
}
