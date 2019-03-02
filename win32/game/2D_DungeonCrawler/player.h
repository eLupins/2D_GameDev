#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "entity.h"
#include "simple_json.h"

/**
*
* @brief Spawn a new player entity
* @param position place the player 
* @return a pointer to the player entity
*
**/
Entity *player_new(Vector2D position);

/**
*
* @brief Get a pointer to the player entity
* @return a pointer to hte player entity
*
**/
Entity *player_get();

/**
*
* @brief sets the player position to the one specified
* @note risk of solid collision
* @param position the new position for the player 
*
**/
void player_set_position(Vector2D position);

Entity *player_spawn(Vector2D position, SJson *args);


#endif