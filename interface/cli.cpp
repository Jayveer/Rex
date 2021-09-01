#include "cli.h"

CLI::CLI(int argc, char** argv) {
	this->argc = argc;
	this->argv = argv;
}

CLI::~CLI() {

}

bool isDar(std::string& filepath) {
	return getExtension(filepath) == ".dar";
}

bool isStage(std::string& filepath) {
	if (getExtension(filepath) == ".DIR" || getExtension(filepath) == ".dir") {
		std::string filename = getExtensionlessName(filepath);
		if (filenameContainsString(filepath, "STAGE") || filenameContainsString(filepath, "stage"))
			return true;
	}

	return false;
}

void CLI::processCommands() {
	while (currentArg < 2 && isCommand(argv[currentArg])) {
		setCommand(argv[currentArg]);
		currentArg++;
	}
}

void CLI::setCommand(char* arg) {

	printf("command not recognised\n");
}

void CLI::processArgs() {
	processFile();
}

void CLI::processFile() {
	std::string input = argv[currentArg];
	std::string output = "";
	currentArg++;


	if (currentArg == argc - 1) output = argv[currentArg];

	if (isDar(input)) {
		Dar dar = Dar(input);
		dar.extractAll(output);

		exit(); return;
	}

	if (isStage(input)) {
		Stage stage = Stage(input);
		stage.open();
		stage.extractAll(output);

		exit(); return;
	}

}

bool CLI::checkInput() {
	if (argc > 1 && argc <= 3) return true;
	printUsage();
	return false;
}

void CLI::run(std::string programName, std::string version) {
	printf("Running %s v%s: Visit https://github.com/Jayveer/Rex for updates:\n", programName.c_str(), version.c_str());
	if (!checkInput()) return;
	processArgs();
}

bool CLI::isCommand(char* arg) {
	return arg[0] == 0x2D;
}

void CLI::printUsage() {
	printf(this->USAGE_MESSAGE);
}

void CLI::exit() {
	printf(this->EXIT_MESSAGE);
}