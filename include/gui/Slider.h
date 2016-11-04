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
#ifndef ALONE_SLIDER_H
#define ALONE_SLIDER_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "gui/ProgressBar.h"
#include "gui/Button.h"

#define SLIDER_SCENENODE_PARSER_TYPE_STRING "Slider"
#define SLIDER_SCENENODE_PARSER_PROGRESS_BAR_RESOURCE_STRING "progressBarResource"
#define SLIDER_SCENENODE_PARSER_BUTTON_RESOURCE_STRING "buttonResource"
#define SLIDER_SCENENODE_PARSER_STEP "step"
#define SLIDER_SCENENODE_PARSER_VALUE "value"
#define SLIDER_SCENENODE_PARSER_ALLIGN_STRING "allign"

enum AllignY {
    AllignY_center = 0,
    AllignY_top = 1,
    AllignY_bottom = 2,
    AllignY_none = 3
};
/*
 * Slider is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode blablaNode;" at the begining of Slider struct,
 * if you want code to work with Slider like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL (by calling SceneNode_init)
 * or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct Slider {
   struct SceneNode sceneNode;
   struct ProgressBar* base;
   struct Button* button;
   unsigned char step;
   enum AllignY buttonAllignY;
   bool isGeometryChanged;
   bool isButtonMoving;
   SDL_Point lastMouseCoordinates;
};

struct Slider* Slider_construct(struct ResourceManager* const resourceManager,
                                struct Renderer* renderer, const char* const sliderResId);
void Slider_destruct(struct Slider* slider);

unsigned char Slider_save(
        const struct Slider* const slider, struct ResourceManager* const resourceManager,
        const char* const sliderResId);
void Slider_control(struct SceneNode* sceneNode, struct EventManager* eventManager);
void Slider_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);
void Slider_render(struct SceneNode* sceneNode, struct Renderer* renderer);
void Slider_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_SLIDER_H
