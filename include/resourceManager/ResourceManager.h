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
#ifndef ALONE_RESOURCEMANAGER_H
#define ALONE_RESOURCEMANAGER_H

#include <stddef.h>
#include "TextureResource.h"
#include "TextResource.h"
#include "SoundResource.h"
#include "ScriptResource.h"

struct ResourceManager {
    char** textureResourcesIDs;
    char** textResourcesIDs;
    char** scriptResourcesIDs;
    char** soundResourcesIDs;
    size_t textureResourceCount;
    size_t textResourcesCount;
    size_t scriptResourcesCount;
    size_t soundResourcesCount;
    struct TextureResource** textureResourcesList;
    struct TextResource** textResourcesList;
    struct ScriptResource** scriptResourcesList;
    struct SoundResource** soundResourcesList;
};

struct ResourceManager* ResourceManager_construct();
void ResourceManager_destruct(struct ResourceManager* resourceManager);

struct TextureResource* ResourceManager_loadTextureResource(struct ResourceManager* resourceManager,
                                                            const char* const resId);
struct TextResource* ResourceManager_loadTextResource(struct ResourceManager* resourceManager,
                                                            const char* const resId);
struct ScriptResource* ResourceManager_loadScriptResource(struct ResourceManager* resourceManager,
                                                            const char* const resId);
struct SoundResource* ResourceManager_loadSoundResource(struct ResourceManager* resourceManager,
                                                            const char* const resId);
void ResourceManager_saveExistingTextResource(struct ResourceManager* resourceManager, const char* const resId);
void ResourceManager_saveNewTextResource(struct ResourceManager* resourceManager, struct TextResource* textResource,
                                         const char* const resId);

#endif //ALONE_RESOURCEMANAGER_H
