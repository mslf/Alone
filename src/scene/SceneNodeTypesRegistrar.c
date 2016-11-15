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

const char* const SCENENODE_TYPES_REGISTRAR_ERR = 
                    "SceneNodeTypesRegistrar_constructSceneNode: constructing SceneNode failed!";
const char* const SCENENODE_TYPES_REGISTRAR_ERR_NO_TYPE = 
                    "SceneNodeTypesRegistrar_constructSceneNode: suitable SceneNode type haven't detected!";
const char* const SCENENODE_TYPES_REGISTRAR_ERR_TYPE = 
                    "SceneNodeTypesRegistrar_constructSceneNode: SceneNode type string doesn't equal to required type string!";
const char* const SCENENODE_TYPES_REGISTRAR_ERR_NO_REGISTERED_TYPE = 
                    "SceneNodeTypesRegistrar_constructSceneNode: SceneNode type haven't registered!";

struct SceneNodeTypesRegistrar* SceneNodeTypesRegistrar_construct() {
    struct SceneNodeTypesRegistrar* SNTR = NULL;
    SNTR = (struct SceneNodeTypesRegistrar*)calloc(1, sizeof(struct SceneNodeTypesRegistrar));
    if (!SNTR)
        return NULL;
    SNTR->sceneNodeTypesList = (struct SceneNodeType*)malloc(sizeof(struct SceneNodeType) 
                                                * SCENE_NODE_TYPES_REGISTRAR_TYPES_REALLOCATION_STEP);
    if (!SNTR->sceneNodeTypesList) {
        SceneNodeTypesRegistrar_destruct(SNTR);
        return NULL;
    }
    SNTR->allocatedSceneNodeTypes = SCENE_NODE_TYPES_REGISTRAR_TYPES_REALLOCATION_STEP;
    return SNTR;
}

void SceneNodeTypesRegistrar_destruct(struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar) {
    if (!sceneNodeTypesRegistrar)
        return;
    if (sceneNodeTypesRegistrar->sceneNodeTypesList) {
        size_t i;
        for (i = 0; i < sceneNodeTypesRegistrar->sceneNodeTypesCount; i++)
            free(sceneNodeTypesRegistrar->sceneNodeTypesList[i].type);
        free(sceneNodeTypesRegistrar->sceneNodeTypesList);
    }
    free(sceneNodeTypesRegistrar);
}

static unsigned char SceneNodeTypesRegistrar_reallocateTypesList(struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar) {
    if (!sceneNodeTypesRegistrar)
        return 1;
    struct SceneNodeType* sceneNodeTypesList = NULL;
    size_t i;
    size_t newSize = sceneNodeTypesRegistrar->allocatedSceneNodeTypes + SCENE_NODE_TYPES_REGISTRAR_TYPES_REALLOCATION_STEP;
    sceneNodeTypesList = (struct SceneNodeType*)malloc(sizeof(struct SceneNodeType) * newSize);
    if (!sceneNodeTypesList)
        return 2;
    for (i = 0; i < sceneNodeTypesRegistrar->sceneNodeTypesCount; i++)
        sceneNodeTypesList[i] = sceneNodeTypesRegistrar->sceneNodeTypesList[i];
    free(sceneNodeTypesRegistrar->sceneNodeTypesList);
    sceneNodeTypesRegistrar->sceneNodeTypesList = sceneNodeTypesList;
    sceneNodeTypesRegistrar->allocatedSceneNodeTypes = newSize;
    return 0;
}

unsigned char SceneNodeTypesRegistrar_registerNewSceneNodeType(struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                      const char* const typeString,
                                                      struct SceneNode* (*constructor)(
                                                          struct ResourceManager* const resourceManager, 
                                                          struct Renderer* const renderer,
                                                          struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                          struct TextParser* const textParser)) {
    if (!sceneNodeTypesRegistrar || !typeString || !constructor)
        return 1;
    size_t i;
    for (i = 0; i < sceneNodeTypesRegistrar->sceneNodeTypesCount; i++)
        if (strcmp(typeString, sceneNodeTypesRegistrar->sceneNodeTypesList[i].type) == 0)
            return 0;
    if (sceneNodeTypesRegistrar->sceneNodeTypesCount >= sceneNodeTypesRegistrar->allocatedSceneNodeTypes)
        if (SceneNodeTypesRegistrar_reallocateTypesList(sceneNodeTypesRegistrar))
            return 2;
    char* tempString = (char*)malloc(sizeof(char) * (strlen(typeString) + 1));
    if (!tempString)
        return 3;
    strcpy(tempString, typeString);
    sceneNodeTypesRegistrar->sceneNodeTypesList[sceneNodeTypesRegistrar->sceneNodeTypesCount].type = tempString;
    sceneNodeTypesRegistrar->sceneNodeTypesList[sceneNodeTypesRegistrar->sceneNodeTypesCount].construct = constructor;
    sceneNodeTypesRegistrar->sceneNodeTypesCount++;
    return 0;
}

