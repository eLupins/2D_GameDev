#include "level_graph.h"

typedef struct {

	LevelNode *startRoom;
	LevelNode *currentRoom;	

}NodeManager;

static NodeManager node_manager = { 0 };

LevelNode* level_node_init(char* lvlArray) {
	
	/* Node keeps track of one room at a time **/
	LevelNode* node = NULL;
	
	if (!lvlArray) {
		slog("ERROR: Cannot create a node from no file");
		return NULL;
	}

	node = (LevelNode*)malloc(sizeof(LevelNode));
	memset(node, 0, sizeof(LevelNode));
	strncpy(node->filename, lvlArray, MAX_FILE_LENGTH);
	return node;
}

void gen_new_node(char* filename) {

	LevelNode *node = NULL;
	int rando = 0;
	int done = 0;


	if (!filename) {
		slog("ERROR: Cannot create a new from a null filename");
		return;
	}

	node = level_node_init(filename);
	
	if (node_manager.currentRoom == NULL) {
		node_manager.currentRoom = node;
		node_manager.startRoom = node;
		return;
	}
	
	while (!done) {

		/*
		*
		*The below block is to check to see if rooms up/down/right/left are taken or not
		*If they're all taken for whatever reason, we'll be stuck in an infinite loop of choosing a random number
		*
		*/
		if (node_manager.currentRoom->up != NULL
			&& node_manager.currentRoom->right != NULL
			&& node_manager.currentRoom->left != NULL
			&& node_manager.currentRoom->down != NULL) 
		{

			return;

		}
		rando = rand() % 4; // random number from 0-3
		//rando = 0; //test variable; makes all rooms spawn only up for testing 

		switch (rando) {

		case 0:
			if (node_manager.currentRoom->up == NULL) {
				node_manager.currentRoom->up = node;
				node->down = node_manager.currentRoom; //set the down node to the current room before changing it to the new node
				node_manager.currentRoom = node;
				done = 1;
			}
			break;

		case 1:

			if (node_manager.currentRoom->right == NULL) {
				node_manager.currentRoom->right = node;
				node->left = node_manager.currentRoom; //set the left  node to the current room before changing it to the new room
				node_manager.currentRoom = node;
				done = 1;
			}
			break;

		case 2:

			if (node_manager.currentRoom->down == NULL) {
				node_manager.currentRoom->down = node;
				node->up = node_manager.currentRoom; //set the up node to the current room before changing it to the new node
				node_manager.currentRoom = node;
				done = 1;
			}
			break;

		default:
			if (node_manager.currentRoom->left == NULL) {
				node_manager.currentRoom->left = node;
				node->right = node_manager.currentRoom; //set the right node to the current room before changing it to the new node
				node_manager.currentRoom = node;
				done = 1;
			}
			break;
		}
	}
}

void create_level(char* masterList) {

	int maxRooms = 6;
	int minRooms = 2;
	int i = 0;
	char liveRooms[10][MAX_FILE_LENGTH];
	int roomTypes = 0;
	FILE* file = NULL;
	file = fopen(masterList, "r");
	char buffer[MAX_FILE_LENGTH] = "";

	if (!file) {

		slog("ERROR: Cannot read an invalid file");
		return;

	}

	while (fscanf(file, "%s\n", buffer) != EOF) {

		//liveRooms[i] = (char*)malloc(sizeof(char) * 512);

		memset(liveRooms[i], 0, sizeof(char) * MAX_FILE_LENGTH);
		strncpy(liveRooms[i], buffer, MAX_FILE_LENGTH);
		slog("Line %i = %s ", i, liveRooms[i]);
		i++;
		roomTypes++;
		
	}

	for (i = 0; i < maxRooms; i++) {

		if (i >= minRooms) {

			//40% chance that rooms will stop generating
			if (rand() % 10 <= 3) {

				break;
			}
		}

		gen_new_node(liveRooms[rand() % roomTypes]);

	}
}

char* begin_level() {

	char filename[MAX_FILE_LENGTH] = "";
	node_manager.currentRoom = node_manager.startRoom;
	strncpy(filename, node_manager.startRoom->filename, MAX_FILE_LENGTH);
	return filename;

}


void door_check(Entity *player) {

	Uint32 height = gf2d_graphics_get_rendheight();
	Uint32 width = gf2d_graphics_get_rendwidth();
	char jsonFileName[MAX_FILE_LENGTH] = "";
	LevelInfo *linfo;

	if (player->position.x <= 0) {
	 // player went left
		slog("player attempting to move left");
		if (node_manager.currentRoom->left != NULL) {
			//delete current level
			level_clear();
			gf2d_entity_free_all_except(player);
			//load new level
			strncpy(jsonFileName, node_manager.currentRoom->left->filename, MAX_FILE_LENGTH);
			linfo = level_info_load(jsonFileName);
			node_manager.currentRoom = node_manager.currentRoom->left;
			level_init(linfo, 1);
			//move player
			player->position.x = width * .75f;
			player->position.y = height / 2.0f;
		}
	}

	else if (player->position.x >= width) {
		//player went right
		slog("player attempting to move right");
		if (node_manager.currentRoom->right != NULL) {
			//delete current level
			level_clear();
			gf2d_entity_free_all_except(player);
			//load new level
			strncpy(jsonFileName, node_manager.currentRoom->right->filename, MAX_FILE_LENGTH);
			linfo = level_info_load(jsonFileName);
			node_manager.currentRoom = node_manager.currentRoom->right;
			level_init(linfo, 1);
			//move player
			player->position.x = width * .25f;
			player->position.y = height / 2.0f;
		}
	}

	else if (player->position.y <= 0) {
		//player went up
		slog("player attempting to move up");
		if (node_manager.currentRoom->up != NULL) {
			//delete current level
			level_clear();
			gf2d_entity_free_all_except(player);
			//load new level
			strncpy(jsonFileName, node_manager.currentRoom->up->filename, MAX_FILE_LENGTH);
			linfo = level_info_load(jsonFileName);
			node_manager.currentRoom = node_manager.currentRoom->up;
			level_init(linfo, 1);		
			//move player
			player->position.y = height * .75f;
			player->position.x = width / 2.0f;
		}
	}

	else if (player->position.y >= height) {
		//player went down
		slog("player attempting to move down");
		if (node_manager.currentRoom->down != NULL) {
			//delete current level
			level_clear();
			gf2d_entity_free_all_except(player);
			//load new level
			strncpy(jsonFileName, node_manager.currentRoom->down->filename, MAX_FILE_LENGTH);
			linfo = level_info_load(jsonFileName);
			node_manager.currentRoom = node_manager.currentRoom->down;
			level_init(linfo, 1);
			//move player
			player->position.y = height * .25f;
			player->position.x = width / 2.0f;
		}
	}
}