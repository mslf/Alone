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
#include "scene/Scene.h"
#include "textParser/TextParser.h"
#include "gui/Button.h"
#include "gui/CheckBox.h"
#include "gui/ContextMenu.h"
#include "gui/ListBox.h"
#include "gui/ProgressBar.h"
#include "gui/Slider.h"
#include "gui/TextBox.h"
#include "item/Item.h"
#include "level/Level.h"
#include "sprite/Sprite.h"
#include "text/Text.h"
#include "user/User.h"

const char* const SCENE_ERR_SCENE_NODE_TYPE_NOT_DETECTED =
        "Scene_constructSceneNode: suitable SceneNode type haven't detected or constructing SceneNode failed!";

unsigned char Scene_reallocateSceneNodesList(struct Scene* scene) {
    if (!scene)
        return 1;
    struct SceneNode** sceneNodesList = NULL;
    size_t i;
    size_t newSize = scene->allocatedSceneNodesCount + SCENE_NODES_REALLOCATION_STEP;
    if (!(sceneNodesList = (struct SceneNode**)malloc(sizeof(struct SceneNode*) * newSize))) {
        free(sceneNodesList);
        return 2;
    }
    for (i = 0; i < newSize; i++)
        sceneNodesList[i] = NULL;
    for (i = 0; i < scene->sceneNodesCount; i++)
        sceneNodesList[i] = scene->sceneNodesList[i];
    free(scene->sceneNodesList);
    scene->sceneNodesList = sceneNodesList;
    scene->allocatedSceneNodesCount = newSize;
    return 0;
}

unsigned char Scene_reallocateEventControllersList(struct Scene* scene) {
    if (!scene)
        return 1;
    struct ScriptResource** eventControllersList = NULL;
    size_t i;
    size_t newSize = scene->allocatedEventControllersCount + EVENT_CONTROLLERS_REALLOOCATION_STEP;
    if (!(eventControllersList = (struct ScriptResource**)malloc(sizeof(struct ScriptResource*) * newSize))) {
        free(eventControllersList);
        return 2;
    }
    for (i = 0; i < newSize; i++)
        eventControllersList[i] = NULL;
    for (i = 0; i < scene->eventControllersCount; i++)
        eventControllersList[i] = scene->eventControllersList[i];
    free(scene->eventControllersList);
    scene->eventControllersList = eventControllersList;
    scene->allocatedEventControllersCount = newSize;
    return 0;
}

