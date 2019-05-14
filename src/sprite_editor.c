#include "sprite_editor.h"
#include "gf2d_graphics.h"
#define MAX_NUM_HEADS 6
#define MAX_NUM_HATS 2

static int curHead = 0;
static Window *win = NULL;


void update_player_frame(Entity *playerref) {

	if (!playerref) {
		slog("ERROR: No reference to player!");
		return;
	}

	playerref->actor.frame = curHead;

}

void draw_sprite_editor(Window *win) {

	if (!win) {
		slog("ERROR: No window!");
		return;
	}

	win = gf2d_window_new();

	SDL_Renderer *rend = gf2d_graphics_get_renderer();
	Rect rect =  gf2d_rect(200, 200, 600, 500);
	gf2d_rect_draw(rect, gf2d_color(255, 255, 255, 255));
	gf2d_window_draw(win);
	gf2d_draw_window_border_generic(rect, vector4d(255, 255, 255, 255));
	
	slog("DRAW_SPRITE_EDITOR");
}


void launch_sprite_editor() {

	draw_sprite_editor(win);

}



void change_head(int state) {

	if (state > 0) {
		curHead++;
		if (curHead >= MAX_NUM_HEADS) {
			curHead = 0;
		}
	}

	else {
		curHead--;
		if (curHead < 0) {
			curHead = MAX_NUM_HEADS - 1;
		}
	}
}


void change_hat(int state) {



}


void save_changes() {


	//overwrite player's sprite s
	//player_get()->actor.sprite = gf2d_sprite_load_image();

}