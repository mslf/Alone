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
 * @file GameManager.c
 * @author mslf
 * @date 10 Aug 2016
 * @brief File containing implementation of #GameManager.
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "GameManager.h"
#include "sprite/Sprite.h"
#include "text/Text.h"
#include "gui/Button.h"
#include "gui/CheckBox.h"
#include "gui/ContextMenu.h"
#include "gui/ListBox.h"
#include "gui/ProgressBar.h"
#include "gui/Slider.h"
#include "gui/TextBox.h"
#include "item/Item.h"
#include "user/User.h"
#include "level/Level.h"

/**
 * @brief Error message strings for #GameManager.
 */
static const struct GameManager_errorMessages {
    const char* const errConstructingRegistrar;
    /**< Will be displayed when constructing GameManager#sceneNodeTypesRegistrar failed. */
    const char* const errRegisterTypes;
    /**< Will be displayed when register #SceneNode inheritor types to GameManager#sceneNodeTypesRegistrar failed. */
    const char* const errAllocScenesStack;
    /**< Will be displayed when allocating memory for GameManager#scenesStack failed. */
}GameManager_errorMessages = {
    "GameManager_construct: constructing SceneNodeTypesRegistrar failed!",
    "GameManager_construct: registering SceneNode inheritor types failed!",
    "GameManager_construct: allocating memory for scenesStack failed!"};

static const char* const GAME_MANAGER_DEFAULT_SETTINGS_PATH = "Alone.settings";
/**< Path, where #GameManager will try to load settings. */

/**
 * @brief Strings for #GameManager internal events.
 */
static const struct GameManager_events {
    const char* const commandExit;
    /**< String for #GameManager 'exit command' #GameEvent. */
    const char* const commandPushScene;
    /**< String for #GameManager 'push scene command' #GameEvent. */
    const char* const commandPopScene;
    /**< String for #GameManager 'pop scene command' #GameEvent. */
}GameManager_events = {
    "GameManager_exit",
    "GameManager_pushScene",
    "GameManager_popScene"};

/**
 * @brief Registers #SceneNode inheritor types to the GameManager#sceneNodeTypesRegistrar.
 * @param gm Pointer to a #GameManager, where types will be registered.
 * @return #GameManager_errors value.
 * @see #GameManager_errors
 * @see #SceneNodeTypesRegistrar
 */
static enum GameManager_errors GameManager_registerSceneNodeTypes(struct GameManager* gm) {
    assert(gm);
    unsigned char result = 0;
    result += (SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar,
                                                               SpriteSceneNode_parserStrings.type,
                                                               Sprite_construct) != 0);
    result += (SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar,
                                                               TextSceneNode_parserStrings.type,
                                                               Text_construct) != 0);
    result += (SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               ButtonSceneNode_parserStrings.type,
                                                               Button_construct) != 0);
    result += (SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               CheckBoxSceneNode_parserString.type,
                                                               CheckBox_construct) != 0);
    result += (SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               ProgressBarSceneNode_parserStrings.type,
                                                               ProgressBar_construct) != 0);
    result += (SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               ContextMenuSceneNode_parserStrings.type,
                                                               ContextMenu_construct) != 0);
    result += (SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               SliderSceneNode_parserStrings.type,
                                                               Slider_construct) != 0);
    result += (SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               TextBoxSceneNode_parserStrings.type,
                                                               TextBox_construct) != 0);
    result += (SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               ListBoxSceneNode_parserStrings.type,
                                                               ListBox_construct) != 0);
    /*result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               ITEM_SCENENODE_PARSER_TYPE_STRING,
                                                               NULL);
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               USER_SCENENODE_PARSER_TYPE_STRING,
                                                               NULL);
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               LEVEL_SCENENODE_PARSER_TYPE_STRING,
                                                               NULL);*/
    if (result)
        return GM_ERR_REGISTER_TYPES;
    return GM_NO_ERRORS;
}