struct SceneNode* Scene_constructSceneNode(struct ResourceManager* resourceManager, struct Renderer* renderer,
                                           const char* const sceneNodeTypeString, 
                                           const char* const sceneNodeResId) {
    struct SceneNode* sceneNode = NULL;
    // Typed construction routine here
    if (strcmp(sceneNodeTypeString, BUTTON_SCENENODE_PARSER_TYPE_STRING) == 0)
        sceneNode = (struct SceneNode*)Button_construct(resourceManager, renderer, sceneNodeResId);
    if (strcmp(sceneNodeTypeString, CHECK_BOX_SCENENODE_PARSER_TYPE_STRING) == 0)
        sceneNode = (struct SceneNode*)CheckBox_construct(resourceManager, renderer, sceneNodeResId);
    if (strcmp(sceneNodeTypeString, CONTEXT_MENU_SCENENODE_PARSER_TYPE_STRING) == 0)
        sceneNode = (struct SceneNode*)ContextMenu_construct(resourceManager, sceneNodeResId);
    if (strcmp(sceneNodeTypeString, LIST_BOX_SCENENODE_PARSER_TYPE_STRING) == 0)
        sceneNode = (struct SceneNode*)ListBox_construct(resourceManager, sceneNodeResId);
    if (strcmp(sceneNodeTypeString, PROGRESS_BAR_SCENENODE_PARSER_TYPE_STRING) == 0)
        sceneNode = (struct SceneNode*)ProgressBar_construct(resourceManager, sceneNodeResId);
    if (strcmp(sceneNodeTypeString, SLIDER_SCENENODE_PARSER_TYPE_STRING) == 0)
        sceneNode = (struct SceneNode*)Slider_construct(resourceManager, sceneNodeResId);
    if (strcmp(sceneNodeTypeString, TEXT_BOX_SCENENODE_PARSER_TYPE_STRING) == 0)
        sceneNode = (struct SceneNode*)TextBox_construct(resourceManager, sceneNodeResId);
    if (strcmp(sceneNodeTypeString, ITEM_SCENENODE_PARSER_TYPE_STRING) == 0)
        sceneNode = (struct SceneNode*)Item_construct(resourceManager, sceneNodeResId);
    if (strcmp(sceneNodeTypeString, LEVEL_SCENENODE_PARSER_TYPE_STRING) == 0)
        sceneNode = (struct SceneNode*)Level_construct(resourceManager, sceneNodeResId);
    if (strcmp(sceneNodeTypeString, SPRITE_SCENENODE_PARSER_TYPE_STRING) == 0)
        sceneNode = (struct SceneNode*)Sprite_construct(resourceManager, renderer, sceneNodeResId);
    if (strcmp(sceneNodeTypeString, TEXT_SCENENODE_PARSER_TYPE_STRING) == 0)
        sceneNode = (struct SceneNode*)Text_construct(resourceManager, renderer, sceneNodeResId);
    if (strcmp(sceneNodeTypeString, USER_SCENENODE_PARSER_TYPE_STRING) == 0)
        sceneNode = (struct SceneNode*)User_construct(resourceManager, sceneNodeResId);
    if (!sceneNode) {
        char tempString[600];
        sprintf(tempString, "%s ResourceID: %s", SCENE_ERR_SCENE_NODE_TYPE_NOT_DETECTED, sceneNodeResId);
        Logger_log(resourceManager->logger, tempString);
        return NULL;
    }
    return sceneNode;
}

unsigned char Scene_initSceneNode(struct Scene* scene, size_t index, struct TextParser* sceneTextParser) {
    if (!scene || !sceneTextParser || index > scene->sceneNodesCount - 1)
        return 1;
    char tempString[100];
    sprintf(tempString, "%ld", index);
    TextParser_getItemsCount(sceneTextParser, tempString);
    if (sceneTextParser->lastError)
        return 2;
    // Don't care about errors here, because these setters are optional
    scene->sceneNodesList[index]->coordinates.x = (int)TextParser_getInt(sceneTextParser, tempString, 0);
    scene->sceneNodesList[index]->coordinates.y = (int)TextParser_getInt(sceneTextParser, tempString, 1);
    scene->sceneNodesList[index]->rotatePointCoordinates.x = (int)TextParser_getInt(sceneTextParser, tempString, 2);
    scene->sceneNodesList[index]->rotatePointCoordinates.y = (int)TextParser_getInt(sceneTextParser, tempString, 3);
    scene->sceneNodesList[index]->flip = (SDL_RendererFlip)TextParser_getInt(sceneTextParser, tempString, 4);
    scene->sceneNodesList[index]->angle = TextParser_getDouble(sceneTextParser, tempString, 5);
    scene->sceneNodesList[index]->scaleX = TextParser_getDouble(sceneTextParser, tempString, 6);
    scene->sceneNodesList[index]->scaleY = TextParser_getDouble(sceneTextParser, tempString, 7);
    return 0;
}

