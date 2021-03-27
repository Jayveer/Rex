#include "stage.h"

Stage::Stage(std::string filename) {
	this->filename = filename;
}

Stage::Stage(std::string filename, uint32_t sector) {
	this->sector = sector;
	this->filename = filename;
}

Stage::~Stage() {}

int64_t Stage::getNextPageOffset(uint16_t pageID) {
	int numStages = tableSize / 12;
	return (pageID == numStages - 1) ? getFileSize(filename) : table[pageID + 1].offset * sector;
}

void Stage::open() {
	std::ifstream stageDir;
	stageDir.open(filename, std::ios::binary);
	stageDir.read((char*)&this->tableSize, 4);

	int numStages = tableSize / 12;

	table.resize(numStages);
	stageDir.read((char*)&this->table[0], tableSize);
}

void Stage::extractFiles(int64_t size, int64_t offset, std::string& output) {
	std::ifstream fileDat;
	fileDat.open(filename, std::ios::binary);
	fileDat.seekg(offset);

	uint8_t* pageData = new uint8_t[size];
	fileDat.read((char*)pageData, size);
	fileDat.close();

	DataConfig cnf = DataConfig(pageData, size);
	cnf.setHandler(this);
	cnf.setWorkDir(output);
	cnf.read(BINARY);

	delete[] pageData;
}

int getStrLength(char* str) {
	for (int i = 0; i < 8; i++) {
		if (str[i] == '\0')
			return i;
	}

	return 9;
}

void Stage::extract(uint16_t pageID, std::string output) {
	int numStages = tableSize / 12;
	if (pageID > numStages) return;

	int strlen = getStrLength(table[pageID].name);

	std::string stageName;
	stageName.reserve(strlen);
	stageName.assign(table[pageID].name, strlen);

	if (strlen == 9) stageName[8] = '\0';

	updateDir("stage", output);
	updateDir(stageName, output);
	int64_t nextOffset = getNextPageOffset(pageID);
	int64_t offset = table[pageID].offset * sector;
	int64_t size = nextOffset - offset;
	extractFiles(size, offset, output);
}

void Stage::extractAll(std::string output) {
	int numStages = tableSize / 12;

	for (int i = 0; i < numStages; i++) {
		extract(i, output);
	}
}

////

int32_t Stage::getSectorSize() { 
	return this->sector;  
};

int64_t Stage::getNextSectorOffset(int64_t currentOffset) {
	return getAlignment(currentOffset, sector);
};

void Stage::processQar(std::string filename, std::string region, std::string* workDir) {};
void Stage::processAfp(std::string filename, std::string region, std::string* workDir) {};
void Stage::processVram(std::string filename, std::string region, std::string* workDir) {};
void Stage::processEnc(std::string region, uint8_t** section, int64_t size, int64_t sectionSize) {};

void Stage::processFile(std::string filename, std::string region, std::string* workDir, uint8_t* fileData, int size) {
	writeDataToFile(fileData, size, filename, *workDir);
};