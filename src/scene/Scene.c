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

const char* const SCENE_ERR_SCENE_NODE_TYPE_NOT_DETECTED =
        "Scene_constructSceneNode: suitable SceneNode type haven't detected or constructing SceneNode failed!";
const char* const SCENE_ERR_SCENE_NODE_DEF =
        "Scene_init: definition of SceneNode haven't found!";

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

unsigned char Scene_initSceneNode(struct Scene* scene, const char* const sceneNode, struct TextParser* sceneTextParser) {
    if (!scene || !sceneTextParser || !sceneNode)
        return 1;
    TextParser_getItemsCount(sceneTextParser, sceneNode);
    if (sceneTextParser->lastError)
        return 2;
    size_t index = scene->sceneNodesCount - 1;
    // Don't care about errors here, because these setters are optional
    scene->sceneNodesList[index]->coordinates.x = (int)TextParser_getInt(sceneTextParser, sceneNode, 1);
    scene->sceneNodesList[index]->coordinates.y = (int)TextParser_getInt(sceneTextParser, sceneNode, 2);
    scene->sceneNodesList[index]->rotatePointCoordinates.x = (int)TextParser_getInt(sceneTextParser, sceneNode, 3);
    scene->sceneNodesList[index]->rotatePointCoordinates.y = (int)TextParser_getInt(sceneTextParser, sceneNode, 4);
    scene->sceneNodesList[index]->flip = (SDL_RendererFlip)TextParser_getInt(sceneTextParser, sceneNode, 5);
    scene->sceneNodesList[index]->angle = TextParser_getDouble(sceneTextParser, sceneNode, 6);
    scene->sceneNodesList[index]->scaleX = TextParser_getDouble(sceneTextParser, sceneNode, 7);
    scene->sceneNodesList[index]->scaleY = TextParser_getDouble(sceneTextParser, sceneNode, 8);
    return 0;
}

void Scene_destructSceneNode(struct SceneNode* sceneNode) {
    if (!sceneNode)
        return;
    if (!sceneNode->destruct)
        return;
    sceneNode->destruct(sceneNode);
}

unsigned char Scene_init(struct Scene* scene, struct ResourceManager* resourceManager,
                         struct Renderer* renderer, struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar, 
                         struct TextParser* sceneTextParser) {
    if (!scene || !resourceManager || !renderer || !sceneTextParser)
        return 1;
    size_t i;
    size_t count;
    count = TextParser_getItemsCount(sceneTextParser, SCENE_PARSER_SCENE_NODES_STRING);
    if (sceneTextParser->lastError)
        return 2;
    scene->sceneNodesList = (struct SceneNode**)malloc(sizeof(struct SceneNode*) * count);
    if(!scene->sceneNodesList)
        return 3;
    scene->allocatedSceneNodesCount = count;
    scene->sceneNodesCount = 0;
    count = TextParser_getItemsCount(sceneTextParser, SCENE_PARSER_EVENT_CONTROLLERS_STRING);
    if (sceneTextParser->lastError)
        return 4;
    scene->eventControllersList = (struct ScriptResource**)malloc(sizeof(struct ScriptResource*) * count);
    if (!scene->eventControllersList)
        return 5;
    scene->allocatedEventControllersCount = count;
    scene->eventControllersCount = 0;
    char* tempString = NULL;
    for (i = 0; i < scene->allocatedSceneNodesCount; i++) {
        tempString = TextParser_getString(sceneTextParser, SCENE_PARSER_SCENE_NODES_STRING, i);
        if (tempString) {
            char* tempSceneNodeRes = NULL;
            tempSceneNodeRes = TextParser_getString(sceneTextParser, tempString, 0);
            if (!tempSceneNodeRes) {
                char tempErrString[600];
                sprintf(tempErrString, "%s Name: <%s>", SCENE_ERR_SCENE_NODE_DEF, tempString);
                Logger_log(resourceManager->logger, tempErrString);
            }
            if (Scene_addSceneNode(scene, resourceManager, renderer, sceneNodeTypesRegistrar, tempSceneNodeRes) == 0)
                Scene_initSceneNode(scene, tempString, sceneTextParser);
        }
    }
    for (i = 0; i < scene->allocatedEventControllersCount; i++) {
        tempString = TextParser_getString(sceneTextParser, SCENE_PARSER_EVENT_CONTROLLERS_STRING, i);
        if (tempString)
            Scene_addEventControllerScript(scene, resourceManager, tempString);
    }
    return 0;
}

