#include "player.h"
#include "simple_logger.h"
#include "camera.h"
#include "level.h"
#include "gf2d_input.h"
#include "entity_common.h"
#include "level_graph.h"

static Entity *_player = NULL;
static Entity * projectile = NULL;
static int projectileCount = 0;

void player_draw(Entity *self);
void player_think(Entity *self);
void player_update(Entity *self);
int  player_touch(Entity *self,Entity *other);
int  player_damage(Entity *self,int amount, Entity *source);
void player_die(Entity *self);

typedef struct
{
    int baseSpeed;
    int maxSpeed;
    float baseAcceleration;
    float shields,shieldMax;
    float charge,chargeMax;
    float energy,energyMax;
    int capacitors,capacitorsMax;
    int attackType;
	int score;

}PlayerData;


static PlayerData playerData = {
    3,50,3.6,
    0,100,
    0,100,
    0,100,
    0,0};

Entity *player_get()
{
    return _player;
}

void player_set_position(Vector2D position)
{
    if (!_player)
    {
        slog("no player loaded");
        return;
    }
    vector2d_copy(_player->position,position);
    vector2d_copy(_player->body.position,position);
}

void player_spawn(Vector2D position,SJson *args)
{
    if (_player != NULL)
    {
        vector2d_copy(_player->position,position);
        level_add_entity(_player);
        return;
    }
    player_new(position);
}

Entity *player_new(Vector2D position)
{
    Entity *self;
    self = gf2d_entity_new();
    if (!self)return NULL;
    
    gf2d_line_cpy(self->name,"player");
    self->parent = NULL;
    
    self->shape = gf2d_shape_rect(-16, -16, 30, 60);
    gf2d_body_set(
        &self->body,
        "player",
//        0,//no layer
        ALL_LAYERS &~ PICKUP_LAYER,//player layers
        1,
        position,
        vector2d(0,0),
        10,
        1,
        0,
        &self->shape,
        self,
        NULL,
        NULL);

    gf2d_actor_load(&self->actor,"actors/player.actor");
    gf2d_actor_set_action(&self->actor,"idle");

    self->sound[0] = gf2d_sound_load("sounds/jump_10.wav",1,-1);

    
    vector2d_copy(self->position,position);
    
    vector2d_copy(self->scale,self->actor.al->scale);
    vector2d_set(self->scaleCenter,64,64);
    vector3d_set(self->rotation,64,64,0);
    vector2d_set(self->flip,1,0);
    
    self->think = player_think;
    self->draw = player_draw;
    self->update = player_update;
    self->touch = player_touch;
    self->damage = player_damage;
    self->die = player_die;
    self->free = level_remove_entity;

    self->data = (void*)&playerData;
    _player = self;
    level_add_entity(self);
    return self;
}

void player_draw(Entity *self)
{
    //additional player drawings can go here
}
//INPUT HERE
void player_think(Entity *self)
{
	Entity *proj = NULL;
    switch (self->state)
    {
        case ES_Idle:
            if (gf2d_input_command_down("walkleft"))
            {
                self->flip.x = 1;
                if (!entity_left_check(self,1))
                {   
                    self->velocity.x -= 1.25;
                }
            }
            if (gf2d_input_command_down("walkright"))
            {
                self->flip.x = 0;
                if (!entity_right_check(self,1
				))
                {
                    self->velocity.x += 1.25;
                }
            }
			/** Add ons; walking up and walking down**/
			if (gf2d_input_command_down("walkup")) {
				self->velocity.y -= 1.25;

			}
			if (gf2d_input_command_down("walkdown")) {
				self->velocity.y += 1.25;
			}
			///////////////////////////////////////////
            if (((gf2d_input_command_pressed("jump"))&&(self->grounded))&&(!self->jumpcool))
            {
                self->velocity.y -= 10;
                self->jumpcool = gf2d_actor_get_frames_remaining(&self->actor);
                gf2d_sound_play(self->sound[0],0,1,-1,-1);
                gf2d_actor_set_action(&self->actor,"jump");
            }
            if (gf2d_input_command_released("melee"))
            {
                gf2d_actor_set_action(&self->actor,"hack");
                self->cooldown = gf2d_actor_get_frames_remaining(&self->actor);
                slog("cooldown set to %i",self->cooldown);
                self->state = ES_Attacking;
			
				
            }
            break;
        default:
            break;
    }
}

void player_update(Entity *self)
{

    Vector2D camPosition = {0,0};
    if (!self)return;
    
    camera_set_position(camPosition);
    if (self->jumpcool > 0) self->jumpcool -= 0.2;
    else self->jumpcool = 0;
    if (self->actor.at == ART_END)
    {
        gf2d_actor_set_action(&self->actor,"idle");
    }
    //world clipping

    
    // walk dampening
    if (self->velocity.x)
    {
        self->velocity.x *= 0.8;
        if (fabs(self->velocity.x) < 1)self->velocity.x = 0;
    }
	/** Add on; apply walk dampening to y axis movement  **/
	if (self->velocity.y) {
		self->velocity.y *= 0.8;
		if (fabs(self->velocity.y) < 1)
			self->velocity.y = 0;

	}
	/** Get rid of apply gravity since its not needed for a top down game*/
  //  entity_apply_gravity(self);
    entity_world_snap(self);    // error correction for collision system
	door_check(self);
    switch (self->state)
    {
        case ES_Idle:
            break;
        case ES_Seeking:
            break;
        case ES_Charging:
        case ES_Attacking:            
        case ES_Pain:
        case ES_Cooldown:
            self->cooldown--;
            if (self->cooldown <= 0)
            {
                self->state = ES_Idle;
            }
            break;
        case ES_Dying:
            return;
        case ES_Dead:
            return;
    }
    
}

int player_touch(Entity *self,Entity *other)
{
    return 0;// player does not touch
}

int player_damage(Entity *self,int amount, Entity *source)
{
    slog("player taking %i damage!",amount);
    self->health -= amount;
    if (self->health <= 0)self->health = 0;
    self->die(self);
    return amount;//todo factor in shields
}

void player_die(Entity *self)
{
    
}

/*eol@eof*/
