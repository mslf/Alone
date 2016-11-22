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
 * @file ResourceManager.c
 * @author mslf
 * @date 10 Aug 2016
 * @brief File containing implementation of #ResourceManager.
 */
#include "resourceManager/ResourceManager.h"
#include <string.h>

/**
 * @brief Error message strings for #ResourceManager.
 */
static const struct ResourceManager_errorMessages {
    const char* const errAlloc;
    /**< Will be displayed when allocating memory for #ResourceManager failed. */
    const char* const errTextureResListAlloc;
    /**< Will be displayed when allocating memory for ResourceManager#textureResourcesList faiiled. */
    const char* const errTextResListAlloc;
    /**< Will be displayed when allocating memory for ResourceManager#textResourcesList failed. */
    const char* const errScriptResListAlloc;
    /**< Will be displayed when allocating memory for ResourceManager#scriptResourcesList failed. */
    const char* const errSoundResListAlloc;
    /**< Will be displayed when allocating memory for ResourceManager#soundResourcesList failed. */
    const char* const errLoadTextureRes;
    /**< Will be displayed when loading or generating #TextureResource failed. */
    const char* const errGeneratingTextureRes;
    /**< Will be displayed when generating #TextureResource from text failed.*/
    const char* const errLoadTextRes;
    /**< Will be displayed when loading #TextResource failed. */
    const char* const errLoadScriptRes;
    /**< Will be displayed when loading #ScriptResource failed. */
    const char* const errLoadSoundRes;
    /**< Will be displayed when loading #SoundResource failed. */
}ResourceManager_errorMessages = {
    "ResourceManager_construct: allocating memory failed!",
    "ResourceManager_construct: allocating for textureResourcesList failed!",
    "ResourceManager_construct: allocating for textResourcesList failed!",
    "ResourceManager_construct: allocating for scriptResourcesList failed!",
    "ResourceManager_construct: allocating for soundResourcesList failed!",
    "ResourceManager_loadTextureResource: constructing TextureResource failed!",
    "ResourceManager_loadTextureResourceFromText: Failed constructng non-cached texture!",
    "ResourceManager_loadTextResource: constructing TextResource failed!",
    "ResourceManager_loadScriptResource: constructing ScriptResource failed!",
    "ResourceManager_loadSoundResource: constructing SoundResource failed!"};

/**
 * @brief Allocates memory for ResourceManager#textureResourcesList and
 * sets ResourceManager#allocatedTextureResourcesCount.
 * @param rm Pointer to a #ResourceManager, where to allocate memory.
 * @return #ResourceManager_errors value.
 * @see #ResourceManager_errors
 * @see #ResourceManager_constants
 */ 
static enum ResourceManager_errors ResourceManager_constructTextureResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return RM_ERR_NULL_ARGUMENT;
    if (!(rm->textureResourcesList = (struct TextureResource**)malloc(
            sizeof(struct TextureResource*) * RM_INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES))) {
        Logger_log(rm->logger, ResourceManager_errorMessages.errTextureResListAlloc);
        return RM_ERR_ALLOC;
    }
    rm->allocatedTextureResourcesCount = RM_INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    return RM_NO_ERRORS;
}

/**
 * @brief Allocates memory for ResourceManager#textResourcesList and
 * sets ResourceManager#allocatedTextResourcesCount.
 * @param rm Pointer to a #ResourceManager, where to allocate memory.
 * @return #ResourceManager_errors value.
 * @see #ResourceManager_errors
 * @see #ResourceManager_constants
 */ 
static enum ResourceManager_errors ResourceManager_constructTextResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return RM_ERR_NULL_ARGUMENT;
    if (!(rm->textResourcesList = (struct TextResource**)malloc(
            sizeof(struct TextResource*) * RM_INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES))) {
        Logger_log(rm->logger, ResourceManager_errorMessages.errTextResListAlloc);
        return RM_ERR_ALLOC;
    }
    rm->allocatedTextResourcesCount = RM_INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES;
    return RM_NO_ERRORS;
}

