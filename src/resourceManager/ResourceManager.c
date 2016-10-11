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
#include "resourceManager/ResourceManager.h"
#include <string.h>

const char* const RESOURCE_MANAGER_ERR_ALLOC =
        "ResourceManager: constructor: allocating memory failed!";
const char* const RESOURCE_MANAGER_ERR_TEXTURE_RESOURCES_LIST_ALLOC =
        "ResourceManager: constructor: allocating for textureResourcesList failed!";
const char* const RESOURCE_MANAGER_ERR_TEXT_RESOURCES_LIST_ALLOC =
        "ResourceManager: constructor: allocating for textResourcesList failed!";
const char* const RESOURCE_MANAGER_ERR_SCRIPT_RESOURCES_LIST_ALLOC =
        "ResourceManager: constructor: allocating for scriptResourcesList failed!";
const char* const RESOURCE_MANAGER_ERR_SOUND_RESOURCES_LIST_ALLOC =
        "ResourceManager: constructor: allocating for soundResourcesList failed!";
const char* const RESOURCE_MANAGER_ERR_LOAD_TEXTURE_RES =
        "ResourceManager: TextureResource loader: constructing TextureResource failed!";
const char* const RESOURCE_MANAGER_ERR_LOAD_TEXT_RES =
        "ResourceManager: TextResource loader: constructing TextResource failed!";
const char* const RESOURCE_MANAGER_ERR_LOAD_SCRIPT_RES =
        "ResourceManager: ScriptResource loader: constructing ScriptResource failed!";
const char* const RESOURCE_MANAGER_ERR_LOAD_SOUND_RES =
        "ResourceManager: SoundResource loader: constructing SoundResource failed!";

unsigned char ResourceManager_constructTextureResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return 1;
    rm->textureResourcesList = NULL;
    rm->textureResourcesCount = 0;
    if (!(rm->textureResourcesList = (struct TextureResource**)malloc(
            sizeof(struct TextureResource*) * INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES))) {
        Logger_log(rm->logger, RESOURCE_MANAGER_ERR_TEXTURE_RESOURCES_LIST_ALLOC);
        return 2;
    }
    rm->allocatedTextureResourcesCount = INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    return 0;
}

unsigned char ResourceManager_constructTextResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return 1;
    rm->textResourcesList = NULL;
    rm->textResourcesCount = 0;
    if (!(rm->textResourcesList = (struct TextResource**)malloc(
            sizeof(struct TextResource*) * INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES))) {
        Logger_log(rm->logger, RESOURCE_MANAGER_ERR_TEXT_RESOURCES_LIST_ALLOC);
        return  2;
    }
    rm->allocatedTextResourcesCount = INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES;
    return 0;
}

unsigned char ResourceManager_constructScriptResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return 1;
    rm->scriptResourcesList = NULL;
    rm->scriptResourcesCount = 0;
    if (!(rm->scriptResourcesList = (struct ScriptResource**)malloc(
            sizeof(struct ScriptResource*) * INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES))) {
        Logger_log(rm->logger, RESOURCE_MANAGER_ERR_SCRIPT_RESOURCES_LIST_ALLOC);
        return 2;
    }
    rm->allocatedScriptResourcesCount = INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES;
    return 0;
}

unsigned char ResourceManager_constructSoundResourcesMap(struct ResourceManager* rm) {
    if (!rm)
        return 1;
    rm->soundResourcesList = NULL;
    rm->soundResourcesCount = 0;
    if (!(rm->soundResourcesList = (struct SoundResource**)malloc(
            sizeof(struct SoundResource*) * INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES))) {
        Logger_log(rm->logger, RESOURCE_MANAGER_ERR_SOUND_RESOURCES_LIST_ALLOC);
        return 2;
    }
    rm->allocatedSoundResourcesCount = INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES;
    return 0;
}

struct ResourceManager* ResourceManager_construct(struct Logger* logger) {
    struct ResourceManager* rm = NULL;
    rm = (struct ResourceManager*)malloc(sizeof(struct ResourceManager));
    if (!rm) {
        Logger_log(logger, RESOURCE_MANAGER_ERR_ALLOC);
        return NULL;
    }
    rm->logger = logger;
    unsigned char result = 0;
    result += ResourceManager_constructTextureResourcesList(rm);
    result += ResourceManager_constructTextResourcesList(rm);
    result += ResourceManager_constructScriptResourcesList(rm);
    result += ResourceManager_constructSoundResourcesMap(rm);
    if (result) {
        ResourceManager_destruct(rm);
        return NULL;
    }
    return rm;
}

