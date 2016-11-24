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
 * @file Scene.h
 * @author mslf
 * @date 11 Aug 2016
 * @brief File containing #Scene and it's stuff.
 */
#ifndef ALONE_SCENE_H
#define ALONE_SCENE_H

#include <stddef.h>
#include "scene/SceneNodeTypesRegistrar.h"
#include "scene/SceneNode.h"
#include "resourceManager/ResourceManager.h"
#include "eventManager/EventManager.h"

/**
 * @brief Strings which are used for constructing #Scene from #TextParser.
 * @see Scene_construct
 * @note Scene_parserStrings_parserStrings#nodes doesn't contain actual 
 * settings (such an ID or initial coordinates, etc) of the constructed #SceneNode.
 * It contains array of #SceneNode names, which will be found in #TextParser, so they will contain actual data.
 * @note Scene_parserStrings_parserStrings#controllers doesn't contain actual 
 * settings (such an ID period) of the constructed #PeriodicEventController.
 * It contains array of #PeriodicEventController names, which will be found in #TextParser, so they will contain actual data. 
 */
static const struct Scene_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #Scene resource from other. */
    const char* const nodes;
    /**< List of strings with name of #SceneNode, which #Scene will find in #TextParser. */
    const char* const controllers;
    /**< List of strings with name of #PeriodicEventController, which #Scene will find in #TextParser. */
}Scene_parserStrings= {
    "Scene",
    "sceneNodes",
    "eventControllers"};

/**
 * @brief Error codes for #Scene.
 */
enum Scene_errors {
    SCENE_NO_ERRORS = 0,
    /**< All right, no errors. */
    SCENE_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's argument is NULL. */
    SCENE_ERR_ALLOC_NODES_LIST = 2,
    /**< Allocating memory for Scene#sceneNodesList failed. */
    SCENE_ERR_ALLOC_CONTROLLERS_LIST = 3,
    /**< Allocating memory for Scene#eventControllersList failed. */
    SCENE_ERR_NO_NODE_DEF = 4,
    /**< Definition of #SceneNode string (from #TextParser, Scene_parserStrings#nodes) haven't found. */
    SCENE_ERR_CONSTRUCTING_TEXT_PARSER = 5,
    /**< Constructing new #TextParser for some required reason failed. */
    SCENE_ERR_NO_NODES = 6,
    /**< No Scene_parserStrings#nodes string found in #TextParser. */
    SCENE_ERR_NO_CONTROLLERS = 7,
    /**< No Scene_parserStrings#controllers string found in #TextParser. */
    SCENE_ERR_CONSTRUCTING_NODE = 8,
    /**< Constructing new #SceneNode to add to the Scene#sceneNodesList failed. */
    SCENE_ERR_SAVING = 9,
    /**< Saving #Scene failed due to internal reason. */
    SCENE_ERR_CONSTRUCTING_CONTROLLER = 10,
    /**< Constructing new #ScriptResource to add to the Scene#eventControllersList failed. */
};
    
/**
 * @brief Some initial constants for #Scene.
 */
enum Scene_constants{
    SCENE_SCENE_NODES_REALLOCATION_STEP = 10,
    /**< Init alocating number and reallocating step for Scene#sceneNodesList. */
    SCENE_EVENT_CONTROLLERS_REALLOOCATION_STEP = 10
    /**< Init alocating number and reallocating step for Scene#eventControllersList. */
};

/**
 * @brief #ScriptResource with counter for periodic execution.
 */
struct PeriodicEventController {
    struct ScriptResource* script;
    /**< Periodic controller of the #Scene. */
    size_t counter;
    /**< Current missed frames counter. */
    size_t period;
    /**< Period of the #ScriptResourcce execution. */
};

/**
 * @brief Represents some array of #SceneNode and #ScriptResource, which are controls its behaviour.
 */
struct Scene {
    struct TextResource* sceneResource;
    /**< #TextResource with settings of this #Scene.*/
    struct SceneNode** sceneNodesList;
    /**< Array of pointers to #SceneNode. #Scene contaings these #SceneNode and it is in charge of them.*/
    struct PeriodicEventController* eventControllersList;
    /**< Array of pointers to #PeriodicEventController, which are controls this #Scene. */
    size_t allocatedSceneNodesCount;
    /**< Allocated number of #SceneNode in the Scene#sceneNodesList. */
    size_t sceneNodesCount;
    /**< Current number of existing #SceneNode in the Scene#sceneNodesList. */
    size_t allocatedEventControllersCount;
    /**< Allocated number of #PeriodicEventController in the Scene#eventControllersList. */
    size_t eventControllersCount;
    /**< Current number of existing #PeriodicEventController in the Scene#eventControllersList. */
};

