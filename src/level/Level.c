//
// Created by mslf on 8/11/16.
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
#include "level/Level.h"

struct Level* Level_construct(struct ResourceManager* const resourceManager, const char* const levelResId) {

}

void Level_destruct(struct Level* level) {

}

void Level_save(
        const struct Level* const level, struct ResourceManager* const resourceManager,
        const char* const levelResId) {

}

void Level_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {

}

void Level_update(struct SceneNode* sceneNode, struct EventManager* eventManager) {

}

void Level_render(struct SceneNode* sceneNode, struct Renderer* renderer) {

}

void Level_sound(struct SceneNode* sceneNode, struct Musican* musican) {

}

void Level_addBackgroundModule(struct Level* level, const char* const textureResId, SDL_Point coordinates) {

}

void Level_removeBackgroundModule(struct Level* level, SDL_Point coordinates) {

}

void Level_addActiveModule(struct Level* level, const char* const activeModuleResId, SDL_Point coordinates) {

}

void Level_removeActiveModule(struct Level* level, SDL_Point coordinates) {

}

void Level_addConnection(struct Level* level, struct MacroConnection macroConnection) {

}

void Level_removeConnection(struct Level* level, size_t index) {

}