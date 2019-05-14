#include "bat.h"
#include "entity_common.h"
#include "level_graph.h"
#include "projectile.h"
#include "level.h"
#include "player.h"
#include "monster.h"
#include "gf2d_collision.h"
//#include "gf2d_space.h"


///bat just wanders around. hurts the player if it crashes into it. killing it doesnt award anything

Entity *batboy = NULL;

//void bat_draw(Entity *self);
void bat_think(Entity *self);
void bat_update(Entity *self);
int bat_touch(Entity *self, Entity *other);
void bat_die(Entity *self);


typedef struct {

	int health;
	float speed;

}BatData;

BatData battyData = { 3, 4.0 };


Entity *get_bat() {
	return batboy;
}

Entity *bat_new(Vector2D position, const char *name, char *actorfile) {

	Entity *self;
	self = gf2d_entity_new();
	if (!self) {
		slog("ERROR: Cannot create a new projectile");
		return NULL;
	}

	gf2d_line_cpy(self->name, name);
	self->parent = NULL;
	//SDL_QueryTexture(self->actor.sprite, NULL, NULL, &w, &h);
	//slog("PROJECTILE.C - QUERYTEXTURE RESULTS: ", w, h);
	self->shape = gf2d_shape_rect(-16, -16, 30, 30);
	self->damageCount = 1;
	gf2d_body_set(
		&self->body,
		name,
		//        0,//no layer
		ALL_LAYERS &~PICKUP_LAYER,//player layers
		1,
		player_get()->position,
		vector2d(0, 0),
		10,
		1,
		2,
		&self->shape,
		self,
		NULL,
		NULL);

	gf2d_actor_load(&self->actor, actorfile);


	gf2d_actor_set_action(&self->actor, "idle");

	self->sound[0] = gf2d_sound_load("music/thrown.wav", 1, -1);
	self->sound[1] = gf2d_sound_load("music/explosion.wav", 1, -1);

	vector2d_copy(self->position, position);

	vector2d_copy(self->scale, self->scale);
	vector2d_set(self->scaleCenter, 18, 18);
	vector3d_set(self->rotation, 64, 64, 0);
	vector2d_set(self->flip, 1, 0);

	/////////////////////////////////////////////////////////////////

	//self->pe = pe;


	self->think = bat_think;

	self->update = bat_update;

	//self->die = bat_die;
	self->free = level_remove_entity;

	self->data = (void*)&battyData;


	batboy = self;
	level_add_entity(self);
	gf2d_entity_new(self);

	return self;

}

void bat_spawn(Vector2D position, char *args) {
	batboy = bat_new(position, "bat", args);
	batboy->update = bat_update;
	level_add_entity(batboy);

}

void bat_think(Entity  *self) {

	//monster_touch(self);
	bat_update(self);
	return;

}

void bat_update(Entity *self) {

	static int i = 0;
	printf("Bla\n");

	if (i++ % 10 == 0) {
		if (self->health > 0) {
			int a = (((float)rand()) / RAND_MAX) > 0.5f;

			self->velocity.x = (a ? -1 : 1) * ((float)rand()) / RAND_MAX;
			self->velocity.y = (a ? 1 : -1) * ((float)rand()) / RAND_MAX;
		//	self->velocity.x = ((float)rand()) / RAND_MAX;
		}
	}
}

void bat_free(Entity *self) {

	self->dead;

}