/**
 * @brief Constructs #Scene and inits it from #TextParser.
 * @param resourceManager Pointer to a #ResourceManager for loading all needed resources. Can be NULL.
 * @param renderer Pointer to a #Renderer for creating #SceneNode, that needs #Renderer. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar for creating
 * <B>any registered before</B>#SceneNode. Can be NULL.
 * @param sceneResId String with ID (path) to the #Scene resource. Can be NULL.
 * @return Pointer to a #Scene or NULL if something general failed.
 * @note #Scene will be created even if some #SceneNode or #ScriptResource creating failed. 
 * So returning empty #Scene is normal.
 * @note Use this function in pair with Scene_destruct().
 * @see #SceneNode
 * @see #SceneNodeTypesRegistrar
 */
struct Scene* Scene_construct(struct ResourceManager* const resourceManager,
                              struct Renderer* renderer,
                              struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar, 
                              const char* const sceneResId);

/**
 * @brief Destructs #Scene and frees memory, used by it.
 * Also destructs all #SceneNodes in Scene#sceneNodesList.
 * @param scene Pointer to a #Scene. Can be NULL. Can be not fully initialized.
 */
void Scene_destruct (struct Scene* scene);

/**
 * @brief Constructs and adds <B>any registered before</B> type of #SceneNode to a Scene#sceneNodesList,
 * increases Scene#sceneNodesCount and reallocates Scene#sceneNodesList (if needed).
 * @param scene Pointer to a #Scene where to add #SceneNode. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager for loading all needed by #SceneNode resources. Can be NULL.
 * @param renderer Pointer to a #Renderer for creating #SceneNode, that needs #Renderer. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar for creating
 * <B>any registered before</B>#SceneNode. Can be NULL.
 * @param sceneNodeResId String with ID (path) to the #SceneNode resource. Can be NULL.
 * @return #Scene_errors value.
 * @see #Scene_errors
 * @see #SceneNode
 * @see #SceneNodeTypesRegistrar
 */
enum Scene_errors Scene_addSceneNode(struct Scene* scene,
                                     struct ResourceManager* const resourceManager,
                                     struct Renderer* renderer,
                                     struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                     const char* const sceneNodeResId);

/**
 * @brief Destructs and removes #SceneNode from Scene#sceneNodesList.
 * Decreases Scene#sceneNodesCount and shifts Scene#sceneNodesList to the left.
 * @param scene Pointer to a #Scene, where to remove #SceneNode. Can be NULL.
 * @param index Index of #SceneNode in Scene#sceneNodesList. Can be any number.
 * @see #SceneNode
 */
void Scene_removeSceneNode(struct Scene* const scene, size_t index);

/**
 * @brief Saves #Scene to the filesystem via #ResourceManager.
 * @param scene Pointer to a #Scene to be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used 
 * to save Scene#sceneResource. Can be NULL.
 * @param sceneResId Path string, where #ResourceManager will 
 * save Scene#sceneResource. Can be NULL.
 * @return #Scene_errors value.
 * @see #Scene_errors
 */
enum Scene_errors Scene_save(struct Scene* const scene,
                             struct ResourceManager* const resourceManager,
                             const char* const sceneResId);

/**
 * @brief Adds #PeriodicEventController to the Scene#eventControllersList.
 * increases Scene#eventControllersCount and reallocates Scene#eventControllersList (if needed).
 * @param scene Pointer to a #Scene where to add #PeriodicEventController. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to load #PeriodicEventController. Can be NULL.
 * @param scriptResId String with ID (path) to the #ScriptResource resource. Can be NULL.
 * @param period Execution period number to set PeriodicEventController#period.
 * @return #Scene_errors value.
 * @see #Scene_errors
 * @see #PeriodicEventController
 */
enum Scene_errors Scene_addEventControllerScript(struct Scene* scene,
                                                 struct ResourceManager* resourceManager,
                                                 const char* const scriptResId,
                                                 size_t period);

/**
 * @brief Removes #PeriodicEventController from Scene#eventControllersList.
 * Decreases Scene#eventControllersCount and shifts Scene#eventControllersList to the left.
 * @param scene Pointer to a #Scene, where to remove #SceneNode. Can be NULL.
 * @param scriptResId String with ID (path) of #ScriptResource in #PeriodicEventController in 
 * Scene#sceneNodesList to be removed. Can be NULL.
 * @see #SceneNode
 * @see #PeriodicEventController
 */
void Scene_removeEventControllerScript(struct Scene* scene,
                                       const char* const scriptResId);

/**
 * @brief Executes #PeriodicEventController in Scene#eventControllersList.
 * @param scene Pointer to a #Scene where to execute #PeriodicEventController. Can be NULL.
 * @see #PeriodicEventController
 */
void Scene_update(struct Scene* scene);
#endif //ALONE_SCENE_H
