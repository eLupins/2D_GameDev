#include "UI.h"
#include "gf2d_shape.h"
#include "gf2d_draw.h"
#include "gf2d_text.h"
#include "gf2d_sprite.h"
#include "player.h"
#include "gf2d_entity.h"
#include "simple_logger.h"
#include "SDL_FontCache.h"
#include "gf2d_graphics.h"
//#include <SDL_ttf.h>

typedef struct
{
	Sprite *hud;
	float   healthPercent;
	float   energyPercent;
	float   opponentPercent;
	int     score;
	float   alert;

}GUI;

static GUI gui = { 0 };

void gui_close_hud()
{
	gf2d_sprite_free(gui.hud);
}

void gui_setup_hud()
{
	memset(&gui, 0, sizeof(GUI));
	gui.hud = gf2d_sprite_load_image("images/ui/coin.png");
	atexit(gui_close_hud);
}

void gui_draw_percent_bar_horizontal(Rect rect, float percent, Vector4D fgColor, Vector4D bgColor, int left)
{
	SDL_Rect r;
	r = gf2d_rect_to_sdl_rect(rect);
	gf2d_draw_solid_rect(r, bgColor);
	if (left)
	{
		r = gf2d_rect_to_sdl_rect(gf2d_rect(rect.x, rect.y, (float)rect.w*percent, rect.h));
	}
	else
	{
		r = gf2d_rect_to_sdl_rect(gf2d_rect(rect.x + (1 - percent)*rect.w, rect.y, (float)rect.w*percent, rect.h));
	}
	gf2d_draw_solid_rect(r, fgColor);
}

void gui_draw_percent_bar_vertical(Rect rect, float percent, Vector4D fgColor, Vector4D bgColor, int top)
{
	SDL_Rect r;
	r = gf2d_rect_to_sdl_rect(rect);
	gf2d_draw_solid_rect(r, bgColor);
	if (top)
	{
		r = gf2d_rect_to_sdl_rect(gf2d_rect(rect.x, rect.y + (1 - percent)*rect.h, rect.w, rect.h*percent));
	}
	else
	{
		r = gf2d_rect_to_sdl_rect(gf2d_rect(rect.x, rect.y, rect.w, rect.h*percent));
	}
	gf2d_draw_solid_rect(r, fgColor);
}

void gui_draw_hud()
{
	Vector4D color = { 255,255,255,255 };
	if (gui.healthPercent < 0.2)
	{
		gui.alert = (gui.alert + 0.02);
		if (gui.alert >= M_PI)gui.alert = 0;
		color.y = color.z = sin(gui.alert) * 255;
	}
	gf2d_sprite_draw(
		gui.hud, vector2d(0, 0),
		NULL,
		NULL,
		NULL,
		NULL,
		&color,
		0);
	gui_set_score();
	//gui_get_score(gf2d_rect(10,10,145,10), gui.score, vector4d((1 - gui.score) * 255, gui.score * 255, 0, 255), vector4d(128, 0, 0, 128), 1);
	//gui_draw_percent_bar_horizontal(gf2d_rect(40, 40, 145, 10), gui.healthPercent, vector4d((1 - gui.healthPercent) * 255, gui.healthPercent * 255, 0, 255), vector4d(128, 0, 0, 128), 1);
	//gui_draw_percent_bar_horizontal(gf2d_rect(10, 20, 145, 10), gui.energyPercent, vector4d(0, 255, 255, 255), vector4d(0, 64, 64, 255), 1);
	//gui_draw_percent_bar_horizontal(gf2d_rect(1045, 10, 145, 10), gui.opponentPercent, vector4d(255, 0, 64, 255), vector4d(64, 0, 32, 255), 0);
}

void gui_set_health(float health)
{
	gui.healthPercent = health;
}

void gui_set_energy(float energy)
{
	gui.energyPercent = energy;
}

void gui_set_opponent_health(float health)
{
	gui.opponentPercent = health;
}

void gui_set_score() {

	int score = player_get()->score;
	//SDL_Surface scoreSurf = TTF_RenderText_SOlid
	//SDL_Renderer *renderer = NULL;
	//renderer = SDL_CreateRenderer(
	FC_Font* font = FC_CreateFont();
	FC_LoadFont(font, gf2d_graphics_get_renderer(), "fonts/CELTG___.TTF", 20, FC_MakeColor(253, 250, 103, 255), TTF_STYLE_NORMAL);
	FC_Draw(font, gf2d_graphics_get_renderer(), 30, 30, "Gold: %i", score);
	FC_FreeFont(font);
	//SDL_Surface scoreSurf = TTF_RenderText_Solid("

	//printf("%i", score); <-- for debugging
	//slog(score);
}


/*eol@eof*/