//? Messy. Refactor.
// Remember you can create some static functions and call them with 0 overhead.
struct SceneNode* SceneNodeTypesRegistrar_constructSceneNode(struct ResourceManager* resourceManager,
                                                             struct Renderer* renderer,
                                                             struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                             const char* const resId,
                                                             const char* const requiredTypeString) {
    if (!resourceManager || !renderer || !sceneNodeTypesRegistrar || !resId)
        return NULL;
    struct TextResource* tempTextResource = ResourceManager_loadTextResource(resourceManager, resId, 0);
    if (!tempTextResource)
        return NULL;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructFromTextResource(resourceManager->logger, tempTextResource);
    if (!textParser) {
        tempTextResource->pointersCount--;
        return NULL;
    }
    const char* typeString = TextParser_getString(textParser, TEXT_PARSER_TYPE_STRING, 0);
    if (!typeString) {
        char tempErrString[600];
        sprintf(tempErrString, "%s Resource ID: %s", SCENENODE_TYPES_REGISTRAR_ERR_NO_TYPE, resId);
        Logger_log(renderer->logger, tempErrString);
        sprintf(tempErrString, "%s Resource ID: %s", SCENENODE_TYPES_REGISTRAR_ERR, resId);
        Logger_log(renderer->logger, tempErrString);
        tempTextResource->pointersCount--;
        TextParser_destruct(textParser);
        return NULL;
    }
    size_t i;
    bool found = false;
    size_t foundIndex = 0;
    for (i = 0; i < sceneNodeTypesRegistrar->sceneNodeTypesCount; i++)
        if (strcmp(typeString, sceneNodeTypesRegistrar->sceneNodeTypesList[i].type) == 0) {
            found = true;
            foundIndex = i;
            break;
        }
    if (!found) {
        char tempErrString[600];
        sprintf(tempErrString, "%s Type: %s", SCENENODE_TYPES_REGISTRAR_ERR_NO_REGISTERED_TYPE, typeString);
        Logger_log(renderer->logger, tempErrString);
        sprintf(tempErrString, "%s Resource ID: %s", SCENENODE_TYPES_REGISTRAR_ERR, resId);
        Logger_log(renderer->logger, tempErrString);
        tempTextResource->pointersCount--;
        TextParser_destruct(textParser);
        return NULL;
    }
    // We check 'requiredTypeString' only if it not NULL
    if (requiredTypeString) {
        const char* tempTypeString = TextParser_getString(textParser, TEXT_PARSER_TYPE_STRING, 0);
        if (strcmp(tempTypeString, requiredTypeString) != 0) {
            char tempErrString[600];
            sprintf(tempErrString, "%s Type: %s", SCENENODE_TYPES_REGISTRAR_ERR_TYPE, typeString);
            Logger_log(renderer->logger, tempErrString);
            sprintf(tempErrString, "%s Resource ID: %s", SCENENODE_TYPES_REGISTRAR_ERR, resId);
            Logger_log(renderer->logger, tempErrString);
            tempTextResource->pointersCount--;
            TextParser_destruct(textParser);
            return NULL;
        }
    }
    struct SceneNode* tempSceneNode = 
                    sceneNodeTypesRegistrar->sceneNodeTypesList[foundIndex].construct(resourceManager, renderer,
                                                                             sceneNodeTypesRegistrar, textParser);
    TextParser_destruct(textParser);
    if (!tempSceneNode) {
        tempTextResource->pointersCount--;
        char tempErrString[600];
        sprintf(tempErrString, "%s Resource ID: %s", SCENENODE_TYPES_REGISTRAR_ERR, resId);
        Logger_log(renderer->logger, tempErrString);
        return NULL;
    }
    tempSceneNode->sceneNodeTextResource = tempTextResource;
    return tempSceneNode;
}
