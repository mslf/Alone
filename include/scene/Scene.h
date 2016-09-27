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
/*
 * Scene is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode* blablaNode;" at the begining of Scene struct,
 * if you want code to work with Scene like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct Scene {
    struct SceneNode** sceneNodesList;
};

struct Scene* Scene_construct(struct ResourceManager* const resourceManager, const char* const resId);
void Scene_destruct (struct Scene* scene);
void Scene_addSceneNode(
        struct Scene* const, struct ResourceManager* const resourceManager, const char* const resId);
void Scene_removeSceneNode(
        struct Scene* const, struct ResourceManager* const resourceManager, const char* const resId);
struct SceneNode* const * const Scene_getSceneNodesList(struct Scene* const);
void Scene_save(struct ResourceManager* const resourceManager, const char* const resId);

#endif //ALONE_SCENE_H
