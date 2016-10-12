//
// Created by mslf on 8/10/16.
//
/*
	Copyright 2016 Golikov Vitaliy

	This file is part of Alone.

	Alone is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Alone is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Alone. If not, see <http://www.gnu.org/licenses/>.
*/
#include "musican/Musican.h"
#include <SDL2/SDL.h>

const char* const MUSICAN_ERR_ALLOC = "Musican: constructor: allocating memory failed!";
const char* const MUSICAN_ERR_SDL_INIT_AUDIO = "Musican: constructor: SDL_Init audio failed!";
const char* const MUSICAN_ERR_MIX_OPEN_AUDIO = "Musican: constructor: SDL_mixer opening audio failed!";

struct Musican* Musican_construct(struct Logger* logger) {
    struct Musican* musican = NULL;
    musican = (struct Musican*)malloc(sizeof(struct Musican*));
    if (!musican) {
        Logger_log(logger, MUSICAN_ERR_ALLOC);
        Musican_destruct(musican);
        return NULL;
    }
    musican->isInitialized = 0;
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        char tempString[600];
        sprintf(tempString, "%s. SDL error: %s", MUSICAN_ERR_SDL_INIT_AUDIO, SDL_GetError);
        Logger_log(logger, tempString);
        Musican_destruct(musican);
        return NULL;
    }
    if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
        char tempString[600];
        sprintf(tempString, "%s. SDL_mixer error: %s", MUSICAN_ERR_MIX_OPEN_AUDIO, Mix_GetError);
        Logger_log(logger, tempString);
        Musican_destruct(musican);
        return NULL;
    }
    musican->isInitialized = 1;
    musican->logger = logger;
    return musican;
}

void Musican_destruct(struct Musican* musican) {
    if (musican)
        free(musican);
    Mix_Quit();
    SDL_Quit();
}

void Musican_playSound(struct Musican* musican, struct SoundResource* soundResource, int loops) {
    if (musican && soundResource && musican->isInitialized)
        Mix_PlayChannel(-1, soundResource->sound, loops);
}