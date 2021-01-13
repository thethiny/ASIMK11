#include "..\pch.h"
#include "mk11.h"

MK11::CamStruct sCamStruct;
MK11::IntroStruct sIntroStruct, sIntroStruct2;
MK11::ActiveMods sActiveMods;

const char* MK11::szCharacters[] = {
		"SUBZERO",
		"SCORPION",
		"LIUKANG",
		"CASSIECAGE",
		"JOHNNYCAGE"
};
//char MK11::szCharacterSelected[100] = "Select Character";

void MK11Hooks::IntroSwap(char* dest, char* source, uint64_t length)
{
	char* p1_string;
	char* p1_string_replace;
	bool found = false;
	if (sIntroStruct.bEnabled && sIntroStruct.PName[0] && sIntroStruct.PChar[0] && sIntroStruct.PName2[0] && sIntroStruct.PChar2[0]) // Populated
	{
		p1_string = new char[strlen(sIntroStruct.PName) + 25 + 1]; // 1 Null Terminator
		sprintf(p1_string, "%s_%c_CHARINTRO_SCRIPTASSETS", sIntroStruct.PName, sIntroStruct.PChar[0]);

		if (!strcmp(p1_string, source)) // Found the string I want to replace
		{
			p1_string_replace = new char[strlen(sIntroStruct.PName2) + 25 + 1]; // 1 Null Terminator
			sprintf(p1_string_replace, "%s_%c_CHARINTRO_SCRIPTASSETS", sIntroStruct.PName2, sIntroStruct.PChar2[0]);
			std::cout << "IntroSwap::Swapping " << source << " <-> " << p1_string_replace << std::endl;
			source = p1_string_replace;
			found = true;
		}
	}

	char* p2_string;
	char* p2_string_replace;
	if (!found && sIntroStruct2.bEnabled && sIntroStruct2.PName[0] && sIntroStruct2.PChar[0] && sIntroStruct2.PName2[0] && sIntroStruct2.PChar2[0]) // Populated
	{
		p2_string = new char[strlen(sIntroStruct2.PName) + 25 + 1]; // 1 Null Terminator
		sprintf(p2_string, "%s_%c_CHARINTRO_SCRIPTASSETS", sIntroStruct2.PName, sIntroStruct2.PChar[0]);

		if (!strcmp(p2_string, source)) // Found the string I want to replace
		{
			p2_string_replace = new char[strlen(sIntroStruct2.PName2) + 25 + 1]; // 1 Null Terminator
			sprintf(p2_string_replace, "%s_%c_CHARINTRO_SCRIPTASSETS", sIntroStruct2.PName2, sIntroStruct2.PChar2[0]);
			std::cout << "IntroSwap::Swapping " << source << " <-> " << p2_string_replace << std::endl;
			source = p2_string_replace;
		}
	}

	length = strlen(source) +1; // Auto Get Length + Null Terminator
	memcpy(dest, source, length);
	//std::cout << "IntroSwap::Loaded " << source << std::endl;
}