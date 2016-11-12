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
#ifndef ALONE_EVENTMANAGER_H
#define ALONE_EVENTMANAGER_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "logger/Logger.h"
#include "eventManager/GameEvent.h"

enum EventManager_constants{
    EM_INITIAL_NUMBER_ALLOCATED_EVENTS = 100,
    EM_INITIAL_NUMBER_ALLOCATED_SDL_EVENTS = 20
};

enum EventManager_errors {
    EM_NO_ERRORS = 0,
    EM_ERR_NULL_ARGUMENT = 1,
    EM_ERR_REALLOC_SDL_EVENTS_LIST = 2,
    EM_ERR_REALLOC_GAME_EVENTS_LIST = 3,
    EM_ERR_REALLOC_CUSTOM_GAME_EVENTS_LIST = 4
};

struct EventManager {
    struct Logger* logger;
    struct GameEvent** gameEventsList;
    struct GameEvent** customGameEventsList;
    SDL_Event* sdlEventsList;
    size_t gameEventsCount;
    size_t customGameEventsCount;
    size_t sdlEventsCount;
    size_t allocatedGameEventsCount;
    size_t allocatedCustomGameEventsCount;
    size_t allocatedSdlEventsCount;
    bool quit;
};

struct EventManager* EventManager_construct(struct Logger* logger);
void EventManager_destruct(struct EventManager* em);

enum EventManager_errors EventManager_addEvent(struct EventManager* em, struct GameEvent* gameEvent);
enum EventManager_errors EventManager_removeEvent(struct EventManager* em, struct GameEvent* gameEvent);
void EventManager_destructNeedlesEvents(struct EventManager* em);
enum EventManager_errors EventManager_generateCustomEventsList(struct EventManager* em, const char* const channel);
enum EventManager_errors EventManager_updateSdlEvents(struct EventManager* em);

#endif //ALONE_EVENTMANAGER_H
