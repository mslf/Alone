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
 * @file Item.c
 * @author mslf
 * @date 3 Dec 2016
 * @brief File containing implementation of #Item.
 */

#include <assert.h>
#include "standardUser/Item.h"

/**
 * @brief Error message strings for #Item.
 */
static const struct ItemSceneNode_errorMessages {
    const char* const errPickedGameEventRes;
    /**< Will be displayed when #TextParser have no ItemSceneNode_parserStrings#pickedEventRes. */
    const char* const errPickedGameEvent;
    /**< Will be displayed when constructing Item#pickedEvent failed. */
    const char* const errBodyRes;
    /**< Will be displayed when #TextParser have no ItemSceneNode_parserStrings#bodyRes. */
    const char* const errInventoryItemRes;
    /**< Will be displayed when #TextParser have no ItemSceneNode_parserStrings#inventoryItemRes. */
    const char* const warnCount;
    /**< Will be displayed when #TextParser have no ItemSceneNode_parserStrings#count. */
}ItemSceneNode_errorMessages = {
    "Item_tryGetSettingsFromTextParser: pickedEventResource string haven't found!",
    "Item_changePickedEventResource: constructing GameEvent failed!",
    "Item_loadBodyResource: bodyResource string haven't found!",
    "Item_tryGetSettingsFromTextParser: inventoryItemRes string haven't found!",
    "Item_tryGetSettingsFromTextParser: count string haven't found. Assuminng count = 1."};

/**
 * @brief Function for loading and constructing Item#body.
 * @param item Pointer to a #Item where to construct Item#body.
 * @param resourceManager Pointer to a #ResourceManager for loading #Body resource.
 * @param renderer Pointer to a #Renderer for constructing Item#body.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing Item#body.
 * @param textParser Pointer to a #TextParser, where function will get 
 * ItemSceneNode_parserStrings#bodyRes.
 * @return #ItemSceneNode_errors value.
 * @see #Item
 * @see #Body
 * @see #ResourceManager
 * @see #Renderer
 * @see #SceneNodeTypesRegistrar
 * @see #TextParser
 * @see #ItemSceneNode_parserStrings
 * @see #ItemSceneNode_errors
 */
static enum ItemSceneNode_errors Item_loadBodyResource(struct Item* item,
                                               struct ResourceManager* resourceManager,
                                               struct Renderer* renderer,
                                               struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                               struct TextParser* textParser) {
    assert(item);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    const char* const tempResId = TextParser_getString(textParser, ItemSceneNode_parserStrings.bodyRes, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, ItemSceneNode_errorMessages.errBodyRes);
        return ITEM_ERR_NO_BODY_RES;
    }
    item->body = (struct Body*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                renderer,
                                                                sceneNodeTypesRegistrar,
                                                                tempResId,
                                                                BodySceneNode_parserStrings.type);
    if (!item->body)
        return ITEM_ERR_CONSTRUCTING_BODY;
    return ITEM_NO_ERRORS;
}
    
/**
 * @brief Inits #Item from #TextParser.
 * @param item Pointer to a #Item to be initialized.
 * @param resourceManager Pointer to a #ResourceManager to load Item#body and Item#pickedEventResource.
 * @param renderer Pointer to a #Renderer to create Item#body.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar
 * for constructing Item#body.
 * @param textParser Pointer to a #TextParser with data strings for creating #Item.
 * @return #ItemSceneNode_errors value.
 * @see #ItemSceneNode_errors
 * @see #Sprite
 * @see #Item
 */
static enum ItemSceneNode_errors Item_tryGetSettingsFromTextParser(struct Item* item,
                                                                   struct ResourceManager* resourceManager,
                                                                   struct Renderer* renderer,
                                                                   struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                                   struct TextParser* textParser) {
    assert(item);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    const char* tempPickedEventString = TextParser_getString(textParser, ItemSceneNode_parserStrings.pickedEventRes, 0);
    Item_changePickedEventResource(item, resourceManager, tempPickedEventString);
    item->inventoryItemRes = TextParser_getString(textParser, ItemSceneNode_parserStrings.inventoryItemRes, 0);
    if (textParser->lastError) {
        Logger_log(renderer->logger, ItemSceneNode_errorMessages.errInventoryItemRes);
        return ITEM_ERR_NO_INVENTORY_ITEM;
    }
    item->count = (size_t)TextParser_addInt(textParser, ItemSceneNode_parserStrings.count, 0);
    if (textParser->lastError) {
        Logger_log(renderer->logger, ItemSceneNode_errorMessages.warnCount);
        item->count = 1;
    }
    enum ItemSceneNode_errors result = Item_loadBodyResource(item,
                                                             resourceManager,
                                                             renderer,
                                                             sceneNodeTypesRegistrar,
                                                             textParser);
    if (result)
        return result;
    return ITEM_NO_ERRORS;
}

