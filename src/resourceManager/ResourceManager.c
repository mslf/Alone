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

unsigned char ResourceManager_constructTextureResourcesMap(struct ResourceManager* rm) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return 3;
    }
    rm->textureResourcesList = NULL;
    rm->textureResourcesIds = NULL;
    rm->textureResourcesCount = 0;
    if (!(rm->textureResourcesList = (struct TextureResource**)malloc(
            sizeof(struct TextureResource*) * INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES))) {
        fprintf(stderr, "TextureResourcesList allocating failed!\n");
        return 1;
    }
    if (!(rm->textureResourcesIds = (char**)malloc(
            sizeof(char*) * INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES))) {
        fprintf(stderr, "TextureResourcesIds allocating failed!\n");
        return 2;
    }
    size_t i;
    for (i = 0; i < INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES; i++)
        rm->textureResourcesIds[i] = NULL;
    rm->allocatedTextureResourcesCount = INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    return 0;

}

unsigned char ResourceManager_constructTextResourcesMap(struct ResourceManager* rm) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return 3;
    }
    rm->textResourcesList = NULL;
    rm->textResourcesIds = NULL;
    rm->textResourcesCount = 0;
    if (!(rm->textResourcesList = (struct TextResource**)malloc(
            sizeof(struct TextResource*) * INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES))) {
        fprintf(stderr, "TextResourcesList allocating failed!\n");
        ResourceManager_destruct(rm);
        return  1;
    }
    if (!(rm->textResourcesIds = (char**)malloc(
            sizeof(char*) * INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES))) {
        fprintf(stderr, "TextResourcesIds allocating failed!\n");
        return 2;
    }
    size_t i;
    for (i = 0; i < INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES; i++)
        rm->textResourcesIds[i] = NULL;
    rm->allocatedTextResourcesCount = INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES;
    return 0;
}

unsigned char ResourceManager_constructScriptResourcesMap(struct ResourceManager* rm) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return 3;
    }
    rm->scriptResourcesList = NULL;
    rm->scriptResourcesIds = NULL;
    rm->scriptResourcesCount = 0;
    if (!(rm->scriptResourcesList = (struct ScriptResource**)malloc(
            sizeof(struct ScriptResource*) * INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES))) {
        fprintf(stderr, "ScriptResourcesList allocating failed!\n");
        ResourceManager_destruct(rm);
        return 1;
    }
    if (!(rm->scriptResourcesIds = (char**)malloc(
            sizeof(char*) * INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES))) {
        fprintf(stderr, "ScriptResourcesIds allocating failed!\n");
        return 2;
    }
    size_t i;
    for (i = 0; i < INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES; i++)
        rm->scriptResourcesIds[i] = NULL;
    rm->allocatedScriptResourcesCount = INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES;
    return 0;
}