/**
 * @brief Allocates memory for ResourceManager#scriptResourcesList and
 * sets ResourceManager#allocatedScriptResourcesCount.
 * @param rm Pointer to a #ResourceManager, where to allocate memory.
 * @return #ResourceManager_errors value.
 * @see #ResourceManager_errors
 * @see #ResourceManager_constants
 */ 
static enum ResourceManager_errors ResourceManager_constructScriptResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return RM_ERR_NULL_ARGUMENT;
    if (!(rm->scriptResourcesList = (struct ScriptResource**)malloc(
            sizeof(struct ScriptResource*) * RM_INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES))) {
        Logger_log(rm->logger, ResourceManager_errorMessages.errScriptResListAlloc);
        return RM_ERR_ALLOC;
    }
    rm->allocatedScriptResourcesCount = RM_INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES;
    return RM_NO_ERRORS;
}

/**
 * @brief Allocates memory for ResourceManager#soundResourcesList and
 * sets ResourceManager#allocatedSoundResourcesCount.
 * @param rm Pointer to a #ResourceManager, where to allocate memory.
 * @return #ResourceManager_errors value.
 * @see #ResourceManager_errors
 * @see #ResourceManager_constants
 */ 
static enum ResourceManager_errors ResourceManager_constructSoundResourcesMap(struct ResourceManager* rm) {
    if (!rm)
        return RM_ERR_NULL_ARGUMENT;
    if (!(rm->soundResourcesList = (struct SoundResource**)malloc(
            sizeof(struct SoundResource*) * RM_INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES))) {
        Logger_log(rm->logger, ResourceManager_errorMessages.errSoundResListAlloc);
        return RM_ERR_ALLOC;
    }
    rm->allocatedSoundResourcesCount = RM_INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES;
    return RM_NO_ERRORS;
}

struct ResourceManager* ResourceManager_construct(struct Logger* logger) {
    struct ResourceManager* rm = NULL;
    rm = (struct ResourceManager*)calloc(1, sizeof(struct ResourceManager));
    if (!rm) {
        Logger_log(logger, ResourceManager_errorMessages.errAlloc);
        return NULL;
    }
    rm->logger = logger;
    enum ResourceManager_errors constructingTextureResList = RM_NO_ERRORS;
    enum ResourceManager_errors constructingTextResList = RM_NO_ERRORS;
    enum ResourceManager_errors constructingScriptResList = RM_NO_ERRORS;
    enum ResourceManager_errors constructingSoundResList = RM_NO_ERRORS;
    constructingTextureResList = ResourceManager_constructTextureResourcesList(rm);
    constructingTextResList = ResourceManager_constructTextResourcesList(rm);
    constructingScriptResList = ResourceManager_constructScriptResourcesList(rm);
    constructingSoundResList = ResourceManager_constructSoundResourcesMap(rm);
    if (constructingTextureResList || constructingTextResList 
        || constructingScriptResList || constructingSoundResList) {
        ResourceManager_destruct(rm);
        return NULL;
    }
    return rm;
}

/**
 * @brief Destructs ResourceManager#textureResourcesList and frees memory, used by it.
 * Also destructs all #TextureResource in that list.
 * @param rm Pointer to a #ResourceManager. Can be NULL. Can be not fully initialized.
 */
void ResourceManager_destructTextureResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return;
    if (rm->textureResourcesList) {
        if (rm->textureResourcesCount > 0)
            for (size_t i = 0; i < rm->textureResourcesCount; i++)
                TextureResource_destruct(rm->textureResourcesList[i]);
        free(rm->textureResourcesList);
    }
}

/**
 * @brief Destructs ResourceManager#textResourcesList and frees memory, used by it.
 * Also destructs all #TextResource in that list.
 * @param rm Pointer to a #ResourceManager. Can be NULL. Can be not fully initialized.
 */
