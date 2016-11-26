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
/**
 * @file SceneNode.c
 * @author mslf
 * @date 11 Aug 2016
 * @brief File containing implementation of #SceneNode.
 */
#include "scene/SceneNode.h"

void SceneNode_init(struct SceneNode* sceneNode) {
    if (!sceneNode)
        return;
    sceneNode->nodeType = SCENE_NODE_SIMPLE;
    sceneNode->isActive = true;
    sceneNode->coordinates.x = 0;
    sceneNode->coordinates.y = 0;
    sceneNode->type = NULL;
    sceneNode->control = NULL;
    sceneNode->render = NULL;
    sceneNode->update = NULL;
    sceneNode->sound = NULL;
    sceneNode->destruct = NULL;
    sceneNode->sceneNodeTextResource = NULL;
}

void SceneNode_initDynamic(struct DynamicSceneNode* dynamicSceneNode) {
    if (!dynamicSceneNode)
        return;
    SceneNode_init((struct SceneNode*)dynamicSceneNode);
    dynamicSceneNode->sceneNode.nodeType = SCENE_NODE_DYNAMIC;
    dynamicSceneNode->angle = 0.0;
    dynamicSceneNode->flip = SDL_FLIP_NONE;
    dynamicSceneNode->rotatePointCoordinates.x = 0;
    dynamicSceneNode->rotatePointCoordinates.y = 0;
    dynamicSceneNode->scaleX = 1.0;
    dynamicSceneNode->scaleY = 1.0;
}

void SceneNode_initPhysical(struct PhysicalSceneNode* physicalSceneNode) {
    if (physicalSceneNode)
        return;
    SceneNode_initDynamic((struct DynamicSceneNode*)physicalSceneNode);
    physicalSceneNode->dynamicSceneNode.sceneNode.nodeType = SCENE_NODE_PHYSICAL;
    physicalSceneNode->angleVel = 0.0;
    physicalSceneNode->velX = 0.0;
    physicalSceneNode->velY = 0.0;
}
