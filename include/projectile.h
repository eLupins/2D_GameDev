#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "gf2d_entity.h"
#include "simple_json.h"

/**
* @brief create a new projectile entity
* @param position - provide a position in world space 
*/
Entity *projectile_new(Vector2D position);

/**
* @brief spawns a projectile into the world 
* @param position - provide a position in world space
* @param args -provide a json file to load the projectile data from
*/
void projectile_spawn(Vector2D position, SJson *args);

/**
* @brief gets and returns the existing projectile 
*/
Entity *get_projectile();

/**
* @brief free the projectile 
* @param self - provide the projectile entity to free 
*/
void projectile_free(Entity *self);

/**
* @brief hacky way to have a particle system run when the projectile dies
*/
void particle_sys_start();

#endif