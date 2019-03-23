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
	memset(&node, 0, sizeof(LevelNode));
	strncpy(node->filename, lvlArray, 512);
	return node;
}

void gen_new_node(char* filename) {

	LevelNode *node = NULL;
	int rando = rand() % 4; // random number from 0-3

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

	switch (rando){

	case 0:
		if (node_manager.currentRoom->up != NULL) {
			node_manager.currentRoom->up = node;
			node->down = node_manager.currentRoom; //set the down node to the current room before changing it to the new node
			node_manager.currentRoom = node;
		}
		break;

	case 1: 
		
		if (node_manager.currentRoom->right != NULL) {
			node_manager.currentRoom->right = node;
			node->left= node_manager.currentRoom; //set the down node to the current room before changing it to the new node
			node_manager.currentRoom = node;
		}
		break;

	case 2:

		if (node_manager.currentRoom->down != NULL) {
			node_manager.currentRoom->down = node;
			node->up = node_manager.currentRoom; //set the down node to the current room before changing it to the new node
			node_manager.currentRoom = node;
		}
		break;

	default:
		if (node_manager.currentRoom->left != NULL) {
			node_manager.currentRoom->left = node;
			node->right = node_manager.currentRoom; //set the down node to the current room before changing it to the new node
			node_manager.currentRoom = node;
		}
		break;
	}

}