void ResourceManager_destructTextResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return;
    if (rm->textResourcesList) {
        if (rm->textResourcesCount > 0)
            for (size_t i = 0; i < rm->textResourcesCount; i++)
                TextResource_destruct(rm->textResourcesList[i]);
        free(rm->textResourcesList);
    }
}

/**
 * @brief Destructs ResourceManager#scriptResourcesList and frees memory, used by it.
 * Also destructs all #ScriptResource in that list.
 * @param rm Pointer to a #ResourceManager. Can be NULL. Can be not fully initialized.
 */
void ResourceManager_destructScriptResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return;
    if (rm->scriptResourcesList) {
        if (rm->scriptResourcesCount > 0)
            for (size_t i = 0; i < rm->scriptResourcesCount; i++)
                ScriptResource_destruct(rm->scriptResourcesList[i]);
        free(rm->scriptResourcesList);
    }
}

/**
 * @brief Destructs ResourceManager#soundResourcesList and frees memory, used by it.
 * Also destructs all #SoundResource in that list.
 * @param rm Pointer to a #ResourceManager. Can be NULL. Can be not fully initialized.
 */
void ResourceManager_destructSoundResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return;
    if (rm->soundResourcesList) {
        if (rm->soundResourcesCount > 0)
            for (size_t i = 0; i < rm->soundResourcesCount; i++)
                SoundResource_destruct(rm->soundResourcesList[i]);
        free(rm->soundResourcesList);
    }
}

/**
 * @brief Destructs ResourceManager and frees memory, used by it.
 * @param rm Pointer to a #ResourceManager. Can be NULL. Can be not fully initialized.
 */
void ResourceManager_destruct(struct ResourceManager* rm) {
    if (!rm)
        return;
    ResourceManager_destructTextureResourcesList(rm);
    ResourceManager_destructTextResourcesList(rm);
    ResourceManager_destructScriptResourcesList(rm);
    ResourceManager_destructSoundResourcesList(rm);
    free(rm);
}

/**
 * @brief Reallocates memory for the ResourceManager#textureResourcesList.
 * ResourceManager#allocatedTextureResourcesCount will be increased by 
 * #RM_INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES.
 * @param rm Pointer to a #ResourceManager. Can be NULL.
 * @return #ResourceManager_errors value.
 * @see #ResourceManager_errors
 */
static enum ResourceManager_errors ResourceManager_reallocateTextureResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return RM_ERR_NULL_ARGUMENT;
    struct TextureResource** textureResourcesList = NULL;
    size_t newSize = rm->allocatedTextureResourcesCount + RM_INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    if (!(textureResourcesList = (struct TextureResource**)malloc(sizeof(struct TextureResource*) * newSize))) {
        Logger_log(rm->logger, ResourceManager_errorMessages.errTextureResListAlloc);
        return RM_ERR_ALLOC;
    }
    for (size_t i = 0; i < rm->textureResourcesCount; i++)
        textureResourcesList[i] = rm->textureResourcesList[i];
    free(rm->textureResourcesList);
    rm->textureResourcesList = textureResourcesList;
    rm->allocatedTextureResourcesCount = newSize;
    return RM_NO_ERRORS;
}

/**
 * @brief Reallocates memory for the ResourceManager#textResourcesList.
 * ResourceManager#allocatedTextResourcesCount will be increased by 
 * #RM_INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES.
 * @param rm Pointer to a #ResourceManager. Can be NULL.
 * @return #ResourceManager_errors value.
 * @see #ResourceManager_errors
 */
