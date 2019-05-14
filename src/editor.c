#include "gf2d_elements.h"
#include "editor.h"
#include "level.h"
#include "gf2d_text.h"
#include "gf2d_mouse.h"
#include "gf2d_shape.h"
#include "simple_logger.h"
#include "simple_json.h"
#include "windows_common.h"
#include "player.h"

typedef struct EditorData_S
{
    TextLine filename;
    LevelInfo *level;       /**<working level*/
}EditorData;

EditorData editorData = {0};
static Window *_editor = NULL;

int editor_draw(Window *win)
{
    Vector2D mouse;
    Rect tileHighlight;
    if (!editorData.level)return 0;
    
    mouse = gf2d_mouse_get_position();
    
    tileHighlight.x = (Uint32)(mouse.x / editorData.level->tileSize.x) * editorData.level->tileSize.x;
    tileHighlight.y = (Uint32)(mouse.y / editorData.level->tileSize.y) * editorData.level->tileSize.y;
    tileHighlight.w = editorData.level->tileSize.x;
    tileHighlight.h = editorData.level->tileSize.y;
    gf2d_rect_draw(tileHighlight,gf2d_color(255,200,0,255));

    /*draw rects around the tile the mouse is over*/
    return 0; 
}

void editor_new_map(void *data)
{
    editorData.level = level_info_create(
        "images/backgrounds/bg_flat.png",
        "music/13_Cave_of_Time.mp3",
        "images/backgrounds/floor.png",
        vector2d(32,32),
        vector2d(38,24));
    level_init(editorData.level,0);
}



int editor_update(Window *win,List *updateList)
{

	gf2d_mouse_load("actors/mouse.actor");
	gf2d_mouse_update();
    int i,count;
    Element *e;
    Vector2D mouse,tile;
    if (!win)return 0;
    if (!updateList)return 0;
    
    mouse = gf2d_mouse_get_position();
    if ((editorData.level != NULL)&&(!gf2d_mouse_in_rect(win->dimensions)))
    {
        if (gf2d_mouse_button_pressed(0) || gf2d_mouse_button_held(0))
        {
            tile = level_position_to_tile(editorData.level, mouse);
            level_update_tile(editorData.level,tile,1);
            level_make_tile_layer(editorData.level);
        }
        if (gf2d_mouse_button_pressed(2) || gf2d_mouse_button_held(2))
        {
            tile = level_position_to_tile(editorData.level, mouse);
            level_update_tile(editorData.level,tile,0);
            level_make_tile_layer(editorData.level);
        }
    }
    count = gf2d_list_get_count(updateList);
    for (i = 0; i < count; i++)
    {
        e = (Element *)gf2d_list_get_nth(updateList,i);
        if (!e)continue;
		switch (e->index)
		{

			// @brief modified the editor window to have sprites load instead of world building
			// Each case is a different option for a head sprite. 
		case 110:
			slog("HEAD 0");
			//if I tried to load the pprites separately, they wouldn't size up well compared to the original sprite.
			//reload the heads.png spritesheet and shift the frame every time. if I dont reload the spritesheet,
			//it only flickers to the new sprite temporarily 
			player_get()->actor.sprite = gf2d_sprite_load_all("images/editor_sprites/heads.png", 128, 128, 6, 1);
			player_get()->actor.frame = 0;
			break;
		case 120:
			slog("HEAD 1");
		
			player_get()->actor.sprite = gf2d_sprite_load_all("images/editor_sprites/heads.png", 128, 128, 6, 1);
			player_get()->actor.frame = 1;
	
			break;
		case 130:
			slog("HEAD 2");
			player_get()->actor.sprite = gf2d_sprite_load_all("images/editor_sprites/heads.png", 128, 128, 6, 1);
			player_get()->actor.frame = 2;
			break;
		case 140:
			slog("HEAD 3");
			player_get()->actor.sprite = gf2d_sprite_load_all("images/editor_sprites/heads.png", 128, 128, 6, 1);
			player_get()->actor.frame = 3;
			break;

		case 150:
			slog("HEAD 4");
			player_get()->actor.sprite = gf2d_sprite_load_all("images/editor_sprites/heads.png", 128, 128, 6, 1);
			player_get()->actor.frame = 5;
		}
    }


    return 0;
}

void editor_launch()
{
    if (_editor != NULL)return;
    _editor = gf2d_window_load("config/editor_main.json");
    if (!_editor)
    {
        slog("failed to load editor window");
        return;
    }
    _editor->update = editor_update;
    _editor->draw = editor_draw;
}

/*eol@eof*/
