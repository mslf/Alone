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
/**
 * @file TextureResource.h
 * @author mslf
 * @date 21 Sep 2016
 * @brief File containing #TextureResource and it's stuff.
 */
#ifndef ALONE_TEXTURERESOURCE_H
#define ALONE_TEXTURERESOURCE_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

struct Renderer;

/**
 * @brief Resource, which is represents drawable texture.
 */
struct TextureResource {
    SDL_Texture* texture;
    /**< SDL texture. */
    size_t pointersCount;
    /**< Count of pointers to this resource. */
    char* id;
    /**< ID (path or special path if generated from text) string of this resource. */
    bool isCreatedWithResourceManager;
    /**< Flag which is says us "Did this #TextureResource were created with #ResourceManager or not. */
};

/**
 * @brief Constructs #TextureResource and inits it.
 * @param renderer Pointer to a #Renderer for creating TextureResource#texture. Can be NULL.
 * @param path String with path to the image file. Can be NULL.
 * @return Pointer to a #TextureResource or NULL, if something failed.
 * @see #Renderer
 */
struct TextureResource* TextureResource_construct(struct Renderer* renderer, const char* const path);

/**
 * @brief Constructs #TextureResource and inits it from text string and some parameters.
 * @param renderer Pointer to a #Renderer for creating TextureResource#texture. Can be NULL.
 * @param text String with text to be rendered in TextureResource#texture. Can be NULL.
 * @param fontPath Sting with path to the ttf font, used for rendering TextureResource#texture. Can be NULL.
 * @param size Vertical size of TextureResource#texture.
 * @param color SDL_Color of text rendered in TextureResource#texture.
 * @return Pointer to a #TextureResource or NULL, if something failed.
 * @see #Renderer
 */
struct TextureResource* TextureResource_constructFromText(struct Renderer* renderer,
                                                          const char* const text,
                                                          const char* const fontPath,
                                                          int size, SDL_Color color);

/**
 * @brief Destructs #TextureResource and frees memory, used by it.
 * @param textureResource Pointer to a #TextureResource. Can be NULL. Can be not fully initialized.
 */
void TextureResource_destruct(struct TextureResource* textureResource);

/**
 * @brief Decreases TextureResource#pointersCount.
 * Useful for deffered resource managment by #ResourceManager.
 * @param textureResource Pointer to a #TextureResource. Can be NULL.
 * @see #ResourceManager
 */
void TextureResource_decreasePointersCounter(struct TextureResource* textureResource);

/**
 * @brief Converts some parameters to the special ID.
 * @param text String with text to rendered in TextureResource#texture. Can be NULL.
 * @param fontPath Sting with path to the ttf font, used for rendering TextureResource#texture. Can be NULL.
 * @param size Vertical size of text string in TextureResource#texture.
 * @param color SDL_Color of text rendered in TextureResource#texture.
 * @return String with special ID for generated from text #TextureResource or NULL, if something failed.
 * @see TextureResource_constructFromText()
 * @note You should free memory, used by that string by yourself.
 */
char* TextureResource_convertTextParametersToString(const char* const text,
                                                    const char* const fontPath,
                                                    int size,
                                                    SDL_Color color);

#endif //ALONE_TEXTURERESOURCE_H
