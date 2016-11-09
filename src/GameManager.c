//
// Created by mslf on 8/10/16.
//
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
#include <stdlib.h>
#include <stdio.h>
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

const char* const GAME_MANAGER_ERR_REGISTRAR_CONSTRUCTING =
        "GameManager_construct: constructing sceneNodeTypesRegistrar failed!";
const char* const GAME_MANAGER_ERR_REGISTRAR_REGISTER =
        "GameManager_construct: registering SceneNode types failed!";
const char* const GAME_MANAGER_ERR_SCENES_STACK_ALLOC =
        "GameManager_construct: allocating memory for scenesStack failed!";
const char* const GAME_MANAGER_DEFAULT_SETTINGS_PATH = "Alone.settings";
const char* const GAME_MANAGER_COMMAND_EXIT = "GameManager_exit";
const char* const GAME_MANAGER_COMMAND_PUSH = "GameManager_pushScene";
const char* const GAME_MANAGER_COMMAND_POP = "GameManager_popScene";

static unsigned char GameManager_registerSceneNodeTypes(struct GameManager* gm) {
    unsigned char result = 0;
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar,
                                                               SPRITE_SCENENODE_PARSER_TYPE_STRING,
                                                               Sprite_construct);
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar,
                                                               TEXT_SCENENODE_PARSER_TYPE_STRING,
                                                               Text_construct);
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               BUTTON_SCENENODE_PARSER_TYPE_STRING,
                                                               Button_construct);
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               CHECK_BOX_SCENENODE_PARSER_TYPE_STRING,
                                                               CheckBox_construct);
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               PROGRESS_BAR_SCENENODE_PARSER_TYPE_STRING,
                                                               ProgressBar_construct);
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               CONTEXT_MENU_SCENENODE_PARSER_TYPE_STRING,
                                                               ContextMenu_construct);
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               SLIDER_SCENENODE_PARSER_TYPE_STRING,
                                                               Slider_construct);
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               TEXT_BOX_SCENENODE_PARSER_TYPE_STRING,
                                                               TextBox_construct);
    /*result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               LIST_BOX_SCENENODE_PARSER_TYPE_STRING,
                                                               NULL);
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               ITEM_SCENENODE_PARSER_TYPE_STRING,
                                                               NULL);
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               USER_SCENENODE_PARSER_TYPE_STRING,
                                                               NULL);
    result += SceneNodeTypesRegistrar_registerNewSceneNodeType(gm->sceneNodeTypesRegistrar, 
                                                               LEVEL_SCENENODE_PARSER_TYPE_STRING,
                                                               NULL);*/
    return result;
}

struct GameManager* GameManager_construct() {
    struct GameManager* gm = NULL;
    gm = (struct GameManager*)calloc(1, sizeof(struct GameManager));
    if (gm) {
        unsigned char result = 0;
        gm->logger.state = LoggerEnabledToStderr;
        if (!(gm->eventManager = EventManager_construct(&(gm->logger))))
            result++;
        if (!(gm->resourceManager = ResourceManager_construct(&(gm->logger))))
            result++;
        gm->settings = Settings_construct(gm->resourceManager, GAME_MANAGER_DEFAULT_SETTINGS_PATH);
        if (!(gm->musican = Musican_construct(&(gm->logger),
                                                gm->settings->isMusicActive, gm->settings->isSoundActive)))
            result++;
        if (!(gm->renderer = Renderer_construct(&(gm->logger), gm->settings)))
            result++;
        if (!(gm->sceneNodeTypesRegistrar = SceneNodeTypesRegistrar_construct())) {
            Logger_log(&(gm->logger), GAME_MANAGER_ERR_REGISTRAR_CONSTRUCTING);
            result++;
        }
        if (GameManager_registerSceneNodeTypes(gm)) {
            Logger_log(&(gm->logger), GAME_MANAGER_ERR_REGISTRAR_REGISTER);
            result++;
        }
        if (!(gm->scenesStack = (struct Scene**)malloc(sizeof(struct Scene*) * INITIAL_NUMBER_ALLOCATED_SCENES))) {
            Logger_log(&(gm->logger), GAME_MANAGER_ERR_SCENES_STACK_ALLOC);
            result++;
        }
        if (result) {
            GameManager_destruct(gm);
            return NULL;
        }
        gm->allocatedScenesCount = INITIAL_NUMBER_ALLOCATED_SCENES;
        gm->scenesCount = 0;
        GameManager_pushScene(gm, gm->settings->mainScene);
    }
    return gm;
}

