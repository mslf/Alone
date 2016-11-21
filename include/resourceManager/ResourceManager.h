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
 * @file ResourceManager.h
 * @author mslf
 * @date 10 Aug 2016
 * @brief File containing #ResourceManager and it's stuff.
 */
#ifndef ALONE_RESOURCEMANAGER_H
#define ALONE_RESOURCEMANAGER_H

#include <stddef.h>
#include "logger/Logger.h"
#include "TextureResource.h"
#include "TextResource.h"
#include "SoundResource.h"
#include "ScriptResource.h"

/**
 * @brief Some initial constants for #ResourceManager.
 */
enum ResourceManager_constants{
    INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES = 10,
    /**< Init alocating number and reallocating step for ResourceManager#textureResourcesList. */
    INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES = 100,
    /**< Init alocating number and reallocating step for ResourceManager#textResourcesList. */
    INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES = 10,
    /**< Init alocating number and reallocating step for ResourceManager#scriptResourcesList. */
    INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES = 10
    /**< Init alocating number and reallocating step for ResourceManager#soundResourcesList. */
};

/**
 * @brief Resource subsystem.
 * Loads and stores various resources in arrays. Provide effective managment of same resources
 * (each resources will be stored in array once).
 * There is only one exeption: unique #TextResource. If loader of #TextResource wants, he can load one #TextResource twice 
 * and it will be stored in array twice.
 * @note #ResourceManager takes care of each resource 
 * (with only one exeption for ResourceManager_loadTextureResourceFromText()), 
 * which was loaded by ResourceManager_loadTextureResource(),
 * ResourceManager_loadTextureResourceFromText(), ResourceManager_loadTextResource(),
 * ResourceManager_loadScriptResource() and ResourceManager_loadSoundResource(). 
 * So, only #ResourceManager should destruct these resources.
 * @see ResourceManager_loadTextureResource()
 * @see ResourceManager_loadTextureResourceFromText()
 * @see ResourceManager_loadTextResource()
 * @see ResourceManager_loadScriptResource()
 * @see ResourceManager_loadSoundResource()
 */
struct ResourceManager {
    struct Logger* logger;
    /**< Pointer to a #Logger for logging purpose. */
    struct TextureResource** textureResourcesList;
    /**< Array of pointers to #TextureResource. */
    struct TextResource** textResourcesList;
    /**< Array of pointers to #TextResource. */
    struct ScriptResource** scriptResourcesList;
    /**< Array of pointers to #ScriptResource. */
    struct SoundResource** soundResourcesList;
    /**< Array of pointers to #SoundResource. */
    size_t textureResourcesCount;
    /**< Current number of existing #TextureResource in the ResourceManager#textureResourcesList. */
    size_t textResourcesCount;
    /**< Current number of existing #TextResource in the ResourceManager#textResourcesList. */
    size_t scriptResourcesCount;
    /**< Current number of existing #ScriptResource in the ResourceManager#scriptResourcesList. */
    size_t soundResourcesCount;
    /**< Current number of existing #SoundResource in the ResourceManager#soundResourcesList. */
    size_t allocatedTextureResourcesCount;
    /**< Allocated number of #TextureResource in the ResourceManager#textureResourcesList. */
    size_t allocatedTextResourcesCount;
    /**< Allocated number of #TextResource in the ResourceManager#textResourcesList. */
    size_t allocatedScriptResourcesCount;
    /**< Allocated number of #ScriptResource in the ResourceManager#scriptResourcesList. */
    size_t allocatedSoundResourcesCount;
    /**< Allocated number of #SoundResource in the ResourceManager#soundResourcesList. */
};

struct ResourceManager* ResourceManager_construct(struct Logger* logger);
void ResourceManager_destruct(struct ResourceManager* rm);

struct TextureResource* ResourceManager_loadTextureResource(struct ResourceManager* rm, struct Renderer* renderer,
                                                            const char* const textureResId);
struct TextureResource* ResourceManager_loadTextureResourceFromText(struct ResourceManager* rm,
                                                                    struct Renderer* renderer, const char* const text,
                                                                    const char* const fontPath,
                                                                    int size, SDL_Color color);
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
unsigned char ResourceManager_saveTextResource(struct ResourceManager* rm,
                                              struct TextResource* textResource, const char* const textResId);
#endif //ALONE_RESOURCEMANAGER_H