void Scene_destructSceneNode(struct SceneNode* sceneNode) {
    if (!sceneNode)
        return;
    // Typed destruction routine here
    if (!strcmp(sceneNode->type, BUTTON_SCENENODE_PARSER_TYPE_STRING))
        Button_destruct((struct Button*)sceneNode);
    if (!strcmp(sceneNode->type, CHECK_BOX_SCENENODE_PARSER_TYPE_STRING))
        CheckBox_destruct((struct CheckBox*)sceneNode);
    if (!strcmp(sceneNode->type, CONTEXT_MENU_SCENENODE_PARSER_TYPE_STRING))
        ContextMenu_destruct((struct ContextMenu*)sceneNode);
    if (!strcmp(sceneNode->type, LIST_BOX_SCENENODE_PARSER_TYPE_STRING))
        ListBox_destruct((struct ListBox*)sceneNode);
    if (!strcmp(sceneNode->type, PROGRESS_BAR_SCENENODE_PARSER_TYPE_STRING))
        ProgressBar_destruct((struct ProgressBar*)sceneNode);
    if (!strcmp(sceneNode->type, SLIDER_SCENENODE_PARSER_TYPE_STRING))
        Slider_destruct((struct Slider*)sceneNode);
    if (!strcmp(sceneNode->type, TEXT_BOX_SCENENODE_PARSER_TYPE_STRING))
        TextBox_destruct((struct TextBox*)sceneNode);
    if (!strcmp(sceneNode->type, ITEM_SCENENODE_PARSER_TYPE_STRING))
        Item_destruct((struct Item*)sceneNode);
    if (!strcmp(sceneNode->type, LEVEL_SCENENODE_PARSER_TYPE_STRING))
        Level_destruct((struct Level*)sceneNode);
    if (!strcmp(sceneNode->type, SPRITE_SCENENODE_PARSER_TYPE_STRING))
        Sprite_destruct((struct Sprite*)sceneNode);
    if (!strcmp(sceneNode->type, TEXT_PARSER_TYPE_STRING))
        Text_destruct((struct Text*)sceneNode);
    if (!strcmp(sceneNode->type, USER_SCENENODE_PARSER_TYPE_STRING))
        User_destruct((struct User*)sceneNode);
}

unsigned char Scene_init(struct Scene* scene, struct ResourceManager* resourceManager, struct Renderer* renderer,
                         struct TextParser* sceneTextParser) {
    size_t i;
    size_t count;
    count = TextParser_getItemsCount(sceneTextParser, SCENE_PARSER_SCENE_NODES_STRING);
    if (sceneTextParser->lastError)
        return 1;
    scene->sceneNodesList = (struct SceneNode**)malloc(sizeof(struct SceneNode*) * count);
    if(!scene->sceneNodesList)
        return 2;
    scene->allocatedSceneNodesCount = count;
    scene->sceneNodesCount = 0;
    count = TextParser_getItemsCount(sceneTextParser, SCENE_PARSER_EVENT_CONTROLLERS_STRING);
    if (sceneTextParser->lastError)
        return 3;
    scene->eventControllersList = (struct ScriptResource**)malloc(sizeof(struct ScriptResource*) * count);
    if (!scene->eventControllersList)
        return 4;
    scene->allocatedEventControllersCount = count;
    scene->eventControllersCount = 0;
    char* tempString = NULL;
    for (i = 0; i < scene->allocatedSceneNodesCount; i++) {
        tempString = TextParser_getString(sceneTextParser, SCENE_PARSER_SCENE_NODES_STRING, i);
        if (tempString) {
            if (Scene_addSceneNode(scene, resourceManager, renderer, tempString) == 0)
                Scene_initSceneNode(scene, scene->sceneNodesCount - 1, sceneTextParser);
        }
    }
    for (i = 0; i < scene->allocatedEventControllersCount; i++) {
        tempString = TextParser_getString(sceneTextParser, SCENE_PARSER_EVENT_CONTROLLERS_STRING, i);
        if (tempString)
            Scene_addEventControllerScript(scene, resourceManager, tempString);
    }
    return 0;
}

