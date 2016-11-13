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
 * @file GameEvent.c
 * @author mslf
 * @date 12 Aug 2016
 * @brief File containing implementation of #GameEvent.
 */
#include <stdlib.h>
#include <string.h>
#include "eventManager/GameEvent.h"

struct GameEvent* GameEvent_construct(const char* const channel, struct SceneNode* sender, const char* const command,
                                      const char* const data) {
    if (!command && !data)
        return NULL;
    struct GameEvent* gameEvent = NULL;
    gameEvent = (struct GameEvent*)calloc(1, sizeof(struct GameEvent));
    if (!gameEvent)
        return NULL;
    if (channel) {
        gameEvent->eventChannel = (char*)malloc(sizeof(char) * (strlen(channel) + 1));
        if (!gameEvent->eventChannel) {
            GameEvent_destruct(gameEvent);
            return NULL;
        }
        strcpy(gameEvent->eventChannel, channel);
    }
    if (command) {
        gameEvent->command = (char*)malloc(sizeof(char) * (strlen(command) + 1));
        if (!gameEvent->command) {
            GameEvent_destruct(gameEvent);
            return NULL;
        }
        strcpy(gameEvent->command, command);
    }
    if (data) {
        gameEvent->data = (char*)malloc(sizeof(char) * (strlen(data) + 1));
        if (!gameEvent->data) {
            GameEvent_destruct(gameEvent);
            return NULL;
        }
        strcpy(gameEvent->data, data);
    }
    gameEvent->sender = sender;
    gameEvent->isNeeded = true;
    return gameEvent;
}

struct GameEvent* GameEvent_constructFromTextParser(struct TextParser* textParser, struct SceneNode* sender) {
    if (!textParser || !sender)
        return NULL;
    char* tempTypeString = TextParser_getString(textParser, TEXT_PARSER_TYPE_STRING, 0);
    if (!tempTypeString)
        return NULL;
    if (strcmp(tempTypeString, GamaEvent_parserStrings.type) != 0)
        return NULL;
    char* tempChannelString = TextParser_getString(textParser, GamaEvent_parserStrings.channel, 0);
    if (!tempChannelString)
        return NULL;
    char* tempCommandString = TextParser_getString(textParser, GamaEvent_parserStrings.command, 0);
    if (!tempCommandString)
        return NULL;
    char* tempDataString = TextParser_getString(textParser, GamaEvent_parserStrings.data, 0);
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
