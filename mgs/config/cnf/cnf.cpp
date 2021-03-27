#include "cnf.h"

DataConfig::DataConfig(std::string filename) {
	this->filename = filename;
}

DataConfig::DataConfig(uint8_t* data, int size) {
	this->data = data;
	this->dataSize = size;
}

DataConfig::~DataConfig() {
}

void DataConfig::setWorkDir(std::string& workdir) {
	this->workdir = &workdir;
}

void DataConfig::setHandler(CNFHandler* cnfHandler) {
	this->cnfHandler = cnfHandler;
}

const char* DataConfig::getRegionForID(uint32_t id) {
	for (int i = 0; i < 7; i++) {
		if (cnfRegion[i].id == id)
			return cnfRegion[i].name;
	}
	return "";
}

void DataConfig::readFlag(std::string line) {
	std::stringstream lineStream(line);
	std::vector<std::string> tokens = { std::istream_iterator<std::string>{lineStream}, std::istream_iterator<std::string>{} };
	std::string flagName = tokens[0];

	currentRegion = flagName;
}

std::string DataConfig::getDarName() {
	return currentRegion == "resident" ? "res_mdl" + std::to_string(resDarCount) : "stg_tex" + std::to_string(cacDarCount);
}

void DataConfig::readFile(DataCNFTag tag, uint8_t* data, int size) {
	std::string outputName = tag.id ? intToHexString(tag.id) : getDarName();
	outputName = outputName + "." + getExtForID(tag.extension);

	if (cnfHandler) cnfHandler->processFile(outputName, currentRegion, workdir, data, size);
}

void DataConfig::startSection() {
	section = &data[dataPtr];
}

void DataConfig::endSection(DataCNFTag tag) {
	dataPtr += tag.offset;
	dataPtr += cnfHandler->getNextSectorOffset(dataPtr);
}

void DataConfig::readCache(DataCNFTag tag, DataCNFTag nextTag) {
	if (!tag.offset) startSection();

	int size = nextTag.offset - tag.offset;

	switch (tag.extension) {
	case 0x62:
		readFile(tag, &section[tag.offset], size);
		break;
	case 0x64:
		cacDarCount++;
		readFile(tag, &section[tag.offset], size);
		break;
	case 0xFF:
		endSection(tag);
		break;
	default:
		readFile(tag, &section[tag.offset], size);
	}
}

void DataConfig::readRegion(DataCNFTag tag) {
	switch (tag.extension) {
	case 0x62:
		if (currentRegion == "sound") currentRegion = "stage";
		readFile(tag,  &data[dataPtr], tag.offset);
		break;
	case 0x64:
		if (currentRegion != "resident") cacDarCount++;
		readFile(tag,  &data[dataPtr], tag.offset);
		if (currentRegion == "resident") resDarCount++;
		break;
	default:
		readFile(tag, &data[dataPtr], tag.offset);
	}

	endSection(tag);
}

void DataConfig::readLine(DataCNFTag tag, DataCNFTag nextTag) {
	currentRegion = getRegionForID(tag.region);
	currentRegion == "cache" ? readCache(tag, nextTag) : readRegion(tag);
}

void DataConfig::readLine(std::string line) {
	switch (line.at(0)) {
	case 0x2E:
		readFlag(&line[1]);
		break;
	case 0x3F:
		if (cnfHandler) cnfHandler->processAfp(&line[1], currentRegion, workdir);
		break;
	case 0x40:
		if (cnfHandler) cnfHandler->processQar(&line[1], currentRegion, workdir);
		break;
	default:
		if (cnfHandler) cnfHandler->processFile(line, currentRegion, workdir, NULL, 0);
	}
}

void DataConfig::initBinaryStream() {
	cnf = (DataCNF*)data;
	dataPtr += cnf->cnfSize * cnfHandler->getSectorSize();
}

void DataConfig::initTextStream() {
	std::string cnfTxt = std::string((char*)data, dataSize);
	textStream.str(cnfTxt);
}

void DataConfig::readBinary() {
	initBinaryStream();

	for (int i = 0; cnf->tags[i].region != 0; i++) {
		readLine(cnf->tags[i], cnf->tags[i + 1]);
	}
}

void DataConfig::readASCII() {
	initTextStream();

	std::string line;
	while (std::getline(textStream, line)) {
		readLine(line);
	}
}

void DataConfig::read(CNFTYPE type) {
	type == BINARY ? readBinary() : readASCII();
}