void ResourceManager_destructTextureResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return;
    size_t i;
    if (rm->textureResourcesList) {
        if (rm->textureResourcesCount > 0)
            for (i = 0; i < rm->textureResourcesCount; i++)
                TextureResource_destruct(rm->textureResourcesList[i]);
        free(rm->textureResourcesList);
    }
}

void ResourceManager_destructTextResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return;
    size_t i;
    if (rm->textResourcesList) {
        if (rm->textResourcesCount > 0)
            for (i = 0; i < rm->textResourcesCount; i++)
                TextResource_destruct(rm->textResourcesList[i]);
        free(rm->textResourcesList);
    }
}

void ResourceManager_destructScriptResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return;
    size_t i;
    if (rm->scriptResourcesList) {
        if (rm->scriptResourcesCount > 0)
            for (i = 0; i < rm->scriptResourcesCount; i++)
                ScriptResource_destruct(rm->scriptResourcesList[i]);
        free(rm->scriptResourcesList);
    }
}

void ResourceManager_destructSoundResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return;
    size_t i;
    if (rm->soundResourcesList) {
        if (rm->soundResourcesCount > 0)
            for (i = 0; i < rm->soundResourcesCount; i++)
                SoundResource_destruct(rm->soundResourcesList[i]);
        free(rm->soundResourcesList);
    }
}

void ResourceManager_destruct(struct ResourceManager* rm) {
    if (!rm)
        return;
    ResourceManager_destructTextureResourcesList(rm);
    ResourceManager_destructTextResourcesList(rm);
    ResourceManager_destructScriptResourcesList(rm);
    ResourceManager_destructSoundResourcesList(rm);
    free(rm);
}

unsigned char ResourceManager_reallocateTextureResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return 1;
    struct TextureResource** textureResourcesList = NULL;
    size_t i;
    size_t newSize = rm->allocatedTextureResourcesCount + INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    if (!(textureResourcesList = (struct TextureResource**)malloc(sizeof(struct TextureResource*) * newSize))) {
        Logger_log(rm->logger, RESOURCE_MANAGER_ERR_TEXTURE_RESOURCES_LIST_ALLOC);
        return 2;
    }
    for (i = 0; i < rm->textureResourcesCount; i++)
        textureResourcesList[i] = rm->textureResourcesList[i];
    free(rm->textureResourcesList);
    rm->textureResourcesList = textureResourcesList;
    rm->allocatedTextureResourcesCount = newSize;
    return 0;
}

unsigned char ResourceManager_reallocateTextResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return 1;
    struct TextResource** textResourcesList = NULL;
    size_t i;
    size_t newSize = rm->allocatedTextResourcesCount + INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    if (!(textResourcesList = (struct TextResource**)malloc(sizeof(struct TextResource*) * newSize))) {
        Logger_log(rm->logger, RESOURCE_MANAGER_ERR_TEXT_RESOURCES_LIST_ALLOC);
        return 2;
    }
    for (i = 0; i < rm->textResourcesCount; i++)
        textResourcesList[i] = rm->textResourcesList[i];
    free(rm->textResourcesList);
    rm->textResourcesList = textResourcesList;
    rm->allocatedTextResourcesCount = newSize;
    return 0;
}

unsigned char ResourceManager_reallocateScriptResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return 1;
    struct ScriptResource** scriptResourcesList = NULL;
    size_t i;
    size_t newSize = rm->allocatedScriptResourcesCount + INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    if (!(scriptResourcesList = (struct ScriptResource**)malloc(sizeof(struct ScriptResource*) * newSize))) {
        Logger_log(rm->logger, RESOURCE_MANAGER_ERR_SCRIPT_RESOURCES_LIST_ALLOC);
        return 2;
    }
    for (i = 0; i < rm->scriptResourcesCount; i++)
        scriptResourcesList[i] = rm->scriptResourcesList[i];
    free(rm->scriptResourcesList);
    rm->scriptResourcesList = scriptResourcesList;
    rm->allocatedScriptResourcesCount = newSize;
    return 0;
}

unsigned char ResourceManager_reallocateSoundResourcesList(struct ResourceManager* rm) {
    if (!rm)
        return 1;
    struct SoundResource** soundResourcesList = NULL;
    size_t i;
    size_t newSize = rm->allocatedSoundResourcesCount + INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    if (!(soundResourcesList = (struct SoundResource**)malloc(sizeof(struct SoundResource*) * newSize))) {
        Logger_log(rm->logger, RESOURCE_MANAGER_ERR_TEXTURE_RESOURCES_LIST_ALLOC);
        return 2;
    }
    for (i = 0; i < rm->soundResourcesCount; i++)
        soundResourcesList[i] = rm->soundResourcesList[i];
    free(rm->soundResourcesList);
    rm->soundResourcesList = soundResourcesList;
    rm->allocatedSoundResourcesCount = newSize;
    return 0;
}

