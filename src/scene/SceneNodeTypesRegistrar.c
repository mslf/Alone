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
 * @file SceneNodeTypesRegistrar.c
 * @author mslf
 * @date 5 Nov 2016
 * @brief File containing implementation of #SceneNodeTypesRegistrar.
 */
#include <stddef.h>
#include "scene/SceneNodeTypesRegistrar.h"

/**
 * @brief Error message strings for #SceneNodeTypesRegistrar.
 */
static const struct SNTR_errorMessages {
    const char* const errConstructingNode;
    /**< Will be displayed when constructing #SceneNode inheritor type object failed. */
    const char* const errNoTypeString;
    /**< Will be displayed when any type string haven't found in #TextParser. */
    const char* const errType;
    /**< Will be displayed when wishful type string haven't found in #TextParser. */
    const char* const errNoRegisteredType;
    /**< Will be displayed when SceneNodeTypesRegistrar#sceneNodeTypesList doesn't contain registered 
     *type of #SceneNode inheritor with type string like that, which was loaded from #TextResource type string. */
}SNTR_errorMessages = {
    "SceneNodeTypesRegistrar_constructSceneNode: constructing SceneNode failed!",
    "SceneNodeTypesRegistrar_constructSceneNode: suitable SceneNode type haven't detected!",
    "SceneNodeTypesRegistrar_constructSceneNode: SceneNode type string doesn't equal to required type string!",
    "SceneNodeTypesRegistrar_constructSceneNode: SceneNode type haven't registered!"};

struct SceneNodeTypesRegistrar* SceneNodeTypesRegistrar_construct() {
    struct SceneNodeTypesRegistrar* SNTR = NULL;
    SNTR = (struct SceneNodeTypesRegistrar*)calloc(1, sizeof(struct SceneNodeTypesRegistrar));
    if (!SNTR)
        return NULL;
    SNTR->sceneNodeTypesList = (struct SceneNodeType*)malloc(sizeof(struct SceneNodeType) * SNTR_TYPES_REALLOCATION_STEP);
    if (!SNTR->sceneNodeTypesList) {
        SceneNodeTypesRegistrar_destruct(SNTR);
        return NULL;
    }
    SNTR->allocatedSceneNodeTypes = SNTR_TYPES_REALLOCATION_STEP;
    return SNTR;
}

void SceneNodeTypesRegistrar_destruct(struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar) {
    if (!sceneNodeTypesRegistrar)
        return;
    if (sceneNodeTypesRegistrar->sceneNodeTypesList) {
        for (size_t i = 0; i < sceneNodeTypesRegistrar->sceneNodeTypesCount; i++)
            free(sceneNodeTypesRegistrar->sceneNodeTypesList[i].type);
        free(sceneNodeTypesRegistrar->sceneNodeTypesList);
    }
    free(sceneNodeTypesRegistrar);
}

/**
 * @brief Reallocates memory for SceneNodeTypesRegistrar#sceneNodeTypesList, 
 * increases SceneNodeTypesRegistrar#allocatedSceneNodeTypes by SNTR_TYPES_REALLOCATION_STEP.
 * @param sceneNodeTypesRegistrar Pointer to a SceneNodeTypesRegistrar, where function will 
 * reallocate SceneNodeTypesRegistrar#sceneNodeTypesList. Can be NULL.
 * @return #SNTR_errors value.
 * @see #SNTR_errors
 */
static enum SNTR_errors SceneNodeTypesRegistrar_reallocateTypesList(struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar) {
    if (!sceneNodeTypesRegistrar)
        return SNTR_ERR_NULL_ARGUMENT;
    struct SceneNodeType* sceneNodeTypesList = NULL;
    
    size_t newSize = sceneNodeTypesRegistrar->allocatedSceneNodeTypes + SNTR_TYPES_REALLOCATION_STEP;
    sceneNodeTypesList = (struct SceneNodeType*)malloc(sizeof(struct SceneNodeType) * newSize);
    if (!sceneNodeTypesList)
        return SNTR_ERR_ALLOC_LIST;
    for (size_t i = 0; i < sceneNodeTypesRegistrar->sceneNodeTypesCount; i++)
        sceneNodeTypesList[i] = sceneNodeTypesRegistrar->sceneNodeTypesList[i];
    free(sceneNodeTypesRegistrar->sceneNodeTypesList);
    sceneNodeTypesRegistrar->sceneNodeTypesList = sceneNodeTypesList;
    sceneNodeTypesRegistrar->allocatedSceneNodeTypes = newSize;
    return SNTR_NO_ERRORS;
}