unsigned char ResourceManager_constructSoundResourcesMap(struct ResourceManager* rm) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return 3;
    }
    rm->soundResourcesList = NULL;
    rm->soundResourcesIds = NULL;
    rm->soundResourcesCount = 0;
    if (!(rm->soundResourcesList = (struct SoundResource**)malloc(
            sizeof(struct SoundResource*) * INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES))) {
        fprintf(stderr, "SoundResourcesList allocating failed!\n");
        ResourceManager_destruct(rm);
        return 1;
    }
    if (!(rm->soundResourcesIds = (char**)malloc(
            sizeof(char*) * INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES))) {
        fprintf(stderr, "SoundResourcesIds allocating failed!\n");
        return 2;
    }
    size_t i;
    for (i = 0; i < INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES; i++)
        rm->soundResourcesIds[i] = NULL;
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
    if (rm) {
        size_t i;
        if (rm->textureResourcesList) {
            if (rm->textureResourcesCount > 0)
                for (i = 0; i < rm->textureResourcesCount; i++)
                    TextureResource_destruct(rm->textureResourcesList[i]);
            free(rm->textureResourcesList);
        }
        if (rm->textureResourcesIds) {
            if (rm->textureResourcesCount > 0)
                for (i = 0; i < rm->textureResourcesCount; i++)
                    free(rm->textureResourcesIds[i]);
            free(rm->textureResourcesIds);
        }
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_destructTextResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        size_t i;
        if (rm->textResourcesList) {
            if (rm->textResourcesCount > 0)
                for (i = 0; i < rm->textResourcesCount; i++)
                    TextResource_destruct(rm->textResourcesList[i]);
            free(rm->textResourcesList);
        }
        if (rm->textResourcesIds) {
            if (rm->textResourcesCount > 0)
                for (i = 0; i < rm->textResourcesCount; i++)
                    free(rm->textResourcesIds[i]);
            free(rm->textResourcesIds);
        }
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_destructScriptResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        size_t i;
        if (rm->scriptResourcesList) {
            if (rm->scriptResourcesCount > 0)
                for (i = 0; i < rm->scriptResourcesCount; i++)
                    ScriptResource_destruct(rm->scriptResourcesList[i]);
            free(rm->scriptResourcesList);
        }
        if (rm->scriptResourcesIds) {
            if (rm->scriptResourcesCount > 0)
                for (i = 0; i < rm->scriptResourcesCount; i++)
                    free(rm->scriptResourcesIds[i]);
            free(rm->scriptResourcesIds);
        }
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_destructSoundResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        size_t i;
        if (rm->soundResourcesList) {
            if (rm->soundResourcesCount > 0)
                for (i = 0; i < rm->soundResourcesCount; i++)
                    SoundResource_destruct(rm->soundResourcesList[i]);
            free(rm->soundResourcesList);
        }
        if (rm->soundResourcesIds) {
            if (rm->soundResourcesCount > 0)
                for (i = 0; i < rm->soundResourcesCount; i++)
                    free(rm->soundResourcesIds[i]);
            free(rm->soundResourcesIds);
        }
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_destruct(struct ResourceManager* rm) {
    if (rm) {
        ResourceManager_destructTextureResourcesMap(rm);
        ResourceManager_destructTextResourcesMap(rm);
        ResourceManager_destructScriptResourcesMap(rm);
        ResourceManager_destructSoundResourcesMap(rm);
        free(rm);
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_reallocateTextureResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        struct TextureResource** textureResourcesList = NULL;
        char** textureResourceIds = NULL;
        size_t i;
        size_t newSize = rm->allocatedTextureResourcesCount + INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
        unsigned char result = 0;
        if (!(textureResourcesList = (struct TextureResource**)malloc(sizeof(struct TextureResource*) * newSize)))
            result++;
        if (!(textureResourceIds = (char**)malloc(sizeof(const char*) * newSize)))
            result++;
        for (i = 0; i < newSize; i++)
            textureResourceIds[i] = NULL;
        if (result) {
            fprintf(stderr, "TextureResourcesMap reallocating failed!\n");
            if (textureResourcesList)
                free(textureResourcesList);
            if (textureResourceIds)
                free(textureResourceIds);
        } else {
            for (i = 0; i < rm->textureResourcesCount; i++) {
                textureResourcesList[i] = rm->textureResourcesList[i];
                textureResourceIds[i] = rm->textureResourcesIds[i];
            }
            free(rm->textureResourcesList);
            free(rm->textureResourcesIds);
            rm->textureResourcesList = textureResourcesList;
            rm->textureResourcesIds = textureResourceIds;
            rm->allocatedTextureResourcesCount = newSize;
        }
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_reallocateTextResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        struct TextResource** textResourcesList = NULL;
        char** textResourceIds = NULL;
        size_t i;
        size_t newSize = rm->allocatedTextResourcesCount + INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES;
        unsigned char result = 0;
        if (!(textResourcesList = (struct TextResource**)malloc(sizeof(struct TextResource*) * newSize)))
            result++;
        if (!(textResourceIds = (char**)malloc(sizeof(const char*) * newSize)))
            result++;
        for (i = 0; i < newSize; i++)
            textResourceIds[i] = NULL;
        if (result) {
            fprintf(stderr, "TextResourcesMap reallocating failed!\n");
            if (textResourcesList)
                free(textResourcesList);
            if (textResourceIds)
                free(textResourceIds);
        } else {
            for (i = 0; i < rm->textResourcesCount; i++) {
                textResourcesList[i] = rm->textResourcesList[i];
                textResourceIds[i] = rm->textResourcesIds[i];
            }
            free(rm->textResourcesList);
            free(rm->textResourcesIds);
            rm->textResourcesList = textResourcesList;
            rm->textResourcesIds = textResourceIds;
            rm->allocatedTextResourcesCount = newSize;
        }
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_reallocateScriptResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        struct ScriptResource** scriptResourcesList = NULL;
        char** scriptResourceIds = NULL;
        size_t i;
        size_t newSize = rm->allocatedScriptResourcesCount + INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES;
        unsigned char result = 0;
        if (!(scriptResourcesList = (struct ScriptResource**)malloc(sizeof(struct ScriptResource*) * newSize)))
            result++;
        if (!(scriptResourceIds = (char**)malloc(sizeof(const char*) * newSize)))
            result++;
        for (i = 0; i < newSize; i++)
            scriptResourceIds[i] = NULL;
        if (result) {
            fprintf(stderr, "ScriptResourcesMap reallocating failed!\n");
            if (scriptResourcesList)
                free(scriptResourcesList);
            if (scriptResourceIds)
                free(scriptResourceIds);
        } else {
            for (i = 0; i < rm->scriptResourcesCount; i++) {
                scriptResourcesList[i] = rm->scriptResourcesList[i];
                scriptResourceIds[i] = rm->scriptResourcesIds[i];
            }
            free(rm->scriptResourcesList);
            free(rm->scriptResourcesIds);
            rm->scriptResourcesList = scriptResourcesList;
            rm->scriptResourcesIds = scriptResourceIds;
            rm->allocatedScriptResourcesCount = newSize;
        }
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_reallocateSoundResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        struct SoundResource** soundResourcesList = NULL;
        char** soundResourceIds = NULL;
        size_t i;
        size_t newSize = rm->allocatedSoundResourcesCount + INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES;
        unsigned char result = 0;
        if (!(soundResourcesList = (struct SoundResource**)malloc(sizeof(struct SoundResource*) * newSize)))
            result++;
        if (!(soundResourceIds = (char**)malloc(sizeof(const char*) * newSize)))
            result++;
        for (i = 0; i < newSize; i++)
            soundResourceIds[i] = NULL;
        if (result) {
            fprintf(stderr, "SoundResourcesMap reallocating failed!\n");
            if (soundResourcesList)
                free(soundResourcesList);
            if (soundResourceIds)
                free(soundResourceIds);
        } else {
            for (i = 0; i < rm->soundResourcesCount; i++) {
                soundResourcesList[i] = rm->soundResourcesList[i];
                soundResourceIds[i] = rm->soundResourcesIds[i];
            }
            free(rm->soundResourcesList);
            free(rm->soundResourcesIds);
            rm->soundResourcesList = soundResourcesList;
            rm->soundResourcesIds = soundResourceIds;
            rm->allocatedSoundResourcesCount = newSize;
        }
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

struct TextureResource* ResourceManager_loadTextureResource(struct ResourceManager* rm, struct Renderer* renderer,
                                                            const char* const textureResId) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return NULL;
    }
    if (!textureResId) {
        fprintf(stderr, "NULL pointer to TextureResId!\n");
        return NULL;
    }
    if (!renderer) {
        fprintf(stderr, "NULL pointer to Renderer!\n");
        return NULL;
    }
    unsigned  char found = 0;
    size_t foundIndex = 0;
    size_t i;
    // Firstly, try to find existing textureResource in map and return it.
    // If no textureResource found, try to load and add new, then return it.
    for (i = 0; i < rm->textureResourcesCount; i++)
        if (strcmp(textureResId, rm->textureResourcesIds[i]) == 0) {
            found = 1;
            foundIndex = i;
            break;
        }
    if (!found) {
        struct TextureResource* textureResource = NULL;
        textureResource = TextureResource_construct(renderer, textureResId);
        if (!textureResource) {
            fprintf(stderr, "TextureResource constructing failed! ResourceID: %s\n", textureResId);
            return NULL;
        }
        char* resourceManagerOwnTextureResourceId = NULL;
        resourceManagerOwnTextureResourceId = (char*)malloc(sizeof(char) * (strlen(textureResId) + 1));
        if (!resourceManagerOwnTextureResourceId) {
            fprintf(stderr, "TextureResourceId string allocating failed! ResourceID: %s\n", textureResId);
            TextureResource_destruct(textureResource);
            return NULL;
        }
        strcpy(resourceManagerOwnTextureResourceId, textureResId);
        // Try to reallocate (if needed) and add textureResource to the map
        if (rm->textureResourcesCount >= rm->allocatedTextureResourcesCount)
            ResourceManager_reallocateTextureResourcesMap(rm);
        if (rm->textureResourcesCount >= rm->allocatedTextureResourcesCount) {
            TextureResource_destruct(textureResource);
            free(resourceManagerOwnTextureResourceId);
            return NULL;
        }
        rm->textureResourcesList[rm->textureResourcesCount] = textureResource;
        rm->textureResourcesIds[rm->textureResourcesCount] = resourceManagerOwnTextureResourceId;
        rm->textureResourcesCount++;
        return textureResource;

    }
    return rm->textureResourcesList[foundIndex];
}

struct TextResource* ResourceManager_loadTextResource(struct ResourceManager* rm,
                                                      const char* const textResId, unsigned char unique) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return NULL;
    }
    if (!textResId) {
        fprintf(stderr, "NULL pointer to textResId!\n");
        return NULL;
    }
    unsigned  char found = 0;
    size_t foundIndex = 0;
    size_t i;
    // Firstly, try to find existing non-unique textResource in map and return it.
    // If no textResource found, try to load and add new, then return it.
    if (!unique)
        for (i = 0; i < rm->textResourcesCount; i++)
            if (!rm->textResourcesList[i]->isUnique && strcmp(textResId, rm->textResourcesIds[i]) == 0) {
                found = 1;
                foundIndex = i;
                break;
            }
    if (found)
        return rm->textResourcesList[foundIndex];
    struct TextResource* textResource = NULL;
    textResource = TextResource_construct(textResId, unique);
    if (!textResource) {
        fprintf(stderr, "TextResource constructing failed! ResourceID: %s\n", textResId);
        return NULL;
    }
    char* resourceManagerOwnTextResourceId = NULL;
    resourceManagerOwnTextResourceId = (char*)malloc(sizeof(char) * (strlen(textResId) + 1));
    if (!resourceManagerOwnTextResourceId) {
        fprintf(stderr, "TextResourceId string allocating failed! ResourceID: %s\n", textResId);
        TextResource_destruct(textResource);
        return NULL;
    }
    strcpy(resourceManagerOwnTextResourceId, textResId);
    // Try to reallocate (if needed) and add textResource to the map
    if (rm->textResourcesCount >= rm->allocatedTextResourcesCount)
        ResourceManager_reallocateTextResourcesMap(rm);
    if (rm->textResourcesCount >= rm->allocatedTextResourcesCount) {
        TextResource_destruct(textResource);
        free(resourceManagerOwnTextResourceId);
        return NULL;
    }
    rm->textResourcesList[rm->textResourcesCount] = textResource;
    rm->textResourcesIds[rm->textResourcesCount] = resourceManagerOwnTextResourceId;
    rm->textResourcesCount++;
    return textResource;
}

