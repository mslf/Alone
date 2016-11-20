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
#include <renderer/Renderer.h>
#include <SDL2/SDL_ttf.h>
#include "resourceManager/TextureResource.h"

const char* const TEXTURE_RESOURCE_ERR_IMG_LOADING_TEXTURE =
        "TextureResource_construct: loading texture failed!";
const char* const TEXTURE_RESOURCE_ERR_OPENING_FONT = "TextureResource_constructFromText: loading font failed!";
const char* const TEXTURE_RESOURCE_ERR_RENDERING_TEXT =
        "TextureResource_constructFromText: rendering text to surface failed!";
const char* const TEXTURE_RESOURCE_ERR_CREATING_TEXTURE_FROM_SURFACE =
        "TextureResource_constructFromText: creating texture from surface failed!";

struct TextureResource* TextureResource_construct(struct Renderer* renderer, const char* const path) {
    if (!renderer || !path)
        return NULL;
    struct TextureResource* textureResource = NULL;
    textureResource = (struct TextureResource*)calloc(1, sizeof(struct TextureResource));
    if (!textureResource)
        return  NULL;
    textureResource->texture = NULL;
    textureResource->id = (char*)malloc(sizeof(char) * (strlen(path) + 1));
    if (!textureResource->id) {
        TextureResource_destruct(textureResource);
        return NULL;
    }
    strcpy(textureResource->id, path);
    textureResource->texture = IMG_LoadTexture(renderer->renderer, path);
    if (!textureResource->texture) {
        Logger_log(renderer->logger, "%s SDL_image error: %s",
                   TEXTURE_RESOURCE_ERR_IMG_LOADING_TEXTURE,
                   IMG_GetError());
        TextureResource_destruct(textureResource);
        return NULL;
    }
    textureResource->pointersCount = 1;
    return  textureResource;
}

char* TextureResource_convertTextParametersToString(const char* const text, const char* const fontPath, int size,
                                                    SDL_Color color) {
    if (!text || !fontPath || size <= 0)
        return NULL;
    char* id = NULL;
    // 100 is a magic const (to fit size and color)
    id = (char*)malloc(sizeof(char) * (strlen(text) + strlen(fontPath) + 100));
    if (!id)
        return NULL;
    sprintf(id, "%s:%s:%d:%d:%d:%d:%d", fontPath, text, size, color.r, color.g, color.b, color.a);
    return id;
}

struct TextureResource* TextureResource_constructFromText(struct Renderer* renderer, const char* const text,
                                                          const char* const fontPath, int size, SDL_Color color) {
    if (!renderer || !text || !fontPath || size <= 0)
        return NULL;
    struct TextureResource* textureResource = NULL;
    textureResource = (struct TextureResource*)calloc(1, sizeof(struct TextureResource));
    if (!textureResource)
        return  NULL;
    TTF_Font* font = TTF_OpenFont(fontPath, size);
    if (!font) {
        Logger_log(renderer->logger, "%s SDL_ttf error: %s", TEXTURE_RESOURCE_ERR_OPENING_FONT, TTF_GetError());
        TextureResource_destruct(textureResource);
        return  NULL;
    }
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(font, text, color);
    if (!textSurface) {
        Logger_log(renderer->logger, "%s SDL_ttf error: %s", TEXTURE_RESOURCE_ERR_RENDERING_TEXT, TTF_GetError());
        TextureResource_destruct(textureResource);
        TTF_CloseFont(font);
        return  NULL;
    }
    textureResource->texture = SDL_CreateTextureFromSurface(renderer->renderer, textSurface);
    if (!textureResource->texture) {
        Logger_log(renderer->logger, "%s SDL error: %s",
                   TEXTURE_RESOURCE_ERR_CREATING_TEXTURE_FROM_SURFACE,
                   SDL_GetError());
        TextureResource_destruct(textureResource);
        TTF_CloseFont(font);
        SDL_FreeSurface(textSurface);
        return  NULL;
    }
    textureResource->id = TextureResource_convertTextParametersToString(text, fontPath, size, color);
    textureResource->pointersCount = 1;
    TTF_CloseFont(font);
    SDL_FreeSurface(textSurface);
    return textureResource;
}

void TextureResource_destruct(struct TextureResource* textureResource) {
    if (!textureResource)
        return;
    if (textureResource->id)
        free(textureResource->id);
    if (textureResource->texture)
        SDL_DestroyTexture(textureResource->texture);
    free(textureResource);
}

void TextureResource_decreasePointersCounter(struct TextureResource* textureResource) {
    if (textureResource && textureResource->pointersCount > 0)
        textureResource->pointersCount--;
}
