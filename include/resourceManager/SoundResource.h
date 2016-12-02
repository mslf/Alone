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
/**
 * @file SoundResource.h
 * @author mslf
 * @date 22 Sep 2016
 * @brief File containing #SoundResource and its stuff.
 */
#ifndef ALONE_SOUNDRESOURCE_H
#define ALONE_SOUNDRESOURCE_H

#include <SDL2/SDL_mixer.h>

/**
 * @brief Resource, which is represents playable sound (not music!).
 */
struct SoundResource {
    size_t pointersCount;
    /**< Count of pointers to this resource. */
    char* id;
    /**< ID (path) string of this resource. */
    Mix_Chunk* sound;
    /**< SDL_mixer sound. */
};

/**
 * @brief Constructs #SoundResource and inits it.
 * @param path String with path to the wav sound file. Can be NULL.
 * @return Pointer to a #SoundResource or NULL, if something failed.
 */
struct SoundResource* SoundResource_construct(const char* const path);

/**
 * @brief Destructs #SoundResource and frees memory, used by it.
 * @param soundResource Pointer to a #SoundResource. Can be NULL. Can be not fully initialized.
 */
void SoundResource_destruct(struct SoundResource* soundResource);

/**
 * @brief Decreases SoundResource#pointersCount.
 * Useful for deffered resource managment by #ResourceManager.
 * @param soundResource Pointer to a #SoundResource. Can be NULL.
 * @see #ResourceManager
 */
void SoundResource_decreasePointersCounter(struct SoundResource* soundResource);

#endif //ALONE_SOUNDRESOURCE_H
