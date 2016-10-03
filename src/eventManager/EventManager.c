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
        if (!(em->sdlEventsList = (SDL_Event*)malloc(
                sizeof(SDL_Event) * INITIAL_NUMBER_ALLOCATED_SDL_EVENTS))) {
            fprintf(stderr, "SDLEventsList allocating failed!\n");
            free(em->gameEventsList);
            free(em);
            return NULL;
        }
        em->allocatedSdlEventsCount = INITIAL_NUMBER_ALLOCATED_SDL_EVENTS;
        em->sdlEventsCount = 0;
    }
    return em;
}

void EventManager_destruct(struct EventManager* em) {
    free(em->gameEventsList);
    free(em->sdlEventsList);
    free(em);
}

void EventManager_addEvent(struct EventManager* em, struct GameEvent* gameEvent) {

}

void EventManager_removeEvent(struct EventManager* em, size_t index) {

}

const struct GameEvent* const * const EventManager_getCustomEvents(struct EventManager* em,
                                                                   const char* const channel) {

}

void EventManager_updateSdlEvents(struct EventManager* em) {

}