static enum ResourceManager_errors ResourceManager_reallocateTextResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return RM_ERR_NULL_ARGUMENT;
    struct TextResource** textResourcesList = NULL;
    size_t newSize = rm->allocatedTextResourcesCount + RM_INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES;
    if (!(textResourcesList = (struct TextResource**)malloc(sizeof(struct TextResource*) * newSize))) {
        Logger_log(rm->logger, ResourceManager_errorMessages.errTextResListAlloc);
        return RM_ERR_ALLOC;
    }
    for (size_t i = 0; i < rm->textResourcesCount; i++)
        textResourcesList[i] = rm->textResourcesList[i];
    free(rm->textResourcesList);
    rm->textResourcesList = textResourcesList;
    rm->allocatedTextResourcesCount = newSize;
    return RM_NO_ERRORS;
}

/**
 * @brief Reallocates memory for the ResourceManager#scriptResourcesList.
 * ResourceManager#allocatedScriptResourcesCount will be increased by 
 * #RM_INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES.
 * @param rm Pointer to a #ResourceManager. Can be NULL.
 * @return #ResourceManager_errors value.
 * @see #ResourceManager_errors
 */
static enum ResourceManager_errors ResourceManager_reallocateScriptResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return RM_ERR_NULL_ARGUMENT;
    struct ScriptResource** scriptResourcesList = NULL;
    size_t newSize = rm->allocatedScriptResourcesCount + RM_INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES;
    if (!(scriptResourcesList = (struct ScriptResource**)malloc(sizeof(struct ScriptResource*) * newSize))) {
        Logger_log(rm->logger, ResourceManager_errorMessages.errScriptResListAlloc);
        return RM_ERR_ALLOC;
    }
    for (size_t i = 0; i < rm->scriptResourcesCount; i++)
        scriptResourcesList[i] = rm->scriptResourcesList[i];
    free(rm->scriptResourcesList);
    rm->scriptResourcesList = scriptResourcesList;
    rm->allocatedScriptResourcesCount = newSize;
    return RM_NO_ERRORS;
}

/**
 * @brief Reallocates memory for the ResourceManager#soundResourcesList.
 * ResourceManager#allocatedSoundResourcesCount will be increased by 
 * #RM_INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES.
 * @param rm Pointer to a #ResourceManager. Can be NULL.
 * @return #ResourceManager_errors value.
 * @see #ResourceManager_errors
 */
static enum ResourceManager_errors ResourceManager_reallocateSoundResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return RM_ERR_NULL_ARGUMENT;
    struct SoundResource** soundResourcesList = NULL;
    size_t newSize = rm->allocatedSoundResourcesCount + RM_INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES;
    if (!(soundResourcesList = (struct SoundResource**)malloc(sizeof(struct SoundResource*) * newSize))) {
        Logger_log(rm->logger, ResourceManager_errorMessages.errSoundResListAlloc);
        return RM_ERR_ALLOC;
    }
    for (size_t i = 0; i < rm->soundResourcesCount; i++)
        soundResourcesList[i] = rm->soundResourcesList[i];
    free(rm->soundResourcesList);
    rm->soundResourcesList = soundResourcesList;
    rm->allocatedSoundResourcesCount = newSize;
    return RM_NO_ERRORS;
}

struct TextureResource* ResourceManager_loadTextureResource(struct ResourceManager* rm,
                                                            struct Renderer* renderer,
                                                            const char* const textureResId) {
    if (!rm || !textureResId || !renderer)
        return NULL;
    bool found = false;
    size_t foundIndex = 0;
    // Firstly, try to find existing textureResource in list and return it.
    // If no textureResource found, try to load and add new, then return it.
    for (size_t i = 0; i < rm->textureResourcesCount; i++)
        if (strcmp(textureResId, rm->textureResourcesList[i]->id) == 0) {
            found = true;
            foundIndex = i;
            break;
        }
    if (!found) {
        struct TextureResource* textureResource = NULL;
        textureResource = TextureResource_construct(renderer, textureResId);
        if (!textureResource) {
            Logger_log(rm->logger, "%s ResourceID: %s", ResourceManager_errorMessages.errLoadTextureRes, textureResId);
            return NULL;
        }
        // Try to reallocate (if needed) and add textureResource to the list
        if (rm->textureResourcesCount >= rm->allocatedTextureResourcesCount)
            if (ResourceManager_reallocateTextureResourcesList(rm)) {
                TextureResource_destruct(textureResource);
                return NULL;
            }
        rm->textureResourcesList[rm->textureResourcesCount] = textureResource;
        rm->textureResourcesCount++;
        return textureResource;
    }
    return rm->textureResourcesList[foundIndex];
}

