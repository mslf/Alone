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
#ifndef ALONE_CONTEXTMENU_H
#define ALONE_CONTEXTMENU_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "gui/Button.h"

#define CONTEXT_MENU_SCENENODE_PARSER_TYPE_STRING "ContextMenu"
#define CONTEXT_MENU_SCENENODE_PARSER_PROTOTYPES "optionPrototypeButtonResources"
#define CONTEXT_MENU_SCENENODE_PARSER_OPTIONS_LIST "menuOptions"
enum {
    CONTEXT_MENU_SCENENODE_INITIAL_NUMBER_ALLOCATED_MENU_OPTIONS = 3
};
/*
 * ContextMenu is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode blablaNode;" at the begining of ContextMenu struct,
 * if you want code to work with ContextMenu like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL (by calling SceneNode_init)
 * or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct ContextMenu {
    struct SceneNode sceneNode;
    char* onlyOneMenuOptionPrototype;
    char* topMenuOptionPrototype;
    char* middleMenuOptionPrototype;
    char* lowerMenuOptionPrototype;
    struct Button** menuOptionsList;
    size_t allocatedMenuOptions;
    size_t menuOptionsCount;
    bool isGeometryChanged;
};

struct ContextMenu* ContextMenu_construct(struct ResourceManager* const resourceManager, struct Renderer* renderer,
                                          const char* const contextMenuResId);
void ContextMenu_destruct(struct ContextMenu* contextMenu);

unsigned char ContextMenu_addMenuOption(struct ContextMenu* contextMenu, struct ResourceManager* const resourceManager,
                                        struct Renderer* renderer, const char* const focusedEventRes,
                                        const char* const pressedEventRes, const char* const labelText);
void ContextMenu_removeMenuOption(struct ContextMenu* contextMenu, const char* const label,
                                  struct ResourceManager* const resourceManager,
                                  struct Renderer* renderer);
unsigned char ContextMenu_save(
        const struct ContextMenu* const contextMenu, struct ResourceManager* const resourceManager,
        const char* const contextMenuResId);
void ContextMenu_control(struct SceneNode* sceneNode, struct EventManager* eventManager);
void ContextMenu_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);
void ContextMenu_render(struct SceneNode* sceneNode, struct Renderer* renderer);
void ContextMenu_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_CONTEXTMENU_H
