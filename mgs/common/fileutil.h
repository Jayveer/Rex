#pragma once
#include <fstream>
#include <filesystem>

inline
void updateDir(const std::string& path, std::string& output) {
	std::filesystem::path p{ output };
	p.append(path);
	output = p.u8string();
}

inline
void resetDir(std::string& output) {
	std::filesystem::path p{ output };
	output = p.parent_path().u8string();
}

inline
std::string getCurrentDir(const std::string& output) {
	std::filesystem::path p{ output };
	return p.filename().u8string();
}

inline
std::string getExtension(const std::string& output) {
	std::filesystem::path p{ output };
	return p.extension().u8string();
}

inline
std::string getExtensionlessName(const std::string& output) {
	std::filesystem::path p{ output };
	return p.stem().u8string();
}

inline
bool filenameContainsString(const std::string& output, const std::string& string) {
	std::string filename = getExtensionlessName(output);

	if (filename.find(string) != std::string::npos)
		return true;

	return false;
}

inline
bool isDirectory(const std::string& output) {
	std::filesystem::path p{ output };
	return std::filesystem::is_directory(p);
}

inline
bool fileExists(const std::string& output) {
	std::filesystem::path p{ output };
	return std::filesystem::exists(p);
}

inline
int64_t getFileSize(const std::string& input) {
	return std::filesystem::file_size(input);
}

inline
int64_t getAlignment(int64_t currentOffset, int64_t alignSize) {
	uint64_t step = (alignSize - (currentOffset % alignSize));
	if (step != alignSize)
		return step;
	return 0;
}

inline
void writeDataToFile(uint8_t* data, int size, const std::string& filename, std::string& output) {
	if (!std::filesystem::exists(output))
		std::filesystem::create_directories(output);

	updateDir(filename, output);
	std::ofstream ofs(output, std::ofstream::binary);
	ofs.write((char*)data, size);
	ofs.close();
	resetDir(output);
}