struct TextureResource* ResourceManager_loadTextureResourceFromText(struct ResourceManager* rm,
                                                                    struct Renderer* renderer,
                                                                    const char* const text,
                                                                    const char* const fontPath,
                                                                    int size, SDL_Color color) {
    if (!text || !renderer || !fontPath || size <= 0)
        return NULL;
    if (rm) {
        char* textureResId = TextureResource_convertTextParametersToString(text, fontPath, size, color);
        if (!textureResId)
            return NULL;
        bool found = false;
        size_t foundIndex = 0;
        // Firstly, try to find existing textureResource in list and return it.
        // If no textureResource found, try to load and add new, then return it.
        for (size_t i = 0; i < rm->textureResourcesCount; i++)
            if (strcmp(textureResId, rm->textureResourcesList[i]->id) == 0) {
                found = true;
                foundIndex = i;
                break;
            }
        if (!found) {
            struct TextureResource* textureResource = NULL;
            textureResource = TextureResource_constructFromText(renderer, text, fontPath, size, color);
            if (!textureResource) {
                Logger_log(rm->logger, "%s ResourceID: %s", ResourceManager_errorMessages.errLoadTextureRes, textureResId);
                free(textureResId);
                return NULL;
            }
            textureResource->isCreatedWithResourceManager = true;
            // Try to reallocate (if needed) and add textureResource to the list
            if (rm->textureResourcesCount >= rm->allocatedTextureResourcesCount)
                if (ResourceManager_reallocateTextureResourcesList(rm)) {
                    TextureResource_destruct(textureResource);
                    free(textureResId);
                    return NULL;
                }
            rm->textureResourcesList[rm->textureResourcesCount] = textureResource;
            rm->textureResourcesCount++;
            free(textureResId);
            return textureResource;
        }
        free(textureResId);
        return rm->textureResourcesList[foundIndex];
    } else { // If we don't want to cache text texture (WARNING! You SHOULD destruct this textureResource by yourself!)
        struct TextureResource* textureResource = NULL;
        textureResource = TextureResource_constructFromText(renderer, text, fontPath, size, color);
        if (!textureResource) {
            Logger_log(renderer->logger, ResourceManager_errorMessages.errGeneratingTextureRes);
            return NULL;
        }
        return textureResource;
    }
}

struct TextResource* ResourceManager_loadTextResource(struct ResourceManager* rm,
                                                      const char* const textResId,
                                                      bool unique) {
    if (!rm || !textResId)
        return NULL;
    bool found = false;
    size_t foundIndex = 0;
    // Firstly, try to find existing non-unique textResource in list and return it.
    // If no textResource found, try to load and add new, then return it.
    if (!unique)
        for (size_t i = 0; i < rm->textResourcesCount; i++)
            if (!rm->textResourcesList[i]->isUnique && strcmp(textResId, rm->textResourcesList[i]->id) == 0) {
                found = true;
                foundIndex = i;
                break;
            }
    if (found)
        return rm->textResourcesList[foundIndex];
    struct TextResource* textResource = NULL;
    textResource = TextResource_construct(textResId, unique);
    if (!textResource) {
        Logger_log(rm->logger, "%s ResourceID: %s", ResourceManager_errorMessages.errLoadTextRes, textResId);
        return NULL;
    }
    // Try to reallocate (if needed) and add textResource to the list
    if (rm->textResourcesCount >= rm->allocatedTextResourcesCount)
        if (ResourceManager_reallocateTextResourcesList(rm)) {
            TextResource_destruct(textResource);
            return NULL;
        }
    rm->textResourcesList[rm->textResourcesCount] = textResource;
    rm->textResourcesCount++;
    return textResource;
}

