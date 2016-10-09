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
        return 1;
    }
    rm->textureResourcesList = NULL;
    rm->textureResourcesCount = 0;
    if (!(rm->textureResourcesList = (struct TextureResource**)malloc(
            sizeof(struct TextureResource*) * INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES))) {
        fprintf(stderr, "TextureResourcesList allocating failed!\n");
        return 2;
    }
    rm->allocatedTextureResourcesCount = INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    return 0;

}

unsigned char ResourceManager_constructTextResourcesMap(struct ResourceManager* rm) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return 1;
    }
    rm->textResourcesList = NULL;
    rm->textResourcesCount = 0;
    if (!(rm->textResourcesList = (struct TextResource**)malloc(
            sizeof(struct TextResource*) * INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES))) {
        fprintf(stderr, "TextResourcesList allocating failed!\n");
        ResourceManager_destruct(rm);
        return  2;
    }
    rm->allocatedTextResourcesCount = INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES;
    return 0;
}

unsigned char ResourceManager_constructScriptResourcesMap(struct ResourceManager* rm) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return 1;
    }
    rm->scriptResourcesList = NULL;
    rm->scriptResourcesCount = 0;
    if (!(rm->scriptResourcesList = (struct ScriptResource**)malloc(
            sizeof(struct ScriptResource*) * INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES))) {
        fprintf(stderr, "ScriptResourcesList allocating failed!\n");
        ResourceManager_destruct(rm);
        return 2;
    }
    rm->allocatedScriptResourcesCount = INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES;
    return 0;
}

unsigned char ResourceManager_constructSoundResourcesMap(struct ResourceManager* rm) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return 1;
    }
    rm->soundResourcesList = NULL;
    rm->soundResourcesCount = 0;
    if (!(rm->soundResourcesList = (struct SoundResource**)malloc(
            sizeof(struct SoundResource*) * INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES))) {
        fprintf(stderr, "SoundResourcesList allocating failed!\n");
        ResourceManager_destruct(rm);
        return 2;
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
    if (rm) {
        size_t i;
        if (rm->textureResourcesList) {
            if (rm->textureResourcesCount > 0)
                for (i = 0; i < rm->textureResourcesCount; i++)
                    TextureResource_destruct(rm->textureResourcesList[i]);
            free(rm->textureResourcesList);
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

unsigned char ResourceManager_reallocateTextureResourcesMap(struct ResourceManager* rm) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return 1;
    }
    struct TextureResource** textureResourcesList = NULL;
    size_t i;
    size_t newSize = rm->allocatedTextureResourcesCount + INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    if (!(textureResourcesList = (struct TextureResource**)malloc(sizeof(struct TextureResource*) * newSize))) {
        fprintf(stderr, "TextureResourcesMap reallocating failed!\n");
        if (textureResourcesList)
            free(textureResourcesList);
        return 2;
    }
    for (i = 0; i < rm->textureResourcesCount; i++)
        textureResourcesList[i] = rm->textureResourcesList[i];
    free(rm->textureResourcesList);
    rm->textureResourcesList = textureResourcesList;
    rm->allocatedTextureResourcesCount = newSize;
    return 0;
        
}

unsigned char ResourceManager_reallocateTextResourcesMap(struct ResourceManager* rm) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return 1;
    }
    struct TextResource** textResourcesList = NULL;
    size_t i;
    size_t newSize = rm->allocatedTextResourcesCount + INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    if (!(textResourcesList = (struct TextResource**)malloc(sizeof(struct TextResource*) * newSize))) {
        fprintf(stderr, "TextResourcesMap reallocating failed!\n");
        if (textResourcesList)
            free(textResourcesList);
        return 2;
    }
    for (i = 0; i < rm->textResourcesCount; i++)
        textResourcesList[i] = rm->textResourcesList[i];
    free(rm->textResourcesList);
    rm->textResourcesList = textResourcesList;
    rm->allocatedTextResourcesCount = newSize;
    return 0;
}

