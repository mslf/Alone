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
#ifndef ALONE_LISTBOX_H
#define ALONE_LISTBOX_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "gui/TextBox.h"
#include "gui/Button.h"
#include "gui/ContextMenu.h"

#define LIST_BOX_SCENENODE_PARSER_TYPE_STRING "ListBox"
#define LIST_BOX_SCENENODE_PARSER_TEXT_BOX_RES_STRING "textBoxResource"
#define LIST_BOX_SCENENODE_PARSER_BUTTON_RES_STRING "buttonResource"
#define LIST_BOX_SCENENODE_PARSER_CONTEXT_MENU_RES_STRING "contextMenuResource"
/*
 * ListBox is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode blablaNode;" at the begining of ListBox struct,
 * if you want code to work with ListBox like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL (by calling SceneNode_init)
 * or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct ListBox {
    struct SceneNode sceneNode;
    struct TextBox* textBox;
    struct Button* button;
    struct ContextMenu* contextMenu;
    bool isContextMenuShown;
    bool isGeometryChanged;
    bool isStringExistInList;
};

struct SceneNode* ListBox_construct(struct ResourceManager* const resourceManager,
                                    struct Renderer* const renderer,
                                    struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                    struct TextParser* const textParser);
void ListBox_destruct(struct SceneNode* listBox);

void ListBox_save(
        const struct ListBox* const listBox, struct ResourceManager* const resourceManager,
        const char* const listBoxResId);
void ListBox_control(struct SceneNode* sceneNode, struct EventManager* eventManager);
void ListBox_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);
void ListBox_render(struct SceneNode* sceneNode, struct Renderer* renderer);
void ListBox_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_LISTBOX_H