struct ScriptResource* ResourceManager_loadScriptResource(struct ResourceManager* rm,
                                                          const char* const scriptResId) {
    if (!rm || !scriptResId)
        return NULL;
    bool found = false;
    size_t foundIndex = 0;
    // Firstly, try to find existing scriptResource in list and return it.
    // If no scriptResource found, try to load and add new, then return it.
    for (size_t i = 0; i < rm->scriptResourcesCount; i++)
        if (strcmp(scriptResId, rm->scriptResourcesList[i]->id) == 0) {
            found = true;
            foundIndex = i;
            break;
        }
    if (!found) {
        struct ScriptResource* scriptResource = NULL;
        scriptResource = ScriptResource_construct(scriptResId);
        if (!scriptResource) {
            Logger_log(rm->logger, "%s ResourceID: %s", ResourceManager_errorMessages.errLoadScriptRes, scriptResId);
            return NULL;
        }
        // Try to reallocate (if needed) and add scriptResource to the list
        if (rm->scriptResourcesCount >= rm->allocatedScriptResourcesCount)
            if (ResourceManager_reallocateScriptResourcesList(rm)) {
                ScriptResource_destruct(scriptResource);
                return NULL;
            }
        rm->scriptResourcesList[rm->scriptResourcesCount] = scriptResource;
        rm->scriptResourcesCount++;
        return scriptResource;
    }
    return rm->scriptResourcesList[foundIndex];
}

struct SoundResource* ResourceManager_loadSoundResource(struct ResourceManager* rm,
                                                        const char* const soundResId) {
    if (!rm || !soundResId)
        return NULL;
    bool found = false;
    size_t foundIndex = 0;
    // Firstly, try to find existing soundResource in list and return it.
    // If no soundResource found, try to load and add new, then return it.
    for (size_t i = 0; i < rm->soundResourcesCount; i++)
        if (strcmp(soundResId, rm->soundResourcesList[i]->id) == 0) {
            found = true;
            foundIndex = i;
            break;
        }
    if (!found) {
        struct SoundResource* soundResource = NULL;
        soundResource = SoundResource_construct(soundResId);
        if (!soundResource) {
            Logger_log(rm->logger, "%s ResourceID: %s. SDL_mixer error: %s",
                       ResourceManager_errorMessages.errLoadSoundRes,
                       soundResId,
                       Mix_GetError());
            return NULL;
        }
        // Try to reallocate (if needed) and add soundResource to the list
        if (rm->soundResourcesCount >= rm->allocatedSoundResourcesCount)
            if (ResourceManager_reallocateSoundResourcesList(rm)) {
                SoundResource_destruct(soundResource);
                return NULL;
            }
        rm->soundResourcesList[rm->soundResourcesCount] = soundResource;
        rm->soundResourcesCount++;
        return soundResource;
    }
    return rm->soundResourcesList[foundIndex];
}

enum ResourceManager_errors ResourceManager_saveTextResource(struct ResourceManager* rm,
                                                             struct TextResource* textResource,
                                                             const char* const textResId) {

    if (!rm || !textResId || !textResource)
        return RM_ERR_NULL_ARGUMENT;
    size_t result = 0;
    bool found = false;
    result = TextResource_save(textResource, textResId);
    for (size_t i = 0; i < rm->textResourcesCount; i++)
        if (textResource == rm->textResourcesList[i]) {
            found = true;
            break;
        }
    if (!found) {
        // Try to reallocate (if needed) and add textResource to the list
        if (rm->textResourcesCount >= rm->allocatedTextResourcesCount)
            if (ResourceManager_reallocateTextResourcesList(rm))
                return RM_ERR_ALLOC;
        rm->textResourcesList[rm->textResourcesCount] = textResource;
        rm->textResourcesCount++;
    }
    if (result)
        return RM_ERR_SAVING;
    return RM_NO_ERRORS;
}

