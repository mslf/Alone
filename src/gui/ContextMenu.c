//
// Created by mslf on 8/13/16.
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
#include "gui/ContextMenu.h"

struct ContextMenu* ContextMenu_construct(struct ResourceManager* const resourceManager,
                                          const char* const contextMenuResId) {

}

void ContextMenu_destruct(struct ContextMenu* contextMenu) {

}

void ContextMenu_addMenuOption(struct ContextMenu* contextMenu, struct ResourceManager* const resourceManager,
                               const char* const elementResId) {

}

void ContextMenu_removeMenuOption(struct ContextMenu* contextMenu, struct ResourceManager* const resourceManager,
                                  const char* const elementResId) {

}

void ContextMenu_save(
        const struct ContextMenu* const contextMenu, struct ResourceManager* const resourceManager,
        const char* const contextMenuResId) {

}

void ContextMenu_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {

}

void ContextMenu_update(struct SceneNode* sceneNode, struct EventManager* eventManager) {

}

void ContextMenu_render(struct SceneNode* sceneNode, struct Renderer* renderer) {

}

void ContextMenu_sound(struct SceneNode* sceneNode, struct Musican* musican) {

}