struct SceneNode* Item_construct(struct ResourceManager* const resourceManager,
                                 struct Renderer* const renderer,
                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                 struct TextParser* const textParser) {
    if (!resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return NULL;
    struct Item* item = NULL;
    item = (struct Item*)calloc(1, sizeof(struct Item));
    if (!item)
        return NULL;
    SceneNode_initPhysical(&(item->physicalSceneNode));
    Logger_saveUsedFlagAndSetToFalse(renderer->logger);
    if (Item_tryGetSettingsFromTextParser(item, resourceManager, renderer, sceneNodeTypesRegistrar, textParser)) {
        if (renderer->logger->wasUsed)
            Logger_log(renderer->logger, "\tin file: %s", textParser->file);
        Logger_revertUsedFlag(renderer->logger);
        Item_destruct((struct SceneNode*)item);
        return NULL;
    }
    if (renderer->logger->wasUsed)
        Logger_log(renderer->logger, "\tin file: %s", textParser->file);
    Logger_revertUsedFlag(renderer->logger);
    item->physicalSceneNode.dynamicSceneNode.sceneNode.update = Item_update;
    item->physicalSceneNode.dynamicSceneNode.sceneNode.render = Item_render;
    item->physicalSceneNode.dynamicSceneNode.sceneNode.destruct = Item_destruct;
    item->physicalSceneNode.dynamicSceneNode.sceneNode.type = 
            (char*)malloc(sizeof(char) * (strlen(ItemSceneNode_parserStrings.type) + 1));
    if (!item->physicalSceneNode.dynamicSceneNode.sceneNode.type) {
        Item_destruct((struct SceneNode*)item);
        return NULL;
    }
    strcpy(item->physicalSceneNode.dynamicSceneNode.sceneNode.type, ItemSceneNode_parserStrings.type);
    item->isGeometryChanged = true;
    return (struct SceneNode*)item;
}

void Item_destruct(struct SceneNode* item) {
    if (!item)
        return;
    struct Item* tempItem = (struct Item*)item;
    if (tempItem->body)
        Body_destruct((struct SceneNode*)tempItem->body);
    TextResource_decreasePointersCounter(tempItem->pickedEventResource);
    if (tempItem->pickedEvent)
        tempItem->pickedEvent->isNeeded = false;
    TextResource_decreasePointersCounter(item->sceneNodeTextResource);
    if (item->type)
        free(item->type);
    free(item);
}

enum ItemSceneNode_errors Item_changePickedEventResource(struct Item* item,
                                                         struct ResourceManager* resourceManager,
                                                         const char* const pickedEventResId) {
    if (!item || !resourceManager || !pickedEventResId)
        return ITEM_ERR_NULL_ARGUMENT;
    struct TextResource* newGameEventTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                                     pickedEventResId, 0);
    if (!newGameEventTextResource)
        return ITEM_ERR_NO_PICKED_EVENT_RES;
    struct TextParser* textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                                         newGameEventTextResource);
    if (!textParser) {
        TextResource_decreasePointersCounter(newGameEventTextResource);
        return ITEM_ERR_CONSTRUCTIG_TEXT_PARSER;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)item);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, ItemSceneNode_errorMessages.errPickedGameEvent);
        TextResource_decreasePointersCounter(newGameEventTextResource);
        TextParser_destruct(textParser);
        return ITEM_ERR_CONSTRUCTING_PICKED_EVENT;
    }
    TextParser_destruct(textParser);
    if (item->pickedEvent)
        item->pickedEvent->isNeeded = false;
    TextResource_decreasePointersCounter(item->pickedEventResource);
    item->pickedEventResource= newGameEventTextResource;
    item->pickedEvent = newGameEvent;
    return ITEM_NO_ERRORS;
}

enum ItemSceneNode_errors Item_save(const struct Item* const item,
                                    struct ResourceManager* const resourceManager,
                                    const char* const itemResId){
    if (!item || !resourceManager || !itemResId)
        return ITEM_ERR_NULL_ARGUMENT;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return ITEM_ERR_CONSTRUCTIG_TEXT_PARSER;
    result += (TextParser_addString(textParser, TextParser_standartTypeString, ItemSceneNode_parserStrings.type) != 0);
    result += (TextParser_addString(textParser, ItemSceneNode_parserStrings.bodyRes,
                                   ((struct SceneNode*)(item->body))->sceneNodeTextResource->id) != 0);
    result += (TextParser_addString(textParser, ItemSceneNode_parserStrings.inventoryItemRes,
                                   item->inventoryItemRes) != 0);
    result += (TextParser_addString(textParser, ItemSceneNode_parserStrings.pickedEventRes,
                                   item->pickedEventResource->id) != 0);
    result += (TextParser_addInt(textParser, ItemSceneNode_parserStrings.count,
                                   item->count) != 0);
    char* tempString = TextParser_convertToText(textParser);
    result += (textParser->lastError != 0);
    result += (TextResource_updateContent(((struct SceneNode*)(item))->sceneNodeTextResource, tempString) != 0);
    result += (ResourceManager_saveTextResource(resourceManager,
                                               ((struct SceneNode*)(item))->sceneNodeTextResource, itemResId) != 0);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    if (result)
        return ITEM_ERR_SAVING;
    return ITEM_NO_ERRORS;
}

void Item_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager || !renderer)
        return;
    struct Item* item = (struct Item*)sceneNode;
    if (item->isGeometryChanged) {
        item->physicalSceneNode.logicalSize = item->body->logicalSize;
        struct SceneNode tempBodySceneNode = item->body->physicalSceneNode.dynamicSceneNode.sceneNode;
        item->body->physicalSceneNode = item->physicalSceneNode;
        item->body->physicalSceneNode.dynamicSceneNode.sceneNode = tempBodySceneNode;
        ((struct SceneNode*)(item->body))->coordinates = ((struct SceneNode*)(item))->coordinates;
        item->isGeometryChanged = false;
    }
    Body_update((struct SceneNode*)item->body, eventManager, renderer);
}

void Item_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct Item* item = (struct Item*)sceneNode;
    Body_render((struct SceneNode*)item->body, renderer);
}