struct TextureResource* ResourceManager_loadTextureResource(struct ResourceManager* rm, struct Renderer* renderer,
                                                            const char* const textureResId) {
    if (!rm || !textureResId || !renderer)
        return NULL;
    unsigned  char found = 0;
    size_t foundIndex = 0;
    size_t i;
    // Firstly, try to find existing textureResource in list and return it.
    // If no textureResource found, try to load and add new, then return it.
    for (i = 0; i < rm->textureResourcesCount; i++)
        if (strcmp(textureResId, rm->textureResourcesList[i]->id) == 0) {
            found = 1;
            foundIndex = i;
            break;
        }
    if (!found) {
        struct TextureResource* textureResource = NULL;
        textureResource = TextureResource_construct(renderer, textureResId);
        if (!textureResource) {
            char tempString[600];
            sprintf(tempString, "%s ResourceID: %s. SDL_image Error: %s", RESOURCE_MANAGER_ERR_LOAD_TEXTURE_RES,
                    textureResId, IMG_GetError());
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

struct TextResource* ResourceManager_loadTextResource(struct ResourceManager* rm,
                                                      const char* const textResId, unsigned char unique) {
    if (!rm || !textResId)
        return NULL;
    unsigned  char found = 0;
    size_t foundIndex = 0;
    size_t i;
    // Firstly, try to find existing non-unique textResource in list and return it.
    // If no textResource found, try to load and add new, then return it.
    if (!unique)
        for (i = 0; i < rm->textResourcesCount; i++)
            if (!rm->textResourcesList[i]->isUnique && strcmp(textResId, rm->textResourcesList[i]->id) == 0) {
                found = 1;
                foundIndex = i;
                break;
            }
    if (found)
        return rm->textResourcesList[foundIndex];
    struct TextResource* textResource = NULL;
    textResource = TextResource_construct(textResId, unique);
    if (!textResource) {
        char tempString[600];
        sprintf(tempString, "%s ResourceID: %s", RESOURCE_MANAGER_ERR_LOAD_TEXT_RES, textResId);
        Logger_log(rm->logger, tempString);
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
    unsigned  char found = 0;
    size_t foundIndex = 0;
    size_t i;
    // Firstly, try to find existing scriptResource in list and return it.
    // If no scriptResource found, try to load and add new, then return it.
    for (i = 0; i < rm->scriptResourcesCount; i++)
        if (strcmp(scriptResId, rm->scriptResourcesList[i]->id) == 0) {
            found = 1;
            foundIndex = i;
            break;
        }
    if (!found) {
        struct ScriptResource* scriptResource = NULL;
        scriptResource = ScriptResource_construct(scriptResId);
        if (!scriptResource) {
            char tempString[600];
            sprintf(tempString, "%s ResourceID: %s", RESOURCE_MANAGER_ERR_LOAD_SCRIPT_RES, scriptResId);
            Logger_log(rm->logger, tempString);
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
    unsigned  char found = 0;
    size_t foundIndex = 0;
    size_t i;
    // Firstly, try to find existing soundResource in list and return it.
    // If no soundResource found, try to load and add new, then return it.
    for (i = 0; i < rm->soundResourcesCount; i++)
        if (strcmp(soundResId, rm->soundResourcesList[i]->id) == 0) {
            found = 1;
            foundIndex = i;
            break;
        }
    if (!found) {
        struct SoundResource* soundResource = NULL;
        soundResource = SoundResource_construct(soundResId);
        if (!soundResource) {
            char tempString[600];
            sprintf(tempString, "%s ResourceID: %s. SDL_mixer Error: %s", RESOURCE_MANAGER_ERR_LOAD_SCRIPT_RES,
                    soundResId, Mix_GetError());
            Logger_log(rm->logger, tempString);
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

unsigned char ResourceManager_saveTextResource(struct ResourceManager* rm,
                                      struct TextResource* textResource, const char* const textResId) {

    if (!rm || !textResId || !textResource)
        return 1;
    size_t result = 0;
    size_t i;
    unsigned  char found = 0;
    result = TextResource_save(textResource, textResId);
    for (i = 0; i < rm->textResourcesCount; i++)
        if (textResource == rm->textResourcesList[i]) {
            found = 1;
            break;
        }
    if (!found) {
        // Try to reallocate (if needed) and add textResource to the list
        if (rm->textResourcesCount >= rm->allocatedTextResourcesCount)
            if (ResourceManager_reallocateTextResourcesList(rm))
                return 2;
        rm->textResourcesList[rm->textResourcesCount] = textResource;
        rm->textResourcesCount++;
    }
    return (result + 2);
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