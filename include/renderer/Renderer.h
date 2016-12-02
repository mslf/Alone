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
 * @file Renderer.h
 * @author mslf
 * @date 10 Aug 2016
 * @brief File containing #Renderer and its stuff.
 */
#ifndef ALONE_RENDERER_H
#define ALONE_RENDERER_H

#include <SDL2/SDL.h>
#include "logger/Logger.h"

struct Settings;

/**
 * @brief Rendering subsystem.
 * @note All coordinates of #SceneNode and camera position are virtual (in Upixels). 
 * They are relativy projected on the real screen using Renderer#virtualScreenSize by Renderer_convertCoordinates().
 */
 struct Renderer {
     struct Logger* logger;
     /**< Pointer to a #Logger for logging purpose. */
     SDL_Window* window;
     /**< Represents a SDL_Window, where all graphic things will happen. */
     SDL_Renderer* renderer;
     /**< SDL_Renderer for drawing and creating textures. */
     SDL_Point cameraPosition;
     /**< Current camera position for relative moving all #SceneNodes and theirs graphic. */
     SDL_Point currentScreenSize;
     /**< Current physical display size in pixels. */
     SDL_Point virtualScreenSize;
     /**< Current virtual display size in Upixels (unit pixels). */
};

/**
 * @brief Constructs #Renderer, inits it and SDL_image and SDL_ttf.
 * @param logger Pointer to a #Logger to set Renderer#logger. Can be NULL.
 * @param settings Pointer to a #Settings, where #Renderer will grab display and window settings. Can be NULL.
 * @return Pointer to an initialized #Renderer or NULL if something failed 
 * (mempry allocating or initializing SDL subsystems).
 * @note Use this function in pair with Renderer_destruct().
 * @see #Settings
 */
struct Renderer* Renderer_construct(struct Logger* logger, struct Settings* settings);

/**
 * @brief Destructs #Renderer and frees memory, used by it.
 * Also it closes SDL subsystems (SDL_image, SDL_ttf) and SDL at all.
 * @param renderer Pointer to a #Renderer. Can be NULL. Can be not fully initialized.
 */
void Renderer_destruct(struct Renderer* renderer);

/**
 * @brief Converts Upixels absolute coordinates to the real pixels, 
 * based on Renderer#virtualScreenSize, Renderer#currentScreenSize and Renderer#cameraPosition.
 * @param renderer Pointer to a #Renderer with needed numbers. Can be NULL.
 * @param point SDL_Point with world absolute coordinates in Upixels.
 * @return SDL_Point with a screen and camera relative coordinates in real pixels.
 */
SDL_Point Renderer_convertCoordinates(struct Renderer* renderer, SDL_Point point);

/**
 * @brief Converts Upixels absolute coordinates to the real pixels, 
 * based on Renderer#virtualScreenSize and Renderer#currentScreenSize.
 * @param renderer Pointer to a #Renderer with needed numbers. Can be NULL.
 * @param point SDL_Point with world absolute coordinates in Upixels.
 * @return SDL_Point with a screen relative coordinates in real pixels.
 */
SDL_Point Renderer_convertCoordinatesA(struct Renderer* renderer, SDL_Point point);

/**
 * @brief Converts real pixels relative coordinates to the Upixels, 
 * based on Renderer#virtualScreenSize, Renderer#currentScreenSize and Renderer#cameraPosition.
 * @param renderer Pointer to a #Renderer with needed numbers. Can be NULL.
 * @param point SDL_Point with a screen and camera relative coordinates in real pixels.
 * @return SDL_Point with world absolute coordinates in Upixels.
 */
SDL_Point Renderer_convertCoordinatesBack(struct Renderer* renderer, SDL_Point point);

/**
 * @brief Converts real pixels relative coordinates to the Upixels, 
 * based on Renderer#virtualScreenSize and Renderer#currentScreenSize.
 * @param renderer Pointer to a #Renderer with needed numbers. Can be NULL.
 * @param point SDL_Point with a screen relative coordinates in real pixels.
 * @return SDL_Point with world absolute coordinates in Upixels.
 */
SDL_Point Renderer_convertCoordinatesBackA(struct Renderer* renderer, SDL_Point point);
#endif //ALONE_RENDERER_H