unsigned char ResourceManager_reallocateScriptResourcesMap(struct ResourceManager* rm) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return 1;
    }
    struct ScriptResource** scriptResourcesList = NULL;
    size_t i;
    size_t newSize = rm->allocatedScriptResourcesCount + INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    if (!(scriptResourcesList = (struct ScriptResource**)malloc(sizeof(struct ScriptResource*) * newSize))) {
        fprintf(stderr, "ScriptResourcesMap reallocating failed!\n");
        if (scriptResourcesList)
            free(scriptResourcesList);
        return 2;
    }
    for (i = 0; i < rm->scriptResourcesCount; i++)
        scriptResourcesList[i] = rm->scriptResourcesList[i];
    free(rm->scriptResourcesList);
    rm->scriptResourcesList = scriptResourcesList;
    rm->allocatedScriptResourcesCount = newSize;
    return 0;
}

unsigned char ResourceManager_reallocateSoundResourcesMap(struct ResourceManager* rm) {
    if (!rm) {
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
        return 1;
    }
    struct SoundResource** soundResourcesList = NULL;
    size_t i;
    size_t newSize = rm->allocatedSoundResourcesCount + INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
    if (!(soundResourcesList = (struct SoundResource**)malloc(sizeof(struct SoundResource*) * newSize))) {
        fprintf(stderr, "SoundResourcesMap reallocating failed!\n");
        if (soundResourcesList)
            free(soundResourcesList);
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
            fprintf(stderr, "TextureResource constructing failed! ResourceID: %s\n", textureResId);
            return NULL;
        }
        // Try to reallocate (if needed) and add textureResource to the list
        if (rm->textureResourcesCount >= rm->allocatedTextureResourcesCount)
            if (ResourceManager_reallocateTextureResourcesMap(rm)) {
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
        fprintf(stderr, "TextResource constructing failed! ResourceID: %s\n", textResId);
        return NULL;
    }
    // Try to reallocate (if needed) and add textResource to the list
    if (rm->textResourcesCount >= rm->allocatedTextResourcesCount)
        if (ResourceManager_reallocateTextResourcesMap(rm)) {
            TextResource_destruct(textResource);
            return NULL;
        }
    rm->textResourcesList[rm->textResourcesCount] = textResource;
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
            fprintf(stderr, "ScriptResource constructing failed! ResourceId: %s\n", scriptResId);
            return NULL;
        }
        // Try to reallocate (if needed) and add scriptResource to the list
        if (rm->scriptResourcesCount >= rm->allocatedScriptResourcesCount)
            if (ResourceManager_reallocateScriptResourcesMap(rm)) {
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
            fprintf(stderr, "SoundResource constructing failed! ResourceID: %s\n"
                    "SDL_mixer Error: %s\n", soundResId, Mix_GetError());
            return NULL;
        }
        // Try to reallocate (if needed) and add soundResource to the list
        if (rm->soundResourcesCount >= rm->allocatedSoundResourcesCount)
            if (ResourceManager_reallocateSoundResourcesMap(rm)) {
                SoundResource_destruct(soundResource);
                return NULL;
            }
        rm->soundResourcesList[rm->soundResourcesCount] = soundResource;
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
            // Try to reallocate (if needed) and add textResource to the list
            if (rm->textResourcesCount >= rm->allocatedTextResourcesCount)
                if (!ResourceManager_reallocateTextResourcesMap(rm)) {
                    rm->textResourcesList[rm->textResourcesCount] = textResource;
                    rm->textResourcesCount++;
                } else
                    fprintf(stderr, "Warning: Adding new unique textResource failed!\n"
                            "Destroying unique %s will be impossible!\n", textResId);
        }
        result = TextResource_save(textResource, textResId);
        if (result != 0) {
            fprintf(stderr, "Saving TextResource failed! ResourceID: %s\nError code: %d\n", textResId, result);
        }
    }
}