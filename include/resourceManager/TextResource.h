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
 * @file TextResource.h
 * @author mslf
 * @date 22 Sep 2016
 * @brief File containing #TextResource and its stuff.
 */
#ifndef ALONE_TEXTRESOURCE_H
#define ALONE_TEXTRESOURCE_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Error codes for #TextResource.
 */
enum TextResource_errors {
    TEXT_RESOURCE_NO_ERRORS = 0,
    /**< All right, no errors. */
    TEXT_RESOURCE_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's argument is NULL. */
    TEXT_RESOURCE_ERR_ALLOC = 2,
    /**< Allocating memory for TextResource#text failed. */
    TEXT_RESOURCE_ERR_OPENNING = 3,
    /**< Openning file via SDL failed. */
    TEXT_RESOURCE_ERR_WRITING = 4
    /**< Writing TextResource#text via SDL to opened file failed. */
};

/**
 * @brief Resource, which is represents changeable string.
 * Also, you can save it back to the filesystem using TextResource_save().
 */
struct TextResource {
    size_t pointersCount;
    /**< Count of pointers to this resource. */
    char* id;
    /**< ID (path) string of this resource. */
    char* text;
    /**< Containing text string. */
    bool isUnique;
    /**< Flag indicates uniqueness of this #TextResource. */
};

/**
 * @brief Constructs #TextResource and inits it.
 * @param path String with path to the plain text file. Can be NULL.
 * @param unique Flag to set TextResource#unique.
 * @return Pointer to a #TextResource or NULL, if something failed.
 */
struct TextResource* TextResource_construct(const char* const path, bool unique);

/**
 * @brief Destructs #TextResource and frees memory, used by it.
 * @param textResource Pointer to a #TextResource. Can be NULL. Can be not fully initialized.
 */
void TextResource_destruct(struct TextResource* textResource);

/**
 * @brief Decreases TextResource#pointersCount.
 * Useful for deffered resource managment by #ResourceManager.
 * @param textResource Pointer to a #TextResource. Can be NULL.
 * @see #ResourceManager
 */
void TextResource_decreasePointersCounter(struct TextResource* textResource);

/**
 * @brief Updates TextResource#text string.
 * @param textResource Pointer to a #TextResource where to update TextResource#text string. Can be NULL.
 * @param text String, to be copyied to the TextResource#text. Can be NULL.
 * @return #TextResource_errors value.
 * @see #TextResource_errors
 */
enum TextResource_errors TextResource_updateContent(struct TextResource* textResource, const char* const text);

/**
 * @brief Writes TextResource#text to the file in the filesystem.
 * @param textResource Pointer to a #TextResource whose TextResource#text will be saved. Can be NULL.
 * @param path String with a path to the file in filesystem (if doesn't exist then will be created) where 
 * TextResource#text will be wrote. Can be NULL. If it is NULL, then path will be given from TextResource#id.
 * @return #TextResource_errors value.
 * @see #TextResource_errors
 */
enum TextResource_errors TextResource_save(struct TextResource* textResource, const char* const path);
#endif //ALONE_TEXTRESOURCE_H
