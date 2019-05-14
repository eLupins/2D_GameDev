#ifndef __BAT_H__
#define __BAT_H__

#include "gf2d_entity.h"
#include "simple_json.h"

Entity *bat_new(Vector2D position);

Entity *get_bat();

void bat_free(Entity *self);

void bat_spawn(Vector2D position,char *args);



#endif