//
// Created by mslf on 9/22/16.
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
#include "resourceManager/TextResource.h"
#include <SDL2/SDL.h>

struct TextResource* TextResource_construct(const char* const path, unsigned char unique) {
    struct TextResource* textResource = NULL;
    textResource = (struct TextResource*)malloc(sizeof(struct TextResource));
    if (textResource) {
        SDL_RWops* file = SDL_RWFromFile(path, "r");
        size_t size = 0;
        size_t totalRead = 0;
        size_t read = 1;
        char* bufString;
        textResource->text = NULL;
        textResource->id = (char*)malloc(sizeof(char) * (strlen(path) + 1));
        if (!textResource->id) {
            TextResource_destruct(textResource);
            return NULL;
        }
        strcpy(textResource->id, path);
        if (!file) {
            TextResource_destruct(textResource);
            return  NULL;
        }
        size = SDL_RWsize(file);
        textResource->text = (char*)malloc(sizeof(char) * (size + 1));
        if (!textResource->text) {
            TextResource_destruct(textResource);
            SDL_RWclose(file);
            return NULL;
        }
        bufString = textResource->text;
        while (totalRead < size && read != 0) {
            read = SDL_RWread(file, bufString, 1, (size - totalRead));
            totalRead += read;
            bufString += read;
        }
        SDL_RWclose(file);
        if (totalRead != size) {
            TextResource_destruct(textResource);
            return NULL;
        }
        textResource->text[totalRead] = '\0';
        textResource->isUnique = unique;
    }
    return textResource;
}

void TextResource_destruct(struct TextResource* textResource) {
    if (textResource) {
        if (textResource->id)
            free(textResource->id);
        if (textResource->text)
            free(textResource->text);
        free(textResource);
    }
}

unsigned char TextResource_save(struct TextResource* textResource, const char* const path) {
    if (textResource) {
        SDL_RWops *file = NULL;
        if (path)
            SDL_RWFromFile(path, "w");
        else
            SDL_RWFromFile(textResource->id, "w");
        if(file) {
            size_t len = SDL_strlen(textResource->text);
            if (SDL_RWwrite(file, textResource->text, 1, len) != len) {
                // Writing error
                SDL_RWclose(file);
                return 1;
            }
            SDL_RWclose(file);
        } else {
            // File openning error
            return 2;
        }
    } else {
        // NULL pointer to textResource
        return 3;
    }
    return 0;
}