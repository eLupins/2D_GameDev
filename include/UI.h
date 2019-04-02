#ifndef __UI_H__
#define __UI_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include "simple_logger.h"
#include "gf2d_sprite.h"

#define TEXT_LENGTH 512

typedef struct UI {

	Uint8 inUse;
	Uint64 id;
	Sprite *sprite;
	Vector4D color;
	Uint8 closable;
	SDL_Rect window; //size
	Vector2D pos, scale, scaleCenter, rotation, flip;
	float currentAnim;

	void(*onClick) (struct UI* self); /* called for onclick action */
	void(*free)(struct UI *self); /* Called when a UI is freed */
	
	Uint8 pressed;

	char text[TEXT_LENGTH];
	TTF_Font *font;
	SDL_Color textcolor;
	SDL_Surface *surface;
	SDL_Texture *texture;
	int rectW, rectH;
	int padding;

}UIWindow;


void UI_system_init(int MaxUI);

UIWindow *new_window();

void free_ui(UIWindow *self);

void free_all_ui();

void ui_draw(UIWindow *self);

void ui_draw_all();

void ui_update(UIWindow *self);

void ui_update_all();

void ui_change_text(UIWindow *self, char *text, Uint32 wraplength);

#endif 