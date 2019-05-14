#ifndef __SAVE_DATA_H__
#define __SAVE_DATA_H__

#include "gf2d_vector.h"
#include "player.h"

/*
* The world is procedurally generated so there's going 
* to be some data we care about 
*/
typedef struct data_s {

	int playerlevel;
	int playerscore;
	Sprite *playerSprite;
	int playerhealth;

}SaveData;

/*
* @brief save certain game data to a file 
* @param  data - pointer to the struct of data we want to save
* @param sprite - pointer to the player's current sprite
* @param plevel - players current level
* @param plevel - players current score
* @param plevel - players current health
*/
void save(SaveData *data,  Sprite *spr, int plevel, int pscore, int phealth);

void read(SaveData *self);

void load(SaveData *self, Entity *player);

#endif