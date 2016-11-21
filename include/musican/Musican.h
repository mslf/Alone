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
 * @file Musican.h
 * @author mslf
 * @date 10 Aug 2016
 * @brief File containing #Musican and it's stuff.
 */
#ifndef ALONE_MUSICAN_H
#define ALONE_MUSICAN_H

#include <stdbool.h>
#include "logger/Logger.h"
#include "resourceManager/SoundResource.h"

/**
 * @brief Music subsystem.
 * @note Music playing is not implemented yet.
 */
struct Musican {
    struct Logger* logger;
    /**< Pointer to a #Logger for logging purpose. */
    bool isSoundActive;
    /**< Flag indicates are we need to play sounds or not. */
    bool isMusicActive;
    /**< Flag indicates are we need to play musics or not. */
};

/**
 * @brief Constructs #Musican and inits SDL audio subsystem and SDL_mixer.
 * @param logger Pointer to a #Logger to set Musican#logger. Can be NULL.
 * @param sound Flag to set Musican#isSoundActive.
 * @param music Flag to set Musican#isMusicActive.
 * @return Pointer to an initialized #Musican or NULL, if constructing #Musican or 
 * initializing SDL audio or SDL_mixer failed.
 * @note Use this function in pair with Musican_destruct().
 */
struct Musican* Musican_construct(struct Logger* logger, bool sound, bool music);

/**
 * @brief Destructs #Musican and frees memory, used by it.
 * @param musican Pointer to a #Musican. Can be NULL. Can be not fully initialized.
 */
void Musican_destruct(struct Musican* musican);

/**
 * @brief Function for playing sound.
 * @param musican Pointer to a #Musican. Can be NULL.
 * @param soundResource Pointer to a #SoundResource to be played. Can be NULL.
 * @param loops Number of playing sound loops.
 */
void Musican_playSound(struct Musican* musican, struct SoundResource* soundResource, int loops);

#endif //ALONE_MUSICAN_H
