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

struct Musican* Musican_construct() {
    struct Musican* musican = NULL;
    musican = (struct Musican*)malloc(sizeof(struct Musican*));
    if (musican) {
        musican->isInitialized = 0;
        if (SDL_Init(SDL_INIT_AUDIO) < 0) {
            fprintf(stderr, "SDL audio initalizing failed! SDL Error: %s\n", SDL_GetError());
            Musican_destruct(musican);
            return NULL;
        }
        if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
            fprintf(stderr, "SDL_mixer initalizing failed! SDL_mixer Error: %s\n", Mix_GetError());
            Musican_destruct(musican);
            return NULL;
        }
        musican->isInitialized = 1;
    } else {
        fprintf(stderr, "Musican initalizing failed!\n");
        Musican_destruct(musican);
        return NULL;
    }
    return musican;
}

void Musican_destruct(struct Musican* musican) {
    free(musican);
}

void Musican_playSound(struct Musican* musican, struct SoundResource* soundResource, int loops) {

}