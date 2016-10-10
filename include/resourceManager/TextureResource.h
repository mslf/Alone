//
// Created by mslf on 9/21/16.
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
#ifndef ALONE_TEXTURERESOURCE_H
#define ALONE_TEXTURERESOURCE_H

#include <SDL2/SDL_image.h>
struct Renderer;

struct TextureResource {
    size_t pointersCount;
    char* id;
    SDL_Texture* texture;
};

struct TextureResource* TextureResource_construct(struct Renderer* renderer, const char* const path);
void TextureResource_destruct(struct TextureResource* textureResource);

#endif //ALONE_TEXTURERESOURCE_H
