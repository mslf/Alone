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
#include <stdlib.h>
#include <string.h>
#include "eventManager/GameEvent.h"

struct GameEvent* GameEvent_construct(const char* const channel, struct SceneNode* sender, const char* const command,
                                      const char* const data) {
    struct GameEvent* gameEvent = NULL;
    gameEvent = (struct GameEvent*)calloc(1, sizeof(struct GameEvent));
    if (!gameEvent)
        return NULL;
    gameEvent->eventChannel = (char*)malloc(sizeof(char) * (strlen(channel) + 1));
    if (!gameEvent->eventChannel) {
        GameEvent_destruct(gameEvent);
        return NULL;
    }
    strcpy(gameEvent->eventChannel, channel);
    gameEvent->command = (char*)malloc(sizeof(char) * (strlen(command) + 1));
    if (!gameEvent->command) {
        GameEvent_destruct(gameEvent);
        return NULL;
    }
    strcpy(gameEvent->command, command);
    gameEvent->data = (char*)malloc(sizeof(char) * (strlen(data) + 1));
    if (!gameEvent->data) {
        GameEvent_destruct(gameEvent);
        return NULL;
    }
    strcpy(gameEvent->data, data);
    gameEvent->sender = sender;
    gameEvent->isNeeded = true;
    return  gameEvent;
}

struct GameEvent* GameEvent_constructFromTextParser(struct TextParser* textParser, struct SceneNode* sender) {
    if (!textParser || !sender)
        return NULL;
    char* tempTypeString = TextParser_getString(textParser, TEXT_PARSER_TYPE_STRING, 0);
    if (!tempTypeString)
        return NULL;
    if (strcmp(tempTypeString, GAME_EVENT_PARSER_TYPE_STRING) != 0)
        return NULL;
    char* tempChannelString = TextParser_getString(textParser, GAME_EVENT_PARSER_CHANNEL_STRING, 0);
    if (!tempChannelString)
        return NULL;
    char* tempCommandString = TextParser_getString(textParser, GAME_EVENT_PARSER_COMMAND_STRING, 0);
    if (!tempCommandString)
        return NULL;
    char* tempDataString = TextParser_getString(textParser, GAME_EVENT_PARSER_DATA_STRING, 0);
    if (!tempDataString)
        return NULL;
    return GameEvent_construct(tempChannelString, sender, tempCommandString, tempDataString);
}

void GameEvent_destruct(struct GameEvent* gameEvent) {
    if (!gameEvent)
        return;
    if (gameEvent->eventChannel)
        free(gameEvent->eventChannel);
    if (gameEvent->command)
        free(gameEvent->command);
    if (gameEvent->data)
        free(gameEvent->data);
    free(gameEvent);
}
