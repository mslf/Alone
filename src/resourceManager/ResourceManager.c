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
    if (rm) {
        rm->textureResourcesList = NULL;
        rm->textureResourcesIds = NULL;
        rm->textureResourcesCount = 0;
        if (!(rm->textureResourcesList = (struct TextureResource**)malloc(
                sizeof(struct TextureResource*) * INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES))) {
            fprintf(stderr, "TextureResourcesList allocating failed!\n");
            return 1;
        }
        if (!(rm->textureResourcesIds = (const char**)malloc(
                sizeof(char*) * INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES))) {
            fprintf(stderr, "TextureResourcesIds allocating failed!\n");
            return 2;
        }
        rm->allocatedTextureResourcesCount = INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
        return 0;
    }
    fprintf(stderr, "NULL pointer to ResourceManager!\n");
    return 3;
}

unsigned char ResourceManager_constructTextResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        rm->textResourcesList = NULL;
        rm->textResourcesIds = NULL;
        rm->textResourcesCount = 0;
        if (!(rm->textResourcesList = (struct TextResource**)malloc(
                sizeof(struct TextResource*) * INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES))) {
            fprintf(stderr, "TextResourcesList allocating failed!\n");
            ResourceManager_destruct(rm);
            return  1;
        }
        if (!(rm->textResourcesIds = (const char**)malloc(
                sizeof(char*) * INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES))) {
            fprintf(stderr, "TextResourcesIds allocating failed!\n");
            return 2;
        }
        rm->allocatedTextResourcesCount = INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES;
        return 0;
    }
    fprintf(stderr, "NULL pointer to ResourceManager!\n");
    return 3;
}

unsigned char ResourceManager_constructScriptResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        rm->scriptResourcesList = NULL;
        rm->scriptResourcesIds = NULL;
        rm->scriptResourcesCount = 0;
        if (!(rm->scriptResourcesList = (struct ScriptResource**)malloc(
                sizeof(struct ScriptResource*) * INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES))) {
            fprintf(stderr, "ScriptResourcesList allocating failed!\n");
            ResourceManager_destruct(rm);
            return 1;
        }
        if (!(rm->scriptResourcesIds = (const char**)malloc(
                sizeof(char*) * INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES))) {
            fprintf(stderr, "ScriptResourcesIds allocating failed!\n");
            return 2;
        }
        rm->allocatedScriptResourcesCount = INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES;
        return 0;
    }
    fprintf(stderr, "NULL pointer to ResourceManager!\n");
    return 3;
}

unsigned char ResourceManager_constructSoundResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        rm->soundResourcesList = NULL;
        rm->soundResourcesIds = NULL;
        rm->soundResourcesCount = 0;
        if (!(rm->soundResourcesList = (struct SoundResource**)malloc(
                sizeof(struct SoundResource*) * INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES))) {
            fprintf(stderr, "SoundResourcesList allocating failed!\n");
            ResourceManager_destruct(rm);
            return 1;
        }
        if (!(rm->soundResourcesIds = (const char**)malloc(
                sizeof(char*) * INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES))) {
            fprintf(stderr, "SoundResourcesIds allocating failed!\n");
            return 2;
        }
        rm->allocatedSoundResourcesCount = INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES;
        return 0;
    }
    fprintf(stderr, "NULL pointer to ResourceManager!\n");
    return 3;
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
        if (rm->textureResourcesList)
            if (rm->textureResourcesCount > 0) {
                for (i = 0; i < rm->textureResourcesCount; i++)
                    TextureResource_destruct(rm->textureResourcesList[i]);
                free(rm->textureResourcesList);
            }
        if (rm->textureResourcesIds)
            free(rm->textureResourcesIds);
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_destructTextResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        size_t i;
        if (rm->textResourcesList)
            if (rm->textResourcesCount > 0) {
                for (i = 0; i < rm->textResourcesCount; i++)
                    TextResource_destruct(rm->textResourcesList[i]);
                free(rm->textResourcesList);
            }
        if (rm->textResourcesIds)
            free(rm->textResourcesIds);
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_destructScriptResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        size_t i;
        if (rm->scriptResourcesList)
            if (rm->scriptResourcesCount > 0) {
                for (i = 0; i < rm->scriptResourcesCount; i++)
                    ScriptResource_destruct(rm->scriptResourcesList[i]);
                free(rm->scriptResourcesList);
            }
        if (rm->scriptResourcesIds)
            free(rm->scriptResourcesIds);
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_destructSoundResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        size_t i;
        if (rm->soundResourcesList)
            if (rm->soundResourcesCount > 0) {
                for (i = 0; i < rm->soundResourcesCount; i++)
                    SoundResource_destruct(rm->soundResourcesList[i]);
                free(rm->soundResourcesList);
            }
        if (rm->soundResourcesIds)
            free(rm->soundResourcesIds);
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
        const char** textureResourceIds = NULL;
        size_t i;
        unsigned char result = 0;
        if (!(textureResourcesList = (struct TextureResource**)malloc(
                sizeof(struct TextureResource*) * (rm->allocatedTextureResourcesCount +
                                                   INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES))))

            result++;
        if (!(textureResourceIds = (const char**)malloc(
                sizeof(const char*) * (rm->allocatedTextureResourcesCount +
                                       INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES))))
            result++;
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
            rm->allocatedTextureResourcesCount += INITIAL_NUMBER_ALLOCATED_TEXTURE_RESOURCES;
        }
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_reallocateTextResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        struct TextResource** textResourcesList = NULL;
        const char** textResourceIds = NULL;
        size_t i;
        unsigned char result = 0;
        if (!(textResourcesList = (struct TextResource**)malloc(
                sizeof(struct TextResource*) * (rm->allocatedTextResourcesCount +
                                                INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES))))

            result++;
        if (!(textResourceIds = (const char**)malloc(
                sizeof(const char*) * (rm->allocatedTextResourcesCount +
                                       INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES))))
            result++;
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
            rm->allocatedTextResourcesCount += INITIAL_NUMBER_ALLOCATED_TEXT_RESOURCES;
        }
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_reallocateScriptResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        struct ScriptResource** scriptResourcesList = NULL;
        const char** scriptResourceIds = NULL;
        size_t i;
        unsigned char result = 0;
        if (!(scriptResourcesList = (struct ScriptResource**)malloc(
                sizeof(struct ScriptResource*) * (rm->allocatedScriptResourcesCount +
                                                  INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES))))

            result++;
        if (!(scriptResourceIds = (const char**)malloc(
                sizeof(const char*) * (rm->allocatedScriptResourcesCount +
                                       INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES))))
            result++;
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
            rm->allocatedScriptResourcesCount += INITIAL_NUMBER_ALLOCATED_SCRIPT_RESOURCES;
        }
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

