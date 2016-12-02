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
 * @file GameManager.h
 * @author mslf
 * @date 10 Aug 2016
 * @brief File containing #GameManager and its stuff.
 */
#ifndef ALONE_GAMEMANAGER_H
#define ALONE_GAMEMANAGER_H

#include "logger/Logger.h"
#include "renderer/Renderer.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "eventManager/EventManager.h"
#include "settings/Settings.h"
#include "scene/SceneNodeTypesRegistrar.h"
#include "scene/Scene.h"

/**
 * @brief Some initial constants for #GameManager.
 */
enum GameManager_constants{
    GM_INITIAL_NUMBER_ALLOCATED_SCENES = 10
    /**< Init alocating number and reallocating step for GameManager#scenesStack. */
};

/**
 * @brief Error codes for #GameManager.
 */
enum GameManager_errors {
    GM_NO_ERRORS = 0,
    /**< All right, no errors. */
    GM_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's argument is NULL. */
    GM_ERR_REGISTER_TYPES = 2,
    /**< Register #SceneNode inheritor types to GameManager#sceneNodeTypesRegistrar failed. */
    GM_ERR_ALLOC_SCENES_STACK = 3,
    /**< Allocating memory for GameManager#scenesStack failed. */
    GM_ERR_EXISTING_SCENE = 4,
    /**< You are trying to push already existing #Scene in GameManager#scenesStack. */
    GM_ERR_SCENE_CONSTRUCTING = 5,
    /**< Constructing #Scene failed. */
    GM_ERR_SAVING = 9,
    /**< Saving #GameManager failed due to internal reason. */
    GM_ERR_CONSTRUCTING_CONTROLLER = 10,
    /**< Constructing new #ScriptResource to add to the GameManager#eventControllersList failed. */
};

/**
 * @brief Main system of Alone engine.
 */
struct GameManager {
    struct Logger logger;
    /**< Logging subsystem. */
    struct EventManager* eventManager;
    /**< Event subsystem. */
    struct ResourceManager* resourceManager;
    /**< Resource managment subsystem. */
    struct Renderer* renderer;
    /**< Rendering subsystem. */
    struct Musican* musican;
    /**< Music and sound subsystem. */
    struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar;
    /**< #SceneNode inheritor types registrar subsystem. */
    struct Scene** scenesStack;
    /**< Array of pointers to #Scene. */
    struct Settings* settings;
    /**< Current settings. */
    size_t allocatedScenesCount;
    /**< Current allocated number of #Scene in GameManager#scenesStack. */
    size_t scenesCount;
    /**< Current number of existing #Scene in GameManager#scenesStack. */
};

/**
 * @brief Constructs #GameManager and inits all subsystems.
 * @return Pointer to an initialized #GameManager or NULL, if something failed.
 * @note Use this function in pair with GameManager_destruct().
 */
struct GameManager* GameManager_construct();

/**
 * @brief Destructs #GameManager and its subsystems, and then frees memory, used by it.
 * @param gm Pointer to a #GameManager. Can be not fully initialized. Can be NULL.
 */
void GameManager_destruct(struct GameManager* gm);

/**
 * @brief Main loop of Alone engine is here.
 * @param gm Pointer to a #GameManager. Can be NULL.
 * @return #GameManager_errors value.
 * @see #GameManager_errors
 */
enum GameManager_errors GameManager_main(struct GameManager* gm);

/**
 * @brief Constructs and pushes #Scene on the GameManager#scenesStack.
 * Reallocates GameManager#scenesStack and increases GameManager#allocatedScenesCount, if needed.
 * Increases GameManager#scenesCount. 
 * @param gm Pointer to a #GameManager, where #Scene will be pushed. Can be NULL.
 * @param resId String with ID (path) of #Scene resource.
 * @return #GameManager_errors value.
 * @see #GameManager_errors
 */
enum GameManager_errors GameManager_pushScene(struct GameManager* gm, const char* const resId);

/**
 * @brief Destructs and pops #Scene from the GameManager#scenesStack.
 * Decreases GameManager#scenesCount.
 * @param gm Pointer to a #GameManager, where #Scene will be poped. Can be NULL.
 */
void GameManager_popScene(struct GameManager* gm);
#endif //ALONE_GAMEMANAGER_H
