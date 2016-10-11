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

#include <stddef.h>
#include <SDL2/SDL.h>
#include "logger/Logger.h"
#include "eventManager/GameEvent.h"

enum {
    INITIAL_NUMBER_ALLOCATED_EVENTS = 100,
    INITIAL_NUMBER_ALLOCATED_SDL_EVENTS = 20
};

struct EventManager {
    struct Logger* logger;
    struct GameEvent** gameEventsList;
    struct GameEvent** customGameEventsList;
    SDL_Event* sdlEventsList;
    size_t gameEventsCount;
    size_t allocatedGameEventsCount;
    size_t customGameEventsCount;
    size_t allocatedCustomGameEventsCount;
    size_t sdlEventsCount;
    size_t allocatedSdlEventsCount;
    unsigned char quit;
};

struct EventManager* EventManager_construct(struct Logger* logger);
void EventManager_destruct(struct EventManager* em);

unsigned char EventManager_addEvent(struct EventManager* em, struct GameEvent* gameEvent);
unsigned char EventManager_removeEvent(struct EventManager* em, size_t index);
unsigned char EventManager_generateCustomEventsList(struct EventManager* em, const char* const channel);
unsigned char EventManager_updateSdlEvents(struct EventManager* em);

#endif //ALONE_EVENTMANAGER_H
