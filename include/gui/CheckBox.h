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

#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
/*
 * CheckBox is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode* blablaNode;" at the begining of CheckBox struct,
 * if you want code to work with CheckBox like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct CheckBox {
    struct SceneNode* sceneNode;
    struct TextResource* checkBoxResource;
    struct TextResource* spriteResource;
    struct TextResource* checkedEventResource;
    struct TextureResource* textureResource;
    struct SoundResource* focusedSoundResource;
    struct SoundResource* checkedSoundResource;
    struct GameEvent* checkedEvent;
    unsigned char isChecked;
    SDL_Rect* srcRect;
    SDL_Rect* dstRect;
};

struct CheckBox* CheckBox_construct(struct ResourceManager* const resourceManager, const char* const checkBoxResId);
void CheckBox_destruct(struct CheckBox* checkBox);

void CheckBox_save(
        const struct CheckBox* const checkBox, struct ResourceManager* const resourceManager,
        const char* const checkBoxResId);
void CheckBox_control(struct SceneNode* sceneNode, struct EventManager* eventManager);
void CheckBox_update(struct SceneNode* sceneNode, struct EventManager* eventManager);
void CheckBox_render(struct SceneNode* sceneNode, struct Renderer* renderer);
void CheckBox_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_CHECKBOX_H