struct GameManager* GameManager_construct() {
    struct GameManager* gm = NULL;
    gm = (struct GameManager*)calloc(1, sizeof(struct GameManager));
    if (gm) {
        unsigned char result = 0;
        gm->logger.state = LOGGER_ENABLED_STDERR;
        result += !(gm->eventManager = EventManager_construct(&(gm->logger)));
        result += !(gm->resourceManager = ResourceManager_construct(&(gm->logger)));
        gm->settings = Settings_construct(gm->resourceManager, GAME_MANAGER_DEFAULT_SETTINGS_PATH);
        result += !(gm->musican = Musican_construct(&(gm->logger),
                                                    gm->settings->isMusicActive,
                                                    gm->settings->isSoundActive));
        result += !(gm->renderer = Renderer_construct(&(gm->logger), gm->settings));
        if (!(gm->sceneNodeTypesRegistrar = SceneNodeTypesRegistrar_construct())) {
            Logger_log(&(gm->logger), GameManager_errorMessages.errConstructingRegistrar);
            result++;
        }
        if (GameManager_registerSceneNodeTypes(gm)) {
            Logger_log(&(gm->logger), GameManager_errorMessages.errRegisterTypes);
            result++;
        }
        if (!(gm->scenesStack = (struct Scene**)malloc(sizeof(struct Scene*) * GM_INITIAL_NUMBER_ALLOCATED_SCENES))) {
            Logger_log(&(gm->logger), GameManager_errorMessages.errAllocScenesStack);
            result++;
        }
        if (result) {
            GameManager_destruct(gm);
            return NULL;
        }
        gm->allocatedScenesCount = GM_INITIAL_NUMBER_ALLOCATED_SCENES;
        GameManager_pushScene(gm, gm->settings->mainScene);
    }
    return gm;
}

/**
 * @brief Handles #GameManager internal events.
 * @param gm Pointer to a #GameManager.
 */
static void GameManager_handleEvents(struct GameManager* gm) {
    assert(gm);
    if (EventManager_generateCustomEventsList(gm->eventManager, "GameManager"))
        return;
    for (size_t i = 0; i < gm->eventManager->customGameEventsCount; i++) {
        if (strcmp(gm->eventManager->customGameEventsList[i]->command, GameManager_events.commandExit) == 0)
            gm->eventManager->quit = true;
        if (strcmp(gm->eventManager->customGameEventsList[i]->command, GameManager_events.commandPushScene) == 0) {
            GameManager_pushScene(gm, gm->eventManager->customGameEventsList[i]->data);
            EventManager_removeEvent(gm->eventManager, gm->eventManager->customGameEventsList[i]);
        }
        if (strcmp(gm->eventManager->customGameEventsList[i]->command, GameManager_events.commandPopScene) == 0) {
            GameManager_popScene(gm);
            EventManager_removeEvent(gm->eventManager, gm->eventManager->customGameEventsList[i]);
        }
    }
}

enum GameManager_errors GameManager_main(struct GameManager* gm) {
    if (!gm)
        return GM_ERR_NULL_ARGUMENT;
    Logger_log(&(gm->logger), "***##_#_######***");
    size_t i;
    unsigned char state = 0;
    unsigned char color = 0;
    unsigned char counter = 0;
    SDL_StartTextInput();
    while(!gm->eventManager->quit) {
        gm->renderer->cameraPosition.x = (-state) *  counter;
        gm->renderer->cameraPosition.y = (-state) * counter;
        EventManager_updateSdlEvents(gm->eventManager);
        SDL_SetRenderDrawColor(gm->renderer->renderer, color, color, color, color);
        SDL_RenderClear(gm->renderer->renderer);
        if (gm->scenesCount > 0) {
            i = gm->scenesCount - 1;
            Scene_update(gm->scenesStack[i]);
            for(size_t j = 0; j < gm->scenesStack[i]->sceneNodesCount; j++) {
                struct SceneNode* tempSceneNode = gm->scenesStack[i]->sceneNodesList[j];
                if (tempSceneNode->isActive) {
                    if (tempSceneNode->control)
                        tempSceneNode->control(tempSceneNode, gm->eventManager);
                    if (tempSceneNode->update)
                        tempSceneNode->update(tempSceneNode, gm->eventManager, gm->renderer);
                    if (tempSceneNode->render)
                        tempSceneNode->render(tempSceneNode, gm->renderer);
                    if (tempSceneNode->sound)
                        tempSceneNode->sound(tempSceneNode, gm->musican);
                }
            }
        }
        SDL_RenderPresent(gm->renderer->renderer);
        GameManager_handleEvents(gm);
        counter++;
        if (counter >= 200) {
            counter = 0;
            switch (state) {
                case 0:
                    color = 255;
                        state = 1;
                    break;
                case 1:
                    color = 0;
                        state = 0;
                    break;
            }
        }
    }
    SDL_StopTextInput();
    return GM_NO_ERRORS;
}