static void GameManager_handleEvents(struct GameManager* gm) {
    if (!gm)
        return;
    if (EventManager_generateCustomEventsList(gm->eventManager, "GameManager"))
        return;
    size_t i;
    for (i = 0; i < gm->eventManager->customGameEventsCount; i++) {
        if (strcmp(gm->eventManager->customGameEventsList[i]->command, GAME_MANAGER_COMMAND_EXIT) == 0)
            gm->eventManager->quit = true;
        if (strcmp(gm->eventManager->customGameEventsList[i]->command, GAME_MANAGER_COMMAND_PUSH) == 0) {
            GameManager_pushScene(gm, gm->eventManager->customGameEventsList[i]->data);
            EventManager_removeEvent(gm->eventManager, gm->eventManager->customGameEventsList[i]);
        }
        if (strcmp(gm->eventManager->customGameEventsList[i]->command, GAME_MANAGER_COMMAND_POP) == 0) {
            GameManager_popScene(gm);
            EventManager_removeEvent(gm->eventManager, gm->eventManager->customGameEventsList[i]);
        }
    }
}

int GameManager_main(struct GameManager* gm) {
    if (!gm)
        return 1;
    Logger_log(&(gm->logger), "***Am I Alive?***");
    size_t i;
    size_t j;
    unsigned char state = 0;
    unsigned char color = 0;
    while(!gm->eventManager->quit) {
        EventManager_updateSdlEvents(gm->eventManager);
        SDL_SetRenderDrawColor(gm->renderer->renderer, (int)(color * 0.8), color,
                               (int)(color * 0.5), 255);
        SDL_RenderClear(gm->renderer->renderer);
        if (gm->scenesCount > 0) {
            i = gm->scenesCount - 1;
            for(j = 0; j < gm->scenesStack[i]->sceneNodesCount; j++) {
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
        switch (state) {
            case 0:
                color++;
                if (color >= 255)
                    state = 1;
                break;
            case 1:
                color--;
                if (color <= 1)
                    state = 0;
                break;
        }
    }
    return 0;
}

void GameManager_destruct(struct GameManager* gm) {
    size_t i;
    if (!gm)
        return;
    if (gm->scenesStack) {
        for (i = 0; i < gm->scenesCount; i++)
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

unsigned char GameManager_reallocateSceneNodesList(struct GameManager* gm) {
    if (!gm)
        return 1;
    struct Scene** scenesStack = NULL;
    size_t i;
    size_t newSize = gm->allocatedScenesCount + INITIAL_NUMBER_ALLOCATED_SCENES;
    scenesStack = (struct Scene**)malloc(sizeof(struct Scene*) * newSize);
    if (!scenesStack)
        return 2;
    for (i = 0; i < gm->scenesCount; i++)
        scenesStack[i] = gm->scenesStack[i];
    free(gm->scenesStack);
    gm->scenesStack = scenesStack;
    gm->allocatedScenesCount = newSize;
    return 0;
}

unsigned char GameManager_pushScene(struct GameManager* gm, const char* const resId) {
    if (!gm || !resId)
        return 1;
    size_t i;
    for (i = 0; i < gm->scenesCount; i++)
        if (strcmp(resId, gm->scenesStack[i]->sceneResource->id) == 0)
            return 2;
    struct Scene* scene = NULL;
    scene = Scene_construct(gm->resourceManager,
                            gm->renderer,
                            gm->sceneNodeTypesRegistrar,
                            resId);
    if (!scene)
        return 3;
    if (gm->scenesCount >= gm->allocatedScenesCount)
        if(GameManager_reallocateSceneNodesList(gm)) {
            Scene_destruct(scene);
            return 4;
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
