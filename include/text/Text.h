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
 * @file Text.h
 * @author mslf
 * @date 11 Aug 2016
 * @brief File containing #Text and its stuff.
 */
#ifndef ALONE_TEXT_H
#define ALONE_TEXT_H

#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "scene/SceneNodeTypesRegistrar.h"

/**
 * @brief Strings which are used for constructing #Text from #TextParser.
 * @see Text_construct
 */
static const struct TextSceneNode_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #Text resource from others. */
    const char* const fontPath;
    /**< String to set Text#fontPath. */
    const char* const text;
    /**< String to set Text#text. */
    const char* const size;
    /**< Number to set Text#size. */
    const char* const color;
    /**< RGBA color (four numbers) to set Text#color. */
}TextSceneNode_parserStrings = {
    "Text",
    "fontPath",
    "text",
    "size",
    "color"};

/**
 * @brief Error codes for #Text.
 */
enum TextSceneNode_errors {
    TEXT_NO_ERRORS = 0,
    /**< All right, no errors. */
    TEXT_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    TEXT_ERR_GENERATING_TEXTURE = 3,
    /**< Generating Text#textureResource failed. */
    TEXT_ERR_GETTING_TEXTURE_SIZE = 4,
    /**< Getting size of Text#textureResource failed. */
    TEXT_ERR_STRING_ALLOC = 5,
    /**< Allocating string for Text#fontPath or Text#text failed. */
    TEXT_ERR_CONSTRUCTIG_TEXT_PARSER = 6,
    /**< Constructing new #TextParser for some required reason failed. */
    TEXT_ERR_SAVING = 7
    /**< Saving #Text failed due to internal reason. */
};
    
/**
 * @brief Standard #DynamicSceneNode inheritor to render some (typically static) text.
 * @warning #Text is an inheritor of the #DynamicSceneNode.
 * It means, that other code is use <B>pointer casting</B> to #Text from #DynamicSceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct DynamicSceneNode blablaNode;</tt> at the begining of #Text struct, 
 * if you want code to work with #Text like with a #DynamicSceneNode. 
 * More, you <B>SHOULD</B> initialize <tt>blablaNode</tt> by calling SceneNode_initDynamic().
 * Also function pointers in DynamicSceneNode#sceneNode <B>SHOULD</B> be initialized to your function implementation.
 * Don't forget to add this warning comment to your own new #DynamicSceneNode inheritors.
 * @see Text_update()
 * @see Text_render()
 */
struct Text {
    struct DynamicSceneNode dynamicSceneNode;
    /**< Inheritance stuff. */
    struct TextureResource* textureResource;
    /**< #TextureResource where Text#text will be rendered. */
    char* fontPath;
    /**< String with path to the ttf font to draw Text#text in Text#textureResource. */
    char* text;
    /**< String with text to be rendered in Text#textureResource. */
    int size;
    /**< Vertical size of text in Text#textureResource in Upixels. */
    SDL_Color color;
    /**< RGBA color of text in Text#textureResource. */
    SDL_Rect srcRect;
    /**< Source rectangle of texture in Text#textureResource. */
    SDL_Rect dstRect;
    /**< Destination rectangle of texture in Texet#textureResource. 
     * Scale, projecting (from real pixels to Upixels) and camera offset changes are performed. */
};

/**
 * @brief Default #Text.
 */
static const struct Text TextSceneNode_default = {
    .text = "Sample",
    .size = 12,
    .color = {0, 0, 0, 0},
    .fontPath = "data/default.ttf"
};

