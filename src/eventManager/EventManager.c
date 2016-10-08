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
#include "eventManager/EventManager.h"
#include "string.h"

struct EventManager* EventManager_construct() {
    struct EventManager* em = NULL;
    em = (struct EventManager*)malloc(sizeof(struct EventManager));
    if (em) {
        if (!(em->gameEventsList = (struct GameEvent**)malloc(
                sizeof(struct GameEvent*) * INITIAL_NUMBER_ALLOCATED_EVENTS))) {
            fprintf(stderr, "GameEventsList allocating failed!\n");
            free(em);
            return NULL;
        }
        em->allocatedGameEventsCount = INITIAL_NUMBER_ALLOCATED_EVENTS;
        em->gameEventsCount = 0;
        if (!(em->customGameEventsList = (struct GameEvent**)malloc(
                sizeof(struct GameEvent*) * INITIAL_NUMBER_ALLOCATED_EVENTS))) {
            fprintf(stderr, "CustomGameEventsList allocating failed!\n");
            free(em->gameEventsList);
            free(em);
            return NULL;
        }
        em->allocatedCustomGameEventsCount = INITIAL_NUMBER_ALLOCATED_EVENTS;
        em->customGameEventsCount = 0;
        if (!(em->sdlEventsList = (SDL_Event*)malloc(
                sizeof(SDL_Event) * INITIAL_NUMBER_ALLOCATED_SDL_EVENTS))) {
            fprintf(stderr, "SDLEventsList allocating failed!\n");
            free(em->gameEventsList);
            free(em->customGameEventsList);
            free(em);
            return NULL;
        }
        em->allocatedSdlEventsCount = INITIAL_NUMBER_ALLOCATED_SDL_EVENTS;
        em->sdlEventsCount = 0;
        em->quit = 0;
    }
    return em;
}

void EventManager_reallocateGameEventsList(struct EventManager* em) {
    if (em) {
        struct GameEvent** tList = NULL;
        size_t i;
        if (!(tList = (struct GameEvent**)malloc(
                sizeof(struct GameEvent*) * (em->allocatedGameEventsCount + INITIAL_NUMBER_ALLOCATED_EVENTS)))) {
            fprintf(stderr, "GameEventsList reallocating failed!\n");
        } else {
            for (i = 0; i < em->gameEventsCount; i++)
                tList[i] = em->gameEventsList[i];
            free(em->gameEventsList);
            em->gameEventsList = tList;
            em->allocatedGameEventsCount += INITIAL_NUMBER_ALLOCATED_EVENTS;
        }
    } else
        fprintf(stderr, "NULL pointer to EventManager!\n");
}

void EventManager_reallocateCustomGameEventsList(struct EventManager* em) {
    if (em) {
        struct GameEvent **tList = NULL;
        size_t i;
        if (!(tList = (struct GameEvent **) malloc(
                sizeof(struct GameEvent *) * (em->allocatedCustomGameEventsCount + INITIAL_NUMBER_ALLOCATED_EVENTS)))) {
            fprintf(stderr, "CustomGameEventsList reallocating failed!\n");
        } else {
            for (i = 0; i < em->customGameEventsCount; i++)
                tList[i] = em->customGameEventsList[i];
            free(em->customGameEventsList);
            em->customGameEventsList = tList;
            em->allocatedCustomGameEventsCount += INITIAL_NUMBER_ALLOCATED_EVENTS;
        }
    } else
        fprintf(stderr, "NULL pointer to EventManager!\n");
}

void EventManager_reallocateSdlEventsList(struct EventManager* em) {
    if (em) {
        SDL_Event *tList = NULL;
        size_t i;
        if (!(tList = (SDL_Event *) malloc(
                sizeof(SDL_Event) * (em->allocatedSdlEventsCount + INITIAL_NUMBER_ALLOCATED_SDL_EVENTS)))) {
            fprintf(stderr, "SDLEventsList reallocating failed!\n");
        } else {
            for (i = 0; i < em->sdlEventsCount; i++)
                tList[i] = em->sdlEventsList[i];
            free(em->sdlEventsList);
            em->sdlEventsList = tList;
        }
    } else
        fprintf(stderr, "NULL pointer to EventManager!\n");
}

void EventManager_destruct(struct EventManager* em) {
    free(em->gameEventsList);
    free(em->sdlEventsList);
    free(em);
}

void EventManager_addEvent(struct EventManager* em, struct GameEvent* gameEvent) {
    if (em) {
        if (gameEvent) {
            if (em->gameEventsCount >= em->allocatedGameEventsCount)
                EventManager_reallocateGameEventsList(em);
            // Try to reallocate (if needed) and add gameEvent
            if (em->gameEventsCount < em->allocatedGameEventsCount) {
                em->gameEventsList[em->gameEventsCount] = gameEvent;
                em->gameEventsCount++;
            }
        } else
            fprintf(stderr, "NULL pointer to GameEvent!\n");
    } else
        fprintf(stderr, "NULL pointer to EventManager!\n");
}

void EventManager_removeEvent(struct EventManager* em, size_t index) {
    if (em) {
        if (index < em->gameEventsCount) {
            size_t i;
            em->gameEventsCount--;
            for (i = index; i < em->gameEventsCount; i++)
                em->gameEventsList[i] = em->gameEventsList[i + 1];
        } else
            fprintf(stderr, "Removable GameEvent's index out of range!\n");
    } else
        fprintf(stderr, "NULL pointer to EventManager!\n");
}

unsigned char EventManager_generateCustomEventsList(struct EventManager* em, const char* const channel) {
    if (em) {
        fprintf(stderr, "NULL pointer to EventManager!\n");
        return 1;
    }
    if (channel) {
        fprintf(stderr, "NULL pointer to channel string while getting CustomEvents!\n");
        return 2;
    }

    size_t i;
    for (i = 0; i < em->gameEventsCount; i++)
        if (strcmp(em->gameEventsList[i]->eventChannel, channel) == 0) {
            if (em->customGameEventsCount >= em->allocatedCustomGameEventsCount)
                EventManager_reallocateCustomGameEventsList(em);
            // Try to reallocate (if needed) and add customGameEvent
            if (em->customGameEventsCount >= em->allocatedCustomGameEventsCount)
                return 3;
            em->customGameEventsList[em->customGameEventsCount] = em->gameEventsList[i];
            em->customGameEventsCount++;
        }
    return 0;
}

void EventManager_updateSdlEvents(struct EventManager* em) {
    if (em) {
        em->sdlEventsCount = 0;
        while(SDL_PollEvent(&(em->sdlEventsList[em->sdlEventsCount]))) {
            if(em->sdlEventsList[em->sdlEventsCount].type == SDL_QUIT)
                em->quit = 1;
            em->sdlEventsCount++;
            if (em->sdlEventsCount >= em->allocatedSdlEventsCount)
                EventManager_reallocateSdlEventsList(em);
        }
    } else
        fprintf(stderr, "NULL pointer to EventManager!\n");
}