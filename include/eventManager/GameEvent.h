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

#include <stdbool.h>
#include "textParser/TextParser.h"

struct SceneNode;

#define GAME_EVENT_PARSER_TYPE_STRING "GameEvent"
#define GAME_EVENT_PARSER_CHANNEL_STRING "channel"
#define GAME_EVENT_PARSER_COMMAND_STRING "command"
#define GAME_EVENT_PARSER_DATA_STRING "data"

struct GameEvent {
    char* eventChannel;
    struct SceneNode* sender;
    char* command;
    char* data;
    bool isNeeded;
};

struct GameEvent* GameEvent_construct(const char* const channel, struct SceneNode* sender, const char* const command,
                                      const char* const data);
struct GameEvent* GameEvent_constructFromTextParser(struct TextParser* textParser, struct SceneNode* sender);
void GameEvent_destruct(struct GameEvent* gameEvent);

#endif //ALONE_GAMEEVENT_H
