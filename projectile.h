#include "entity_common.h"

typedef struct ProjectileData {

	Uint8 inuse;
	Uint64 id;

	//* Graphics *//
	Sprite *sprite;
	Vector2D scale;
	Vector2D flip;
	Vector2D facing;

	//* Physics *//
	Shape shape;
	Body body;
	float velocity;

	//* Other *//
	int damage;
	float kick;
	float lifetime;
	ParticleEmitter *pe;

} Projectile;

void projectile_update();

void projectile_define();

void spawn_projectile(Entity *self, Projectile projectile); // return entity pointer 

void draw_projectile(Projectile projectile);

void projectile_think();

void delete_projectile();

int projectile_deal_damage(Entity *target, Entity *inflictor, Entity *attacker, int damage, Vector2D kick);