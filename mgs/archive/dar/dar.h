#pragma once
#include <fstream>
#include <filesystem>
#include "../../common/util.h"
#include "../../common/fileutil.h"
#include "../../common/ext_table.h"

struct DarEntry {
	uint16_t strcode;
	uint16_t extension;
	uint32_t size;
	uint8_t data[];
};

class Dar {
public:
	Dar(std::string filename);
	~Dar();

	void extractAll(std::string& output);
	uint8_t* findFile(uint16_t id, uint16_t ext, int& size);
private:
	uint8_t* darData;
	int dataSize;
};