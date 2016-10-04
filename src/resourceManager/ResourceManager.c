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

unsigned char ResourceManager_constructTextureResourcesMap(struct ResourceManager* rm) {
    rm->textureResourcesList = NULL;
    rm->textureResourcesIds = NULL;
    rm->textureResourcesCount = 0;
    if (!(rm->textureResourcesList = (struct TextureResource**)malloc(
            sizeof(struct TextureResource*) * INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES))) {
        fprintf(stderr, "TextureResourcesList allocating failed!\n");
        return 1;
    }
    rm->allocatedtextureResourcesCount = INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    return 0;
}

unsigned char ResourceManager_constructTextResourcesMap(struct ResourceManager* rm) {
    rm->textResourcesList = NULL;
    rm->textResourcesIds = NULL;
    rm->textResourcesCount = 0;
    if (!(rm->textResourcesList = (struct TextResource**)malloc(
            sizeof(struct TextResource*) * INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES))) {
        fprintf(stderr, "TextResourcesList allocating failed!\n");
        ResourceManager_destruct(rm);
        return  1;
    }
    rm->allocatedTextResourcesCount = INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES;
    return 0;
}

unsigned char ResourceManager_constructScriptResourcesMap(struct ResourceManager* rm) {
    rm->scriptResourcesList = NULL;
    rm->scriptResourcesIds = NULL;
    rm->scriptResourcesCount = 0;
    if (!(rm->scriptResourcesList = (struct ScriptResource**)malloc(
            sizeof(struct ScriptResource*) * INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES))) {
        fprintf(stderr, "ScriptResourcesList allocating failed!\n");
        ResourceManager_destruct(rm);
        return 1;
    }
    rm->allocatedScriptResourcesCount = INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES;
    return 0;
}

unsigned char ResourceManager_constructSoundResourcesMap(struct ResourceManager* rm) {
    rm->soundResourcesList = NULL;
    rm->soundResourcesIds = NULL;
    rm->soundResourcesCount = 0;
    if (!(rm->soundResourcesList = (struct SoundResource**)malloc(
            sizeof(struct SoundResource*) * INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES))) {
        fprintf(stderr, "SoundResourcesList allocating failed!\n");
        ResourceManager_destruct(rm);
        return 1;
    }
    rm->allocatedSoundResourcesCount = INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES;
    return 0;
}

struct ResourceManager* ResourceManager_construct() {
    struct ResourceManager* rm = NULL;
    rm = (struct ResourceManager*)malloc(sizeof(struct ResourceManager));
    if (rm) {
        unsigned char result = 0;
        result += ResourceManager_constructTextureResourcesMap(rm);
        result += ResourceManager_constructTextResourcesMap(rm);
        result += ResourceManager_constructScriptResourcesMap(rm);
        result += ResourceManager_constructSoundResourcesMap(rm);
        if (result) {
            ResourceManager_destruct(rm);
            return NULL;
        }
    }
    return rm;
}

void ResourceManager_destructTextureResourcesMap(struct ResourceManager* rm) {
    size_t i;
    if (rm->textureResourcesList)
        if (rm->textureResourcesCount > 0) {
            for (i = 0; i < rm->textureResourcesCount; i++)
                TextureResource_destruct(rm->textureResourcesList[i]);
            free(rm->textureResourcesList);
        }
    if (rm->textureResourcesIds)
        if (rm->textureResourcesCount > 0) {
            for (i = 0; i < rm->textureResourcesCount; i++)
                free(rm->textureResourcesIds[i]);
            free(rm->textureResourcesIds);
        }
}

void ResourceManager_destructTextResourcesMap(struct ResourceManager* rm) {
    size_t i;
    if (rm->textResourcesList)
        if (rm->textResourcesCount > 0) {
            for (i = 0; i < rm->textResourcesCount; i++)
                TextResource_destruct(rm->textResourcesList[i]);
            free(rm->textResourcesList);
        }
    if (rm->textResourcesIds)
        if (rm->textResourcesCount > 0) {
            for (i = 0; i < rm->textResourcesCount; i++)
                free(rm->textResourcesIds[i]);
            free(rm->textResourcesIds);
        }
}

void ResourceManager_destructScriptResourcesMap(struct ResourceManager* rm) {
    size_t i;
    if (rm->scriptResourcesList)
        if (rm->scriptResourcesCount > 0) {
            for (i = 0; i < rm->scriptResourcesCount; i++)
                ScriptResource_destruct(rm->scriptResourcesList[i]);
            free(rm->scriptResourcesList);
        }
    if (rm->scriptResourcesIds)
        if (rm->scriptResourcesCount > 0) {
            for (i = 0; i < rm->scriptResourcesCount; i++)
                free(rm->scriptResourcesIds[i]);
            free(rm->scriptResourcesIds);
        }
}

void ResourceManager_destructSoundResourcesMap(struct ResourceManager* rm) {
    size_t i;
    if (rm->soundResourcesList)
        if (rm->soundResourcesCount > 0) {
            for (i = 0; i < rm->soundResourcesCount; i++)
                SoundResource_destruct(rm->soundResourcesList[i]);
            free(rm->soundResourcesList);
        }
    if (rm->soundResourcesIds)
        if (rm->soundResourcesCount > 0) {
            for (i = 0; i < rm->soundResourcesCount; i++)
                free(rm->soundResourcesIds[i]);
            free(rm->soundResourcesIds);
        }
}

void ResourceManager_destruct(struct ResourceManager* rm) {
    ResourceManager_destructTextureResourcesMap(rm);
    ResourceManager_destructTextResourcesMap(rm);
    ResourceManager_destructScriptResourcesMap(rm);
    ResourceManager_destructSoundResourcesMap(rm);
    free(rm);
}

struct TextureResource* ResourceManager_loadTextureResource(struct ResourceManager* rm,
                                                            const char* const textureResId) {

}

struct TextResource* ResourceManager_loadTextResource(struct ResourceManager* rm,
                                                      const char* const textResId, unsigned char unique) {

}

struct ScriptResource* ResourceManager_loadScriptResource(struct ResourceManager* rm,
                                                          const char* const scriptResId) {

}

struct SoundResource* ResourceManager_loadSoundResource(struct ResourceManager* rm,
                                                        const char* const soundResId) {

}

void ResourceManager_saveTextResource(struct ResourceManager* rm,
                                      struct TextResource* textResource, const char* const textResId) {

}