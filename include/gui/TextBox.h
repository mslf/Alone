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

#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "controller/Controller.h"
#include "eventManager/EventManager.h"
#include "eventManager/GameEvent.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "resourceManager/TextResource.h"
#include "resourceManager/TextureResource.h"
#include "resourceManager/SoundResource.h"
/*
 * TextBox is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode* blablaNode;" at the begining of TextBox struct,
 * if you want code to work with TextBox like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct TextBox {
    struct SceneNode* sceneNode;
    struct TextureResource* textureResource;
    struct TextResource* textResource;
    struct SoundResource* focusedSoundResource;
    struct SoundResource* pressedSoundResource;
    struct GameEvent* pressedEvent;
    SDL_Rect* srcRect;
    SDL_Rect* dstRect;
};

struct TextBox* TextBox_construct(struct ResourceManager* const resourceManager, const char* const resId);
void TextBox_destruct(struct TextBox* textBox);

void TextBox_save(
        const struct TextBox* const textBox, struct ResourceManager* const resourceManager,
        const char* const resId);
void TextBox_control(struct SceneNode* sceneNode, struct Controller* controller);
void TextBox_update(struct SceneNode* sceneNode, struct EventManager* eventManager);
void TextBox_render(struct SceneNode* sceneNode, struct Renderer* renderer);
void TextBox_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_TEXTBOX_H
