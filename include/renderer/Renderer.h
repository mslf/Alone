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
#ifndef ALONE_RENDERER_H
#define ALONE_RENDERER_H

#include <SDL2/SDL.h>
#include "logger/Logger.h"

struct Settings;

 struct Renderer {
     struct Logger* logger;
     SDL_Window* window;
     SDL_Renderer* renderer;
     SDL_Point cameraPosition;
     SDL_Point currentScreenSize;
     SDL_Point virtualScreenSize; // x -- width, y -- heigth
     /*
      * All coordinates of SceneNodes and camera position are virtual. They are relativy projected on the real screen.
      * Also, the camera position is substracted from the rendered SceneNode coordinates.
      */
};

struct Renderer* Renderer_construct(struct Logger* logger, struct Settings* settings);
void Renderer_destruct(struct Renderer* renderer);

SDL_Point Renderer_convertCoordinates(struct Renderer* renderer, SDL_Point point);
SDL_Point Renderer_convertCoordinatesBack(struct Renderer* renderer, SDL_Point point);

#endif //ALONE_RENDERER_H
