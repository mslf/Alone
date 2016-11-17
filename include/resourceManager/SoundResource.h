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
#ifndef ALONE_SOUNDRESOURCE_H
#define ALONE_SOUNDRESOURCE_H

#include <SDL2/SDL_mixer.h>

struct SoundResource {
    size_t pointersCount;
    char* id;
    Mix_Chunk* sound;
};

struct SoundResource* SoundResource_construct(const char* const path);
void SoundResource_destruct(struct SoundResource* soundResource);

/**
 * @brief Decreases SoundResource#pointersCount.
 * Useful for deffered resource managment by #ResourceManager.
 * @param soundResource Pointer to a #SoundResource. Can be NULL.
 * @see #ResourceManager
 */
void SoundResource_decreasePointersCounter(struct SoundResource* soundResource);

#endif //ALONE_SOUNDRESOURCE_H
