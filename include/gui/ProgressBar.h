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
 * @file ProgressBar.h
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing #ProgressBar and its stuff.
 */
#ifndef ALONE_PROGRESSBAR_H
#define ALONE_PROGRESSBAR_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "sprite/Sprite.h"

/**
 * @brief Strings which are used for constructing #ProgressBar from #TextParser.
 * @see ProgressBar_construct()
 */
static const struct ProgressBarSceneNode_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #ProgressBar resource from other. */
    const char* const spriteRes;
    /**< #Sprite resource string, which is used to construct ProgressBar#spriteBase 
     *and ProgressBar#spriteBar. Should have 2 animations. */
    const char* const value;
    /**< String with initial ProgressBar#value number. */
}ProgressBarSceneNode_parserStrings = {
    "ProgressBar",
    "spriteResource",
    "value"};

/**
 * @brief Error codes for #ProgressBar.
 */
enum ProgressBarSceneNode_errors {
    PROGRESS_BAR_NO_ERRORS = 0,
    /**< All right, no errors. */
    PROGRESS_BAR_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    PROGRESS_BAR_ERR_NO_SPRITE_RES = 2,
    /**< No ProgressBarSceneNode_parserStrings#spriteRes string found in #TextParser. */
    PROGRESS_BAR_ERR_CONSTRUCTIG_SPRITE = 3,
    /**< Constructing ProgressBar#spriteBase or ProgressBar#spriteBar failed. */
    PROGRESS_BAR_ERR_NOT_ENOUGH_ANIMATIONS = 4,
    /**< Constructed ProgressBar#sprite doesn't have enough animations. */
    PROGRESS_BAR_ERR_CONSTRUCTIG_TEXT_PARSER = 5,
    /**< Constructing new #TextParser for some required reason failed. */
    PROGRESS_BAR_ERR_SAVING = 6
    /**< Saving #ProgressBar failed due to internal reason. */
};
        
/**
 * @brief Standard Gui element for drawing some progress.
 * It draws part (in %) of ProgressBar#spriteBar on ProgressBar#spriteBase.
 * @warning #ProgressBar is an inheritor of the SceneNode.
 * It means, that other code is use <B>pointer casting</B> to #ProgressBar from #SceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct SceneNode blablaNode;</tt> at the begining of #ProgressBar struct, 
 * if you want code to work with #ProgressBar like with a #SceneNode. 
 * More, you <B>SHOULD</B> initialize function pointers in <tt>blablaNode</tt> to NULL (by calling SceneNode_init()) 
 * or to your function implementation. 
 * Don't forget to add this warning comment to your own new #SceneNode inheritors.
 * @see #Sprite
 * @see ProgressBar_update()
 * @see ProgressBar_render()
 */
struct ProgressBar {
    struct SceneNode sceneNode;
    /**< Inheritance stuff. */
    struct Sprite* spriteBase;
    /**< #Sprite to be displayed on the screen fully (100%). Should have 2 animations 
     *(first - for itself and second for ProgressBar#spriteBar). */
    struct Sprite* spriteBar;
    /**< #Sprite to be displayed on the screen in ProgressBar#value %. Should have 2 animations 
     *(first - for ProgressBar#spriteBase and second for itself). */
    unsigned char value;
    /**< Value of #ProgressBar that indicates what part of ProgressBar#spriteBar needed to be drawn. 
     *If set manually, then it should be between 0 and 100 (includes 100). */
    bool isGeometryChanged;
    /**< Flag, which is need to be changed when someone tried to move #ProgressBar. */
};

/**
 * @brief Constructs #ProgressBar and inits it from a #TextParser.
 * @param resourceManager Pointer to a #ResourceManager which is used to load additional 
 * resources, needed for #ProgressBar. Can be NULL.
 * @param renderer Pointer to a #Renderer which is used to construct ProgressBar#spriteBase and 
 * ProgressBar#spriteBar. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar which is used to 
 * construct ProgressBar#spriteBase and ProgressBar#spriteBar. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #ProgressBar. Can be NULL.
 * @return Pointer to an initialized #ProgressBar, casted to #SceneNode, or NULL if constructing failed.
 * @note Use only ProgressBar_destruct() or ProgressBar#sceneNode#destruct() to destruct returned #SceneNode.
 * @see #ProgressBar
 * @see #Sprite
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #ProgressBarSceneNode_parserStrings
 */
struct SceneNode* ProgressBar_construct(struct ResourceManager* const resourceManager,
                                        struct Renderer* const renderer,
                                        struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                        struct TextParser* const textParser);

/**
 * @brief Destructs #ProgressBar and frees memory, used by it.
 * Pointer to that function should be at ProgressBar#sceneNode#destruct.
 * @param progressBar Pointer to a #ProgressBar, casted to #SceneNode. Can be not fully initialized. Can be NULL.
 * @warning Use this function only in pair with ProgressBar_construct()!
 * @see ProgressBar_construct()
 */
void ProgressBar_destruct(struct SceneNode* progressBar);

/**
 * @brief Saves #ProgressBar to the filesystem via #ResourceManager.
 * Before saving, it updates ProgressBar#sceneNode#sceneNodeTextResource with the latest changes in #ProgressBar.
 * @param progressBar Pointer to a #ProgressBar which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save ProgressBar#sceneNode#sceneNodeTextResource. Can be NULL.
 * @param progressBarResId Path string, where #ResourceManager will 
 * save ProgressBar#sceneNode#sceneNodeTextResource. Can be NULL.
 * @return #ProgressBarSceneNode_errors value.
 * @see #ProgressBar
 * @see #SceneNode
 * @see #ResourceManager
 * @see #ProgressBarSceneNode_errors
 */
enum ProgressBarSceneNode_errors ProgressBar_save(const struct ProgressBar* const progressBar, 
                                                  struct ResourceManager* const resourceManager,
                                                  const char* const progressBarResId);

/**
 * @brief Function to update #ProgressBar and interacting with #EventManager.
 * Pointer to that function should be at ProgressBar#sceneNode#update.
 * @param sceneNode Pointer to a #ProgressBar, casted to #SceneNode. Can be NULL.
 * @param eventManager Not used. Just here due to univirsal constructors. Can be NULL.
 * @param renderer Pointer to a #Renderer for screen-depend coordinates converting. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #ProgressBar is not accepted!
 * @note Don't use this function for drawing.
 */
void ProgressBar_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);

/**
 * @brief Function to render #ProgressBar on the screen.
 * Pointer to that function should be at ProgressBar#sceneNode#render.
 * @param sceneNode Pointer to a #ProgressBar, casted to #SceneNode. Can be NULL.
 * @param renderer Pointer to a #Renderer for drawing #ProgressBar. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #ProgressBar is not accepted!
 */
void ProgressBar_render(struct SceneNode* sceneNode, struct Renderer* renderer);

#endif //ALONE_PROGRESSBAR_H
