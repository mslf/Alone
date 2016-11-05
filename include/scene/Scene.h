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
#ifndef ALONE_SCENE_H
#define ALONE_SCENE_H

#include "scene/SceneNodeTypesRegistrar.h"
#include "scene/SceneNode.h"
#include "resourceManager/ResourceManager.h"
#include "eventManager/EventManager.h"

#define SCENE_PARSER_TYPE_STRING "Scene"
#define SCENE_PARSER_SCENE_NODES_STRING "sceneNodes"
#define SCENE_PARSER_EVENT_CONTROLLERS_STRING "eventControllers"
enum {
    SCENE_NODES_REALLOCATION_STEP = 10,
    EVENT_CONTROLLERS_REALLOOCATION_STEP = 10
};

struct Scene {
    struct TextResource* sceneResource;
    struct SceneNode** sceneNodesList;
    struct ScriptResource** eventControllersList;
    size_t allocatedSceneNodesCount;
    size_t sceneNodesCount;
    size_t allocatedEventControllersCount;
    size_t eventControllersCount;
};

struct Scene* Scene_construct(struct ResourceManager* const resourceManager,
                              struct Renderer* renderer,
                              struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar, 
                              const char* const sceneResId);
void Scene_destruct (struct Scene* scene);

unsigned char Scene_addSceneNode(struct Scene* scene, struct ResourceManager* const resourceManager,
                                 struct Renderer* renderer,
                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                 const char* const sceneNodeResId);
void Scene_removeSceneNode(struct Scene* const scene, size_t index);
unsigned char Scene_save(struct Scene* const scene, struct ResourceManager* const resourceManager,
                         const char* const sceneResId);
unsigned char Scene_addEventControllerScript(struct Scene* scene, struct ResourceManager* resourceManager,
                                    const char* const scriptResId);
void Scene_removeEventControllerScript(struct Scene* scene, struct ResourceManager* resourceManager,
                                       const char* const scriptResId);
void Scene_update(struct Scene* scene);
#endif //ALONE_SCENE_H