struct Scene* Scene_construct(struct ResourceManager* const resourceManager,
                              struct Renderer* renderer,
                              struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar, 
                              const char* const sceneResId) {
    struct Scene* scene = NULL;
    if (!resourceManager || !renderer || !sceneResId)
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
    if (Scene_init(scene, resourceManager, renderer, sceneNodeTypesRegistrar, sceneTextParser)) {
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

unsigned char Scene_addSceneNode(struct Scene* scene, struct ResourceManager* const resourceManager,
                                 struct Renderer* renderer,
                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                 const char* const sceneNodeResId) {
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
    sceneNode = SceneNodeTypesRegistrar_constructSceneNode(resourceManager, renderer,
                                                                       sceneNodeTypesRegistrar,
                                                                       sceneNodeResId);
    if(!sceneNode) {
        char tempString[600];
        sprintf(tempString, "%s ResourceID: %s", SCENE_ERR_SCENE_NODE_TYPE_NOT_DETECTED, sceneNodeResId);
        Logger_log(resourceManager->logger, tempString);
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
        struct Scene* const scene, size_t index) {
    size_t i;
    if (scene && index < scene->sceneNodesCount) {
        Scene_destructSceneNode(scene->sceneNodesList[index]);
        scene->sceneNodesCount--;
        for (i = index; i < scene->sceneNodesCount; i++)
            scene->sceneNodesList[i] = scene->sceneNodesList[i+1];
    }
}

unsigned char Scene_save(struct Scene* const scene, struct ResourceManager* const resourceManager, const char* const sceneResId) {
    if (!scene || !resourceManager || !sceneResId)
        return 1;
    struct TextParser* textParser = NULL;
    size_t i;
    unsigned char result = 0;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return 2;
    TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING, SCENE_PARSER_TYPE_STRING);
    result += textParser->lastError;
    for (i = 0; i < scene->sceneNodesCount; i++) {
        char tempSceeNodeName[600];
        sprintf(tempSceeNodeName, "%ld", i);
        TextParser_addString(textParser, SCENE_PARSER_SCENE_NODES_STRING, tempSceeNodeName);
        result += textParser->lastError;
        TextParser_addString(textParser, tempSceeNodeName, scene->sceneNodesList[i]->sceneNodeTextResource->id);
        result += textParser->lastError;
        TextParser_addInt(textParser, tempSceeNodeName, scene->sceneNodesList[i]->coordinates.x);
        result += textParser->lastError;
        TextParser_addInt(textParser, tempSceeNodeName, scene->sceneNodesList[i]->coordinates.y);
        result += textParser->lastError;
        TextParser_addInt(textParser, tempSceeNodeName, scene->sceneNodesList[i]->rotatePointCoordinates.x);
        result += textParser->lastError;
        TextParser_addInt(textParser, tempSceeNodeName, scene->sceneNodesList[i]->rotatePointCoordinates.y);
        result += textParser->lastError;
        TextParser_addInt(textParser, tempSceeNodeName, (long)scene->sceneNodesList[i]->flip);
        result += textParser->lastError;
        TextParser_addDouble(textParser, tempSceeNodeName, scene->sceneNodesList[i]->angle);
        result += textParser->lastError;
        TextParser_addDouble(textParser, tempSceeNodeName, scene->sceneNodesList[i]->scaleX);
        result += textParser->lastError;
        TextParser_addDouble(textParser, tempSceeNodeName, scene->sceneNodesList[i]->scaleY);
        result += textParser->lastError;
    }
    for (i = 0; i < scene->eventControllersCount; i++) {
        TextParser_addString(textParser, SCENE_PARSER_EVENT_CONTROLLERS_STRING, scene->eventControllersList[i]->id);
        result += textParser->lastError;
    }
    char* newText = NULL;
    newText = TextParser_convertToText(textParser);
    result += textParser->lastError;
    result += TextResource_updateContent(scene->sceneResource, newText);
    result += ResourceManager_saveTextResource(resourceManager, scene->sceneResource, sceneResId);
    TextParser_destruct(textParser);
    if(newText)
        free(newText);
    if (result)
        return result;
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
