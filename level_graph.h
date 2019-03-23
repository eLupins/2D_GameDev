#include "simple_logger.h"
#include "simple_json.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct levelNode {

	char filename[512];
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

void gen_new_node(char* fileName);