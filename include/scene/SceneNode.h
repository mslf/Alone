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
#ifndef ALONE_SCENENODE_H
#define ALONE_SCENENODE_H

#include <SDL2/SDL.h>
#include "eventManager/EventManager.h"
#include "renderer/Renderer.h"
#include "musican/Musican.h"

struct PhysicalSceneNode {
    struct SceneNode* sceneNode;
    double velX;
    double velY;
    double angleVel;
};

struct SceneNode {
    struct TextResource* sceneNodeTextResource;
    char* type;
    SDL_Point coordinates;
    SDL_Point rotatePointCoordinates;
    SDL_RendererFlip flip;
    double angle;
    double scaleX;
    double scaleY;
    void (*control)(struct SceneNode* sceneNode, struct EventManager* eventManager);
    void (*update)(struct SceneNode* sceneNode, struct EventManager* eventManager);
    void (*render)(struct SceneNode* sceneNode, struct Renderer* renderer);
    void (*sound)(struct SceneNode* sceneNode, struct Musican* musican);
};

struct SceneNode* SceneNode_construct();
void SceneNode_destruct(struct SceneNode* sceneNode);

#endif //ALONE_SCENENODE_H