struct Scene* Scene_construct(struct ResourceManager* const resourceManager, struct Renderer* renderer,
                              const char* const sceneResId) {
    struct Scene* scene = NULL;
    if (!resourceManager)
        return NULL;
    if (!sceneResId)
        return NULL;
    scene = (struct Scene*)malloc(sizeof(struct Scene));
    if (!scene)
        return NULL;
    scene->sceneNodesList = NULL;
    scene->eventControllersList = NULL;
    scene->sceneResource = NULL;
    scene->sceneResource = ResourceManager_loadTextResource(resourceManager, sceneResId, 1);
    if (!scene->sceneResource) {
        Scene_destruct(scene);
        return NULL;
    }
    struct TextParser* sceneTextParser = NULL;
    sceneTextParser = TextParser_constructFromTextResource(resourceManager->logger, scene->sceneResource);
    if (!sceneTextParser) {
        scene->sceneResource->pointersCount--;
        Scene_destruct(scene);
        return NULL;
    }
    char* sceneTypeString = NULL;
    sceneTypeString = TextParser_getString(sceneTextParser, TEXT_PARSER_TYPE_STRING, 0);
    if (sceneTextParser->lastError) {
        scene->sceneResource->pointersCount--;
        Scene_destruct(scene);
        TextParser_destruct(sceneTextParser);
        return NULL;
    }
    if (strcmp(sceneTypeString, SCENE_PARSER_TYPE_STRING) != 0) {
        scene->sceneResource->pointersCount--;
        Scene_destruct(scene);
        TextParser_destruct(sceneTextParser);
        return NULL;
    }
    if (Scene_init(scene, resourceManager, renderer, sceneTextParser)) {
        scene->sceneResource->pointersCount--;
        Scene_destruct(scene);
        TextParser_destruct(sceneTextParser);
        return NULL;
    }
    TextParser_destruct(sceneTextParser);
    return scene;
}

void Scene_destruct (struct Scene* scene) {
    size_t i;
    if (scene) {
        if (scene->eventControllersList)
            free(scene->eventControllersList);
        if (scene->sceneNodesList) {
            for (i = 0; i < scene->sceneNodesCount; i++)
                Scene_destructSceneNode(scene->sceneNodesList[i]);
            free(scene->sceneNodesList);
        }
        if (scene->sceneResource)
            scene->sceneResource->pointersCount--;
        free(scene);
    }
}

unsigned char Scene_addSceneNode(struct Scene* const scene, struct ResourceManager* const resourceManager,
                                 struct Renderer* renderer, const char* const sceneNodeResId) {
    if (!scene || !resourceManager || !sceneNodeResId)
        return 1;
    struct TextResource* sceneNodeTextResource = NULL;
    sceneNodeTextResource = ResourceManager_loadTextResource(resourceManager, sceneNodeResId, 0);
    if (!sceneNodeTextResource)
        return 2;
    struct TextParser* sceneNodeTextParser = NULL;
    sceneNodeTextParser = TextParser_constructFromTextResource(resourceManager->logger, sceneNodeTextResource);
    if (!sceneNodeTextParser)
        return 3;
    char* sceneNodeTypeString = NULL;
    sceneNodeTypeString = TextParser_getString(sceneNodeTextParser, TEXT_PARSER_TYPE_STRING, 0);
    if (sceneNodeTextParser->lastError) {
        sceneNodeTextResource->pointersCount--;
        TextParser_destruct(sceneNodeTextParser);
        return 4;
    }
    // Construct, then try to reallocate (if needed) and add sceneNode to the list
    struct SceneNode* sceneNode = NULL;
    sceneNode = Scene_constructSceneNode(resourceManager, renderer, sceneNodeTypeString, sceneNodeResId);
    if(!sceneNode) {
        sceneNodeTextResource->pointersCount--;
        TextParser_destruct(sceneNodeTextParser);
        return 5;
    }
    if (scene->sceneNodesCount >= scene->allocatedSceneNodesCount)
        if (Scene_reallocateSceneNodesList(scene)) {
            sceneNodeTextResource->pointersCount--;
            TextParser_destruct(sceneNodeTextParser);
            Scene_destructSceneNode(sceneNode);
            return 6;
        }
    TextParser_destruct(sceneNodeTextParser);
    sceneNodeTextResource->pointersCount--;
    scene->sceneNodesList[scene->sceneNodesCount] = sceneNode;
    scene->sceneNodesCount++;
    return 0;
}

