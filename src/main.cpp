#include <iostream>
#include <filesystem>
#include <fstream>

#include "ValveBsp.h"
#include "StreamUtils.h"

void hideLumpFromEngine(BSPHeader_t &header, uint8_t headerIndex) {
	printf("Set size in header of lump %u to 0. It was %i bytes.\n", headerIndex, header.lumps[headerIndex].filelen);
	header.lumps[headerIndex].filelen = 0;
}

bool isSprpId(const char *id) {
	// Should be 'sprp', but I'm reading it in backwards. This won't get confusing at all...
	return id[0] == 'p' && id[1] == 'r' && id[2] == 'p' && id[3] == 's';
}

int downgradeBsp(std::fstream &file) {
	// Header is at the very start of the file.
	BSPHeader_t header = readType<BSPHeader_t>(file);

	// LUMP_GAME_LUMP.
	GameLump_t sprpGameLump{};
	std::streampos posSprpGameLump{};
	{
		Lump_t &gameLumpHeader = header.lumps[HeaderLumpIndex::LUMP_GAME_LUMP];
		file.seekg(gameLumpHeader.fileofs);

		// In theory there could be any number of game lumps (though really, we know there's just 2...).
		int gameLumpCount = readType<int>(file);
		// TODO: Using a flag is stupid!
		bool bFound = false;
		for (int i = 0; i < gameLumpCount; ++i) {
			posSprpGameLump = file.tellg();
			sprpGameLump = readType<GameLump_t>(file);
			if (isSprpId(sprpGameLump.id)) {
				bFound = true;
				break;
			}
		}

		// Check that we got our lump.
		if (!bFound) {
			fprintf(stderr, "Couldn't locate SPRP lump!\n");
			return 1;
		}

		// Debug.
		printf("Located SPRP lump. (Offset: %u) (Size: %u)\n", sprpGameLump.fileofs, sprpGameLump.filelen);

		// We want the CSGO prop lumps!
		if (sprpGameLump.version == 9) {
			printf("BSP is already in SPRP v9 format. Likely, no processing is required.\n");
			return 0;
		}
		if (sprpGameLump.version != 11) {
			fprintf(stderr, "BSP has static prop lump v%i. We want v11, from CS:GO.\n", sprpGameLump.version);
			return 1;
		}
	}

	// https://developer.valvesoftware.com/wiki/Source_BSP_File_Format#Static_props
	file.seekg(sprpGameLump.fileofs);
	// We need to skip past some data here; we don't know the offsets upfront.
	// StaticPropDictLump_t.
	{
		typedef char PropName_t[128];

		int dictEntries = readType<int>(file);
		seekTypeArr<PropName_t>(file, dictEntries);
	}
	// StaticPropLeafLump_t.
	{
		typedef unsigned short LeafEntry_t;

		int leafEntries = readType<int>(file);
		seekTypeArr<LeafEntry_t>(file, leafEntries);
	}
	// StaticPropLump_t entries.
	{
		int numProps = readType<int>(file);

		// Cache entries. Reading all at once is more efficient!
		std::streampos posArrStart = file.tellg();
		std::vector<StaticPropLump11_t> entryBuffer = readTypeArr<StaticPropLump11_t>(file, numProps);

		// Gummy time...!
		file.seekp(posArrStart);
		for (const StaticPropLump11_t &entry: entryBuffer) {
			writeBytes(file, &entry, sizeof(StaticPropLump9_t));
		}

		// Tell the engine we're now in the v9 format.
		sprpGameLump.version = 9;
		// Static prop list is at the end of this game lump. We can get away with just chopping the difference.
		sprpGameLump.filelen -= numProps * int(sizeof(StaticPropLump11_t) - sizeof(StaticPropLump9_t));
		file.seekp(posSprpGameLump);
		writeType<GameLump_t>(file, sprpGameLump);
	}

	// These lumps are actually different in CS:GO... but it's not documented on the VDC!
	// Building cubemaps crashes w/ "ValidateLump: odd size of lump [22/23]".
	// I think that these lumps are unused, though; physics works just fine, and there are no crashes during gameplay!
	// Potentially, the engine does a cursory check over these lumps when writing to a BSP, as it needs to clone/move them around/between files.
	// An empty lump is always valid, though. So we lie and say the lump is empty without removing its data.
	// It's, at most, a few hundred bytes. Who cares.
	hideLumpFromEngine(header, HeaderLumpIndex::LUMP_PROPCOLLISION);
	hideLumpFromEngine(header, HeaderLumpIndex::LUMP_PROPHULLS);

	file.seekp(0);
	writeType<BSPHeader_t>(file, header);

	printf("Successfully downgraded BSP!\n");
	return 0;
}

int main(int argc, char **argv) {
	// TODO: Proper CLI interface.
	if (argc != 2) {
		printf("This tool takes 1 argument: the path of the BSP to modify.\n");
		return 0;
	}

	const char *szTargetBsp = argv[1];

	if (!std::filesystem::exists(szTargetBsp)) {
		fprintf(stderr, "Input BSP '%s' doesn't exist!\n", szTargetBsp);
		return 1;
	}

	// TODO: Handle unable to open file (without exceptions...).
	std::fstream file = openFile(szTargetBsp);
	return downgradeBsp(file);
}
