#include "entity.h"
#include "simple_logger.h"


/**Creating the Entity Manager struct*/
typedef struct {

	Uint32 maxEntities;
	Entity *entityList;
	Uint64 increment;

}EntityManager;

static EntityManager entityManager = { 0 };

/**Closing the entity system*/
void entity_system_close() {

	int i = 0;

	if (entityManager.entityList != NULL) {
		for (i = 0; i < entityManager.maxEntities; i++) {
			entity_free(&entityManager.entityList[i]);
		}
		free(entityManager.entityList);
	}
	memset(&entityManager, 0, sizeof(entityManager));
	slog("Entity system closed");
}

/**Initializing the entity system*/
void entity_system_init(Uint32 maxEntities) {

	if (maxEntities <= 0) {
		slog("ERROR: Cannot initialize entity manager for zero or negative entities");
	}

	memset(&entityManager, 0, sizeof(entityManager)); /**Initialize memory for the entitymanager*/
	entityManager.entityList = (Entity*)(malloc(sizeof(Entity) * maxEntities)); /**Make memory for the entitylist*/

	/**If there's no entityList to allocate memory for...*/
	if (!entityManager.entityList) {
		slog("ERROR: Unable to allocate memory for entity list");
	}

	memset(entityManager.entityList, 0, sizeof(Entity) * maxEntities); /**Initialize memory for the entitylist*/
	entityManager.maxEntities = maxEntities; 

	slog("SUCCESSFUL: Initialized entity system");
	atexit(entity_system_close); /**Initiate system_close at exit*/

}


Entity *entity_new() {

	int i = 0;
	for (i = 0; i < entityManager.maxEntities; i++) {
		/**Make sure there's room in the entity manager to create a new entity*/
		if (entityManager.entityList[i].inUse == 0) {

			memset(&entityManager.entityList[i], 0, sizeof(Entity)); /**Initialize memory for the entity*/
			entityManager.entityList[i].id = entityManager.increment++; /**Set the new entity's id to the increment and then increase the increment for the next entity*/
			entityManager.entityList[i].inUse = 1; /**set the entity's inuse flag to true*/
			vector2d_set(entityManager.entityList[i].scale, 1, 1); /**Make sure your entity's scale isn't 0,0*/
			entityManager.entityList[i].actor.color = vector4d(1, 1, 1, 1); /**Just an opaque white; basically just no real colour */
			return &entityManager.entityList[i];
		}
	}

}

void entity_free(Entity *e) {

	/**If the input just doesn't exist*/
	if (!e) {
		slog("ERROR: Cannot free an entity that is null");
		return;
	}

	if (e->free) {
		e->free(e);
	}

	/**Free up the memory*/
	memset(e, 0, sizeof(Entity));

}

void entity_free_all() {

	for (int i = 0; i < entityManager.maxEntities; i++) {
		entity_free(&entityManager.entityList[i]);
	}
}

void entity_draw(Entity *self) {

	if (!self) {
		slog("ERROR: Cannot draw a null entity!");
		return;
	}

	if (!self->inUse) {
		slog("ERROR: Cannot draw an entity that is not in use!");
		return;
	}

	gf2d_sprite_draw(
		self->actor.sprite,
		self->position,
		&self->scale,
		&self->scaleCenter,
		&self->rotation,
		&self->flip,
		&self->actor.color,
		(Uint32)self->actor.frame);

	if (self->draw != NULL) {
		self->draw(self);
	}
}

void entity_draw_all() {

	int i;

	for (i = 0; i <= entityManager.maxEntities; i++) {
		entity_draw(&entityManager.entityList[i]);
	}
}

void entity_presync_all() {
	/** Ask DJ what this stuff is about */
}


void entity_postsync_all() {
	/** Ask DJ what this stuff is about */
}

void entity_think_all() {

	int i = 0;
	for (i = 0; i < entityManager.maxEntities; i++) {
		if (entityManager.entityList[i].inUse == 0)
			continue;
		if (entityManager.entityList->inUse == 1) {
			entityManager.entityList[i].think(&entityManager.entityList[i]);
		}
	}
}

void entity_update(Entity *self)
{
	if (!self)return;
	if (!self->inUse)return;

	if (self->die != 0)
	{
		entity_free(self);
		return;
	}

	/*collision handles position and velocity*/
	vector2d_add(self->velocity, self->velocity, self->acceleration);

	//gf2d_particle_emitter_update(self->pe); 	/*For particle system*/

	gf2d_actor_next_frame(&self->actor);

	if (self->update != NULL)
	{
		self->update(self);
	}
}

void entity_update_all() {

	int i = 0;
	for (i = 0; entityManager.maxEntities; i++) {

		if (entityManager.entityList[i].inUse == 0)
			continue;
		entity_update(&entityManager.entityList[i]);
	}
}

int entity_do_damage(Entity *target, Entity *inflictor, Entity *attacker, int damage, Vector2D kick) {

	Vector2D k; 	/** Ask DJ what this stuff is about */
	int inflicted;
	if (!target)
		return 0;
	if(!inflictor)
		return 0;
	if (!attacker)
		return 0;
	if (!target)
		return 0;
	if (!damage)
		return 0;
	if (!target->damage)
		return 0;
	
	inflicted = target->damage(target, damage, inflictor);
	vector2d_scale(k, kick, (float)inflicted / (float)damage);
	vector2d_add(target->velocity, k, target->velocity);
	
	return inflicted;

}