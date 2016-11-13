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
 * @file EventManager.c
 * @author mslf
 * @date 10 Aug 2016
 * @brief File containing implementation of #EventManager.
 */
#include "eventManager/EventManager.h"
#include "string.h"

/**
 * @brief Error message strings for #EventManager.
 */
static const struct EventManager_errorMessages{
    const char* const errAlloc;
    /**< Will be displayed when EventManager_construct() fails to allocate memory for the #EventManager. */
    const char* const errGameEventsListAlloc;
    /**< Will be displayed when EventManager_construct() fails to allocate memory for the EventManager#gameEventsList. */
    const char* const errCustomGameEventsListAlloc;
    /**< Will be displayed when EventManager_construct() fails to allocate memory for the EventManager#customGameEventsList. */
    const char* const errSdlEventsListAlloc;
    /**< Will be displayed when EventManager_construct() fails to allocate memory for the EventManager#sdlEventsList. */
}EventManager_errorMessages = {
    "EventManager_construct: allocating memory failed!",
    "EventManager_construct: allocating memory for gameEventsList failed!",
    "EventManager_construct: allocating memory for customGameEventsList failed!",
    "EventManager_construct: allocating memory for sdlEventsList failed!"};

struct EventManager* EventManager_construct(struct Logger* logger) {
    struct EventManager* em = NULL;
    em = (struct EventManager*)calloc(1, sizeof(struct EventManager));
    if (!em) {
        Logger_log(logger, EventManager_errorMessages.errAlloc);
        return NULL;
    }
    if (!(em->gameEventsList = (struct GameEvent**)malloc(
            sizeof(struct GameEvent*) * EM_INITIAL_NUMBER_ALLOCATED_EVENTS))) {
        Logger_log(logger, EventManager_errorMessages.errGameEventsListAlloc);
        EventManager_destruct(em);
        return NULL;
    }
    em->allocatedGameEventsCount = EM_INITIAL_NUMBER_ALLOCATED_EVENTS;
    if (!(em->customGameEventsList = (struct GameEvent**)malloc(
            sizeof(struct GameEvent*) * EM_INITIAL_NUMBER_ALLOCATED_EVENTS))) {
        Logger_log(logger, EventManager_errorMessages.errCustomGameEventsListAlloc);
        EventManager_destruct(em);
        return NULL;
    }
    em->allocatedCustomGameEventsCount = EM_INITIAL_NUMBER_ALLOCATED_EVENTS;
    if (!(em->sdlEventsList = (SDL_Event*)malloc(
            sizeof(SDL_Event) * EM_INITIAL_NUMBER_ALLOCATED_SDL_EVENTS))) {
        Logger_log(logger, EventManager_errorMessages.errSdlEventsListAlloc);
        EventManager_destruct(em);
        return NULL;
    }
    em->allocatedSdlEventsCount = EM_INITIAL_NUMBER_ALLOCATED_SDL_EVENTS;
    em->logger = logger;
    return em;
}

/**
 * @brief Reallocates memory for the EventManager#gameEventsList.
 * EventManager#allocatedGameEventsCount will be increased by #EM_INITIAL_NUMBER_ALLOCATED_EVENTS.
 * @param em Pointer to a #EventManager. Can be NULL.
 * @return #EventManager_errors value.
 * @see #EventManager_errors
 */
static enum EventManager_errors EventManager_reallocateGameEventsList(struct EventManager* em) {
    if (!em)
        return EM_ERR_NULL_ARGUMENT;
    struct GameEvent** tList = NULL;
    size_t newSize = em->allocatedGameEventsCount + EM_INITIAL_NUMBER_ALLOCATED_EVENTS;
    tList = (struct GameEvent**)realloc(em->gameEventsList, sizeof(struct GameEvent*) * newSize);
    if (!tList)
        return EM_ERR_REALLOC_GAME_EVENTS_LIST;
    em->gameEventsList = tList;
    em->allocatedGameEventsCount = newSize;
    return EM_NO_ERRORS;
}

/**
 * @brief Reallocates memory for the EventManager#customGameEventsList.
 * EventManager#allocatedCustomGameEventsCount will be increased by #EM_INITIAL_NUMBER_ALLOCATED_EVENTS.
 * @param em Pointer to a #EventManager. Can be NULL.
 * @return #EventManager_errors value.
 * @see #EventManager_errors
 */
