#include "audio_manager.h"


typedef struct soundManager {

	Uint32 maxSounds;
	AudioSys *soundList;

	Uint64 i;
	//Uint8 initialize;

}SoundManager;

static SoundManager soundManager = { 0 , NULL, 0 };

void audiosys_close() {

	int x = 0;

	//** check to see if the soundList is empty. if it isn't, free the contents **/
	if (soundManager.soundList != NULL) {
		for (x = 0; x < soundManager.maxSounds; x++) {
			sound_free(&soundManager.soundList[x]);
		}
		free(&soundManager.soundList);
	}

	memset(&soundManager, 0, sizeof(soundManager));
	slog("NOTICE: Sound manager closed. \n");

}

void audiosys_init(Uint32 maxSounds, Uint32 channels, Uint32 channelGroups, Uint32 maxMusic, Uint8 enableMP3, Uint8 enableOGG) {

	int flags = 0;

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {

		slog("ERORR: Cannot initialize audio system. %s \n", SDL_GetError());
		return;

	}

	if (enableMP3)
		flags |= MIX_INIT_MP3;

	if (enableOGG)
		flags |= MIX_INIT_OGG;

	if (!Mix_Init(flags) & flags)
		slog("ERROR: Failed to initialize some audio support. \n");

	Mix_AllocateChannels(channels);

	
	
	atextit(Mix_Quit);
	atextit(audiosys_close);
	sound_init(maxSounds);
	slog("SUCCESS: Audio system initialized");
}

void sound_init(Uint32 maxSounds) {

	if (maxSounds <= 0) {
		slog("ERROR: Cannot initialize a sound manager for no sounds. \n");
		return;
	}

	memset(&soundManager, 0, sizeof(soundManager));
	soundManager.soundList = (AudioSys *)malloc(sizeof(AudioSys)* maxSounds);

	if (!soundManager.soundList) {
		slog("ERROR: Did not successfully allocate memory for the sound list.");
		audiosys_close();
		return;
	}

	memset(soundManager.soundList, 0, sizeof(AudioSys)* maxSounds);
	soundManager.maxSounds = maxSounds;
	
	slog("SUCCESSFUL: Intialized sound system. \n");
	atextit(audiosys_close);
}

AudioSys *soundNew() {

	int x = 0;

	for (x = 0; x < soundManager.maxSounds; x++) {
		if (soundManager.soundList[x].inUse == 0) {
			memset(&soundManager.soundList[x], 0, sizeof(AudioSys));
			soundManager.soundList[x].id = soundManager.i++; /** Increment the sound list and id in the soundlist**/
			soundManager.soundList[x].inUse = 1;
			return &soundManager.soundList[x];
		}
	}
	slog("ERROR: No space for additional sounds \n");
	return NULL;
}

AudioSys *sound_get_file_by_path(const char *filepath) {

	int x = 0;

	for (x = 0; x < soundManager.maxSounds; x++) {

		if (strncmp(soundManager.soundList[x].filePath, filepath, FILEPATH_CHAR_LEN) == 0) {
			return &soundManager.soundList[x];
		}
	}
	slog("ERROR: Invalid file path for audio file \n");
	return NULL;
}

AudioSys *sound_load(const char *filepath, float volume, int defaultChannel) {

	AudioSys *sound;

	if (!filepath) {
		slog("ERROR: Empty filepath for audio file\n");
		return NULL;
	}

	sound = sound_get_file_by_path(filepath);

	if (sound) {
		sound->inUse = 1;
		return sound;
	}

	sound = soundNew();
	sound->sound = Mix_LoadWAV(filepath);

	if (!sound->sound) {
		slog("ERROR: failed to load sound file \n");
		sound_free(sound);
		return NULL;
	}

	sound->volume = volume;
	sound->defaultChannel = defaultChannel;
	strncpy(sound->filePath, filepath, FILEPATH_CHAR_LEN);
	return sound;

}

void sound_play(AudioSys *sound, int numLoops, float volume, int channel, int group) {

	float vol = 1.0f;

	if (!sound) {
		slog("ERROR: Cannot play a NULL sound \n");
		return;
	}

	if (volume > 0) {
		vol += volume;
		sound->volume = vol;
	}

	Mix_VolumeChunk(sound->sound, (int)sound->volume);
	Mix_PlayChannel(sound->defaultChannel, sound->sound, numLoops);

}

void play_file_by_path(char *filepath, int numLoops, float volume, int channel, int group) {

	int x = 0;

	for (x = 0; x < soundManager.maxSounds; x++) {
		if (strncmp(soundManager.soundList[x].filePath, filepath, FILEPATH_CHAR_LEN) == 0) {
			sound_play(&soundManager.soundList[x], numLoops, volume, channel, group);
			return;
		}
	}

	slog("ERROR: Filepath did not provide audio \n", filepath);
	return;

}

void sound_adjust_volume(AudioSys * sound, float volume) {

	int x = 0;

	for (x = 0; x < soundManager.maxSounds; x++) {
		if (soundManager.soundList[x].inUse > 0) {
			sound_adjust_volume(&soundManager.soundList[x], volume);
		}
	}
}

void sound_free(AudioSys *sound) {

	if (!sound) {
		slog("ERROR: No audio to free \n");
		return;
	}
	sound->inUse = 0;
}

void sound_delete(AudioSys  *sound) {

	if (!sound) {
		slog("ERROR: No audio to delete \n");
		return;
	}

	if (sound->sound != NULL) {
		Mix_FreeChunk(sound->sound);
	}

	memset(sound, 0, sizeof(AudioSys));

}

void sound_delete_all() {

	int x = 0;

	for (x = 0; x < soundManager.maxSounds; x++) {
		sound_delete(&soundManager.soundList[x]);
	}
}
