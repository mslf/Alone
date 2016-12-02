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
 * @file Body.c
 * @author mslf
 * @date 11 Aug 2016
 * @brief File containing implementation of #Body.
 */
#include <assert.h>
#include "body/Body.h"

/**
 * @brief Error message strings for #Body.
 */
static const struct BodySceneNode_errorMessages {
    const char *const errNoSpriteRes;
    /**< Will be displayed when #TextParser have no BodySceneNode_parserStrings#spriteRes. */
    const char *const warnNoLogicalSize;
    /**< Will be displayed when #TextParser have no BodySceneNode_parserStrings#logicalSize. */
}BodySceneNode_errorMessages = {
        "Body_loadSpriteResource: sprite resource string haven't found!",
        "Body_tryGetSettingsFromTextParser: logical size of body haven't found. Using present sprite size."};

/**
 * @brief Function for loading and constructing Body#sprite.
 * @param body Pointer to a #Body where to construct Body#sprite.
 * @param resourceManager Pointer to a #ResourceManager for loading #Sprite resource.
 * @param renderer Pointer to a #Renderer for constructing Body#sprite.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing Body#sprite.
 * @param textParser Pointer to a #TextParser, where function will get 
 * BodySceneNode_parserStrings#spriteRes.
 * @param usePresentForLogical If set, then Body#logicalSize will be set from constructed Sprite#virtualSize.
 * @return #BodySceneNode_errors value.
 * @see #Body
 * @see #Sprite
 * @see #ResourceManager
 * @see #Renderer
 * @see #SceneNodeTypesRegistrar
 * @see #TextParser
 * @see #BodySceneNode_parserStrings
 * @see #BodySceneNode_errors
 */
static enum BodySceneNode_errors Body_loadSpriteResource(struct Body* body,
                                                         struct ResourceManager* resourceManager,
                                                         struct Renderer* renderer,
                                                         struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                         struct TextParser* textParser,
                                                         bool usePresentForLogical) {
    assert(body);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    const char* const tempResId = TextParser_getString(textParser, BodySceneNode_parserStrings.spriteRes, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, BodySceneNode_errorMessages.errNoSpriteRes);
        return BODY_ERR_NO_SPRITE_RES;
    }
    body->sprite = (struct Sprite*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                                renderer,
                                                                                sceneNodeTypesRegistrar,
                                                                                tempResId,
                                                                                SpriteSceneNode_parserStrings.type);
    if (!body->sprite)
        return BODY_ERR_CONSTRUCTING_SPRITE;
    if (usePresentForLogical)
        body->logicalSize = body->sprite->virtualSize;
    return BODY_NO_ERRORS;
}

/**
 * @brief Inits #Body from #TextParser.
 * @param body Pointer to a #Body to be initialized.
 * @param resourceManager Pointer to a #ResourceManager to load Body#sprite.
 * @param renderer Pointer to a #Renderer to create Body#sprite.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar
 * for constructing Body#sprite.
 * @param textParser Pointer to a #TextParser with data strings for creating #Body.
 * @return #BodySceneNode_errors value.
 * @see #BodySceneNode_errors
 * @see #Sprite
 * @see #Body
 */
static enum BodySceneNode_errors Body_tryGetSettingsFromTextParser(struct Body* body,
                                                                   struct ResourceManager* resourceManager,
                                                                   struct Renderer* renderer,
                                                                   struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                                   struct TextParser* textParser) {
    assert(body);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    bool usePresentForLogical = false;
    body->logicalSize.x = (int)TextParser_getInt(textParser, BodySceneNode_parserStrings.logicalSize, 0);
    body->logicalSize.y = (int)TextParser_getInt(textParser, BodySceneNode_parserStrings.logicalSize, 1);
    if (body->logicalSize.x <= 0 || body->logicalSize.y <= 0) {
        Logger_log(resourceManager->logger, BodySceneNode_errorMessages.warnNoLogicalSize);
        usePresentForLogical = true;
    }
    enum BodySceneNode_errors result = Body_loadSpriteResource(body, resourceManager,
                                                               renderer, sceneNodeTypesRegistrar,
                                                               textParser, usePresentForLogical);
    if (result)
        return result;
    return BODY_NO_ERRORS;
}

