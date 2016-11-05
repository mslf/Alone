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
#include "scene/SceneNode.h"

void SceneNode_init(struct SceneNode* sceneNode) {
    sceneNode->isActive = true;
    sceneNode->angle = 0.0;
    sceneNode->coordinates.x = 0;
    sceneNode->coordinates.y = 0;
    sceneNode->flip = SDL_FLIP_NONE;
    sceneNode->rotatePointCoordinates.x = 0;
    sceneNode->rotatePointCoordinates.y = 0;
    sceneNode->scaleX = 1.0;
    sceneNode->scaleY = 1.0;
    sceneNode->type = NULL;
    sceneNode->control = NULL;
    sceneNode->render = NULL;
    sceneNode->update = NULL;
    sceneNode->sound = NULL;
    sceneNode->destruct = NULL;
    sceneNode->sceneNodeTextResource = NULL;
}
