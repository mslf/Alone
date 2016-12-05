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
 * @file StandardUser.c
 * @author mslf
 * @date 28 Sep 2016
 * @brief File containing implementation of #StandardUser.
 */
#include <assert.h>
#include "standardUser/StandardUser.h"

/**
 * @brief Error message strings for #StandardUser.
 */
static const struct StandardUserSceneNode_errorMessages {
    const char* const errBodyRes;
    /**< Will be displayed when #TextParser have no StandardUserSceneNode_parserStrings#bodyRes. */
}StandardUserSceneNode_errorMessages = {
    "StandardUser_loadBodyResource: bodyResource string haven't found!"};

/**
 * @brief Function for loading and constructing StandardUser#body.
 * @param standardUser Pointer to a #StandardUser where to construct StandardUser#body.
 * @param resourceManager Pointer to a #ResourceManager for loading #Body resource.
 * @param renderer Pointer to a #Renderer for constructing StandardUser#body.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing StandardUser#body.
 * @param textParser Pointer to a #TextParser, where function will get 
 * StandardUserSceneNode_parserStrings#bodyRes.
 * @return #StandardUserSceneNode_errors value.
 * @see #StandardUser
 * @see #Body
 * @see #ResourceManager
 * @see #Renderer
 * @see #SceneNodeTypesRegistrar
 * @see #TextParser
 * @see #StandardUserSceneNode_parserStrings
 * @see #StandardUserSceneNode_errors
 */
static enum StandardUserSceneNode_errors StandardUser_loadBodyResource(struct StandardUser* standardUser,
                                               struct ResourceManager* resourceManager,
                                               struct Renderer* renderer,
                                               struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                               struct TextParser* textParser) {
    assert(standardUser);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    const char* const tempResId = TextParser_getString(textParser, StandardUserSceneNode_parserStrings.bodyRes, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, StandardUserSceneNode_errorMessages.errBodyRes);
        return STD_USER_ERR_NO_BODY_RES;
    }
    standardUser->body = (struct Body*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                renderer,
                                                                sceneNodeTypesRegistrar,
                                                                tempResId,
                                                                BodySceneNode_parserStrings.type);
    if (!standardUser->body)
        return STD_USER_ERR_CONSTRUCTING_BODY;
    return STD_USER_NO_ERRORS;
}

/**
 * @brief Function for initializing StandardUser#microSection from #TextParser.
 * @param standardUser Pointer to a #StandardUser where to add #NanoSection.
 * @param resourceManager Pointer to a #ResourceManager for loading #NanoSection resources.
 * @param textParser Pointer to a #TextParser, where function will get 
 * StandardUserSceneNode_parserStrings#nanoSections.
 * @see #StandardUser
 * @see #MacroSection
 * @see #NanoSection
 * @see #ResourceManager
 * @see #TextParser
 * @see #StandardUserSceneNode_parserStrings
 */
static void StandardUser_loadNanoSections(struct StandardUser* standardUser,
                                       struct ResourceManager* resourceManager,
                                       struct TextParser* textParser){
    assert(standardUser);
    assert(resourceManager);
    assert(textParser);
    // TODO
    /*
    const char* const tempPressedEventResourceString = TextParser_getString(textParser,
                                                          StandardUserSceneNode_parserStrings.pressedEventRes, 0);
    if (!tempPressedEventResourceString)
        Logger_log(resourceManager->logger, StandardUserSceneNode_errorMessages.errPressedGameEventRes);
    StandardUser_changeFocusedEventResource(standardUser, resourceManager, tempFocusedEventResourceString);
    StandardUser_changePressedEventResource(standardUser, resourceManager, tempPressedEventResourceString);*/
}

/**
 * @brief Function for initializing StandardUser#soundResourcesList from #TextParser.
 * @param standardUser Pointer to a #StandardUser where to construct sounds.
 * @param resourceManager Pointer to a #ResourceManager for loading #SoundResource resources.
 * @param textParser Pointer to a #TextParser, where function will get 
 * StandardUserSceneNode_parserStrings#soundsList.
 * @see #StandardUser
 * @see #SoundResource
 * @see #ResourceManager
 * @see #TextParser
 * @see #StandardUserSceneNode_parserStrings
 */
