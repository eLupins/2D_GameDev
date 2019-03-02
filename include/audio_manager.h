#ifndef __AUDIO_MANAGER_H__
#define __AUDIO_MANAGER_H__

#include <SDL_mixer.h>
#include "simple_logger.h"
#include "gf2d_list.h"
#include "gf2d_text.h"


#define FILEPATH_CHAR_LEN 512
#define MAX_SOUND_NUM 64
#define MAX_CHANNEL_NUM 4

typedef struct audioData {

	Uint64 id;
	Uint8 inUse;

	Uint32 ref_count;
	TextLine filePath; /** sound file to be loaded **/
	Mix_Chunk *sound;
	int defaultChannel;
	float volume;

}AudioSys;

/*
*
* @brief initializes the audio system
* @param maxSounds - for the maximum amount of sounds that can be loaded to memory at once
* @param channels - the number of allocated audio channels
* @param maxMusic - the amount of music files that can be supported at once
* @param enableMP3 - will enable MP3 support in the audio system if true
* @param enableOGG - will enbale OGG support in the audio system if true
*
*/
void audiosys_init(Uint32 maxSounds, Uint32 channels, Uint32 maxMusic, Uint8 enableMP3, Uint8 enableOGG);


/*
*
* @brief initializes sound files
* @param max sounds to allocate memory for
*
*/
void sound_init(Uint32 maxSounds);

/*
*
* @brief adds sound into the sound manager
* 
*/
AudioSys *sound_new();

/*
*
* @brief getting audio files by path
* @param filepath - the desired audio file's path in the explorer
*
*/
AudioSys *sound_get_file_by_path(const char *filepath);


/*
*
* @brief load a sound file and return it as a sound pointer
* @param filepath - file to be loaded
* @param volume - what volume to set the audio file to; on a scale from 0.0 to 1.0
* @param defaultChannel - which channel to play the sound file on if not specified
*@return if error, return NULL; if successful, return a pointer to a sound file
*
*/


AudioSys *sound_load(const char *filename, float volume, int defaultChannel);

/*
*
* @brief plays  sound file
* @param sound - sound file to be played
* @param numLoops - number of times to loop the sound file
* @param volume - volume to set the sound file to
* @param channel - which channel to play on; -1 is to use the default channel
*
*/
void sound_play(AudioSys *sound, int numLoops, float volume, int channel, int group);

/*
*
* @brief adjust sound's volume
* @param sound - sound file to be affected
* @param volume - volume adjustment
*
*/

void sound_adjust_volume(AudioSys *sound, float volume);

/*
*
* @brief frees sound from memory
* @param sound file to be freed
*
*/
void sound_free(AudioSys *sound);


/*
*
* @brief deletes a sound file 
* @param sound file to be deleted
*
*/
void sound_delete(AudioSys *sound);


/*
*
* @brief deletes all sound
*
*/
void sound_delete_all();

#endif