void ResourceManager_destructNeedlessTextureResources(struct ResourceManager* rm) {
    if (!rm)
        return;
    size_t i;
    size_t j;
    size_t destructedCount = 0;
    for (i = 0; i < rm->textureResourcesCount; i++)
        if (rm->textureResourcesList[i]->pointersCount <= 0) {
            TextureResource_destruct(rm->textureResourcesList[i]);
            rm->textureResourcesList[i] = NULL;
            destructedCount++;
        }
    i = 0;
    while (i < rm->textureResourcesCount - 1) {
        if (rm->textureResourcesList[i] == NULL) {
            for (j = i + 1; j < rm->textureResourcesCount; j++)
                if (rm->textureResourcesList[j]) {
                    rm->textureResourcesList[i] = rm->textureResourcesList[j];
                    i++;
                }
        }
        i++;
    }
    rm->textureResourcesCount -= destructedCount;
}

void ResourceManager_destructNeedlessTextResources(struct ResourceManager* rm) {
    if (!rm)
        return;
    size_t i;
    size_t j;
    size_t destructedCount = 0;
    for (i = 0; i < rm->textResourcesCount; i++)
        if (rm->textResourcesList[i]->pointersCount <= 0) {
            TextResource_destruct(rm->textResourcesList[i]);
            rm->textResourcesList[i] = NULL;
            destructedCount++;
        }
    i = 0;
    while (i < rm->textResourcesCount - 1) {
        if (rm->textResourcesList[i] == NULL) {
            for (j = i + 1; j < rm->textResourcesCount; j++)
                if (rm->textResourcesList[j]) {
                    rm->textResourcesList[i] = rm->textResourcesList[j];
                    i++;
                }
        }
        i++;
    }
    rm->textResourcesCount -= destructedCount;
}

void ResourceManager_destructNeedlessScriptResources(struct ResourceManager* rm) {
    if (!rm)
        return;
    size_t i;
    size_t j;
    size_t destructedCount = 0;
    for (i = 0; i < rm->scriptResourcesCount; i++)
        if (rm->scriptResourcesList[i]->pointersCount <= 0) {
            ScriptResource_destruct(rm->scriptResourcesList[i]);
            rm->scriptResourcesList[i] = NULL;
            destructedCount++;
        }
    i = 0;
    while (i < rm->scriptResourcesCount - 1) {
        if (rm->scriptResourcesList[i] == NULL) {
            for (j = i + 1; j < rm->scriptResourcesCount; j++)
                if (rm->scriptResourcesList[j]) {
                    rm->scriptResourcesList[i] = rm->scriptResourcesList[j];
                    i++;
                }
        }
        i++;
    }
    rm->scriptResourcesCount -= destructedCount;
}

void ResourceManager_destructNeedlessSoundResources(struct ResourceManager* rm) {
    if (!rm)
        return;
    size_t i;
    size_t j;
    size_t destructedCount = 0;
    for (i = 0; i < rm->soundResourcesCount; i++)
        if (rm->soundResourcesList[i]->pointersCount <= 0) {
            SoundResource_destruct(rm->soundResourcesList[i]);
            rm->soundResourcesList[i] = NULL;
            destructedCount++;
        }
    i = 0;
    while (i < rm->soundResourcesCount - 1) {
        if (rm->soundResourcesList[i] == NULL) {
            for (j = i + 1; j < rm->soundResourcesCount; j++)
                if (rm->soundResourcesList[j]) {
                    rm->soundResourcesList[i] = rm->soundResourcesList[j];
                    i++;
                }
        }
        i++;
    }
    rm->soundResourcesCount -= destructedCount;
}
