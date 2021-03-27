#pragma once

struct EXT_TABLE {
	const char* name;
	uint8_t id;
};

const EXT_TABLE ext_table[16] = {
	{"bin", 0x62},
	{"con", 0x63},
	{"dar", 0x64},
	{"efx", 0x65},
	{"gcx", 0x67},
	{"hzm", 0x68},
	{"img", 0x69},
	{"kmd", 0x6B},
	{"lit", 0x6C},
	{"mdx", 0x6D},
	{"oar", 0x6F},
	{"pcx", 0x70},
	{"rar", 0x72},
	{"sgt", 0x73},
	{"wvx", 0x77},
	{"zmd", 0x7A},
};	

inline
const char* getExtForID(uint8_t id) {
	for (int i = 0; i < 16; i++) {
		if (ext_table[i].id == id)
			return ext_table[i].name;
	}
	return "";
}

inline
uint8_t getIDforExt(const char* ext) {
	for (int i = 0; i < 16; i++) {
		if (!strcmp(ext_table[i].name, ext))
			return ext_table[i].id;
	}
	return -1;
}