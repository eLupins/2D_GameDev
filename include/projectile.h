#ifndef __PROJECTILE_H__
#define __PROJECTILE_H__

#include "gf2d_entity.h"
#include "simple_json.h"

Entity *projectile_new(Vector2D position);

void projectile_spawn(Vector2D position, SJson *args);

Entity *get_projectile();

void projectile_free(Entity *self);

void particle_sys_start();

#endif