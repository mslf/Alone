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
 * @file Sprite.h
 * @author mslf
 * @date 11 Aug 2016
 * @brief File containing #Sprite and its stuff.
 */
#ifndef ALONE_SPRITE_H
#define ALONE_SPRITE_H

#include <stddef.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "scene/SceneNodeTypesRegistrar.h"

/**
 * @brief Strings which are used for constructing #Sprite from #TextParser.
 * @see Sprite_construct
 */
static const struct SpriteSceneNode_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #Sprite resource from others. */
    const char* const frameCountsList;
    /**< Array of numbers to set Animation#framesCount in Sprite#animations. */
    const char* const frameDurationsList;
    /**< Array of numbers to set Animation#frameDuration in Sprite#animations. */
    const char* const frameSize;
    /**< Two to set Sprite#frameSize. */
    const char* const virtualSize;
    /**< Two to set Sprite#virtualSize. */
    const char* const textureRes;
    /**< #TextureResource string, which is used to construct Sprite#textureResource. */
}SpriteSceneNode_parserStrings= {
    "Sprite",
    "framesCountsList",
    "frameDurationsList",
    "frameSize",
    "virtualSize",
    "textureResource"};

/**
 * @brief Error codes for #Sprite.
 */
enum SpriteSceneNode_errors {
    SPRITE_NO_ERRORS = 0,
    /**< All right, no errors. */
    SPRITE_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    SPRITE_ERR_GETTING_TEXTURE_SIZE = 2,
    /**< Getting size of Sprite#textureResource failed. */
    SPRITE_ERR_FRAME_SIZE_NOT_FIT_H = 3,
    /**< GIven (or divided by 2) frame width multiplyed by framesCount doesn't fit into Sprite#textureResource. */
    SPRITE_ERR_FRAME_SIZE_NOT_FIT_V = 4,
    /**< GIven (or divided by 2) frame height multiplyed by animationsCount doesn't fit into Sprite#textureResource. */
    SPRITE_ERR_ANIMATIONS_ALLOC = 5,
    /**< Allocating memory for Sprite#animations failed. */
    SPRITE_ERR_CONSTRUCTING_TEXTURE_RES = 6,
    /**< Constructing Sprite#textureResource failed. */
    SPRITE_ERR_CONSTRUCTIG_TEXT_PARSER = 7,
    /**< Constructing new #TextParser for some required reason failed. */
    SPRITE_ERR_SAVING = 8
    /**< Saving #Sprite failed due to internal reason. */
};    
    
/**
 * @brief Represents animation with constant delay between frames.
 */
struct Animation {
    size_t framesCount;
    /**< Count of frames in this animation. */
    size_t frameDuration;
    /**< Duration of each frame in this animation. */
};

/**
 * @brief Standart #DynamicSceneNode inheritor to render some texture.
 * Texture can be animated and can have a lot of animations.
 * Animations stores in plane texture like this:
 * <tt>A1Frame1 A1Frame2 A1Frame3 ... A1FrameN
 *     A2Frame1 A2Frame2 ... </tt>
 * So, frames of animation places in texture <B>horizontally</B>.
 * Animations itself places in texture <B>vertically</B>.
 * Frame size of each animation is constant.
 * Animations can have defferent count of frames and theirs duration.
 * @warning #Sprite is an inheritor of the #DynamicSceneNode.
 * It means, that other code is use <B>pointer casting</B> to #Sprite from #DynamicSceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct DynamicSceneNode blablaNode;</tt> at the begining of #Sprite struct, 
 * if you want code to work with #Sprite like with a #DynamicSceneNode. 
 * More, you <B>SHOULD</B> initialize <tt>blablaNode</tt> by calling SceneNode_initDynamic().
 * Also function pointers in DynamicSceneNode#sceneNode <B>SHOULD</B> be initialized to your function implementation.
 * Don't forget to add this warning comment to your own new #DynamicSceneNode inheritors.
 * @see #Animation
 * @see Sprite_update()
 * @see Sprite_render()
 */
struct Sprite {
    struct DynamicSceneNode dynamicSceneNode;
    /**< Inheritance stuff. */
    struct TextureResource* textureResource;
    /**< #TextureResource with frames which will be loaded by #ResourceManager. */
    struct Animation* animations;
    /**< Array of #Animation, set from #TextParser. */
    size_t animationsCount;
    /**< Current number of existing #Animation in Sprite#animations. */
    size_t currentAnimation;
    /**< Current active #Animation. */
    size_t currentFrame;
    /**< Current frame in #Animation. */
    SDL_Point frameSize;
    /**< Size of frame in Sprite#textureResource in real pixels. */
    SDL_Point virtualSize;
    /**< Size of frame in Sprite#textureResource in Upixels. */
    size_t renderingsCounter;
    /**< Counter of Sprite_render() calls. Used for updating animation's frame. */
    SDL_Rect srcRect;
    /**< Source rectangle of texture in Sprite#textureResource. */
    SDL_Rect dstRect;
    /**< Destination rectangle of texture in Sprite#textureResource. 
     * Scale, projecting (from real pixels to Upixels) and camera offset changes are performed. */
    SDL_Point percentsToRender;
    /**< Number of Sprite#textureResource % to be rendered. Affects on Sprite#srcRect and Sprite#dstRect. */
};