void ResourceManager_reallocateSoundResourcesMap(struct ResourceManager* rm) {
    if (rm) {
        struct SoundResource** soundResourcesList = NULL;
        const char** soundResourceIds = NULL;
        size_t i;
        unsigned char result = 0;
        if (!(soundResourcesList = (struct SoundResource**)malloc(
                sizeof(struct SoundResource*) * (rm->allocatedSoundResourcesCount +
                                                 INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES))))

            result++;
        if (!(soundResourceIds = (const char**)malloc(
                sizeof(const char*) * (rm->allocatedSoundResourcesCount +
                                       INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES))))
            result++;
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
            rm->allocatedSoundResourcesCount += INITIAL_NUMBER_ALLOCATED_SOUND_RESOURCES;
        }
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}

struct TextureResource* ResourceManager_loadTextureResource(struct ResourceManager* rm, struct Renderer* renderer,
                                                            const char* const textureResId) {
    if (rm) {
        if (renderer) {
            if (textureResId) {
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
                if (found) {
                    return rm->textureResourcesList[foundIndex];
                } else {
                    struct TextureResource* textureResource = NULL;
                    textureResource = TextureResource_construct(renderer, textureResId);
                    if (textureResource) {
                        // Try to reallocate (if needed) and add textureResource to the map
                        if (rm->textureResourcesCount >= rm->allocatedTextureResourcesCount)
                            ResourceManager_reallocateTextureResourcesMap(rm);
                        if (rm->textureResourcesCount < rm->allocatedTextureResourcesCount) {
                            rm->textureResourcesList[rm->textureResourcesCount] = textureResource;
                            rm->textureResourcesIds[rm->textureResourcesCount] = textureResId;
                            rm->textureResourcesCount++;
                        }
                        else {
                            TextureResource_destruct(textureResource);
                            return NULL;
                        }
                    }
                    return textureResource;
                }
            } else
                fprintf(stderr, "NULL pointer to textureResId!\n");
        } else
            fprintf(stderr, "NULL pointer to Renderer!\n");
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
    return NULL;
}

struct TextResource* ResourceManager_loadTextResource(struct ResourceManager* rm,
                                                      const char* const textResId, unsigned char unique) {
    if (rm) {
        if (textResId) {
            unsigned char found = 0;
            if (!unique) {
                size_t foundIndex = 0;
                size_t i;
                // Firstly, try to find existing non-unique textResource in map and return it.
                // If no textResource found, try to load and add new, then return it.
                for (i = 0; i < rm->textResourcesCount; i++)
                    if (strcmp(textResId, rm->textResourcesIds[i]) == 0 && rm->textResourcesList[i]->isUnique == 0) {
                        found = 1;
                        foundIndex = i;
                        break;
                    }
                if (found)
                    return rm->textResourcesList[foundIndex];
            }
            if (unique || !found) {
                struct TextResource* textResource = NULL;
                textResource = TextResource_construct(textResId, unique);
                if (textResource) {
                    // Try to reallocate (if needed) and add textResource to the map
                    if (rm->textResourcesCount >= rm->allocatedTextResourcesCount)
                        ResourceManager_reallocateTextResourcesMap(rm);
                    if (rm->textResourcesCount < rm->allocatedTextResourcesCount) {
                        rm->textResourcesList[rm->textResourcesCount] = textResource;
                        rm->textResourcesIds[rm->textResourcesCount] = textResId;
                        rm->textResourcesCount++;
                    }
                    else {
                        TextResource_destruct(textResource);
                        return NULL;
                    }
                }
                return textResource;
            }
        } else
            fprintf(stderr, "NULL pointer to textResId!\n");
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
    return NULL;
}

struct ScriptResource* ResourceManager_loadScriptResource(struct ResourceManager* rm,
                                                          const char* const scriptResId) {
    if (rm) {
        if (scriptResId) {
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
            if (found) {
                return rm->scriptResourcesList[foundIndex];
            } else {
                struct ScriptResource* scriptResource = NULL;
                scriptResource = ScriptResource_construct(scriptResId);
                if (scriptResource) {
                    // Try to reallocate (if needed) and add scriptResource to the map
                    if (rm->scriptResourcesCount >= rm->allocatedScriptResourcesCount)
                        ResourceManager_reallocateScriptResourcesMap(rm);
                    if (rm->scriptResourcesCount < rm->allocatedScriptResourcesCount) {
                        rm->scriptResourcesList[rm->scriptResourcesCount] = scriptResource;
                        rm->scriptResourcesIds[rm->scriptResourcesCount] = scriptResId;
                        rm->scriptResourcesCount++;
                    }
                    else {
                        ScriptResource_destruct(scriptResource);
                        return NULL;
                    }
                }
                return scriptResource;
            }
        } else
            fprintf(stderr, "NULL pointer to scriptResId!\n");
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
    return NULL;
}

struct SoundResource* ResourceManager_loadSoundResource(struct ResourceManager* rm,
                                                        const char* const soundResId) {
    if (rm) {
        if (soundResId) {
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
            if (found) {
                return rm->soundResourcesList[foundIndex];
            } else {
                struct SoundResource* soundResource = NULL;
                soundResource = SoundResource_construct(soundResId);
                if (soundResource) {
                    // Try to reallocate (if needed) and add soundResource to the map
                    if (rm->soundResourcesCount >= rm->allocatedSoundResourcesCount)
                        ResourceManager_reallocateSoundResourcesMap(rm);
                    if (rm->soundResourcesCount < rm->allocatedSoundResourcesCount) {
                        rm->soundResourcesList[rm->soundResourcesCount] = soundResource;
                        rm->soundResourcesIds[rm->soundResourcesCount] = soundResId;
                        rm->soundResourcesCount++;
                    }
                    else {
                        SoundResource_destruct(soundResource);
                        return NULL;
                    }
                } else {
                    fprintf(stderr, "Loading soundResource failed! SDL_mixer Error: %s\n", Mix_GetError());
                }
                return soundResource;
            }
        } else
            fprintf(stderr, "NULL pointer to soundResId!\n");
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
    return NULL;
}

void ResourceManager_saveTextResource(struct ResourceManager* rm,
                                      struct TextResource* textResource, const char* const textResId) {
    if (rm) {
        if (textResource) {
            if (textResId) {
                size_t i;
                size_t result = 0;
                unsigned  char found = 0;
                for (i = 0; i < rm->textResourcesCount; i++)
                    if (textResource == rm->textResourcesList[i]) {
                        found = 1;
                        break;
                    }
                if (!found) {
                    // Try to reallocate (if needed) and add textResource to the map
                    if (rm->textResourcesCount >= rm->allocatedTextResourcesCount)
                        ResourceManager_reallocateTextResourcesMap(rm);
                    if (rm->textResourcesCount >= rm->allocatedTextResourcesCount) {
                        rm->textResourcesList[rm->textResourcesCount] = textResource;
                        rm->textResourcesIds[rm->textResourcesCount] = textResId;
                        rm->textResourcesCount++;
                    } else
                        fprintf(stderr, "Warning: Adding new unique textResource failed!\n"
                                "Destroying unique %s will be impossible!\n", textResId);
                }
                result = TextResource_save(textResource, textResId);
                if (result != 0) {
                    fprintf(stderr, "Saving textResource %s failed! Code: %d\n", textResId, result);
                }
            } else
                fprintf(stderr, "NULL pointer to textResId!\n");
        } else
            fprintf(stderr, "NULL pointer to textResource!\n");
    } else
        fprintf(stderr, "NULL pointer to ResourceManager!\n");
}