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
#include "resourceManager/TextureResource.h"

struct TextureResource* TextureResource_construct(struct Renderer* renderer, const char* const path) {
    if (!renderer)
        return NULL;
    if (!path)
        return NULL;
    struct TextureResource* textureResource = NULL;
    textureResource = (struct TextureResource*)malloc(sizeof(struct TextureResource));
    if (!textureResource)
        return  NULL;
    textureResource->texture = IMG_LoadTexture(renderer->renderer, path);
    if (!textureResource->texture) {
        TextureResource_destruct(textureResource);
        return NULL;
    }
    return  textureResource;
}

void TextureResource_destruct(struct TextureResource* textureResource) {
    if (textureResource) {
        if (textureResource->texture)
            SDL_DestroyTexture(textureResource->texture);
        free(textureResource);
    }
}