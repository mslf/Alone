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

const char* const EVENT_MANAGER_ERR_ALLOC =
        "EventManager: constructor: allocating memory failed!";
const char* const EVENT_MANAGER_ERR_GAME_EVENTS_LIST_ALLOC =
        "EventManager: constructor: allocating memory for gameEventsList failed!";
const char* const EVENT_MANAGER_ERR_CUSTOM_GAME_EVENTS_LIST_ALLOC =
        "EventManager: constructor: allocating memory for customGameEventsList failed!";
const char* const EVENT_MANAGER_ERR_SDL_EVENTS_LIST_ALLOC =
        "EventManager: constructor: allocating memory for sdlEventsList failed!";

struct EventManager* EventManager_construct(struct Logger* logger) {
    struct EventManager* em = NULL;
    em = (struct EventManager*)malloc(sizeof(struct EventManager));
    if (!em) {
        Logger_log(logger, EVENT_MANAGER_ERR_ALLOC);
        return NULL;
    }
    if (!(em->gameEventsList = (struct GameEvent**)malloc(
            sizeof(struct GameEvent*) * INITIAL_NUMBER_ALLOCATED_EVENTS))) {
        Logger_log(logger, EVENT_MANAGER_ERR_GAME_EVENTS_LIST_ALLOC);
        EventManager_destruct(em);
        return NULL;
    }
    em->allocatedGameEventsCount = INITIAL_NUMBER_ALLOCATED_EVENTS;
    em->gameEventsCount = 0;
    if (!(em->customGameEventsList = (struct GameEvent**)malloc(
            sizeof(struct GameEvent*) * INITIAL_NUMBER_ALLOCATED_EVENTS))) {
        Logger_log(logger, EVENT_MANAGER_ERR_CUSTOM_GAME_EVENTS_LIST_ALLOC);
        EventManager_destruct(em);
        return NULL;
    }
    em->allocatedCustomGameEventsCount = INITIAL_NUMBER_ALLOCATED_EVENTS;
    em->customGameEventsCount = 0;
    if (!(em->sdlEventsList = (SDL_Event*)malloc(
            sizeof(SDL_Event) * INITIAL_NUMBER_ALLOCATED_SDL_EVENTS))) {
        Logger_log(logger, EVENT_MANAGER_ERR_SDL_EVENTS_LIST_ALLOC);
        EventManager_destruct(em);
        return NULL;
    }
    em->allocatedSdlEventsCount = INITIAL_NUMBER_ALLOCATED_SDL_EVENTS;
    em->sdlEventsCount = 0;
    em->quit = 0;
    em->logger = logger;
    return em;
}

unsigned char EventManager_reallocateGameEventsList(struct EventManager* em) {
    if (!em)
        return 1;
    struct GameEvent** tList = NULL;
    size_t i;
    if (!(tList = (struct GameEvent**)malloc(
            sizeof(struct GameEvent*) * (em->allocatedGameEventsCount + INITIAL_NUMBER_ALLOCATED_EVENTS)))) {
        return 2;
    }
    for (i = 0; i < em->gameEventsCount; i++)
        tList[i] = em->gameEventsList[i];
    free(em->gameEventsList);
    em->gameEventsList = tList;
    em->allocatedGameEventsCount += INITIAL_NUMBER_ALLOCATED_EVENTS;
    return 0;
}

unsigned EventManager_reallocateCustomGameEventsList(struct EventManager* em) {
    if (!em)
        return 1;
    struct GameEvent** tList = NULL;
    size_t i;
    if (!(tList = (struct GameEvent**)malloc(
            sizeof(struct GameEvent*) * (em->allocatedGameEventsCount + INITIAL_NUMBER_ALLOCATED_EVENTS)))) {
        return 2;
    }
    for (i = 0; i < em->customGameEventsCount; i++)
        tList[i] = em->customGameEventsList[i];
    free(em->customGameEventsList);
    em->customGameEventsList = tList;
    em->allocatedCustomGameEventsCount += INITIAL_NUMBER_ALLOCATED_EVENTS;
    return 0;
}

