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
#include "scene/SceneNodeTypesRegistrar.h"

#define SPRITE_SCENENODE_PARSER_TYPE_STRING "Sprite"
#define SPRITE_SCENENODE_PARSER_FRAMES_COUNT "animationsListFramesCount"
#define SPRITE_SCENENODE_PARSER_ONE_FRAME_DURATION "animationsListOneFrameDuration"
#define SPRITE_SCENENODE_PARSER_FRAME_WIDTH "frameWidth"
#define SPRITE_SCENENODE_PARSER_FRAME_HEIGHT "frameHeight"
#define SPRITE_SCENENODE_PARSER_VIRTUAL_WIDTH "virtualWidth"
#define SPRITE_SCENENODE_PARSER_VIRTUAL_HEIGHT "virtualHeight"
#define SPRITE_SCENENODE_PARSER_TEXTURE_RESOURCE "textureResource"
#define SPRITE_SCENENODE_DEFAULT_TEXTURE "data/default.png"

enum {
    SPRITE_SCENENODE_DEFAULT_ANIMATIONS_COUNT = 1,
    SPRITE_SCENENODE_DEFAULT_FRAMES_COUNT = 1,
    SPRITE_SCENENODE_DEFAULT_ONE_FRAME_DURATION = 0,
    SPRITE_SCENENODE_DEFAULT_FRAME_WIDTH_DIVIDER = 2,
    SPRITE_SCENENODE_DEFAULT_FRAME_HEIGHT_DIVIDER = 2,
};

struct Animation {
    size_t framesCount;
    size_t oneFrameDuration;
};
/*
 * Sprite is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode blablaNode;" at the begining of Sprite struct,
 * if you want code to work with Sprite like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL (by calling SceneNode_init)
 * or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct Sprite {
    struct SceneNode sceneNode;
    struct TextureResource* textureResource;
    struct Animation* animations;
    size_t animationsCount;
    size_t currentAnimation;
    size_t currentFrame;
    SDL_Point frameSize;
    SDL_Point virtualSize;
    size_t renderingsCounter;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
    SDL_Point percentsToRender;
};

struct Sprite* Sprite_construct(struct ResourceManager* const resourceManager, struct Renderer* const renderer,
                                const char* const spriteResId);
struct SceneNode* Sprite_constructFromTextParser(struct ResourceManager* const resourceManager,
                                                 struct Renderer* const renderer,
                                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                 struct TextParser* const textParser);
void Sprite_destruct(struct SceneNode* sprite);

unsigned char Sprite_changeTextureResource(struct Sprite* sprite, struct ResourceManager* resourceManager,
                                           struct Renderer* renderer, const char* const textureResId);
unsigned char Sprite_save(
        const struct Sprite* const sprite, struct ResourceManager* const resourceManager,
        const char* const spriteResId);
void Sprite_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);
void Sprite_render(struct SceneNode* sceneNode, struct Renderer* renderer);

#endif //ALONE_SPRITE_H
