//#include "projectile.h"
#include "gf2d_particles.h"
#include "gf2d_input.h"
#include "entity_common.h"
#include "camera.h"
#include "player.h"
#include "projectile.h"
#include "simple_logger.h"
#include "level.h"
#include "gf2d_graphics.h"

Entity *projectile = NULL;
static int projectileCount = 4;
ParticleEmitter *pe;
Particle particle;

void projectile_think(Entity *self);
void projectile_update(Entity *self);
//void projectile_free(Entity *self);
void projectile_die(Entity *self);

float lifetime = 3.0;


typedef struct {

	int velocity;
	float life;
	Uint8 canDie;
	float baseAcceleration;

}ProjectileData;

static ProjectileData projData = {

	2,
	5.0,
	0,
	1

};

Entity *get_projectile() {

	return projectile;

}

Entity *projectile_new(Vector2D position, const char *name, char *actorfile) {

	Entity *self;
	int w, h;

	self = gf2d_entity_new();
	if (!self) {
		slog("ERROR: Cannot create a new projectile");
		return NULL;
	}

	gf2d_line_cpy(self->name, name);
	self->parent = player_get();
	SDL_QueryTexture(self->actor.sprite, NULL, NULL, &w, &h);
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
		0,
		&self->shape,
		self,
		NULL,
		NULL);

	gf2d_actor_load(&self->actor, actorfile);

	//bs temporary projectile sprite change for when the player levels up
	//not added to on level up because there are only 3 available sprites for the potion upgrade
	if (player_get()->playerlvl == 1) {
		self->actor.sprite = gf2d_sprite_load_image("images/potion2.png");
		self->damageCount += 1;
	}
	if (player_get()->playerlvl == 2) {
		self->actor.sprite = gf2d_sprite_load_image("images/potion3.png");
		self->damageCount += 1;
	}

	gf2d_actor_set_action(&self->actor, "idle");

	self->sound[0] = gf2d_sound_load("music/thrown.wav", 1, -1);
	self->sound[1] = gf2d_sound_load("music/explosion.wav", 1, -1);

	vector2d_copy(self->position, position);

	vector2d_copy(self->scale, self->scale);
	vector2d_set(self->scaleCenter, 18, 18);
	vector3d_set(self->rotation, 64, 64, 0);
	vector2d_set(self->flip, 1, 0);

/////////////////////////////////////////////////////////////////

	self->pe = pe;

	
	self->think = projectile_think;
	
	self->update = projectile_update;
	
	self->die = projectile_die;
	self->free = level_remove_entity;
	
	self->data = (void*)&projData;


	projectile = self;
	level_add_entity(self);
	gf2d_entity_new(self);

	return self;

}


void projectile_update(Entity *self)
{
	projectile_think(self);
	Entity *playerref = player_get();

	float angle = 40.0f;

///////////////////////	PROJECTILE MOVEMENT////////////////////////


	if (!self) {
		slog("ERROR: Cannot update a projectile that doesn't exist!");
		return;
	}
	if (gf2d_input_command_down("walkup") && gf2d_input_command_released("melee")) {
		self->velocity.y -= 3.0;
		
	}

	if (gf2d_input_command_down("walkdown") && gf2d_input_command_released("melee")) {
		self->velocity.y += 3.0;
	}

	if (gf2d_input_command_down("walkleft") && gf2d_input_command_released("melee")) {
		self->velocity.x -= 3.0;
	}

	if (gf2d_input_command_down("walkright") && gf2d_input_command_released("melee")) {
		self->velocity.x += 3.0;
	}
	

	lifetime -= 0.05;

	slog("LIFETIME %f", lifetime);

	//remove the projectile from the world
	if (lifetime <= 0) {
		
		
		gf2d_sound_play(projectile->sound[1], 0, 1.0, -1, -1);
		self->dead = 1;
		if (self->dead == 1) {
			particle_sys_start(pe);
		}
		lifetime = 3.0;
	
	}
	gf2d_particle_emitter_free(pe);
	entity_world_snap(self);
}

///////////////////////////////////////////////////////////////////////

void particle_sys_start(ParticleEmitter *pe) {

	gf2d_particle_new(pe);
	Shape shape = gf2d_shape_circle(0, 0, 8);
	Sprite *spr = gf2d_sprite_load_image("images/cloud.png");

	pe = gf2d_particle_emitter_new_full(
		70,
		300,
		15,
		PT_Sprite,
		get_projectile()->position,
		vector2d(80, 80),
		vector2d(0, -3),
		vector2d(70, 10),
		vector2d(0, 0.05),
		vector2d(0, 0.01),
		gf2d_color(0.85, 0.55, 0, 1),
		gf2d_color(-0.01, -0.02, 0, 0),
		gf2d_color(0.1, 0.1, 0, 0.1),
		&shape,
		0,
		0,
		0,
		"images/cloud.png",
		32,
		32,
		90,
		0,
		//        SDL_BLENDMODE_BLEND);
		SDL_BLENDMODE_ADD);
	//emitter new full
	//particle_new_full_bulk to make a bulk of particles
	projectile->pe = pe;

	slog("PARTICLE SYS START");
	//level_add_entity(pe);

	gf2d_particle_new_default(pe, 20);
	
	
}


//////////////////////////////////////////////////////////////////////

void projectile_free(Entity *self) {


	int i;
	if (!self)
		return;

	if (self->free)
		self->free(self);

	for (i = 0; i < EntitySoundMax; i++)
	{
		gf2d_sound_free(self->sound[i]);
	}

	gf2d_actor_free(&self->actor);
	gf2d_particle_emitter_free(self->pe);
	memset(self, 0, sizeof(Entity));
}



void projectile_think(Entity *self) {
	
	Collision c = { 0 };
	Shape shape;
	Entity *other;
	ClipFilter filter = { 0 };

	filter.layer = MONSTER_LAYER;
	shape = gf2d_body_to_shape(&self->body);
	gf2d_space_body_collision_test_filter(level_get_space(), shape, &c, filter);

	if (c.collided) {

		if ((!c.body) || (!c.body->data)) {
			return;
		}

		other = (Entity *)c.body->data;
		if (other = player_get()) {
			slog("ERROR: Cannot harm the player!");
			return;
		}

		else {
			gf2d_sound_play(projectile->sound[1], 0, 1.0, -1, -1);
			//gf2d_entity_deal_damage(
			self->dead = 1;
			lifetime = 3.0;

		}

		return;

	}
}

void projectile_die() {





}



void projectile_spawn(Vector2D position, const char *name, SJson *args) {


	projectile = projectile_new(position, name, args);
	gf2d_entity_draw(projectile);
	gf2d_sound_play(projectile->sound[0], 0,1.0,  -1, -1);
	projectile_update(projectile);

}