static void StandardUser_loadSoundResources(struct StandardUser* standardUser,
                                       struct ResourceManager* resourceManager,
                                       struct TextParser* textParser){
    assert(standardUser);
    assert(resourceManager);
    assert(textParser);
    // TODO
    /*
    const char* const tempPressedEventResourceString = TextParser_getString(textParser,
                                                          StandardUserSceneNode_parserStrings.pressedEventRes, 0);
    if (!tempPressedEventResourceString)
        Logger_log(resourceManager->logger, StandardUserSceneNode_errorMessages.errPressedGameEventRes);
    StandardUser_changeFocusedEventResource(standardUser, resourceManager, tempFocusedEventResourceString);
    StandardUser_changePressedEventResource(standardUser, resourceManager, tempPressedEventResourceString);*/
}

/**
 * @brief Function for initializing StandardUser#inventoryItemsList from #TextParser.
 * @param standardUser Pointer to a #StandardUser where to construct events.
 * @param resourceManager Pointer to a #ResourceManager for loading #InventoryItem resources.
 * @param textParser Pointer to a #TextParser, where function will get 
 * StandardUserSceneNode_parserStrings#inventoryItemsList.
 * @see #StandardUser
 * @see #GameEvent
 * @see #ResourceManager
 * @see #TextParser
 * @see #StandardUserSceneNode_parserStrings
 */
static void StandardUser_loadInventoryItems(struct StandardUser* standardUser,
                                       struct ResourceManager* resourceManager,
                                       struct TextParser* textParser){
    assert(standardUser);
    assert(resourceManager);
    assert(textParser);
    // TODO
    /*
    const char* const tempPressedEventResourceString = TextParser_getString(textParser,
                                                          StandardUserSceneNode_parserStrings.pressedEventRes, 0);
    if (!tempPressedEventResourceString)
        Logger_log(resourceManager->logger, StandardUserSceneNode_errorMessages.errPressedGameEventRes);
    StandardUser_changeFocusedEventResource(standardUser, resourceManager, tempFocusedEventResourceString);
    StandardUser_changePressedEventResource(standardUser, resourceManager, tempPressedEventResourceString);*/
}

/**
 * @brief Function for initializing StandardUser#triggeredEventsList from #TextParser.
 * @param standardUser Pointer to a #StandardUser where to construct events.
 * @param resourceManager Pointer to a #ResourceManager for loading #GameEvent resources.
 * @param textParser Pointer to a #TextParser, where function will get 
 * StandardUserSceneNode_parserStrings#triggeredEventsList
 * @see #StandardUser
 * @see #GameEvent
 * @see #ResourceManager
 * @see #TextParser
 * @see #StandardUserSceneNode_parserStrings
 */
static void StandardUser_loadTriggeredEvents(struct StandardUser* standardUser,
                                       struct ResourceManager* resourceManager,
                                       struct TextParser* textParser){
    assert(standardUser);
    assert(resourceManager);
    assert(textParser);
    // TODO
    /*
    const char* const tempPressedEventResourceString = TextParser_getString(textParser,
                                                          StandardUserSceneNode_parserStrings.pressedEventRes, 0);
    if (!tempPressedEventResourceString)
        Logger_log(resourceManager->logger, StandardUserSceneNode_errorMessages.errPressedGameEventRes);
    StandardUser_changeFocusedEventResource(standardUser, resourceManager, tempFocusedEventResourceString);
    StandardUser_changePressedEventResource(standardUser, resourceManager, tempPressedEventResourceString);*/
}

/**
 * @brief Function for loading settings and initializing #StandardUser from #TextParser.
 * @param standardUser Pointer to a #StandardUser which will be initialized.
 * @param resourceManager Pointer to a #ResourceManager for loading required resources.
 * @param renderer Pointer to a #Renderer for constructing StandardUser#body and and loading
 * other resources (sounds, events, inventory items).
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing StandardUser#body and and loading
 * other resources (sounds, events, inventory items).
 * @param textParser Pointer to a #TextParser with data strings.
 * @return #StandardUserSceneNode_errors value.
 * @see #StandardUser
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #StandardUserSceneNode_parserStrings
 * @see #StandardUserSceneNode_errors
 */
static enum StandardUserSceneNode_errors StandardUser_tryGetSettingsFromTextParser(struct StandardUser* standardUser,
                                                         struct ResourceManager* resourceManager,
                                                         struct Renderer* renderer,
                                                         struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                         struct TextParser* textParser) {
    assert(standardUser);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    enum StandardUserSceneNode_errors loadingBodyResult = STD_USER_NO_ERRORS;
    loadingBodyResult = StandardUser_loadBodyResource(standardUser, resourceManager, renderer,
                                                    sceneNodeTypesRegistrar, textParser);
    if (loadingBodyResult)
        return loadingBodyResult;
    StandardUser_loadInventoryItems(standardUser, resourceManager, textParser);
    StandardUser_loadTriggeredEvents(standardUser, resourceManager, textParser);
    StandardUser_loadSoundResources(standardUser, resourceManager, textParser);
    StandardUser_loadNanoSections(standardUser, resourceManager, textParser);
    return STD_USER_NO_ERRORS;
}

