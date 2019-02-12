#ifndef __ENTITY_H__
#define __ENTITY_H__


#include "gf2d_text.h"
#include "gf2d_vector.h"
#include "gf2d_actor.h"
#include "gf2d_audio.h"
#include "gf2d_particles.h"
#include "gf2d_collision.h"
#include "gf2d_sprite.h"


/**defining possible entity states*/
typedef enum {

	es_Idle,
	es_Moving,
	es_Pain,
	es_Dead,
	es_Attack,

}EntityState;


typedef struct entity_s {

	Uint8 inUse;
	Uint64 id;

	TextLine name; /**<name of the entity*/
	EntityState state; /**name of the entity*/
	Shape shape;  /**shape of the physics collider*/
	Body body;  /**shape of the physics body for the entity*/

	Vector2D position;  /**position of entity*/
	Vector2D velocity;  /**movement direction*/
	Vector2D acceleration; 
	
	Actor actor;  /**information for animated sprite*/
	Vector2D scale;  /**for scaling the sprite*/
	Vector2D scaleCenter; /**where to scale the sprite from*/
	Vector3D rotation;  /**for rotating the entity*/
	Vector2D flip;  /**to flip the sprite */

	int health;  /**entity health*/


	
	void(*think)(struct entity_S *self);   /**<called before system updates to make decisions / hand input*/
	void(*update)(struct entity_S *self);  /**<called after system entity update*/
	int(*touch)(struct entity_S *self, struct entity_S *other);/**<when this entity touches another entity*/
	void(*draw)(struct entity_s * self);  /**for drawing the entity*/
	void(*die)(struct entity_s * self);  /**for handling entity "death" */
	void(*free)(struct entity_s * self);  /**freeing entity from memory */
	int(*damage)(struct entity_s *self); /**For when the entity takes damage; also stop writing this as void  */

}Entity;

/**
* @brief Initializes the Entity Manager
* @param maxEntities The maximum number of entities that can exist at once
*/
void entity_system_init(uint32_t maxEntities);

/**
* @brief Finds a place in the entity manager thats not in use and returns its address
* @returns An address in memory for the new entity; will return NULL if failed
*/
Entity *entity_new();

/**
* @brief Frees an entity from the entity manager
* @param e is the entity to free 
*/
void entity_free(Entity *e);

/**
* @brief Frees all entities from the entity mananger without closing the system
*/
void entity_free_all();

/**
* @brief Draws the specified entity
* @param *self: the entity to draw
*/
void entity_draw(Entity *self);

/**
* @brief Draws all active entities in the system
*/
void entity_draw_all();

/**
* @brief call all the active entities' think functions 
*/
void entity_think_all();

/**
* @brief update all active entities 
*/
void entity_update_all();

/**
* @brief call before cal lto collision space update to prep all bodies with their entities
*/
void entity_presync_all();

/**
* @brief call after call to collision space update to get all entities in sync with what happened 
*/
void entity_postsync_all();

/**
* @brief To do damage to targeted entity
* @param target: the entity that will take damage
* @param inflictor: the entity doing the damage
* @param attacker: the entity getting credit for the damage
* @param damage: how much damage is the attack doing?
* @param kick: kick damage direction
* @return true if damage was dealt, return false otherwise
*/
int entity_do_damage(Entity *target, Entity *inflictor, Entity *attacker, int damage, Vector2D kick);

#endif