struct ScriptResource* ResourceManager_loadScriptResource(struct ResourceManager* rm,
                                                          const char* const scriptResId) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return NULL;
    }
    if (!scriptResId) {
        fprintf(stderr, "NULL pointer to scriptResId!\n");
        return NULL;
    }
    unsigned  char found = 0;
    size_t foundIndex = 0;
    size_t i;
    // Firstly, try to find existing scriptResource in map and return it.
    // If no scriptResource found, try to load and add new, then return it.
    for (i = 0; i < rm->scriptResourcesCount; i++)
        if (strcmp(scriptResId, rm->scriptResourcesIds[i]) == 0) {
            found = 1;
            foundIndex = i;
            break;
        }
    if (!found) {
        struct ScriptResource* scriptResource = NULL;
        scriptResource = ScriptResource_construct(scriptResId);
        if (!scriptResource) {
            fprintf(stderr, "ScriptResource constructing failed! ResourceId: %s\n", scriptResId);
            return NULL;
        }
        char* resourceManagerOwnScriptResourceId = NULL;
        resourceManagerOwnScriptResourceId = (char*)malloc(sizeof(char) * (strlen(scriptResId) + 1));
        if (!resourceManagerOwnScriptResourceId) {
            fprintf(stderr, "ScriptResourceId string allocating failed! ResourceID: %s\n", scriptResId);
            ScriptResource_destruct(scriptResource);
            return NULL;
        }
        strcpy(resourceManagerOwnScriptResourceId, scriptResId);
        // Try to reallocate (if needed) and add scriptResource to the map
        if (rm->scriptResourcesCount >= rm->allocatedScriptResourcesCount)
            ResourceManager_reallocateScriptResourcesMap(rm);
        if (rm->scriptResourcesCount >= rm->allocatedScriptResourcesCount) {
            ScriptResource_destruct(scriptResource);
            free(resourceManagerOwnScriptResourceId);
            return NULL;
        }
        rm->scriptResourcesList[rm->scriptResourcesCount] = scriptResource;
        rm->scriptResourcesIds[rm->scriptResourcesCount] = resourceManagerOwnScriptResourceId;
        rm->scriptResourcesCount++;
        return scriptResource;

    }
    return rm->scriptResourcesList[foundIndex];
}

