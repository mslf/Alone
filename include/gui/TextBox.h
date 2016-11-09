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
#ifndef ALONE_TEXTBOX_H
#define ALONE_TEXTBOX_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "gui/Button.h"

#define TEXT_BOX_SCENENODE_PARSER_TYPE_STRING "TextBox"
#define TEXT_BOX_SCENENODE_PARSER_BUTTON_RES_STRING "buttonResource"
#define TEXT_BOX_SCENENODE_PARSER_MAX_LENGTH "maxLength"
enum {
    TEXT_BOX_SCENENODE_REALLOC_STRING_LENGTH_STEP = 500,
    TEXT_BOX_SCEENODE_MAX_LENGTH = 10
};
/*
 * TextBox is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode blablaNode;" at the begining of TextBox struct,
 * if you want code to work with TextBox like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL (by calling SceneNode_init)
 * or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct TextBox {
    struct SceneNode sceneNode;
    struct Button* box;
    char* string;
    size_t allocatedChars;
    size_t stringLength;
    size_t maxLength;
    bool haveFocus;
    bool isGeometryChanged;
    bool isStringChanged;
};

struct SceneNode* TextBox_construct(struct ResourceManager* const resourceManager,
                                    struct Renderer* const renderer,
                                    struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                    struct TextParser* const textParser);
void TextBox_destruct(struct SceneNode* textBox);

unsigned char TextBox_save(
        const struct TextBox* const textBox, struct ResourceManager* const resourceManager,
        const char* const textBoxResId);
void TextBox_control(struct SceneNode* sceneNode, struct EventManager* eventManager);
void TextBox_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);
void TextBox_render(struct SceneNode* sceneNode, struct Renderer* renderer);
void TextBox_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_TEXTBOX_H
