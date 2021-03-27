#pragma once
#include "../../common/util.h"
#include "../../common/ext_table.h"

#include <vector>

struct DataCNFTag {
	uint16_t id;
	uint8_t region;
	uint8_t extension;
	uint32_t offset;
};

struct DataCNF {
	uint16_t cnfSize;
	uint16_t dataSize;
	DataCNFTag tags[];
};

struct CNFREGION {
	const char* name;
	int id;
};

enum CNFTYPE {
	ASCII,
	BINARY
};

class CNFHandler {
public:
	virtual int32_t getSectorSize() = 0;
	virtual int64_t getNextSectorOffset(int64_t currentOffset) = 0;
	virtual void processQar(std::string filename, std::string region, std::string* workDir) = 0;
	virtual void processAfp(std::string filename, std::string region, std::string* workDir) = 0;
	virtual void processVram(std::string filename, std::string region, std::string* workDir) = 0;
	virtual void processEnc(std::string region, uint8_t** section, int64_t size, int64_t sectionSize) = 0;
	virtual void processFile(std::string filename, std::string region, std::string* workDir, uint8_t* fileData, int size) = 0;
};

class DataConfig {
public:
	DataConfig(std::string filename);
	DataConfig(uint8_t* data, int size);

	~DataConfig();
	void read(CNFTYPE type = ASCII);
	void setWorkDir(std::string& workdir);
	void setHandler(CNFHandler* cnfHandler);
	const char* getRegionForID(uint32_t id);
private:
	int dataSize = 0;
	DataCNF* cnf = {};
	uint8_t* data = {};
	int resDarCount = 0;
	int cacDarCount = 0;
	std::string filename;
	uint8_t* section = {};
	uint64_t dataPtr = 0;
	std::string* workdir = {};
	CNFHandler* cnfHandler = {};
	std::string currentRegion;
	std::stringstream textStream;

	void readASCII();
	void readBinary();
	void initTextStream();
	void initBinaryStream();
	void readLine(std::string line);
	void readFlag(std::string line);
	void readLine(DataCNFTag tag, DataCNFTag nextTag);	

	std::string getDarName();
	void readRegion(DataCNFTag tag);
	void readCache(DataCNFTag tag, DataCNFTag nextTag);
	void readDar(DataCNFTag tag, uint8_t* data, int size);
	void readBin(DataCNFTag tag, uint8_t* data, int size);
	void readFile(DataCNFTag tag, uint8_t* data, int size);

	void startSection();
	void endSection(DataCNFTag tag);

	const CNFREGION cnfRegion[7] = {
		{ "end",		 0x00 },
		{ "nocache",	 0x6E },
		{ "cache",		 0x63 },
		{ "resident",	 0x72 },
		{ "sound",		 0x73 }
	};
};