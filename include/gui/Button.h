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
#ifndef ALONE_BUTTON_H
#define ALONE_BUTTON_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "sprite/Sprite.h"
#include "text/Text.h"

#define BUTTON_SCENENODE_PARSER_TYPE_STRING "Button"
#define BUTTON_SCENENODE_PARSER_FOCUSED_EVENT_RES_STRING "focusedEventResource"
#define BUTTON_SCENENODE_PARSER_PRESSED_EVENT_RES_STRING "pressedEventResource"
#define BUTTON_SCENENODE_PARSER_FOCUSED_SOUND_RES_STRING "focusedSoundResource"
#define BUTTON_SCENENODE_PARSER_PRESSED_SOUND_RES_STRING "pressedSoundResource"
#define BUTTON_SCENENODE_PARSER_SPRITE_RES_STRING "spriteResource"
#define BUTTON_SCENENODE_PARSER_TEXT_RES_STRING "textResource"
#define BUTTON_SCENENODE_PARSER_LABEL_OFFSET_STRING "labelOffset"

enum ButtonState {
    ButtonState_Normal = 0,
    ButtonState_Focused = 1,
    ButtonState_Pressed = 2
};
/*
 * Button is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode blablaNode;" at the begining of Button struct,
 * if you want code to work with Button like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL (by calling SceneNode_init)
 * or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct Button {
    struct SceneNode sceneNode;
    struct Sprite* sprite; // animation: 0 - normal, 1 - focused, 2 - pressed
    struct Text* label;
    struct SoundResource* focusedSoundResource;
    struct SoundResource* pressedSoundResource;
    struct TextResource* focusedEventResource;
    struct TextResource* pressedEventResource;
    struct GameEvent* focusedEvent;
    struct GameEvent* pressedEvent;
    SDL_Point labelOffset;
    bool isGeometryChanged;
    bool isStateChanged;
    enum ButtonState state;
};

struct Button* Button_construct(struct ResourceManager* const resourceManager, struct Renderer* renderer,
                                const char* const buttonResId);
void Button_destruct(struct Button* button);

unsigned char Button_changePressedEventResource(struct Button* button, struct ResourceManager* resourceManager,
                                                const char* const pressedEventResId);
unsigned char Button_changeFocusedEventResource(struct Button* button, struct ResourceManager* resourceManager,
                                                const char* const focusedEventResId);
unsigned char Button_save(const struct Button* const button, struct ResourceManager* const resourceManager, 
                          const char* const buttonResId);
void Button_control(struct SceneNode* sceneNode, struct EventManager* eventManager);
void Button_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);
void Button_render(struct SceneNode* sceneNode, struct Renderer* renderer);
void Button_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_BUTTON_H