enum SNTR_errors SceneNodeTypesRegistrar_registerNewSceneNodeType(
                                            struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                            const char* const typeString,
                                            struct SceneNode* (*constructor)(
                                                struct ResourceManager* const resourceManager, 
                                                struct Renderer* const renderer,
                                                struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                struct TextParser* const textParser)) {
    if (!sceneNodeTypesRegistrar || !typeString || !constructor)
        return SNTR_ERR_NULL_ARGUMENT;
    for (size_t i = 0; i < sceneNodeTypesRegistrar->sceneNodeTypesCount; i++)
        if (strcmp(typeString, sceneNodeTypesRegistrar->sceneNodeTypesList[i].type) == 0)
            return SNTR_ERR_REGISTERED_BEFORE;
    if (sceneNodeTypesRegistrar->sceneNodeTypesCount >= sceneNodeTypesRegistrar->allocatedSceneNodeTypes)
        if (SceneNodeTypesRegistrar_reallocateTypesList(sceneNodeTypesRegistrar))
            return SNTR_ERR_ALLOC_LIST;
    char* tempString = (char*)malloc(sizeof(char) * (strlen(typeString) + 1));
    if (!tempString)
        return SNTR_ERR_ALLOC_TYPE_STRING;
    strcpy(tempString, typeString);
    sceneNodeTypesRegistrar->sceneNodeTypesList[sceneNodeTypesRegistrar->sceneNodeTypesCount].type = tempString;
    sceneNodeTypesRegistrar->sceneNodeTypesList[sceneNodeTypesRegistrar->sceneNodeTypesCount].construct = constructor;
    sceneNodeTypesRegistrar->sceneNodeTypesCount++;
    return SNTR_NO_ERRORS;
}

/**
 * @brief Checks #SceneNode inheritor type.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar where to find registered types. Can be NULL.
 * @param textParser Pointer to a #TextParser with type string, which will be checked. Can be NULL.
 * @param logger Pointer to a #Logger for logging purpose. Can be NULL.
 * @param foundIndex Pointer to a number, where this function will place index of found SceneNodeType. Can be NULL.
 * @param requiredTypeString String with wishful #SceneNode inheritor type. Can be NULL.
 * @param resId String with ID (path) to the #SceneNode inheritor resource. Can be NULL. 
 * Will be used only for logging purpose.
 * @return #SNTR_errors value.
 * @see #SNTR_errors
 */
static enum SNTR_errors SceneNodeTypesRegistrar_checkType(struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                          struct TextParser* textParser,
                                                          struct Logger* logger,
                                                          size_t* foundIndex,
                                                          const char* const requiredTypeString,
                                                          const char* const resId) {
    // Don't check logger, because it's ok.
    if (!sceneNodeTypesRegistrar || !textParser || !foundIndex || !resId)
        return SNTR_ERR_NULL_ARGUMENT;
    const char* typeString = TextParser_getString(textParser, TEXT_PARSER_TYPE_STRING, 0);
    if (!typeString) {
        Logger_log(logger, "%s Resource ID: %s", SNTR_errorMessages.errNoTypeString, resId);
        Logger_log(logger, "%s Resource ID: %s", SNTR_errorMessages.errConstructingNode, resId);
        return SNTR_ERR_NO_TYPE_STRING;
    }
    bool found = false;
    for (size_t i = 0; i < sceneNodeTypesRegistrar->sceneNodeTypesCount; i++)
        if (strcmp(typeString, sceneNodeTypesRegistrar->sceneNodeTypesList[i].type) == 0) {
            found = true;
            (*foundIndex) = i;
            break;
        }
    if (!found) {
        Logger_log(logger, "%s Type: %s", SNTR_errorMessages.errNoRegisteredType, typeString);
        Logger_log(logger, "%s Resource ID: %s", SNTR_errorMessages.errConstructingNode, resId);
        return SNTR_ERR_NO_REGISTERED_TYPE;
    }
    // We check 'requiredTypeString' only if it not NULL
    if (requiredTypeString) {
        const char* tempTypeString = TextParser_getString(textParser, TEXT_PARSER_TYPE_STRING, 0);
        if (strcmp(tempTypeString, requiredTypeString) != 0) {
            Logger_log(logger, "%s Type: %s", SNTR_errorMessages.errType, typeString);
            Logger_log(logger, "%s Resource ID: %s", SNTR_errorMessages.errConstructingNode, resId);
            return SNTR_ERR_TYPE;
        }
    }
    return SNTR_NO_ERRORS;
}

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
        TextResource_decreasePointersCounter(tempTextResource);
        return NULL;
    }
    size_t foundIndex;
    if (SceneNodeTypesRegistrar_checkType(sceneNodeTypesRegistrar,
                                          textParser, renderer->logger,
                                          &foundIndex,
                                          requiredTypeString,
                                          resId)) {
        TextResource_decreasePointersCounter(tempTextResource);
        TextParser_destruct(textParser);
        return NULL;
    }
    struct SceneNode* tempSceneNode = 
                    sceneNodeTypesRegistrar->sceneNodeTypesList[foundIndex].construct(resourceManager, renderer,
                                                                             sceneNodeTypesRegistrar, textParser);
    TextParser_destruct(textParser);
    if (!tempSceneNode) {
        TextResource_decreasePointersCounter(tempTextResource);
        Logger_log(renderer->logger, "%s Resource ID: %s", SNTR_errorMessages.errConstructingNode, resId);
        return NULL;
    }
    tempSceneNode->sceneNodeTextResource = tempTextResource;
    return tempSceneNode;
}
