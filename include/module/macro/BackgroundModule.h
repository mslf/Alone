//
// Created by mslf on 8/21/16.
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
#ifndef ALONE_BACKGROUNDMODULE_H
#define ALONE_BACKGROUNDMODULE_H

#include <SDL2/SDL.h>
#include "resourceManager/ResourceManager.h"

struct BackgroundModule {
    SDL_Point coordinates;
    double scaleX;
    double scaleY;
    struct TextureResource* textureResource;
};

struct BackgroundModule* BackgroundModule_construct(struct ResourceManager* const resourceManager,
                                                    const char* const * const textureResId, SDL_Point coordinates);
void BackgroundModule_destruct(struct BackgroundModule* backgroundModule);

#endif //ALONE_BACKGROUNDMODULE_H
