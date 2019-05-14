#include <SDL.h>
#include <stdio.h>
#include <SDL_ttf.h>
#include "gf2d_graphics.h"
#include "gf2d_sprite.h"
#include "gf2d_list.h"
#include "gf2d_input.h"
#include "gf2d_audio.h"
#include "gf2d_windows.h"
#include "gf2d_entity.h"
#include "gf2d_mouse.h"
#include "simple_logger.h"
#include "camera.h"
#include "level.h"
#include "editor.h"
#include "windows_common.h"
#include "level_graph.h"
#include "UI.h"
#include "player.h"
#include "gf2d_text.h"
#include "projectile.h"
#include "bat.h"

//SDL_Color col = { 255, 255, 255, 0 };

static int _done = 0;
static Window *_quit = NULL;
char scoreText[32];
Vector2D pos = { 50,-25 };
ParticleEmitter *pe;

void onCancel(void *data)
{
    _quit = NULL;
}
void onExit(void *data)
{
    _done = 1;
    _quit = NULL;
}

int main(int argc, char * argv[])
{
    /*variable declarations*/
    int i;
    LevelInfo *linfo = NULL;
    int editorMode = 0;
    int fullscreen = 0;
	SDL_Event e;
	char jsonFileName[MAX_FILE_LENGTH] = "";
	Sprite *poop = NULL;
	//Font myFont; 
	char* myScore = "Score: ";
	
	
//	myFont.filename = "fonts/SourceSansPro.ttf";

    /*parse args*/
    for (i = 1; i < argc; i++)
    {
        if (strcmp(argv[i],"--editor") == 0)
        {
            editorMode = 1;
        }
        else if (strcmp(argv[i],"--fullscreen") == 0)
        {
            fullscreen = 1;
        }
    }
    
    /*program initializtion*/
    init_logger("gf2d.log");
    slog("---==== BEGIN ====---");
    gf2d_graphics_initialize(
        "gf2d",
        1200,
        720,
        1200,
        720,
        vector4d(0,0,0,255),
        fullscreen);
    gf2d_graphics_set_frame_delay(16);
    gf2d_audio_init(256,16,4,1,1,1);
    gf2d_sprite_init(1024);
    gf2d_action_list_init(128);
//	gf2d_text_init(); //probably overkill but oh well
    gf2d_text_init("config/font.cfg");
    gf2d_input_init("config/input.cfg");
    gf2d_windows_init(128);
    gf2d_entity_system_init(1024);
	
	
	//poop = gf2d_sprite_load_image("images/backgrounds/bot.png");
	

    camera_set_dimensions(0,0,1200,700);
    
	create_level("rooms/lvl1.txt"); //text file list of level .jsons

   SDL_ShowCursor(SDL_DISABLE);
   


    // game specific setup
    if (!editorMode)
    {
       // linfo = level_info_load("config/testworld.json");
		strncpy(jsonFileName, begin_level(), MAX_FILE_LENGTH);
		linfo = level_info_load(jsonFileName);
        level_init(linfo,1);
    }
    else
    {
        // init mouse, editor window
        gf2d_mouse_load("actors/mouse.actor");
        editor_launch();
    }
    
    /*main game loop*/
    while(!_done)
    {
        gf2d_input_update();
        /*update things here*/
        gf2d_windows_update_all();

		SDL_PollEvent(&e);
		switch (e.type)
		{
		case SDL_QUIT:
			_done = 1;
			break;
		default:
			break;
		}
                
        if (!editorMode)
        {
            gf2d_entity_think_all();
            level_update();
        }
        else
        {
		
            gf2d_mouse_update();
        }
		gf2d_particle_emitter_update(pe);
		gf2d_particle_emitter_draw(pe);
        gf2d_graphics_clear_screen();// clears drawing buffers
        // all drawing should happen betweem clear_screen and next_frame
            //backgrounds drawn first
                // DRAW WORLD
                level_draw();
				gui_draw_hud();
			

				//gui_set_energy(2.0f); <-- might need this for health later

                if (!editorMode)
                {
                    gf2d_entity_update_all();
                }
                // Draw entities
				gf2d_windows_draw_all();
            if (editorMode)
            {
                gf2d_mouse_draw();
            }

			else {
				gui_draw_hud();
			}

        gf2d_grahics_next_frame();// render current draw frame and skip to the next frame
        
		if ((gf2d_input_command_down("exit")) && (_quit == NULL))
		{
			_quit = window_yes_no("Exit?", onExit, onCancel, NULL, NULL);
		}

		

		//for opening editor
		if ((gf2d_input_key_released("1"))) {
			
			editor_launch();

		}
   //     slog("Rendering at %f FPS",gf2d_graphics_get_frames_per_second());
    }
    level_info_free(linfo);
    level_clear();
    slog("---==== END ====---");
    return 0;
}
/*eol@eof*/