struct SoundResource* ResourceManager_loadSoundResource(struct ResourceManager* rm,
                                                        const char* const soundResId) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return NULL;
    }
    if (!soundResId) {
        fprintf(stderr, "NULL pointer to soundResId!\n");
        return NULL;
    }
    unsigned  char found = 0;
    size_t foundIndex = 0;
    size_t i;
    // Firstly, try to find existing soundResource in map and return it.
    // If no soundResource found, try to load and add new, then return it.
    for (i = 0; i < rm->soundResourcesCount; i++)
        if (strcmp(soundResId, rm->soundResourcesIds[i]) == 0) {
            found = 1;
            foundIndex = i;
            break;
        }
    if (!found) {
        struct SoundResource* soundResource = NULL;
        soundResource = SoundResource_construct(soundResId);
        if (!soundResource) {
            fprintf(stderr, "SoundResource constructing failed! ResourceID: %s\n"
                    "SDL_mixer Error: %s\n", soundResId, Mix_GetError());
            return NULL;
        }
        char* resourceManagerOwnSoundResourceId = NULL;
        resourceManagerOwnSoundResourceId = (char*)malloc(sizeof(char) * (strlen(soundResId) + 1));
        if (!resourceManagerOwnSoundResourceId) {
            fprintf(stderr, "ScriptResourceId string allocating failed! ResourceID: %s\n", soundResId);
            SoundResource_destruct(soundResource);
            return NULL;
        }
        strcpy(resourceManagerOwnSoundResourceId, soundResId);
        // Try to reallocate (if needed) and add soundResource to the map
        if (rm->soundResourcesCount >= rm->allocatedSoundResourcesCount)
            ResourceManager_reallocateSoundResourcesMap(rm);
        if (rm->soundResourcesCount >= rm->allocatedSoundResourcesCount) {
            SoundResource_destruct(soundResource);
            free(resourceManagerOwnSoundResourceId);
            return NULL;
        }
        rm->soundResourcesList[rm->soundResourcesCount] = soundResource;
        rm->soundResourcesIds[rm->soundResourcesCount] = resourceManagerOwnSoundResourceId;
        rm->soundResourcesCount++;
        return soundResource;

    }
    return rm->soundResourcesList[foundIndex];
}

