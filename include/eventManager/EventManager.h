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
 * @file EventManager.h
 * @author mslf
 * @date 10 Aug 2016
 * @brief File containing #EventManager and its stuff.
 */
#ifndef ALONE_EVENTMANAGER_H
#define ALONE_EVENTMANAGER_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "logger/Logger.h"
#include "eventManager/GameEvent.h"

/**
 * @brief Some initial constants for #EventManager.
 */
enum EventManager_constants{
    EM_INITIAL_NUMBER_ALLOCATED_EVENTS = 100,
    /**< Init alocating number and reallocating step for EventManager#gameEventsList and EventManager#customGameEventsList. */ 
    EM_INITIAL_NUMBER_ALLOCATED_SDL_EVENTS = 20
    /**< Init alocating number and reallocating step for EventManager#sdlEventsList. */
};

/**
 * @brief Error codes for EventManager.
 */
enum EventManager_errors {
    EM_NO_ERRORS = 0,
    /**< All right, no errors. */
    EM_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's argument is NULL. */
    EM_ERR_REALLOC_SDL_EVENTS_LIST = 2,
    /**< Reallocating memory for EventManager#sdlEventsList failed. */
    EM_ERR_REALLOC_GAME_EVENTS_LIST = 3,
    /**< Reallocating memory for EventManager#gameEventsList failed. */
    EM_ERR_REALLOC_CUSTOM_GAME_EVENTS_LIST = 4
    /**< Reallocating memory for EventManager#customGameEventsList failed. */
};

/**
 * @brief Event subsystem.
 * #EventManager provide access to the list with SDL_Events via EventManager#sdlEventsList.
 * Also, #EventManager provide access to the list with GameEvents and custom GameEvents
 * via (EventManager#gameEventsList and EventManager#customGameEventsList).
 * #EventManager contain only unique GameEvents in the EventManager#gameEventsList.
 * EventManager#customGameEventsList contain sorted by channel GameEvents via EventManager_generateCustomEventsList.
 */
struct EventManager {
    struct Logger* logger;
    /**< Provides logging ability to someone, who have pointer to a #EventManager. */ 
    struct GameEvent** gameEventsList;
    /**< List of unique GameEvents. */
    struct GameEvent** customGameEventsList;
    /**< List of sorted by channel GameEvents. */
    SDL_Event* sdlEventsList;
    /**< SDL_Events list. */
    size_t gameEventsCount;
    /**< Current number of existing GameEvents in the EventManager#gameEventsList. */
    size_t customGameEventsCount;
    /**< Current number of existing GameEvents in the EventManager#customGameEventsList. */
    size_t sdlEventsCount;
    /**< Current number of existing SDL_Events in the EventManager#sdlEventsList. */
    size_t allocatedGameEventsCount;
    /**< Allocated number of GameEvents in the EventManager#gameEventsList. */
    size_t allocatedCustomGameEventsCount;
    /**< Allocated number of GameEvents in the EventManager#customGameEventsList. */
    size_t allocatedSdlEventsCount;
    /**< Allocated number of SDL_Events in the EventManager#sdlEventsList. */
    bool quit;
    /**< Quiting flag. Will be true, if user will try to exit the application. */
};

/**
 * @brief Constructs #EventManager and inits it.
 * @param logger #Logger struct to set the EventManager#logger. Can be NULL.
 * @return Pointer to an initialized #EventManager, or NULL if constructing  failed.
 * @see #EventManager_constants
 */
struct EventManager* EventManager_construct(struct Logger* logger);

/**
 * @brief Destructs #EventManager and frees memory, used by it.
 * @param em Pointer to a #EventManager. Can be not fully initialized. Can be NULL.
 * @note Use this funcrion in pair with EventManager_construct().
 * @see EventManager_construct()
 */
void EventManager_destruct(struct EventManager* em);

/**
 * @brief Adds #GameEvent to a EventManager#gameEventsList if it is unique.
 * Also, increases EventManager#gameEventsCount and tryies to reallocate memory if needed.
 * @param em Pointer to a #EventManager. Can be NULL.
 * @param gameEvent Pointer to a #GameEvent to add. Can be NULL.
 * @return #EventManager_errors value.
 * @see #GameEvent
 * @see #EventManager_errors
 */
enum EventManager_errors EventManager_addEvent(struct EventManager* em, struct GameEvent* gameEvent);

/**
 * @brief Removes #GameEvent from a EventManager#gameEventsList.
 * Also, decreases EventManager#gameEventsCount and shifts EventManager#gameEventsList to the left.
 * @param em Pointer to a #EventManager. Can be NULL.
 * @param gameEvent Pointer to a #GameEvent in the EventManager#gameEventsList. Can be NULL.
 * @return #EventManager_errors value.
 * @see #GameEvent
 * @see #EventManager_errors
 * @warning Function doesn't removes #GameEvent from EventManager#customGameEventsList!
 */
enum EventManager_errors EventManager_removeEvent(struct EventManager* em, struct GameEvent* gameEvent);

/**
 * @brief Removes and destructs GameEvents in the EventManager#gameEventsList if GameEvent#isNeeded == false.
 * @param em Pointer to a #EventManager. Can be NULL.
 * @see #GameEvent
 * @warning Function doesn't removes #GameEvent from EventManager#customGameEventsList!
 */
void EventManager_destructNeedlesEvents(struct EventManager* em);

/**
 * @brief Adds #GameEvent to a EventManager#customGameEventsList if GameEvent#channel == channel.
 * Also adds #GameEvent if it is broadcasting.
 * Adds GameEvents from EventManager#gameEventsList. Also, increases EventManager#customGameEventsCount
 * and tryies to reallocate memory if needed.
 * @param em Pointer to a #EventManager. Can be NULL.
 * @param channel Pointer to a const channel string. Can be NULL.
 * GameEvents with the same channel string and with NULL channel (broadcasting) 
 * will be added to EventManager#customGameEventsList.
 * @return #EventManager_errors value.
 * @see #GameEvent
 * @see GameEvent_construct()
 * @see #EventManager_errors
 * @note Last changes in EventManager#customGameEventsList will be lost!
 * @note Function firstly reset EventManager#customGameEventsCount to a 0.
 */
enum EventManager_errors EventManager_generateCustomEventsList(struct EventManager* em, const char* const channel);

/**
 * @brief Updates EventManager#sdlEventsList.
 * Also, increases EventManager#sdlEventsCount
 * and tryies to reallocate memory if needed.
 * @param em Pointer to a #EventManager. Can be NULL.
 * @return #EventManager_errors value.
 * @see #EventManager_errors
 * @note Last changes in EventManager#sdlEventsList will be lost!
 * @note Function firstly reset EventManager#sdlEventsCount to a 0.
 */
enum EventManager_errors EventManager_updateSdlEvents(struct EventManager* em);

#endif //ALONE_EVENTMANAGER_H