struct SceneNode* Body_construct(struct ResourceManager* const resourceManager,
                                 struct Renderer* const renderer,
                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                 struct TextParser* const textParser) {
    if (!resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return NULL;
    struct Body* body = NULL;
    body = (struct Body*)calloc(1, sizeof(struct Body));
    if (!body)
        return NULL;
    SceneNode_initPhysical(&(body->physicalSceneNode));
    if (Body_tryGetSettingsFromTextParser(body, resourceManager, renderer, sceneNodeTypesRegistrar, textParser)) {
        Body_destruct((struct SceneNode*)body);
        return NULL;
    }
    body->physicalSceneNode.dynamicSceneNode.sceneNode.update = Body_update;
    body->physicalSceneNode.dynamicSceneNode.sceneNode.render = Body_render;
    body->physicalSceneNode.dynamicSceneNode.sceneNode.destruct = Body_destruct;
    body->physicalSceneNode.dynamicSceneNode.sceneNode.type = 
            (char*)malloc(sizeof(char) * (strlen(BodySceneNode_parserStrings.type) + 1));
    if (!body->physicalSceneNode.dynamicSceneNode.sceneNode.type) {
        Body_destruct((struct SceneNode*)body);
        return NULL;
    }
    strcpy(body->physicalSceneNode.dynamicSceneNode.sceneNode.type, BodySceneNode_parserStrings.type);
    body->isGeometryChanged = true;
    return (struct SceneNode*)body;
}

void Body_destruct(struct SceneNode* body){
    if (!body)
        return;
    struct Body* tempBody = (struct Body*)body;
    if (tempBody->sprite)
        Sprite_destruct((struct SceneNode*)tempBody->sprite);
    TextResource_decreasePointersCounter(body->sceneNodeTextResource);
    if (body->type)
        free(body->type);
    free(body);
}

enum BodySceneNode_errors Body_save(const struct Body* const body,
                                    struct ResourceManager* const resourceManager,
                                    const char* const bodyResId) {
    if (!body || !resourceManager || !bodyResId)
        return BODY_ERR_NULL_ARGUMENT;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return BODY_ERR_CONSTRUCTIG_TEXT_PARSER;
    result += (TextParser_addString(textParser, TextParser_standartTypeString, BodySceneNode_parserStrings.type) != 0);
    result += (TextParser_addString(textParser, BodySceneNode_parserStrings.spriteRes,
                                    body->sprite->dynamicSceneNode.sceneNode.sceneNodeTextResource->id) != 0);
    result += (TextParser_addInt(textParser, BodySceneNode_parserStrings.logicalSize,
                                 body->logicalSize.x) != 0);
    result += (TextParser_addInt(textParser, BodySceneNode_parserStrings.logicalSize,
                                 body->logicalSize.y) != 0);
    char* tempString = TextParser_convertToText(textParser);
    result += (textParser->lastError != 0);
    result += (TextResource_updateContent(body->physicalSceneNode.dynamicSceneNode.sceneNode.sceneNodeTextResource,
                                          tempString) != 0);
    result += (ResourceManager_saveTextResource(resourceManager,
                                                body->physicalSceneNode.dynamicSceneNode.sceneNode.sceneNodeTextResource,
                                                bodyResId) != 0);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    if (result)
        return BODY_ERR_SAVING;
    return BODY_NO_ERRORS;
}

void Body_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager || !renderer)
        return;
    struct Body* body = (struct Body*)sceneNode;
    if (body->isGeometryChanged) {
        body->physicalSceneNode.logicalSize = body->logicalSize;
        struct SceneNode tempSpriteSceneNode = body->sprite->dynamicSceneNode.sceneNode;
        body->sprite->dynamicSceneNode = body->physicalSceneNode.dynamicSceneNode;
        body->sprite->dynamicSceneNode.sceneNode = tempSpriteSceneNode;
        body->sprite->dynamicSceneNode.sceneNode.coordinates = body->physicalSceneNode.dynamicSceneNode.sceneNode.coordinates;
        body->isGeometryChanged = false;
    }
    Sprite_update((struct SceneNode*)body->sprite, eventManager, renderer);
}

void Body_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct Body* body = (struct Body*)sceneNode;
    Sprite_render((struct SceneNode*)body->sprite, renderer);
}