void ResourceManager_saveTextResource(struct ResourceManager* rm,
                                      struct TextResource* textResource, const char* const textResId) {
    size_t result = 0;
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        result++;
    }
    if (!textResId) {
        fprintf(stderr, "NULL pointer to textResId!\n");
        result++;
    }
    if (!textResource) {
        fprintf(stderr, "NULL pointer to textResource!\n");
        result++;
    }
    if (!result) {
        size_t i;
        unsigned  char found = 0;
        for (i = 0; i < rm->textResourcesCount; i++)
            if (textResource == rm->textResourcesList[i]) {
                found = 1;
                break;
            }
        if (!found) {
            char* resourceManagerOwnTextResourceId = NULL;
            resourceManagerOwnTextResourceId = (char*)malloc(sizeof(char) * (strlen(textResId) + 1));
            if (!resourceManagerOwnTextResourceId) {
                fprintf(stderr, "TextResourceId string allocating failed! ResourceID: %s\n", textResId);
            } else {
                strcpy(resourceManagerOwnTextResourceId, textResId);
                // Try to reallocate (if needed) and add textResource to the map
                if (rm->textResourcesCount >= rm->allocatedTextResourcesCount)
                    ResourceManager_reallocateTextResourcesMap(rm);
                if (rm->textResourcesCount < rm->allocatedTextResourcesCount) {
                    rm->textResourcesList[rm->textResourcesCount] = textResource;
                    rm->textResourcesIds[rm->textResourcesCount] = resourceManagerOwnTextResourceId;
                    rm->textResourcesCount++;
                } else
                    fprintf(stderr, "Warning: Adding new unique textResource failed!\n"
                            "Destroying unique %s will be impossible!\n", textResId);
            }
        }
        result = TextResource_save(textResource, textResId);
        if (result != 0) {
            fprintf(stderr, "Saving TextResource failed! ResourceID: %s\nError code: %d\n", textResId, result);
        }
    }
}