struct SceneNode* StandardUser_construct(struct ResourceManager* const resourceManager,
                                         struct Renderer* const renderer,
                                         struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                         struct TextParser* const textParser) {
    if (!resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return NULL;
    struct StandardUser* standardUser = NULL;
    standardUser = (struct StandardUser*)calloc(1, sizeof(struct StandardUser));
    if (!standardUser)
        return NULL;
    SceneNode_initPhysical(&(standardUser->physicalSceneNode));
    Logger_saveUsedFlagAndSetToFalse(renderer->logger);
    if (StandardUser_tryGetSettingsFromTextParser(standardUser, resourceManager, renderer, sceneNodeTypesRegistrar, textParser)) {
        if (renderer->logger->wasUsed)
            Logger_log(renderer->logger, "\tin file: %s", textParser->file);
        Logger_revertUsedFlag(renderer->logger);
        StandardUser_destruct((struct SceneNode*)standardUser);
        return NULL;
    }
    if (renderer->logger->wasUsed)
        Logger_log(renderer->logger, "\tin file: %s", textParser->file);
    Logger_revertUsedFlag(renderer->logger);
    ((struct SceneNode*)standardUser)->control = StandardUser_control;
    ((struct SceneNode*)standardUser)->update = StandardUser_update;
    ((struct SceneNode*)standardUser)->render = StandardUser_render;
    ((struct SceneNode*)standardUser)->sound = StandardUser_sound;
    ((struct SceneNode*)standardUser)->destruct = StandardUser_destruct;
    ((struct SceneNode*)standardUser)->type = (char*)malloc(sizeof(char) * (strlen(StandardUserSceneNode_parserStrings.type) + 1));
    if (!((struct SceneNode*)standardUser)->type) {
        StandardUser_destruct((struct SceneNode*)standardUser);
        return NULL;
    }
    strcpy(((struct SceneNode*)standardUser)->type, StandardUserSceneNode_parserStrings.type);
    standardUser->isGeometryChanged = true;
    return (struct SceneNode*)standardUser;

}

void StandardUser_destruct(struct SceneNode* standardUser) {
    if (!standardUser)
        return;
    struct StandardUser* tempStdUser = (struct StandardUser*)standardUser;
    if (tempStdUser->body)
        Body_destruct((struct SceneNode*)tempStdUser->body);
    // TODO: Here will be destructing of other internal things of #StandardUser.
    /*TextResource_decreasePointersCounter(tempItem->pickedEventResource);
    if (tempItem->pickedEvent)
        tempItem->pickedEvent->isNeeded = false;*/
    TextResource_decreasePointersCounter(standardUser->sceneNodeTextResource);
    if (standardUser->type)
        free(standardUser->type);
    free(standardUser);
}

enum StandardUserSceneNode_errors StandardUser_save(const struct StandardUser* const standardUser,
                                                    struct ResourceManager* const resourceManager,
                                                    const char* const userResId) {
    // TODO: Saving StandardUser.
}

void StandardUser_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {
    // TODO: Checking keyboard scancode and spawn apropriate event from StandardUser#triggeredEventsList.
}

void StandardUser_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager || !renderer)
        return;
    struct StandardUser* standardUser = (struct StandardUser*)sceneNode;
    if (standardUser->isGeometryChanged) {
        standardUser->physicalSceneNode.logicalSize = standardUser->body->logicalSize;
        struct SceneNode tempBodySceneNode = standardUser->body->physicalSceneNode.dynamicSceneNode.sceneNode;
        standardUser->body->physicalSceneNode = standardUser->physicalSceneNode;
        standardUser->body->physicalSceneNode.dynamicSceneNode.sceneNode = tempBodySceneNode;
        ((struct SceneNode*)(standardUser->body))->coordinates = ((struct SceneNode*)(standardUser))->coordinates;
        standardUser->isGeometryChanged = false;
    }
    Body_update((struct SceneNode*)standardUser->body, eventManager, renderer);
}

void StandardUser_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct StandardUser* standardUser = (struct StandardUser*)sceneNode;
    Body_render((struct SceneNode*)standardUser->body, renderer);
}

void StandardUser_sound(struct SceneNode* sceneNode, struct Musican* musican){
    // TODO: Play needed sound from StandardUser#soundResourcesList.
}
