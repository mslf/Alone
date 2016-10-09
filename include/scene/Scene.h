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
#include "eventManager/EventManager.h"

#define SCENE_PARSER_TYPE_STRING "Scene"
#define SCENE_PARSER_SCENE_NODES_STRING "sceneNodes"
#define SCENE_PARSER_EVENT_CONTROLLERS_STRING "eventControllers"
#define SCENE_NODES_REALLOCATION_STEP 10
#define EVENT_CONTROLLERS_REALLOOCATION_STEP 10
// SceneNode inheritors type strings
// May be I need to move these defines to other place?
#define BUTTON_SCENENODE_PARSER_TYPE_STRING "Button"
#define CHECK_BOX_SCENENODE_PARSER_TYPE_STRING "CheckBox"
#define CONTEXT_MENU_SCENENODE_PARSER_TYPE_STRING "CheckBox"
#define LIST_BOX_SCENENODE_PARSER_TYPE_STRING "ContextMenu"
#define PROGRESS_BAR_SCENENODE_PARSER_TYPE_STRING "ProgressBar"
#define SLIDER_SCENENODE_PARSER_TYPE_STRING "Slider"
#define TEXT_BOX_SCENENODE_PARSER_TYPE_STRING "TextBox"
#define ITEM_SCENENODE_PARSER_TYPE_STRING "Item"
#define LEVEL_SCENENODE_PARSER_TYPE_STRING "Level"
#define SPRITE_SCENENODE_PARSER_TYPE_STRING "Sprite"
#define TEXT_SCENENODE_PARSER_TYPE_STRING "Text"
#define USER_SCENENODE_PARSER_TYPE_STRING "User"

struct Scene {
    struct TextResource* sceneResource;
    struct SceneNode** sceneNodesList;
    struct ScriptResource** eventControllersList;
    size_t allocatedSceneNodesCount;
    size_t sceneNodesCount;
    size_t allocatedEventControllersCount;
    size_t eventControllersCount;
};

struct Scene* Scene_construct(struct ResourceManager* const resourceManager, const char* const sceneResId);
void Scene_destruct (struct Scene* scene);

unsigned char Scene_addSceneNode(
        struct Scene* const scene, struct ResourceManager* const resourceManager, const char* const sceneNodeResId);
void Scene_removeSceneNode(
        struct Scene* const scene, struct ResourceManager* const resourceManager, size_t index);
unsigned char Scene_save(struct Scene* const scene, struct ResourceManager* const resourceManager, const char* const sceneResId);
unsigned char Scene_addEventControllerScript(struct Scene* scene, struct ResourceManager* resourceManager,
                                    const char* const scriptResId);
void Scene_removeEventControllerScript(struct Scene* scene, struct ResourceManager* resourceManager,
                                       const char* const scriptResId);
void Scene_update(struct Scene* scene);
#endif //ALONE_SCENE_H