/**
 * @brief Constructs and inits #Text from #TextParser.
 * @param resourceManager Pointer to a #ResourceManager which is needed to create 
 * (cache) Text#textureResource. Can be NULL.
 * @param renderer Pointer to a #Renderer to create a Text#textureResource. Can be NULL.
 * @param sceneNodeTypesRegistrar Not used. Just here due to univirsal constructors. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #Text. Can be NULL.
 * @return Pointer to an initialized #Text, casted to #SceneNode or NULL, if failed.
 * @note Use only Text_destruct() or Text#dynamicSceneNode#sceneNode#destruct to destruct returned #SceneNode.
 * @see #Text
 * @see #TextureResource
 * @see #TextSceneNode_default
 * @see #ResourceManager
 * @see #Renderer
 */
struct SceneNode* Text_construct(struct ResourceManager* const resourceManager,
                                                 struct Renderer* const renderer,
                                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                 struct TextParser* const textParser);
/**
 * @brief Destructs #Text and frees memory, used by it.
 * Pointer to that function should be at Text#dynamicSceneNode#sceneNode#destruct.
 * @param text Pointer to a #Text, casted to #SceneNode. Can be not fully initialized. Can be NULL.
 * @warning Use this function only in pair with Text_construct()!
 * @see Text_construct()
 */
void Text_destruct(struct SceneNode* text);

/**
 * @brief Regenerates Text#textureResource with a new given settings.
 * @param text Poinnter to a #Text, where Text#textureResource will be regenerated. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager to create (cache) new Text#textureResource. Can be NULL.
 * If it is NULL, then created #TextureResource will not be cached, and you <B>SHOULD</B> destruct it by yourself.
 * @param renderer Pointer to a #Renderer to create a new Text#textureResource. Can be NULL. 
 * @param textString String with text to be rendered in Text#textureResource. Can be NULL.
 * @param fontPath String with path to the ttf font. Can be NULL.
 * @param size Vertical size of text in Upixels, which will be rendered in Text#textureResource.
 * @param color RGBA color of text, which will be rendered in Text#textureResource.
 * @return #TextSceneNode_errors value.
 * @see #TextSceneNode_errors
 * @see #TextureResource
 * @see #ResourceManager
 */
enum TextSceneNode_errors Text_regenerateTexture(struct Text* text,
                                                 struct ResourceManager* resourceManager,
                                                 struct Renderer* renderer,
                                                 const char* const textString,
                                                 const char* const fontPath,
                                                 int size,
                                                 SDL_Color color);

/**
 * @brief Saves #Text to the filesystem via #ResourceManager.
 * Before saving, it updates Text#dynamicSceneNode#sceneNode#sceneNodeTextResource with the latest changes in #Text.
 * @param text Pointer to a #Text which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save Text#dynamicSceneNode#sceneNode#sceneNodeTextResource. Can be NULL.
 * @param textResId Path string, where #ResourceManager will 
 * save Text#dynamicSceneNode#sceneNode#sceneNodeTextResource. Can be NULL.
 * @return #TextSceneNode_errors value.
 * @see #Text
 * @see #SceneNode
 * @see #ResourceManager
 * @see #TextSceneNode_errors
 */
enum TextSceneNode_errors Text_save(const struct  Text* const text,
                                    struct ResourceManager* const resourceManager,
                                    const char* const textResId);

/**
 * @brief Function to update #Text and interacting with #EventManager.
 * Pointer to that function should be at Text#dynamicSceneNode#sceneNode#update.
 * @param sceneNode Pointer to a #Text, casted to #SceneNode. Can be NULL.
 * @param eventManager Not used. Just here due to univirsal constructors. Can be NULL.
 * @param renderer Pointer to a #Renderer for screen-depend coordinates converting. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Text is not accepted!
 * @note Don't use this function for drawing.
 */
void Text_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);

/**
 * @brief Function to render #Text on the screen.
 * Pointer to that function should be at Text#dynamicSceneNod#sceneNode#render.
 * @param sceneNode Pointer to a #Text, casted to #SceneNode. Can be NULL.
 * @param renderer Pointer to a #Renderer for drawing #Text. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Text is not accepted!
 */
void Text_render(struct SceneNode* sceneNode, struct Renderer* renderer);

#endif //ALONE_TEXT_H
