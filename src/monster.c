
#include "monster.h"
#include "level.h"
#include "simple_logger.h"
#include "entity_common.h"

void monster_draw(Entity *self);
void monster_think(Entity *self);
void monster_update(Entity *self);
int  monster_touch(Entity *self,Entity *other);
int  monster_damage(Entity *self,int amount, Entity *source);
void monster_die(Entity *self);
Entity *monster_new(Vector2D position,char *actorFile);

void monster_spawn(Vector2D position, SJson *args)
{
    monster_new(position, sj_get_string_value(args));
}


Entity *monster_new(Vector2D position,char *actorFile)
{

    Entity *self;
    self = gf2d_entity_new();
    if (!self)return NULL;
    
    gf2d_line_cpy(self->name,"monster");
    self->parent = NULL;
                                 //x on left;y the top of the rect, width, height 
    self->shape = gf2d_shape_rect( -60, -60, 57, 57);
    gf2d_body_set(
        &self->body,
        "monster",
//        0,//no layer
        ALL_LAYERS,//all layers
        2,
        position,
        vector2d(0,0),
        10,
        1,
        0,
        &self->shape,
        self,
        NULL,
        NULL);

    gf2d_actor_load(&self->actor,actorFile);
    gf2d_actor_set_action(&self->actor,"idle");
    
   // self->sound[0] = gf2d_sound_load("music/bat_chirp.wav",1,-1);
	self->velocity = vector2d(1.0, 10);
    
    vector2d_copy(self->position,position);
    
    vector2d_copy(self->scale,self->actor.al->scale);
    vector2d_set(self->scaleCenter,64,64);
    vector3d_set(self->rotation,64,64,0);
    vector2d_set(self->flip,0,0);
    
    self->think = monster_think;
    self->draw = monster_draw;
    self->update = monster_update;
    self->touch = monster_touch;
    self->damage = monster_damage;
    self->die = monster_die;
    self->free = level_remove_entity;

    level_add_entity(self);
    return self;
}

void monster_draw(Entity *self)
{
    
}

void monster_think(Entity *self)
{
    //shop menu interacton
}

void monster_update(Entity *self)
{
    entity_apply_gravity(self);

}

int  monster_touch(Entity *self,Entity *other)
{

	Vector2D kick = { 1.0,1.0 };
    slog("monster touch called");
	if (!other)
	{
		return 0;
	}

	if (gf2d_line_cmp(other->name, "player") != 0) {
		gf2d_entity_deal_damage(other, self, self, 1, kick);

	}

	else {

		Vector2D nv = { 0 };
		vector2d_reflect(&nv, self->position, self->velocity);
	}
    entity_damage(other,self,5,10);
    return 0;
}

int  monster_damage(Entity *self,int amount, Entity *source)
{
    return 0;
}

void monster_die(Entity *self)
{
    
}
/*eol@eof*/
