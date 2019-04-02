#include "UI.h"
#include "gf2d_graphics.h"
#include "gf2d_draw.h"

typedef struct uimanager_s {

	Uint32 Max_UI;
	UIWindow *list;
	Uint64 increment;

}UI_Manager;


static UI_Manager ui_manager = { 0 , NULL};

void ui_system_close() {

}

void UI_system_init(int MaxUI) { // do later

	if (!MaxUI) {
		slog("ERROR: Cannot initialize a UI system for no UI");
		return;
	}
}


UIWindow *new_window() {

	int i = 0;
	
	for (i = 0; i < ui_manager.Max_UI; i++){
		if (ui_manager.list[i].inUse == 0) {

			memset(&ui_manager.list[i], 0, sizeof(UIWindow));
			ui_manager.list[i].id = ui_manager.increment++;
			ui_manager.list[i].inUse = 1;
			vector2d_set(ui_manager.list[i].scale, 1, 1);
			ui_manager.list[i].color = vector4d(1, 1, 1, 1); //no color shift; opaque
			return &ui_manager.list[i];
		}
	}
}

void free_ui(UIWindow *self) {

	int i = 0 ;

	if (!self)
		return;

	if (self->free)
		self->free(self);

	/** Memset before you free; good programming practice **/
	memset(self, 0, sizeof(UIWindow));
	free(self);

}

void free_all_ui() {

	int i = 0;

	for (i = 0; i < ui_manager.Max_UI; i++) {
		
		free_ui(&ui_manager.list[i]);

	}
}

void ui_draw(UIWindow *self) {

	if (!self) {
		slog("ERROR: Cannot draw 0 UI");
		return;
	}

	if (!self->inUse) {
		slog("ERROR: Cannot draw a UI thats not in use");
		return;
	}

	if (!(&self->window)) {
		slog("ERROR: Cannot draw a window that doesn't exist!");
		return;
	}

	if (self->sprite)
		gf2d_sprite_draw_image(self->sprite, self->pos);
	else
		gf2d_draw_rect(self->window, self->color);

	if (self->texture != NULL) {

		SDL_RenderCopy(gf2d_graphics_get_renderer(), self->texture, NULL, &self->window);

	}
}

void ui_draw_all() {

	int i = 0;

	for (i = 0; i < ui_manager.Max_UI; i++) {
		if (ui_manager.list[i].inUse) {

			ui_draw(&ui_manager.list[i]);

		}
	}
}

void ui_update(UIWindow *self) {

	if (!self) {
		slog("ERROR: Cannot update a UI that doesn't exist!");
		return;
	}

	self->window.x = self->pos.x;
	self->window.y = self->pos.y;

}

void ui_update_all() {

	int i = 0;

	for (i = 0; i < ui_manager.Max_UI; i++) {
		if (ui_manager.list[i].inUse) {

			ui_update(&ui_manager.list[i]);

		}
	}
}

void ui_change_text(UIWindow *self, char *text, Uint32 wraplength) {

	if (!self) {
		slog("ERROR: Cannot change text for a window that doesn't exist!");
		return;
	}

	strncpy(self->text, text, TEXT_LENGTH);

	self->surface = TTF_RenderText_Blended_Wrapped(self->font, text, self->textcolor, wraplength); //create ttf text
	self->texture = SDL_CreateTextureFromSurface(gf2d_graphics_get_renderer(), self->surface);
	SDL_QueryTexture(self->texture, NULL, NULL, &self->rectW, &self->rectH); //query texture for rendering 
	self->window.h = self->rectH;
	self->window.w = self->rectW;

}