/**
 * @brief Constructs and inits #Sprite from #TextParser.
 * @param resourceManager Pointer to a #ResourceManager which is needed to create 
 * Sprite#textureResource. Can be NULL.
 * @param renderer Pointer to a #Renderer to create a Sprite#textureResource. Can be NULL.
 * @param sceneNodeTypesRegistrar Not used. Just here due to univirsal constructors. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #Sprite. Can be NULL.
 * @return Pointer to an initialized #Sprite, casted to #SceneNode or NULL, if failed.
 * @note Use only Sprite_destruct() or Sprite#dynamicSceneNode#sceneNode#destruct to destruct returned #SceneNode.
 * @see #Sprite
 * @see #TextureResource
 * @see #ResourceManager
 * @see #Renderer
 */
struct SceneNode* Sprite_construct(struct ResourceManager* const resourceManager,
                                                 struct Renderer* const renderer,
                                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                 struct TextParser* const textParser);

/**
 * @brief Destructs #Sprite and frees memory, used by it.
 * Pointer to that function should be at Sprite#dynamicSceneNode#sceneNode#destruct.
 * @param sprite Pointer to a #Sprite, casted to #SceneNode. Can be not fully initialized. Can be NULL.
 * @warning Use this function only in pair with Sprite_construct()!
 * @see Sprite_construct()
 */
void Sprite_destruct(struct SceneNode* sprite);

/**
 * @brief Changes Sprite#textureResource. Animations structure won't be changed.
 * New texture will be checked to be suitable for existing Sprite#animations and Sprite#frameSize.
 * @param sprite Pointer to a #Sprite, where Sprite#textureResource will be changed. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which will be used to load #TextureResource. Can be NULL.
 * @param renderer Pointer to a #Renderer which will be used for constructing #TextureResource. Can be NULL.
 * @param textureResId String with ID (path) of the #TextureResource. Can be NULL.
 * @return #SpriteSceneNode_errors value.
 * @see #SpriteSceneNode_errors
 */
enum SpriteSceneNode_errors Sprite_changeTextureResource(struct Sprite* sprite,
                                                         struct ResourceManager* resourceManager,
                                                         struct Renderer* renderer,
                                                         const char* const textureResId);

/**
 * @brief Saves #Sprite to the filesystem via #ResourceManager.
 * Before saving, it updates Sprite#dynamicSceneNode#sceneNode#sceneNodeSpriteResource with the latest changes in #Sprite.
 * @param sprite Pointer to a #Sprite which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save Sprite#dynamicSceneNode#sceneNode#sceneNodeSpriteResource. Can be NULL.
 * @param spriteResId Path string, where #ResourceManager will 
 * save Sprite#dynamicSceneNode#sceneNode#sceneNodeSpriteResource. Can be NULL.
 * @return #SpriteSceneNode_errors value.
 * @see #Sprite
 * @see #SceneNode
 * @see #ResourceManager
 * @see #SpriteSceneNode_errors
 */
enum SpriteSceneNode_errors Sprite_save(const struct Sprite* const sprite,
                                        struct ResourceManager* const resourceManager,
                                        const char* const spriteResId);

/**
 * @brief Function to update #Sprite and interacting with #EventManager.
 * Pointer to that function should be at Sprite#dynamicSceneNode#sceneNode#update.
 * @param sceneNode Pointer to a #Sprite, casted to #SceneNode. Can be NULL.
 * @param eventManager Not used. Just here due to univirsal constructors. Can be NULL.
 * @param renderer Pointer to a #Renderer for screen-depend coordinates converting. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Sprite is not accepted!
 * @note Don't use this function for drawing.
 */
void Sprite_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);

/**
 * @brief Function to render #Sprite on the screen.
 * Pointer to that function should be at Sprite#dynamicSceneNod#sceneNode#render.
 * @param sceneNode Pointer to a #Sprite, casted to #SceneNode. Can be NULL.
 * @param renderer Pointer to a #Renderer for drawing #Sprite. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Sprite is not accepted!
 */
void Sprite_render(struct SceneNode* sceneNode, struct Renderer* renderer);

#endif //ALONE_SPRITE_H
