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
#ifndef ALONE_TEXTRESOURCE_H
#define ALONE_TEXTRESOURCE_H

#include <stddef.h>

struct TextResource {
    size_t pointersCount;
    char* id;
    char* text;
    unsigned char isUnique;
};

struct TextResource* TextResource_construct(const char* const path, unsigned char unique);
void TextResource_destruct(struct TextResource* textResource);

/**
 * @brief Decreases TextResource#pointersCount.
 * Useful for deffered resource managment by #ResourceManager.
 * @param textResource Pointer to a #TextResource. Can be NULL.
 * @see #ResourceManager
 */
void TextResource_decreasePointersCounter(struct TextResource* textResource);

unsigned char TextResource_updateContent(struct TextResource* textResource, const char* const text);
unsigned char TextResource_save(struct TextResource* textResource, const char* const path);
#endif //ALONE_TEXTRESOURCE_H