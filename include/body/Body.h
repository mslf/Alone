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
 * @file Body.h
 * @author mslf
 * @date 11 Aug 2016
 * @brief File containing #Body and it's stuff.
 */
#ifndef ALONE_BODY_H
#define ALONE_BODY_H

#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "sprite/Sprite.h"
#include "scene/SceneNodeTypesRegistrar.h"

/**
 * @brief Strings which are used for constructing #Body from #TextParser.
 * @see Body_construct
 */
static const struct BodySceneNode_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #Body resource from others. */
    const char* const logicalSize;
    /**< Two numbers to set Body#logicalSize. */
    const char* const spriteRes;
    /**< #Sprite resource string, which is used to construct Body#sprite. */
}BodySceneNode_parserStrings= {
        "Body",
        "logicalSize",
        "spriteResource"};

/**
 * @brief Error codes for #Body.
 */
enum BodySceneNode_errors {
    BODY_NO_ERRORS = 0,
    /**< All right, no errors. */
    BODY_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    BODY_ERR_NO_SPRITE_RES = 2,
    /**< No BodySceneNode_parserStrings#spriteRes string found in #TextParser. */
    BODY_ERR_CONSTRUCTING_SPRITE = 6,
    /**< Constructing Body#sprite failed. */
    BODY_ERR_CONSTRUCTIG_TEXT_PARSER = 7,
    /**< Constructing new #TextParser for some required reason failed. */
    BODY_ERR_SAVING = 8
    /**< Saving #Body failed due to internal reason. */
};

/**
 * @brief Standart #PhysicalSceneNode inheritor which is just a physical version of #Sprite.
 * @warning #Sprite is an inheritor of the #PhysicalSceneNode.
 * It means, that other code is use <B>pointer casting</B> to #Sprite from #PhysicalSceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct PhysicalSceneNode blablaNode;</tt> at the begining of #Sprite struct, 
 * if you want code to work with #Sprite like with a #PhysicalSceneNode. 
 * More, you <B>SHOULD</B> initialize <tt>blablaNode</tt> by calling SceneNode_initPhysical().
 * Also function pointers in PhysicalSceneNode#sceneNode <B>SHOULD</B> be initialized to your function implementation.
 * Don't forget to add this warning comment to your own new #PhysicalSceneNode inheritors.
 * @see Body_update()
 * @see Body_render()
 */
struct Body {
    struct PhysicalSceneNode physicalSceneNode;
    /**< Inheritance stuff. */
    struct Sprite* sprite;
    /**< #Sprite, which is represents image of #Body. */
    SDL_Point logicalSize;
    /**< #Body logical size, which will be used for iteracting with other #Body.
     * I.e. can be less or more then Sprite#virtualSize. */
    bool isGeometryChanged;
    /**< Flag, which is need to be changed when someone tried to iteract with #Body. */
};


/**
 * @brief Constructs and inits #Body from #TextParser.
 * @param resourceManager Pointer to a #ResourceManager which is needed to create 
 * Body#sprite. Can be NULL.
 * @param renderer Pointer to a #Renderer to create a Body#sprite. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar to create a Body#sprite. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #Body. Can be NULL.
 * @return Pointer to an initialized #Body, casted to #SceneNode or NULL, if failed.
 * @note Use only Body_destruct() or Body#physicalSceneNode#dynamicSceneNode#sceneNode#destruct
 * to destruct returned #SceneNode.
 * @see #SceneNodeTypesRegistrar
 * @see #Body
 * @see #TextureResource
 * @see #ResourceManager
 * @see #Renderer
 */
struct SceneNode* Body_construct(struct ResourceManager* const resourceManager,
                                 struct Renderer* const renderer,
                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                 struct TextParser* const textParser);

/**
 * @brief Destructs #Body and frees memory, used by it.
 * Pointer to that function should be at Body#physicalSceneNode#dynamicSceneNode#sceneNode#destruct.
 * @param body Pointer to a #Body, casted to #SceneNode. Can be not fully initialized. Can be NULL.
 * @warning Use this function only in pair with Body_construct()!
 * @see Body_construct()
 */
void Body_destruct(struct SceneNode* body);

/**
 * @brief Saves #Body to the filesystem via #ResourceManager.
 * Before saving, it updates Body#dynamicSceneNode#sceneNode#sceneNodeTextResource with the latest changes in #Body.
 * @param body Pointer to a #Body which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save Body#physicalSceneNode#dynamicSceneNode#sceneNode#sceneNodeBodyResource. Can be NULL.
 * @param spriteResId Path string, where #ResourceManager will 
 * save Body#dynamicSceneNode#sceneNode#sceneNodeBodyResource. Can be NULL.
 * @return #BodySceneNode_errors value.
 * @see #Body
 * @see #SceneNode
 * @see #ResourceManager
 * @see #BodySceneNode_errors
 */
enum BodySceneNode_errors Body_save(const struct Body* const body,
                                    struct ResourceManager* const resourceManager,
                                    const char* const bodyResId);

/**
 * @brief Function to update #Body and interacting with #EventManager.
 * Pointer to that function should be at Body#physicalSceneNode#dynamicSceneNode#sceneNode#update.
 * @param sceneNode Pointer to a #Body, casted to #SceneNode. Can be NULL.
 * @param eventManager Not used. Just here due to univirsal constructors. Can be NULL.
 * @param renderer Pointer to a #Renderer for screen-depend coordinates converting. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Body is not accepted!
 * @note Don't use this function for drawing.
 */
void Body_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);

/**
 * @brief Function to render #Body on the screen.
 * Pointer to that function should be at Body#physicalSceneNode#dynamicSceneNode#sceneNode#render.
 * @param sceneNode Pointer to a #Body, casted to #SceneNode. Can be NULL.
 * @param renderer Pointer to a #Renderer for drawing #Body. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Body is not accepted!
 */
void Body_render(struct SceneNode* sceneNode, struct Renderer* renderer);

#endif //ALONE_BODY_H
