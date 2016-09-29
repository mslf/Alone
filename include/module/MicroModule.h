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
#ifndef ALONE_MICROMODULE_H
#define ALONE_MICROMODULE_H

#include <SDL2/SDL.h>
#include "resourceManager/ResourceManager.h"
#include "module/section/MicroSection.h"
#include "module/section/NanoSection.h"
#include "module/NanoModule.h"
#include "module/Connections.h"

struct MicroModule {
    SDL_Point coordinates;
    double scaleX;
    double scaleY;
    struct TextResource* microModuleResource;
    struct TextResource* spriteResource;
    struct ScriptResource* scriptResource;
    struct TextureResource* textureResource;
    struct SoundResource** soundResourcesList;
    struct MicroSection** microSectionsList;
    struct NanoModule** nanoModulesList;
    struct NanoConnection* nanoConnectionsList;
    size_t microSectionsNumber;
    size_t soundResourcesNumber;
    size_t nanoModulesNumber;
    size_t nanoConnectionsNumber;
    size_t currentAnimation;
    size_t currentFrame;
};

struct MicroModule* MicroModule_construct(struct ResourceManager* const resourceManager,
                                          const char* const microModuleResId);
void MicroModule_destruct(struct MicroModule* microModule);

void MicroModule_save(
        const struct MicroModule* const microModule, struct ResourceManager* const resourceManager,
        const char* const microModuleResId);
void MicroModule_addNanoModule(struct MicroModule* microModule, const char* const nanoModuleResId,
                               SDL_Point coordinates);
void MicroModule_removeNanoModule(struct MicroModule* microModule, SDL_Point coordinates);
void MicroModule_addConnection(struct MicroModule* microModule, struct NanoConnection);
void MicroModule_removeConnection(struct MicroModule* microModule, size_t index);
void MicroModule_addMicroSection(struct MicroModule* microModule, struct MicroSection* microSection);
void MicroModule_removeMicroSection(struct MicroModule* microModule, size_t index);
void MicroModule_addCrossConnection(struct MicroModule*, size_t firstNanoModuleIndex, size_t firstNanoSectionIndex,
                                    size_t microSectionIndex);
void MicroModule_removeCrossConnection(struct MicroModule* microModule, size_t microSectionIndex, size_t index);

#endif //ALONE_MICROMODULE_H
