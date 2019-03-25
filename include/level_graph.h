#include "simple_logger.h"
#include "simple_json.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gf2d_entity.h"
#include "gf2d_graphics.h"
#include "level.h"
#define MAX_FILE_LENGTH 512

typedef struct levelNode {

	char filename[MAX_FILE_LENGTH];
	struct levelNode* up;
	struct levelNode* down;
	struct levelNode* right;
	struct levelNode* left;

}LevelNode;



/*
*@brief initialize a single level node
*@param lvlArray - an array of json level files
*/
LevelNode* level_node_init(char* lvlArray); 

/*
*
*@brief Initialize a new node for a new room
*@param filepath - pass on a file that contains room information
*
*/
void gen_new_node(char* fileName);

/*
*
*@pbrief Generate map of the world
*
*/
void create_level(char* masterList);

char* begin_level();

/*
*
*@brief check to see which door the player is entering
*@param player entity
*/
void door_check(Entity *player);