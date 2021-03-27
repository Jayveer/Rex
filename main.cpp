#include "interface/cli.h"

int main(int argc, char** argv) {
	CLI cli = CLI(argc, argv);
	cli.run("Rex", "1.0");
}