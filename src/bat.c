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


void bat_spawn(Vector2D position, char *args) {
	Entity *self = gf2d_entity_new();
	self->update = bat_update;
	level_add_entity(self);

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

