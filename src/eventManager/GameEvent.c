//
// Created by mslf on 8/12/16.
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
#include "eventManager/GameEvent.h"
#include <stdlib.h>

struct GameEvent* GameEvent_construct(const char* const channel, const char* const senderType,
                                     struct SceneNode* sender, const char* const command, const char* const data) {
    struct GameEvent* gameEvent = NULL;
    gameEvent = (struct GameEvent*)malloc(sizeof(struct GameEvent));
    if (gameEvent) {
        gameEvent->eventChannel = channel;
        gameEvent->senderType = senderType;
        gameEvent->sender = sender;
        gameEvent->command = command;
        gameEvent->data = data;
    }
    return  gameEvent;
}

void GameEvent_destruct(struct GameEvent* gameEvent) {
    free(gameEvent);
}