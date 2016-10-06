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
#include "eventManager/GameEvent.h"

#define INITIAL_NUMBER_ALLOCATED_EVENTS 100
#define INITIAL_NUMBER_ALLOCATED_SDL_EVENTS 20

struct EventManager {
    struct GameEvent** gameEventsList;
    struct GameEvent** customGameEventsList;
    SDL_Event* sdlEventsList;
    size_t gameEventsCount;
    size_t allocatedGameEventsCount;
    size_t customGameEventsCount;
    size_t allocatedCustomGameEventsCount;
    size_t sdlEventsCount;
    size_t allocatedSdlEventsCount;
};

struct EventManager* EventManager_construct();
void EventManager_destruct(struct EventManager* em);

void EventManager_addEvent(struct EventManager* em, struct GameEvent* gameEvent);
void EventManager_removeEvent(struct EventManager* em, size_t index);
unsigned char EventManager_generateCustomEventsList(struct EventManager* em, const char* const channel);
void EventManager_updateSdlEvents(struct EventManager* em);

#endif //ALONE_EVENTMANAGER_H