void GameManager_destruct(struct GameManager* gm) {
    if (!gm)
        return;
    if (gm->scenesStack) {
        for (size_t i = 0; i < gm->scenesCount; i++)
            Scene_destruct(gm->scenesStack[i]);
        free(gm->scenesStack);
    }
    Settings_destruct(gm->settings);
    if (gm->eventManager)
        EventManager_destruct(gm->eventManager);
    if (gm->resourceManager)
        ResourceManager_destruct(gm->resourceManager);
    if (gm->musican)
        Musican_destruct(gm->musican);
    if (gm->renderer)
        Renderer_destruct(gm->renderer);
    if (gm->sceneNodeTypesRegistrar)
        SceneNodeTypesRegistrar_destruct(gm->sceneNodeTypesRegistrar);
    free(gm);
}

/**
 * @brief Reallocates memory for GameManager#scenesStack, increases GameManager#allocatedScenesCount 
 * by GM_INITIAL_NUMBER_ALLOCATED_SCENES.
 * @param gm Pointer to a #GameManager, where function will reallocate GameManager#scenesStack.
 * @return #GameManager_errors value.
 * @see #GameManager_errors
 */
static enum GameManager_errors GameManager_reallocateSceneNodesList(struct GameManager* gm) {
    assert(gm);
    struct Scene** scenesStack = NULL;
    size_t i;
    size_t newSize = gm->allocatedScenesCount + GM_INITIAL_NUMBER_ALLOCATED_SCENES;
    scenesStack = (struct Scene**)realloc(gm->scenesStack, sizeof(struct Scene*) * newSize);
    if (!scenesStack)
        return GM_ERR_ALLOC_SCENES_STACK;
    gm->scenesStack = scenesStack;
    gm->allocatedScenesCount = newSize;
    return GM_NO_ERRORS;
}

enum GameManager_errors GameManager_pushScene(struct GameManager* gm, const char* const resId) {
    if (!gm || !resId)
        return GM_ERR_NULL_ARGUMENT;
    for (size_t i = 0; i < gm->scenesCount; i++)
        if (strcmp(resId, gm->scenesStack[i]->sceneResource->id) == 0)
            return GM_ERR_EXISTING_SCENE;
    struct Scene* scene = NULL;
    scene = Scene_construct(gm->resourceManager,
                            gm->renderer,
                            gm->sceneNodeTypesRegistrar,
                            resId);
    if (!scene)
        return GM_ERR_SCENE_CONSTRUCTING;
    if (gm->scenesCount >= gm->allocatedScenesCount)
        if(GameManager_reallocateSceneNodesList(gm)) {
            Scene_destruct(scene);
            return GM_ERR_ALLOC_SCENES_STACK;
        }
    gm->scenesStack[gm->scenesCount]= scene;
    gm->scenesCount++;
}

void GameManager_popScene(struct GameManager* gm) {
    if (!gm)
        return;
    Scene_destruct(gm->scenesStack[gm->scenesCount - 1]);
    gm->scenesCount--;
}
