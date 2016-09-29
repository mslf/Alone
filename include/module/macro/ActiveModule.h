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
#ifndef ALONE_ACTIVEMODULE_H
#define ALONE_ACTIVEMODULE_H

#include <SDL2/SDL.h>
#include "resourceManager/ResourceManager.h"
#include "module/section/MacroSection.h"
#include "module/section/MicroSection.h"
#include "module/section/NanoSection.h"
#include "module/MicroModule.h"
#include "module/Connections.h"

struct ActiveModule {
    SDL_Point coordinates;
    double scaleX;
    double scaleY;
    struct TextResource* activeModuleResource;
    struct TextResource* spriteResource;
    struct ScriptResource* scriptResource;
    struct TextureResource* textureResource;
    struct SoundResource** soundResourcesList;
    struct MacroSection** macroSectionsList;
    struct MicroModule** microModulesList;
    struct MicroConnection* microConnectionsList;
    size_t macroSectionsNumber;
    size_t soundResourcesNumber;
    size_t microModulesNumber;
    size_t microConnectionsNumber;
    size_t currentAnimation;
    size_t currentFrame;
};

struct ActiveModule* ActiveModule_construct(struct ResourceManager* const resourceManager,
                                            const char* const activeModuleResId);
void ActiveModule_destruct(struct ActiveModule* activeModule);

void ActiveModule_save(
        const struct ActiveModule* const activeModule, struct ResourceManager* const resourceManager,
        const char* const activeModuleResId);
void ActiveModule_addMicroModule(struct ActiveModule* activeModule, const char* const microModuleResId,
                                 SDL_Point coordinates);
void ActiveModule_removeMicroModule(struct ActiveModule* activeModule, SDL_Point coordinates);
void ActiveModule_addConnection(struct ActiveModule* activeModule, struct MicroConnection);
void ActiveModule_removeConnection(struct ActiveModule* activeModule, size_t index);
void ActiveModule_addMacroSection(struct ActiveModule* activeModule, struct MacroSection* macroSection);
void ActiveModule_removeMacroSection(struct ActiveModule* activeModule, size_t index);

#endif //ALONE_ACTIVEMODULE_H
