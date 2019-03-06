#ifndef __ENTITY_COMMON_H__
#define __ENTITY_COMMON_H__

#include "gf2d_entity.h"

int entity_camera_view(Entity *self);
int entity_roof_check(Entity *self, float width);
int entity_ground_check(Entity *self, float width);
int entity_left_check(Entity *self, float width);
int entity_right_check(Entity *self, float width);
void entity_world_snap(Entity *self);
void entity_apply_gravity(Entity *self);

/**
 * @brief push other entity away from self by amount
 */
void entity_push(Entity *self,Entity *other,float amount);

/**
 * @brief damage target entity if possible, applying kick as well
 */
void entity_damage(Entity *target,Entity *killer,int damage,float kick);


#endif
