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
#ifndef ALONE_BODY_H
#define ALONE_BODY_H

#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"

#define ITEM_SCENENODE_PARSER_TYPE_STRING "Body"
/*
 * Body is an inheritor of the PhysicalSceneNode.
 * You SHOULD include the "struct PhysicalSceneNode* blablaNode;" at the begining of Body struct,
 * if you want code to work with Body like with a PhysicalSceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL or to your function implementation.
 * Don't forget to add this warning comment to your own new PhysicalSceneNode inheritors.
 */
struct Body {
    struct PhysicalSceneNode* physicalSceneNode;
    struct TextResource* usedEventResource;
    struct TextResource* spriteResource;
    struct TextureResource* textureResource;
    struct GameEvent* usedEvent;
    size_t currentAnimation;
    size_t currentFrame;
    SDL_Rect* srcRect;
    SDL_Rect* dstRect;
};

struct Body* Body_construct(struct ResourceManager* const resourceManager, const char* const itemResId);
void Body_destruct(struct Body* item);

void Body_save(
        const struct Body* const item, struct ResourceManager* const resourceManager, const char* const itemResId);
void Body_control(struct SceneNode* sceneNode, struct EventManager* eventManager);
void Body_update(struct SceneNode* sceneNode, struct EventManager* eventManager);
void Body_render(struct SceneNode* sceneNode, struct Renderer* renderer);

#endif //ALONE_BODY_H
