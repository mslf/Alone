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
#ifndef ALONE_CHECKBOX_H
#define ALONE_CHECKBOX_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "sprite/Sprite.h"

#define CHECK_BOX_SCENENODE_PARSER_TYPE_STRING "CheckBox"
#define CHECK_BOX_SCENENODE_PARSER_FOCUSED_EVENT_RES_STRING "focusedEventResource"
#define CHECK_BOX_SCENENODE_PARSER_CHECKED_EVENT_RES_STRING "checkedEventResource"
#define CHECK_BOX_SCENENODE_PARSER_UNCHECKED_EVENT_RES_STRING "unCheckedEventResource"
#define CHECK_BOX_SCENENODE_PARSER_FOCUSED_SOUND_RES_STRING "focusedSoundResource"
#define CHECK_BOX_SCENENODE_PARSER_CHECKED_SOUND_RES_STRING "checkedSoundResource"
#define CHECK_BOX_SCENENODE_PARSER_UNCHECKED_SOUND_RES_STRING "unCheckedSoundResource"
#define CHECK_BOX_SCENENODE_PARSER_SPRITE_RES_STRING "spriteResource"

enum CheckBoxState {
    CheckBoxState_UnChecked = 0,
    CheckBoxState_FocusedUnChecked = 1,
    CheckBoxState_Checked = 2,
    CheckBoxState_FocusedChecked = 3
};
/*
 * CheckBox is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode blablaNode;" at the begining of CheckBox struct,
 * if you want code to work with CheckBox like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL (by calling SceneNode_init)
 * or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct CheckBox {
    struct SceneNode sceneNode;
    struct Sprite* sprite; // animation: 0 - normal, 1 - focusedNormal, 2 - checked, 3 - focusedChecked
    struct SoundResource* focusedSoundResource;
    struct SoundResource* checkedSoundResource;
    struct SoundResource* unCheckedSoundResource;
    struct TextResource* focusedEventResource;
    struct TextResource* checkedEventResource;
    struct TextResource* unCheckedEventResource;
    struct GameEvent* focusedEvent;
    struct GameEvent* checkedEvent;
    struct GameEvent* unCheckedEvent;
    bool isGeometryChanged;
    bool isStateChanged;
    enum CheckBoxState state;
};

struct CheckBox* CheckBox_construct(struct ResourceManager* const resourceManager, struct Renderer* renderer,
                                const char* const checkBoxResId);
void CheckBox_destruct(struct CheckBox* checkBox);

unsigned char CheckBox_changeFocusedEventResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                                                const char* const focusedEventResId);
unsigned char CheckBox_changeCheckedEventResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                                                const char* const checkedEventResId);
unsigned char CheckBox_changeUnCheckedEventResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                                                const char* const unCheckeEventResId);
unsigned char CheckBox_save(const struct CheckBox* const checkBox, struct ResourceManager* const resourceManager, 
                          const char* const checkBoxResId);
void CheckBox_control(struct SceneNode* sceneNode, struct EventManager* eventManager);
void CheckBox_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);
void CheckBox_render(struct SceneNode* sceneNode, struct Renderer* renderer);
void CheckBox_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_CHECKBOX_H
