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
#ifndef ALONE_PROGRESSBAR_H
#define ALONE_PROGRESSBAR_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "sprite/Sprite.h"

#define PROGRESS_BAR_SCENENODE_PARSER_TYPE_STRING "ProgressBar"
#define PROGRESS_BAR_SCENENODE_PARSER_SPRITE_RES_STRING "spriteResource"
#define PROGRESS_BAR_SCENENODE_PARSER_INIT_VALUE_STRING "value"
/*
 * ProgressBar is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode blablaNode;" at the begining of ProgressBar struct,
 * if you want code to work with ProgressBar like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL (by calling SceneNode_init)
 * or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct ProgressBar {
    struct SceneNode sceneNode;
    struct Sprite* spriteBase; // animation: 0 - base
    struct Sprite* spriteBar; // animation: 1 - bar
    unsigned char value;
    bool isGeometryChanged;
};

struct ProgressBar* ProgressBar_construct(struct ResourceManager* const resourceManager,
                                          struct Renderer* renderer, const char* const progressBarResId);
void ProgressBar_destruct(struct ProgressBar* progressBar);

unsigned char ProgressBar_save(
        const struct ProgressBar* const progressBar, struct ResourceManager* const resourceManager,
        const char* const progressBarResId);
void ProgressBar_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);
void ProgressBar_render(struct SceneNode* sceneNode, struct Renderer* renderer);

#endif //ALONE_PROGRESSBAR_H
