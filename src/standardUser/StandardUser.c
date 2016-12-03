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
 * @file StandardUser.c
 * @author mslf
 * @date 28 Sep 2016
 * @brief File containing implementation of #StandardUser.
 */
#include "standardUser/StandardUser.h"

struct SceneNode* StandardUser_construct(struct ResourceManager* const resourceManager,
                                         struct Renderer* const renderer,
                                         struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                         struct TextParser* const textParser) {

}

void StandartUser_destruct(struct SceneNode* standardUser) {

}

enum StandardUserSceneNode_errors StandardUser_save(const struct StandardUser* const standardUser,
                                                    struct ResourceManager* const resourceManager,
                                                    const char* const userResId) {

}

void StandartUser_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {

}

void StandartUser_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {

}

void StandartUser_render(struct SceneNode* sceneNode, struct Renderer* renderer) {

}

void StandartUser_sound(struct SceneNode* sceneNode, struct Musican* musican){

}