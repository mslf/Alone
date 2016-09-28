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

#include "scene/SceneNode.h"
#include "resourceManager/ResourceManager.h"
#include "resourceManager/ScriptResource.h"
#include "eventManager/EventManager.h"

struct Scene {
    struct SceneNode** sceneNodesList;
    struct ScriptResource** scriptResourcesList;
    size_t scriptResourcesCount;
};

struct Scene* Scene_construct(struct ResourceManager* const resourceManager, const char* const resId);
void Scene_destruct (struct Scene* scene);

void Scene_addSceneNode(
        struct Scene* const, struct ResourceManager* const resourceManager, const char* const resId);
void Scene_removeSceneNode(
        struct Scene* const, struct ResourceManager* const resourceManager, const char* const resId);
struct SceneNode* const * const Scene_getSceneNodesList(struct Scene* const);
void Scene_save(struct ResourceManager* const resourceManager, const char* const resId);
void Scene_addEventControllerScript(struct Scene* scene, struct ResourceManager* resourceManager,
                                    const char* const resId);
void Scene_removeEventControllerScript(struct Scene* scene, struct ResourceManager* resourceManager,
                                       const char* const resId);
void Scene_update(struct Scene* scene, struct EventManager* eventManager, struct ResourceManager* resourceManager);
#endif //ALONE_SCENE_H
