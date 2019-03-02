#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <SDL.h>
#include "gf2d_text.h"
#include "gf2d_list.h"

typedef enum {

	Idle = 0,
	Press = 1,
	Hold = 2,
	Release = 3

}InputEventType;

/**
*
* @brief Inputs abtract user input collection. Can be setup to trigger callbacks and/or polled for current state
*
**/
typedef struct {

	TextLine command;
	List *keyCodes; /** List of keys that must be pressed together to count as a single input **/

	int numKeyCodes; /** How many key codes are set **/
	Uint32 pressTime;  /** ticks when button was pressed **/
	InputEventType state;  /** updated each frame **/
	void (*onPress)(void *data); /**  callback for press event  **/
	void (*onHold)(void *data);  /**  callback for hold event  **/
	void(*onRelease)(void *data); /**  callback for release vent  **/
	void(*onDelete)(void *data); /**  callback for when input is deleted   **/
	void *data;    /**  pointer to be passed to callbacks  **/


}Input;

/**
*
* @brief initializes the config system
* @param configfile if not NULL it will load the file for user input configuration
*
**/
void gf2d_input_init(char *configFile);

/**
*
* @brief called as often as you want your input data update. every frame, or faster if you like
* @param must be called or input will be stale
*
**/
void gf2d_input_update();

/**
*
* @brief load user configuration from file
* @note must be called or input will be stale
*
**/
void gf2d_input_commands_load(char *configFile);

/**
*
* @brief cleas all user input configuration
*
**/
void gf2d_input_commands_purge();

/**
*
* @brief check if a command was pressed this frame
* @param command the name of the command to check
* @returns true if pressed, false otherwise
*
**/
Uint8 gf2d_input_command_pressed(const char *command);
Uint8 gf2d_input_command_held(const char *command);
Uint8 gf2d_input_command_released(const char *command);


/**
*
* @brief configure callbacks for an input
* @param command the name of the input to cinfugre
* @param onPress the function to call when the input is pressed
* @param onHold the function to call when the input is held
* @param onRelease the function to call when the input is released
* @param onDelete the function to call when the input is deleted from memory to clean up
* @Param data pointer to any custom data you want associated with the input
*
**/
void gf2d_input_set_callbacks(
	char *command,
	void(*onPress)(void *data),
	void(*onHold)(void *data),
	void(*onRelease)(void *data),
	void(*onDelete)(void *data),
	void *data
);


#endif __INPUT_MANAGER_H__