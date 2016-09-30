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
#ifndef ALONE_SPRITE_H
#define ALONE_SPRITE_H

#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
/*
 * Sprite is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode* blablaNode;" at the begining of Sprite struct,
 * if you want code to work with Sprite like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct Sprite {
    struct SceneNode* sceneNode;
    struct TextResource* spriteResource;
    struct TextureResource* textureResource;
    size_t currentAnimation;
    size_t currentFrame;
    SDL_Rect* srcRect;
    SDL_Rect* dstRect;
};

struct Sprite* Sprite_construct(struct ResourceManager* const resourceManager, const char* const spriteResId);
void Sprite_destruct(struct Sprite* sprite);

void Sprite_save(
        const struct Sprite* const sprite, struct ResourceManager* const resourceManager,
        const char* const spriteResId);
void Sprite_update(struct SceneNode* sceneNode, struct EventManager* eventManager);
void Sprite_render(struct SceneNode* sceneNode, struct Renderer* renderer);

#endif //ALONE_SPRITE_H
