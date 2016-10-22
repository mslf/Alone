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
#include "GameManager.h"
#include <stdlib.h>
#include "stdio.h"

const char* const GAME_MANAGER_ERR_SCENES_STACK_ALLOC =
        "GameManager: constructor: allocating memory for scenesStack failed!";
const char* const GAME_MANAGER_DEFAULT_SETTINGS_PATH = "Alone.settings";

struct GameManager* GameManager_construct() {
    struct GameManager* gm = NULL;
    gm = (struct GameManager*)malloc(sizeof(struct GameManager));
    if (gm) {
        unsigned char result = 0;
        gm->settings = NULL;
        gm->resourceManager = NULL;
        gm->renderer = NULL;
        gm->musican = NULL;
        gm->eventManager = NULL;
        gm->logger.state = LoggerEnabledToStderr;
        if (!(gm->eventManager = EventManager_construct(&(gm->logger))))
            result++;
        if (!(gm->resourceManager = ResourceManager_construct(&(gm->logger))))
            result++;
        gm->settings = Settings_construct(gm->resourceManager, GAME_MANAGER_DEFAULT_SETTINGS_PATH);
        if (gm->settings->isSoundActive)
            if (!(gm->musican = Musican_construct(&(gm->logger))))
                result++;
        if (!(gm->renderer = Renderer_construct(&(gm->logger), gm->settings)))
            result++;
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

int GameManager_main(struct GameManager* gm) {
    Logger_log(&(gm->logger), "***Are You Alive?***");
    size_t i;
    size_t j;
    size_t state = 0;
    unsigned char r = 255;
    unsigned char g = 0;
    unsigned char b = 255;
    while(!gm->eventManager->quit) {
        EventManager_updateSdlEvents(gm->eventManager);
        SDL_SetRenderDrawColor(gm->renderer->renderer, r, g, b, 255);
        SDL_RenderClear(gm->renderer->renderer);
        i = gm->scenesCount - 1;
        for(j = 0; j < gm->scenesStack[i]->sceneNodesCount; j++) {
            if (gm->scenesStack[i]->sceneNodesList[j]->control)
                gm->scenesStack[i]->sceneNodesList[j]->control(gm->scenesStack[i]->sceneNodesList[j], 
                                                               gm->eventManager);
            if (gm->scenesStack[i]->sceneNodesList[j]->update)
                gm->scenesStack[i]->sceneNodesList[j]->update(gm->scenesStack[i]->sceneNodesList[j], 
                                                              gm->eventManager, 
                                                              gm->renderer);
            if (gm->scenesStack[i]->sceneNodesList[j]->render)
                gm->scenesStack[i]->sceneNodesList[j]->render(gm->scenesStack[i]->sceneNodesList[j], 
                                                              gm->renderer);
            if (gm->scenesStack[i]->sceneNodesList[j]->sound)
                gm->scenesStack[i]->sceneNodesList[j]->sound(gm->scenesStack[i]->sceneNodesList[j], 
                                                             gm->musican);
        }
        SDL_RenderPresent(gm->renderer->renderer);
        if (state == 0)
            r--;
        if (state == 1) {
            g++;
            b--;
        }
        if (state == 2) {
            r++;
            g--;
        }
        if (state == 3)
            b++;
        if (state == 0 && r <= 1)
            state = 1;
        if (state == 1 && (b <= 1 || g >= 255))
            state = 2;
        if (state == 2 && (g <= 1 || r >= 255))
            state = 3;
        if (state == 3 && b >= 255)
            state = 0;
    }
    return 0;
}

void GameManager_destruct(struct GameManager* gm) {
    size_t i;
    if (gm->scenesStack) {
        for (i = 0; i < gm->scenesCount; i++)
            Scene_destruct(gm->scenesStack[i]);
        free(gm->scenesStack);
    }
    if (gm->eventManager)
        EventManager_destruct(gm->eventManager);
    if (gm->resourceManager)
        ResourceManager_destruct(gm->resourceManager);
    if (gm->musican)
        Musican_destruct(gm->musican);
    if (gm->renderer)
        Renderer_destruct(gm->renderer);
    Settings_destruct(gm->settings);
    free(gm);
}
unsigned char GameManager_reallocateSceneNodesList(struct GameManager* gm) {
    if (!gm)
        return 1;
    struct Scene** scenesStack = NULL;
    size_t i;
    size_t newSize = gm->allocatedScenesCount + INITIAL_NUMBER_ALLOCATED_SCENES;
    if (!(scenesStack = (struct Scene**)malloc(sizeof(struct Scene*) * newSize))) {
        free(scenesStack);
        return 2;
    }
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
    struct Scene* scene = NULL;
    scene = Scene_construct(gm->resourceManager, gm->renderer, resId);
    if (!scene)
        return 2;
    if (gm->scenesCount >= gm->allocatedScenesCount)
        if(GameManager_reallocateSceneNodesList(gm)) {
            Scene_destruct(scene);
            return 3;
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
