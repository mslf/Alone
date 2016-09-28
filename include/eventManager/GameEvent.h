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
#ifndef ALONE_GAMEEVENT_H
#define ALONE_GAMEEVENT_H

//#include "scene/SceneNode.h"
// ^ Circular include ^

struct GameEvent {
    char* eventChannel;
    char* senderType;
    struct SceneNode* sender;
    char* command;
    char* data;
};

struct GameEvent* GameEvent_construct(const char* const channel, const char* const senderType,
                                      struct SceneNode* sender, const char* const command, const char* const data);
void GameEvent_destruct(struct GameEvent* gameEvent);

#endif //ALONE_GAMEEVENT_H