static enum EventManager_errors EventManager_reallocateCustomGameEventsList(struct EventManager* em) {
    if (!em)
        return EM_ERR_NULL_ARGUMENT;
    struct GameEvent** tList = NULL;
    size_t newSize = em->allocatedGameEventsCount + EM_INITIAL_NUMBER_ALLOCATED_EVENTS;
    tList = (struct GameEvent**)realloc(em->customGameEventsList, sizeof(struct GameEvent*) * newSize);
    if (!tList)
        return EM_ERR_REALLOC_CUSTOM_GAME_EVENTS_LIST;
    em->customGameEventsList = tList;
    em->allocatedCustomGameEventsCount = newSize;
    return EM_NO_ERRORS;
}

/**
 * @brief Reallocates memory for the EventManager#sdlEventsList.
 * EventManager#allocatedSdlEventsCount will be increased by #EM_INITIAL_NUMBER_ALLOCATED_SDL_EVENTS.
 * @param em Pointer to a #EventManager. Can be NULL.
 * @return #EventManager_errors value.
 * @see #EventManager_errors
 */
static enum EventManager_errors EventManager_reallocateSdlEventsList(struct EventManager* em) {
    if (!em)
        return EM_ERR_NULL_ARGUMENT;
    SDL_Event* tList = NULL;
    size_t newSize = em->allocatedSdlEventsCount + EM_INITIAL_NUMBER_ALLOCATED_SDL_EVENTS;
    tList = (SDL_Event*)realloc(em->sdlEventsList, sizeof(SDL_Event) * newSize);
    if (!tList)
        return EM_ERR_REALLOC_SDL_EVENTS_LIST;
    em->sdlEventsList = tList;
    em->allocatedSdlEventsCount = newSize;
    return EM_NO_ERRORS;
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

enum EventManager_errors EventManager_addEvent(struct EventManager* em, struct GameEvent* gameEvent) {
    if (!em || !gameEvent)
        return EM_ERR_NULL_ARGUMENT;
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
                return EM_ERR_REALLOC_GAME_EVENTS_LIST;
        em->gameEventsList[em->gameEventsCount] = gameEvent;
        em->gameEventsCount++;
    }
    return EM_NO_ERRORS;
}

enum EventManager_errors EventManager_removeEvent(struct EventManager* em, struct GameEvent* gameEvent) {
    if (!em || !gameEvent)
        return EM_ERR_NULL_ARGUMENT;
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
    return EM_NO_ERRORS;
}

enum EventManager_errors EventManager_generateCustomEventsList(struct EventManager* em, const char* const channel) {
    if (!em || !channel)
        return EM_ERR_NULL_ARGUMENT;
    size_t i;
    em->customGameEventsCount = 0;
    for (i = 0; i < em->gameEventsCount; i++)
        if (strcmp(em->gameEventsList[i]->eventChannel, channel) == 0) {
            // Try to reallocate (if needed) and add customGameEvent
            if (em->customGameEventsCount >= em->allocatedCustomGameEventsCount)
                if (EventManager_reallocateCustomGameEventsList(em))
                    return EM_ERR_REALLOC_CUSTOM_GAME_EVENTS_LIST;
            em->customGameEventsList[em->customGameEventsCount] = em->gameEventsList[i];
            em->customGameEventsCount++;
        }
    return EM_NO_ERRORS;
}

enum EventManager_errors EventManager_updateSdlEvents(struct EventManager* em) {
    if (!em)
        return EM_ERR_NULL_ARGUMENT;
    em->sdlEventsCount = 0;
    while(SDL_PollEvent(&(em->sdlEventsList[em->sdlEventsCount]))) {
        if(em->sdlEventsList[em->sdlEventsCount].type == SDL_QUIT)
            em->quit = true;
        em->sdlEventsCount++;
        if (em->sdlEventsCount >= em->allocatedSdlEventsCount)
            if (EventManager_reallocateSdlEventsList(em))
                return EM_ERR_REALLOC_SDL_EVENTS_LIST;
    }
    return EM_NO_ERRORS;
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
