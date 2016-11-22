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
#include <stdbool.h>
#include "logger/Logger.h"
#include "TextureResource.h"
#include "TextResource.h"
#include "SoundResource.h"
#include "ScriptResource.h"

/**
 * @brief Some initial constants for #ResourceManager.
 */
enum ResourceManager_constants {
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
 * @brief Error codes for #ResourceManager.
 */
enum ResourceManager_errors {
    RM_NO_ERRORS = 0,
    /**< All right, no errors. */
    RM_ERR_NULL_ARGUMENT = 1
    /**< Some of function's argument is NULL. */
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

/**
 * @brief Constructs #ResourceManager and inits it.
 * @param logger Pointer to a logger to set ResourceManager#logger. Can be NULL.
 * @see #ResourceManager_constants
 */
struct ResourceManager* ResourceManager_construct(struct Logger* logger);

/**
 * @brief Destructs #ResourceManager and frees memory, used by it.
 * @note #ResourceManager will destruct <B>all</B> #TextureResource in ResourceManager#textureResourcesList,
 * #TextResource in the ResourceManager#textResourcesList, #ScriptResource in the ResourceManager#scriptResourcesList
 * and #SoundResource in the ResourceManager#soundResourcesList.
 */
void ResourceManager_destruct(struct ResourceManager* rm);

/**
 * @brief Loads #TextureResource from filesystem and adds it to the ResourceManager#textureResourcesList 
 * or finds already exsisting the same #TextureResource in ResourceManager#textureResourcesList 
 * (and increases its TextureResource#pointersCount).
 * Also, ResourceManager#textureResourcesCount will be increased and 
 * ResourceManager#allocatedTextureResourcesCount (if needed), if new #TextureResource loaded.
 * @param rm Pointer to a #ResourceManager where to store #TextureResource. Can be NULL.
 * @param renderer Pointer to a #Renderer for constructing #TextureResource. Can be NULL.
 * @param textureResId String with a path to the #TextureResource file in filesystem. Can be NULL.
 * @return Pointer to a #TextureResource from ResourceManager#textureResourcesList, or NULL if something failed.
 * @see #TextureResource
 * @note Please, don't try to destruct returned #TextureResource.
 */
struct TextureResource* ResourceManager_loadTextureResource(struct ResourceManager* rm,
                                                            struct Renderer* renderer,
                                                            const char* const textureResId);

/**
 * @brief Generates #TextureResource from text and adds it to the ResourceManager#textureResourcesList 
 * or finds already exsisting the same #TextureResource in ResourceManager#textureResourcesList 
 * (and increases its TextureResource#pointersCount).
 * Also, ResourceManager#textureResourcesCount will be increased and 
 * ResourceManager#allocatedTextureResourcesCount (if needed), if new #TextureResource loaded.
 * @param rm Pointer to a #ResourceManager where to store #TextureResource. Can be NULL.
 * If you pass NULL here, then function will generates #TextureResource without saving it 
 * in ResourceManager#textureResourcesList!
 * So, you <B>should</B> destruct it by yourself.
 * @param renderer Pointer to a #Renderer for constructing #TextureResource. Can be NULL.
 * @param text String to be rendered in the #TextureResource. Can be NULL.
 * @param fontPath String with a path to the ttf font in the filesystem. Can be NULL.
 * @param size Vertical size of text in #TextureResource in Upixels.
 * @param color SDL_Color of text in #TextureResource.
 * @return Pointer to a #TextureResource from ResourceManager#textureResourcesList, or NULL if something failed.
 * @see #TextureResource
 * @note Please, don't try to destruct returned #TextureResource (with exeption).
 * @warning Passing NULL #ResourceManager changes behaviour of the function.
 * All returned #TextureResource, generated with NULL #ResourceManager, <B>should</B> be destructed by you.
 */
struct TextureResource* ResourceManager_loadTextureResourceFromText(struct ResourceManager* rm,
                                                                    struct Renderer* renderer,
                                                                    const char* const text,
                                                                    const char* const fontPath,
                                                                    int size, SDL_Color color);

/**
 * @brief Loads #TextResource from filesystem and adds it to the ResourceManager#textResourcesList 
 * or finds already exsisting the same #TextResource in ResourceManager#textResourcesList 
 * (and increases its TextResource#pointersCount).
 * Also, ResourceManager#textResourcesCount will be increased and 
 * ResourceManager#allocatedTextResourcesCount (if needed), if new #TextResource loaded.
 * @param rm Pointer to a #ResourceManager where to store #TextResource. Can be NULL.
 * @param renderer Pointer to a #Renderer for constructing #TextResource. Can be NULL.
 * @param textResId String with a path to the #TextResource file in filesystem. Can be NULL.
 * @param unique Flag to load #TextResource twice and more from filesystem, even it exists in ResourceManager#textResourcesList.
 * @return Pointer to a #TextResource from ResourceManager#textResourcesList, or NULL if something failed.
 * @see #TextResource
 * @note Please, don't try to destruct returned #TextResource.
 * @note This function will find non-unique #TextResource in ResourceManager#textResourcesList only 
 * if you've passed false unique flag. If unique flag is true or #TextResource is unique, then nothing will be found.
 * So, you can't access unique #TextResource from outside world, if someone who loaded this #TextResource is 
 * not you (or not in your scope right now).
 */
struct TextResource* ResourceManager_loadTextResource(struct ResourceManager* rm,
                                                      const char* const textResId,
                                                      bool unique);

/**
 * @brief Loads #ScriptResource from filesystem and adds it to the ResourceManager#scriptResourcesList 
 * or finds already exsisting the same #ScriptResource in ResourceManager#scriptResourcesList 
 * (and increases its ScriptResource#pointersCount).
 * Also, ResourceManager#scriptResourcesCount will be increased and 
 * ResourceManager#allocatedScriptResourcesCount (if needed), if new #ScriptResource loaded.
 * @param rm Pointer to a #ResourceManager where to store #ScriptResource. Can be NULL.
 * @param renderer Pointer to a #Renderer for constructing #ScriptResource. Can be NULL.
 * @param scriptResId String with a path to the #ScriptResource file in filesystem. Can be NULL.
 * @return Pointer to a #ScriptResource from ResourceManager#scriptResourcesList, or NULL if something failed.
 * @see #ScriptResource
 * @note Please, don't try to destruct returned #ScriptResource.
 */
struct ScriptResource* ResourceManager_loadScriptResource(struct ResourceManager* rm,
                                                          const char* const scriptResId);

/**
 * @brief Loads #SoundResource from filesystem and adds it to the ResourceManager#soundResourcesList 
 * or finds already exsisting the same #SoundResource in ResourceManager#soundResourcesList 
 * (and increases its SoundResource#pointersCount).
 * Also, ResourceManager#soundResourcesCount will be increased and 
 * ResourceManager#allocatedSoundResourcesCount (if needed), if new #SoundResource loaded.
 * @param rm Pointer to a #ResourceManager where to store #SoundResource. Can be NULL.
 * @param renderer Pointer to a #Renderer for constructing #SoundResource. Can be NULL.
 * @param soundResId String with a path to the #SoundResource file in filesystem. Can be NULL.
 * @return Pointer to a #SoundResource from ResourceManager#soundResourcesList, or NULL if something failed.
 * @see #SoundResource
 * @note Please, don't try to destruct returned #SoundResource.
 */
struct SoundResource* ResourceManager_loadSoundResource(struct ResourceManager* rm,
                                                        const char* const soundResId);

/**
 * @brief Function for deferred removing and destruction needless 
 * (if TextureResource#pointersCount is 0) #TextureResource in ResourceManager#textureResourcesList.
 * @param rm Pointer to a #ResourceManager. Can be NULL.
 * @see #TextureResource
 */
void ResourceManager_destructNeedlessTextureResources(struct ResourceManager* rm);

/**
 * @brief Function for deferred removing and destruction needless 
 * (if TextResource#pointersCount is 0) #TextResource in ResourceManager#textResourcesList.
 * @param rm Pointer to a #ResourceManager. Can be NULL.
 * @see #TextResource
 */
void ResourceManager_destructNeedlessTextResources(struct ResourceManager* rm);

/**
 * @brief Function for deferred removing and destruction needless 
 * (if ScriptResource#pointersCount is 0) #ScriptResource in ResourceManager#scriptResourcesList.
 * @param rm Pointer to a #ResourceManager. Can be NULL.
 * @see #ScriptResource
 */
void ResourceManager_destructNeedlessScriptResources(struct ResourceManager* rm);

/**
 * @brief Function for deferred removing and destruction needless 
 * (if SoundResource#pointersCount is 0) #SoundResource in ResourceManager#soundResourcesList.
 * @param rm Pointer to a #ResourceManager. Can be NULL.
 * @see #SoundResource
 */
void ResourceManager_destructNeedlessSoundResources(struct ResourceManager* rm);

/**
 * @brief Saves #TextResource in the filesystem.
 * @param rm Pointer to a #ResourceManager. Can be NULL.
 * @param textResource Pointer to a #TextResource, which will be saved. Can be NULL.
 * @param textResId String with a path, where to save #TextResource. Can be NULL.
 * @return #ResourceManager_errors value.
 * @see #TextResource
 * @see #ResourceManager_errors
 */
enum ResourceManager_errors ResourceManager_saveTextResource(struct ResourceManager* rm,
                                                             struct TextResource* textResource,
                                                             const char* const textResId);
#endif //ALONE_RESOURCEMANAGER_H
