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
 * @file GameEvent.h
 * @author mslf
 * @date 12 Aug 2016
 * @brief File containing #GameEvent and it's stuff.
 */
#ifndef ALONE_GAMEEVENT_H
#define ALONE_GAMEEVENT_H

#include <stdbool.h>
#include "textParser/TextParser.h"

struct SceneNode;

/**
 * @brief Strings which are used for constructing #GameEvent from #TextParser.
 * @see GameEvent_constructFromTextParser().
 */
static const struct GameEvent_parserStrings {
    const char* const type; /**< Type string, which used to distinquish #GameEvent resource from others. */
    const char* const channel; /**< Channel string, which used to set GameEvent#channel from #TextParser. */
    const char* const command; /**< Command string, which used to set GameEvent#command from #TextParser. */
    const char* const data; /**< Data stringm which used to set GameEvent#data from #TextParser. */
}GamaEvent_parserStrings = {
    "GameEvent",
    "channel",
    "command",
    "data"};
    
/**
 * @brief Standart structure for #SceneNode communication and more.
 * @see #EventManager
 */
struct GameEvent {
    char* eventChannel; /**< String, which describes in what "channel" that #GameEvent happend. */
    struct SceneNode* sender; /**< Pointer to a sender #SceneNode. */
    char* command; /**< String with some command. */
    char* data; /**< String with some data about #GameEvent. */
    bool isNeeded; /**< Flag, which indicates actuality of #GameEvent. */
};

/**
 * @brief Constructs #GameEvent and inints it.
 * @param channel String to set GameEvent#eventChannel string. Can be NULL.
 * If channel is NULL, then #GameEvent will be global. May be useful for broadcasting events.
 * @param sender Pointer to a #SceneNode which sends that #GameEvent. Can be NULL.
 * If sender is NULL, then we think, that there is no specific sender, or it's jsut not a #SceneNode.
 * @param command String to set GameEvent#command string. Can be NULL.
 * If command is NULL, then we think that #GameEvent is just a notification.
 * @param data String to set GameEvent#data string. Can be NULL.
 * If data is NULL, then we think that #GameEvent don't need any data.
 * May be useful for commands without arguments.
 * @return Pointer to an initialized #GameEvent, or NULL if constructing failed.
 * @note Only one (command or data) string can be NULL at one time.
 */
struct GameEvent* GameEvent_construct(const char* const channel, struct SceneNode* sender, const char* const command,
                                      const char* const data);

/**
 * @brief Constructs #GameEvent and inits it from a #TextParser.
 * @param textParser Pointer to #TextParser with data strings for initializing #GameEvent. Can be NULL.
 * @param sender Pointer to a #SceneNode which sends that #GameEvent. Can be NULL.
 * If sender is NULL, then we think, that there is no specific sender, or it's jsut not a #SceneNode.
 * @return Pointer to an initialized #GameEvent, or NULL if constructing failed.
 * Also, it will return NULL if happened some errors while getting settings from #TextParser.
 * @see GameEvent_construct()
 * @see #TextParser
 * @see #GameEvent_parserStrings
 */
struct GameEvent* GameEvent_constructFromTextParser(struct TextParser* textParser, struct SceneNode* sender);

/**
 * @brief Destructs #GameEvent and frees memory, used by it.
 * Use this function in pair with EventManager_construct() or GameEvent_constructFromTextParser().
 * @param gameEvent Pointer to a #GameEvent. Can be not fully initialized. Can be NULL.
 * @see EventManager_construct()
 * @see GameEvent_constructFromTextParser()
 */
void GameEvent_destruct(struct GameEvent* gameEvent);

#endif //ALONE_GAMEEVENT_H
