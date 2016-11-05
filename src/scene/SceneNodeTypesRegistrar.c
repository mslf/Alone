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
#include "scene/SceneNodeTypesRegistrar.h"

struct SceneNodeTypesRegistrar* SceneNodeTypesRegistrar_construct() {
    
}

void SceneNodeTypesRegistrar_destruct(struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar) {
    
}

bool SceneNodeTypesRegistrar_registerNewSceneNodeType(const char* const typeString,
                                                      struct SceneNode* (*constructor)(
                                                          struct ResourceManager* const resourceManager,
                                                          struct Renderer* const renderer,
                                                          struct TextParser* const textParser)) {
    
}

struct SceneNode* SceneNodeTypesRegistrar_constructSceneNode(struct ResourceManager* resourceManager,
                                                             struct Renderer* renderer, const char* const resId) {
    
}
