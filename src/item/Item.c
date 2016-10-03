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
#include "item/Item.h"

struct Item* Item_construct(struct ResourceManager* const resourceManager, const char* const itemResId) {

}

void Item_destruct(struct Item* item) {

}

void Item_save(
        const struct Item* const item, struct ResourceManager* const resourceManager, const char* const itemResId) {

}

void Item_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {

}

void Item_update(struct SceneNode* sceneNode, struct EventManager* eventManager) {

}

void Item_render(struct SceneNode* sceneNode, struct Renderer* renderer) {

}