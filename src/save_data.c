#include "save_data.h"
#include "gf2d_input.h"
#include "simple_logger.h"

FILE *myFile = NULL;

void save(SaveData *data, Sprite *spr, int plevel, int pscore, int phealth) {

	//error checking 
	if (!data) {
		slog("ERROR: Cannot create a save file without data!");
		return;
	}

	//save information to data struct
	data->playerSprite = spr;
	data->playerlevel = plevel;
	data->playerscore = pscore;
	data->playerhealth = phealth;

	//write the data to my file
	myFile = fopen("save/save.wyb", "wb");
	fwrite(&(*data), sizeof(SaveData), 1, myFile);
	fclose(myFile);
	slog("SAVE FILE GENEREATED.");

}

void read(SaveData *data) {

	//error checking 
	if (!data) {
		slog("ERROR: Cannot read a file with no data!");
		return;
	}

	if (!myFile == NULL) {
		slog("ERROR: Save file is NULL");
		return;
	}

	//open & read
	myFile = fopen("save/save.wyb", "rb");
	fread(&(*data), sizeof(SaveData), 1, myFile);
	fclose(myFile);

}

void load(SaveData *data, Entity *player) {

	//error checking
	if (!data) {
		slog("ERROR: Cannot load save from no data!");
		return;
	}

	if (!player) {
		slog("ERROR: Player data required to load save!");
		return;
	}

	//open & read 
	myFile = fopen("save/save.wyb", "rb");
	fread(&(*data), sizeof(SaveData), 1, myFile);
	fclose(myFile);

	//load that good stuff s
	player->actor.sprite = data->playerSprite;
	player->health = data->playerhealth;
	player->score = data->playerscore;
	player->playerlvl = data->playerlevel;

}
