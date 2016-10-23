//
// Created by mslf on 9/22/16.
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
#include "resourceManager/SoundResource.h"

struct SoundResource* SoundResource_construct(const char* const path) {
    if (!path)
        return NULL;
    struct SoundResource* soundResource = NULL;
    soundResource = (struct SoundResource*)calloc(1, sizeof(struct SoundResource));
    if (!soundResource)
        return NULL;
    soundResource->id = (char*)malloc(sizeof(char) * (strlen(path) + 1));
    if (!soundResource->id) {
        SoundResource_destruct(soundResource);
        return NULL;
    }
    strcpy(soundResource->id, path);
    soundResource->sound = Mix_LoadWAV(path);
    if (!soundResource->sound) {
        SoundResource_destruct(soundResource);
        return NULL;
    }
    soundResource->pointersCount = 1;
    return soundResource;
}

void SoundResource_destruct(struct SoundResource* soundResource) {
    if(soundResource) {
        if (soundResource->id)
            free(soundResource->id);
        if (soundResource->sound)
            Mix_FreeChunk(soundResource->sound);
        free(soundResource);
    }
}
