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

#define INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES 10
#define INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES 100
#define INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES 10
#define INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES 10

struct ResourceManager {
    struct TextureResource** textureResourcesList;
    struct TextResource** textResourcesList;
    struct ScriptResource** scriptResourcesList;
    struct SoundResource** soundResourcesList;
    size_t textureResourcesCount;
    size_t textResourcesCount;
    size_t scriptResourcesCount;
    size_t soundResourcesCount;
    size_t allocatedTextureResourcesCount;
    size_t allocatedTextResourcesCount;
    size_t allocatedScriptResourcesCount;
    size_t allocatedSoundResourcesCount;
};

struct ResourceManager* ResourceManager_construct();
void ResourceManager_destruct(struct ResourceManager* rm);

struct TextureResource* ResourceManager_loadTextureResource(struct ResourceManager* rm, struct Renderer* renderer,
                                                            const char* const textureResId);
struct TextResource* ResourceManager_loadTextResource(struct ResourceManager* rm,
                                                            const char* const textResId, unsigned char unique);
struct ScriptResource* ResourceManager_loadScriptResource(struct ResourceManager* rm,
                                                            const char* const scriptResId);
struct SoundResource* ResourceManager_loadSoundResource(struct ResourceManager* rm,
                                                            const char* const soundResId);
void ResourceManager_destructNeedlessTextureResources(struct ResourceManager* rm);
void ResourceManager_destructNeedlessTextResources(struct ResourceManager* rm);
void ResourceManager_destructNeedlessScriptResources(struct ResourceManager* rm);
void ResourceManager_destructNeedlessSoundResources(struct ResourceManager* rm);
void ResourceManager_saveTextResource(struct ResourceManager* rm,
                                              struct TextResource* textResource, const char* const textResId);
#endif //ALONE_RESOURCEMANAGER_H