unsigned char EventManager_reallocateSdlEventsList(struct EventManager* em) {
    if (!em)
        return 1;
    SDL_Event *tList = NULL;
    size_t i;
    if (!(tList = (SDL_Event *) malloc(
            sizeof(SDL_Event) * (em->allocatedSdlEventsCount + INITIAL_NUMBER_ALLOCATED_SDL_EVENTS)))) {
        return 2;
    }
    for (i = 0; i < em->sdlEventsCount; i++)
        tList[i] = em->sdlEventsList[i];
    free(em->sdlEventsList);
    em->sdlEventsList = tList;
    em->allocatedSdlEventsCount += INITIAL_NUMBER_ALLOCATED_SDL_EVENTS;
    return 0;
}

void EventManager_destruct(struct EventManager* em) {
    if (!em)
        return;
    if (em->gameEventsList) {
        size_t i;
        for (i = 0; i < em->gameEventsCount; i++)
            GameEvent_destruct(em->gameEventsList[i]);
        free(em->gameEventsList);
    }
    if (em->customGameEventsList)
        free(em->customGameEventsList);
    if (em->sdlEventsList)
        free(em->sdlEventsList);
    free(em);
}

unsigned char EventManager_addEvent(struct EventManager* em, struct GameEvent* gameEvent) {
    if (!em)
        return 1;
    if (!gameEvent)
        return 2;
    // Firstly, try to find the same GameEvent in list
    size_t i;
    bool found = false;
    for (i = 0; i < em->gameEventsCount; i++)
        if (em->gameEventsList[i] == gameEvent)
            found = true;
    if (!found) {
        // Try to reallocate (if needed) and add gameEvent
        if (em->gameEventsCount >= em->allocatedGameEventsCount)
            if (EventManager_reallocateGameEventsList(em))
                return 3;
        em->gameEventsList[em->gameEventsCount] = gameEvent;
        em->gameEventsCount++;
    }
    return 0;
}

unsigned char EventManager_removeEvent(struct EventManager* em, struct GameEvent* gameEvent) {
    if (!em || !gameEvent)
        return 1;
    size_t i;
    bool found = false;
    size_t foundIndex = 0;
    for (i = 0; i < em->gameEventsCount; i++)
        if (em->gameEventsList[i] == gameEvent) {
            em->gameEventsCount--;
            found = true;
            foundIndex = i;
            break;
        }
    if (found)
        for (i = foundIndex; i < em->gameEventsCount; i++)
            em->gameEventsList[i] = em->gameEventsList[i + 1];
    return 0;
}

unsigned char EventManager_generateCustomEventsList(struct EventManager* em, const char* const channel) {
    if (!em)
        return 1;
    if (!channel)
        return 2;
    size_t i;
    em->customGameEventsCount = 0;
    for (i = 0; i < em->gameEventsCount; i++)
        if (strcmp(em->gameEventsList[i]->eventChannel, channel) == 0) {
            // Try to reallocate (if needed) and add customGameEvent
            if (em->customGameEventsCount >= em->allocatedCustomGameEventsCount)
                if (EventManager_reallocateCustomGameEventsList(em))
                    return 3;
            em->customGameEventsList[em->customGameEventsCount] = em->gameEventsList[i];
            em->customGameEventsCount++;
        }
    return 0;
}

unsigned char EventManager_updateSdlEvents(struct EventManager* em) {
    if (!em)
        return 1;
    em->sdlEventsCount = 0;
    while(SDL_PollEvent(&(em->sdlEventsList[em->sdlEventsCount]))) {
        if(em->sdlEventsList[em->sdlEventsCount].type == SDL_QUIT)
            em->quit = 1;
        em->sdlEventsCount++;
        if (em->sdlEventsCount >= em->allocatedSdlEventsCount)
            if (EventManager_reallocateSdlEventsList(em))
                return 2;
    }
    return 0;
}

void EventManager_destructNeedlesEvents(struct EventManager* em) {
    size_t i = 0;
    while (i < em->gameEventsCount) {
        if (!em->gameEventsList[i]->isNeeded) {
            EventManager_removeEvent(em, em->gameEventsList[i]);
            GameEvent_destruct(em->gameEventsList[i]);
        }
        else
            i++;
    }
}
