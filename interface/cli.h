#pragma once
#include "../mgs/archive/dar/dar.h"
#include "../mgs/archive/stage/stage.h"

class CLI {
public:
	CLI(int argc, char** argv);
	~CLI();

	void run(std::string programName, std::string version);
	void exit();
private:
	int argc;
	char** argv;
	int currentArg = 1;

	void printUsage();
	bool checkInput();
	void processArgs();
	void processFile();
	void processCommands();
	bool isCommand(char* arg);
	void setCommand(char* arg);

	const char* EXIT_MESSAGE = "Exiting\n";
	const char* USAGE_MESSAGE = "Usage:\t Rex.exe <STAGE.DIR> [OUTPUTDIRECTORY] \n";
};