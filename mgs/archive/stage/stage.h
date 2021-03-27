#pragma once
#include "../../config/cnf/cnf.h"
#include "../../common/fileutil.h"

struct StageTable {
	char name[8];
	uint32_t offset;
};

class Stage : public CNFHandler {
public:
	Stage(std::string filename);
	Stage(std::string filename, uint32_t sector);

	~Stage();

	void open();
	void extractAll(std::string output = "");
	void extract(uint16_t pageID, std::string output = "");
private:
	int sector = 0x800;
	std::string filename;

	int32_t tableSize;
	std::vector<StageTable> table;

	int64_t getNextPageOffset(uint16_t pageID);
	void extractFiles(int64_t size, int64_t offset, std::string& output);

	int32_t getSectorSize();
	int64_t getNextSectorOffset(int64_t currentOffset);
	void processQar(std::string filename, std::string region, std::string* workDir);
	void processAfp(std::string filename, std::string region, std::string* workDir);
	void processVram(std::string filename, std::string region, std::string* workDir);
	void processEnc(std::string region, uint8_t** section, int64_t size, int64_t sectionSize);
	void processFile(std::string filename, std::string region, std::string* workDir, uint8_t* fileData, int size);
};