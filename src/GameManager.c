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
        if (!(gm->eventManager = EventManager_construct())) {
            fprintf(stderr, "EventManager constructing failed!\n");
            result++;
        }
        if (!(gm->resourceManager = ResourceManager_construct())) {
            fprintf(stderr, "ResourceManager constructing failed!\n");
            result++;
        }
        gm->settings = Settings_construct(gm->resourceManager, "Alone.settings");
        if (gm->settings->isSoundActive)
            if (!(gm->musican = Musican_construct())) {
                fprintf(stderr, "Musican constructing failed!\n");
                result++;
            }
        if (!(gm->renderer = Renderer_construct(gm->settings))) {
            fprintf(stderr, "Renderer constructing failed!\n");
            result++;
        }
        if (!(gm->scenesStack = (struct Scene**)malloc(sizeof(struct Scene*) * INITIAL_NUMBER_ALLOCATED_SCENES))) {
            fprintf(stderr, "Scenes stack allocating failed!\n");
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
    printf ("***Am I Alone?***\n");
    while(!gm->eventManager->quit) {
        EventManager_updateSdlEvents(gm->eventManager);
        SDL_SetRenderDrawColor(gm->renderer->renderer,
                               (unsigned char)(rand() % 255),
                               (unsigned char)(rand() % 255),
                               (unsigned char)(rand() % 255),
                               (unsigned char)(rand() % 255));
        SDL_RenderClear(gm->renderer->renderer);
        SDL_RenderPresent(gm->renderer->renderer);
    }
    return 0;
}

void GameManager_destruct(struct GameManager* gm) {
    size_t i;
    if (gm->eventManager)
        EventManager_destruct(gm->eventManager);
    if (gm->resourceManager)
        ResourceManager_destruct(gm->resourceManager);
    if (gm->musican)
        Musican_destruct(gm->musican);
    if (gm->renderer)
        Renderer_destruct(gm->renderer);
    if (gm->scenesStack) {
        for (i = 0; i < gm->scenesCount; i++)
            Scene_destruct(gm->scenesStack[i]);
        free(gm->scenesStack);
    }
    Settings_destruct(gm->settings);
    free(gm);
}

void GameManager_pushScene(struct GameManager* gm, const char* const resId) {

}

void GameManager_popScene(struct GameManager* gm) {

}