void Scene_removeSceneNode(
        struct Scene* const scene, struct ResourceManager* const resourceManager, size_t index) {
    size_t i;
    if (scene && resourceManager && index < scene->sceneNodesCount) {
        Scene_destructSceneNode(scene->sceneNodesList[index]);
        scene->sceneNodesCount--;
        for (i = index; i < scene->sceneNodesCount; i++)
            scene->sceneNodesList[i] = scene->sceneNodesList[i+1];
    }
}

unsigned char Scene_save(struct Scene* const scene, struct ResourceManager* const resourceManager, const char* const sceneResId) {
    if (!scene || !resourceManager || !sceneResId)
        return 1;
    struct TextParser* textParser= NULL;
    size_t i;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return 2;
    TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING, SCENE_PARSER_TYPE_STRING);
    if (textParser->lastError) {
        TextParser_destruct(textParser);
        return 3;
    }
    for (i = 0; i < scene->sceneNodesCount; i++) {
        TextParser_addString(textParser, SCENE_PARSER_SCENE_NODES_STRING,
                             scene->sceneNodesList[i]->sceneNodeTextResource->id);
        if (textParser->lastError) {
            TextParser_destruct(textParser);
            return 4;
        }
    }
    for (i = 0; i < scene->eventControllersCount; i++) {
        TextParser_addString(textParser, SCENE_PARSER_EVENT_CONTROLLERS_STRING, scene->eventControllersList[i]->id);
        if (textParser->lastError) {
            TextParser_destruct(textParser);
            return 5;
        }
    }
    char* newText = NULL;
    newText = TextParser_convertToText(textParser);
    if (textParser->lastError) {
        TextParser_destruct(textParser);
        return 6;
    }
    TextParser_destruct(textParser);
    if (TextResource_updateContent(scene->sceneResource, newText)) {
        free(newText);
        return 7;
    }
    if (ResourceManager_saveTextResource(resourceManager, scene->sceneResource, sceneResId)) {
        free(newText);
        return 8;
    }
    free(newText);
    return 0;
}

unsigned char Scene_addEventControllerScript(struct Scene* scene, struct ResourceManager* resourceManager,
                                    const char* const scriptResId) {
    if (!scene || !resourceManager || !scriptResId)
        return 1;
    struct ScriptResource* scriptResource = NULL;
    scriptResource = ResourceManager_loadScriptResource(resourceManager, scriptResId);
    if (!scriptResource)
        return 2;
    // Try to reallocate (if needed) and add scriptResource to the list
    if (scene->eventControllersCount >= scene->allocatedEventControllersCount)
        if (Scene_reallocateEventControllersList(scene))
            return 3;
    scene->eventControllersList[scene->eventControllersCount] = scriptResource;
    scene->eventControllersCount++;
    return 0;
}

void Scene_removeEventControllerScript(struct Scene* scene, struct ResourceManager* resourceManager,
                                       const char* const scriptResId) {
    size_t i;
    unsigned char found = 0;
    size_t foundIndex = 0;
    if (scene && resourceManager && scriptResId) {
        if (scene->eventControllersCount == 0)
            return; // There is no EventControllers
        for (i = 0; i < scene->eventControllersCount; i++)
            if (strcmp(scene->eventControllersList[i]->id, scriptResId) == 0) {
                found = 1;
                foundIndex = i;
                break;
            }
        if (!found)
            return;
        // There we need to tell to the ResourceManager that we don't need that Resource here.
        scene->eventControllersList[foundIndex] = NULL;
        scene->eventControllersCount--;
        for (i = foundIndex; i < scene->eventControllersCount; i++)
            scene->eventControllersList[i] = scene->eventControllersList[i + 1];
    }
}

void Scene_update(struct Scene* scene) {
    if (!scene)
        return;
    size_t i;
    for (i = 0; i < scene->eventControllersCount; i++)
        // We have sent pointers to ResourceManager, EventManager, GameManager to the script earlier
        // So we just exicute global code in script
        lua_pcall(scene->eventControllersList[i]->luaState, 0, 0, 0);
}
