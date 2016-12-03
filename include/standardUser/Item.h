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
* @file Item.h
* @author mslf
* @date 3 Dec 2016
* @brief File containing #Item and its stuff.
*/
#ifndef ALONE_ITEM_H
#define ALONE_ITEM_H

#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "body/Body.h"
#include "scene/SceneNodeTypesRegistrar.h"

/**
 * @brief Strings which are used for constructing #Item from #TextParser.
 * @see Item_construct
 */
static const struct ItemSceneNode_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #Item resource from others. */
    const char* const bodyRes;
    /**< #Body resource string, which is used to construct Item#body. */
    const char* const inventoryItemRes;
    /**< #InventoryItem resource string, which is used to add appropriate #InventoryItem when #Item
     * will be picked by #StandardUser. */
    const char* const count;
    /**< Number of count #InventoryItem, which is used to set InventoryItem#count when #Item
     * will be picked by #StandardUser. */
    const char* const pickedEventRes;
    /**< #GameEvent resource, which will happen when #Item will be picked by #StandardUser. */
}ItemSceneNode_parserStrings= {
        "Item",
        "bodyResource",
        "inventoryItemResource",
        "count",
        "pickedEventResource"};

/**
 * @brief Error codes for #Item.
 */
enum ItemSceneNode_errors {
    ITEM_NO_ERRORS = 0,
    /**< All right, no errors. */
    ITEM_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    ITEM_ERR_NO_SPRITE_RES = 2,
    /**< No ItemSceneNode_parserStrings#spriteRes string found in #TextParser. */
    ITEM_ERR_CONSTRUCTING_SPRITE = 3,
    /**< Constructing Item#sprite failed. */
    ITEM_ERR_CONSTRUCTIG_TEXT_PARSER = 4,
    /**< Constructing new #TextParser for some required reason failed. */
    ITEM_ERR_SAVING = 5
    /**< Saving #Item failed due to internal reason. */
};

/**
 * @brief Standard #PhysicalSceneNode inheritor which is an physical equivalent of #InventoryItem.
 * @warning #Item is an inheritor of the #PhysicalSceneNode.
 * It means, that other code is use <B>pointer casting</B> to #Item from #PhysicalSceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct PhysicalSceneNode blablaNode;</tt> at the begining of #Item struct,
 * if you want code to work with #Item like with a #PhysicalSceneNode.
 * More, you <B>SHOULD</B> initialize <tt>blablaNode</tt> by calling SceneNode_initPhysical().
 * Also function pointers in PhysicalSceneNode#sceneNode <B>SHOULD</B> be initialized to your function implementation.
 * Don't forget to add this warning comment to your own new #PhysicalSceneNode inheritors.
 * @see Item_update()
 * @see Item_render()
 */
struct Item {
    struct PhysicalSceneNode physicalSceneNode;
    /**< Inheritance stuff. */
    struct Body* body;
    /**< #Body, which is represents physical and graphical part of #Item. */
    const char* inventoryItemRes;
    /**< String with ID (path) of #InventoryItem, which will be constructed when #Item will be picked by #StandardUser. */
    size_t count;
    /**< Count of such #InventoryItem in the that #Item. */
    bool isGeometryChanged;
    /**< Flag, which is need to be changed when someone tried to iteract with #Body. */
    struct TextResource* pickedEventResource;
    /**< #TextResource to construct Item#pickedEvent. */
    struct GameEvent* pickedEvent;
    /**< #GameEvent to be added to the #EventManager when the #Item will be picked by #StandardUser. */
};

/**
 * @brief Constructs and inits #Item from #TextParser.
 * @param resourceManager Pointer to a #ResourceManager which is needed to create 
 * Item#body and Item#pickedEvent. Can be NULL.
 * @param renderer Pointer to a #Renderer to create a Item#body and Item#pickedEvent. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar to create a Item#body. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #Item. Can be NULL.
 * @return Pointer to an initialized #Item, casted to #SceneNode or NULL, if failed.
 * @note Use only Item_destruct() or Item#physicalSceneNode#dynamicSceneNode#sceneNode#destruct
 * to destruct returned #SceneNode.
 * @see #SceneNodeTypesRegistrar
 * @see #Item
 * @see #TextureResource
 * @see #ResourceManager
 * @see #Renderer
 */
struct SceneNode* Item_construct(struct ResourceManager* const resourceManager,
                                 struct Renderer* const renderer,
                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                 struct TextParser* const textParser);

/**
 * @brief Destructs #Item and frees memory, used by it.
 * Pointer to that function should be at Item#physicalSceneNode#dynamicSceneNode#sceneNode#destruct.
 * @param item Pointer to a #Item, casted to #SceneNode. Can be not fully initialized. Can be NULL.
 * @warning Use this function only in pair with Item_construct()!
 * @see Item_construct()
 */
void Item_destruct(struct SceneNode* item);

/**
 * @brief Saves #Item to the filesystem via #ResourceManager.
 * Before saving, it updates Item#physicalSceneNode#dynamicSceneNode#sceneNode#sceneNodeTextResource
 * with the latest changes in #Item.
 * @param item Pointer to a #Item which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save Item#physicalSceneNode#dynamicSceneNode#sceneNode#sceneNodeTextResource. Can be NULL.
 * @param spriteResId Path string, where #ResourceManager will 
 * save Item#physicalSceneNode#dynamicSceneNode#sceneNode#sceneNodeTextResource. Can be NULL.
 * @return #ItemSceneNode_errors value.
 * @see #Item
 * @see #SceneNode
 * @see #ResourceManager
 * @see #ItemSceneNode_errors
 */
enum ItemSceneNode_errors Item_save(const struct Item* const item,
                                    struct ResourceManager* const resourceManager,
                                    const char* const itemResId);

/**
 * @brief Function to update #Item and interacting with #EventManager.
 * Pointer to that function should be at Item#physicalSceneNode#dynamicSceneNode#sceneNode#update.
 * @param sceneNode Pointer to a #Item, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will place its events. Can be NULL.
 * @param renderer Not used. Just here due to univirsal constructors. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Item is not accepted!
 * @note Don't use this function for drawing.
 */
void Item_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);

/**
 * @brief Function to render #Item on the screen.
 * Pointer to that function should be at Item#physicalSceneNode#dynamicSceneNode#sceneNode#render.
 * @param sceneNode Pointer to a #Item, casted to #SceneNode. Can be NULL.
 * @param renderer Pointer to a #Renderer for drawing #Item. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Item is not accepted!
 */
void Item_render(struct SceneNode* sceneNode, struct Renderer* renderer);

#endif //ALONE_ITEM_H
