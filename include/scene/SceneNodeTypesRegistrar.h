//
// Created by mslf on 11/5/16.
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
#ifndef ALONE_SCENENODE_TYPES_REGISTRAR_H
#define ALONE_SCENENODE_TYPES_REGISTRAR_H
#include "textParser/TextParser.h"
#include "resourceManager/ResourceManager.h"
#include "renderer/Renderer.h"
#include "scene/SceneNode.h"

enum {
    SCENE_NODE_TYPES_REGISTRAR_TYPES_REALLOCATION_STEP = 20
};
struct SceneNodeTypesRegistrar;
struct SceneNodeType {
    char* type;
    struct SceneNode* (*construct)(struct ResourceManager* const resourceManager, 
                                    struct Renderer* const renderer,
                                    struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar, 
                                    struct TextParser* const textParser);
};

struct SceneNodeTypesRegistrar {
    struct SceneNodeType* sceneNodeTypesList;
    size_t allocatedSceneNodeTypes;
    size_t sceneNodeTypesCount;
};

struct SceneNodeTypesRegistrar* SceneNodeTypesRegistrar_construct();
void SceneNodeTypesRegistrar_destruct(struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar);

unsigned char SceneNodeTypesRegistrar_registerNewSceneNodeType(struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                      const char* const typeString,
                                                      struct SceneNode* (*constructor)(
                                                          struct ResourceManager* const resourceManager, 
                                                          struct Renderer* const renderer,
                                                          struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                          struct TextParser* const textParser));
struct SceneNode* SceneNodeTypesRegistrar_constructSceneNode(struct ResourceManager* resourceManager,
                                                             struct Renderer* renderer,
                                                             struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                             const char* const resId);
#endif //ALONE_SCENENODE_TYPES_REGISTRAR_H
