//
// Created by mslf on 8/11/16.
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
#ifndef ALONE_NANOMODULE_H
#define ALONE_NANOMODULE_H

#include <SDL2/SDL.h>
#include "resourceManager/ResourceManager.h"
#include "module/section/NanoSection.h"

struct NanoModule {
    SDL_Point coordinates;
    double scaleX;
    double scaleY;
    struct TextResource* nanoModuleResource;
    struct TextResource* spriteResource;
    struct ScriptResource* scriptResource;
    struct TextureResource* textureResource;
    struct NanoSection** nanoSectionsList;
    size_t nanoSectionsNumber;
    size_t currentAnimation;
    size_t currentFrame;
};

struct NanoModule* NanoModule_construct(struct ResourceManager* const resourceManager,
                                        const char* const nanoModuleResId);
void NanoModule_destruct(struct NanoModule* nanoModule);

void NanoModule_save(
        const struct NanoModule* const nanoModule, struct ResourceManager* const resourceManager,
        const char* const nanoModuleResId);
void MicroModule_addNanoSection(struct NanoModule* nanoModule, struct NanoSection* nanoSection);
void MicroModule_removeNanoSection(struct NanoModule* nanoModule, size_t index);

#endif //ALONE_NANOMODULE_H
