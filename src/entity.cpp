

#include "entity.h"
#include "simple_logger.h"
#include <stdlib.h>
#include <iostream>

typedef struct {
	uint32_t maxEntities;
	Entity *entityList;
	uint64_t autoincrement;
}dc_EntityManager;

static dc_EntityManager entityManager = { 0, nullptr, 0 };



void entity_system_init(uint32_t maxEntities) {

	if (maxEntities <= 0) {
		throw std::runtime_error("ERROR: Cannot initialize manager for zero or negative entities");
	}

	memset(&entityManager, 0, sizeof(entityManager)); /**Setting memory for the entity manager */
	entityManager.entityList = (Entity*)(malloc(sizeof(Entity) * maxEntities)); /**Malloc memory for entityList */

	if (!entityManager.entityList) {
		throw std::runtime_error("ERROR: Unable to allocate memory for entity list");
	}

	memset(entityManager.entityList, 0, sizeof(Entity) * maxEntities);
	entityManager.maxEntities = maxEntities;
	
	std::printf("SUCCESSFUL: Initialized entity system\n");
	atexit(entity_system_close); /**initialize system_close at the end of the program */
}



/**
*
*@brief Closes the entity system
*/
void entity_system_close() {

	int i = 0;
	if (entityManager.entityList != NULL) {
		entity_free(&entityManager.maxEntities; i++) {
			entity_free(&entityManager.entityList[i]);
		}
		free(entityManager.entityList);
	}
	memset(&entityManager, 0, sizeof(entityManager));
	std::printf("ENTITY SYSTEM CLOSED.");

}

