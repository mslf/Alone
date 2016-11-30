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
 * @file CheckBox.c
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing implementation of #CheckBox.
 */
#include <assert.h>
#include "gui/CheckBox.h"
#include "textParser/TextParser.h"

/**
 * @brief Error message strings for #CheckBox.
 */
static const struct CheckBoxSceneNode_errorMessages {
    const char* const errFocusedGameEventRes;
    /**< Will be displayed when #TextParser have no CheckBoxSceneNode_parserStrings#focusedEventRes. */
    const char* const errCheckedGameEventRes;
    /**< Will be displayed when #TextParser have no CheckBoxSceneNode_parserStrings#checkhedEventRes. */
    const char* const errUnCheckedGameEventRes;
    /**< Will be displayed when #TextParser have no CheckBoxSceneNode_parserStrings#unCheckedEventRes. */
    const char* const errConstructingFocusedGameEvent;
    /**< Will be displayed when constructing CheckBox#focusedEvent failed. */
    const char* const errConstructingCheckedGameEvent;
    /**< Will be displayed when constructing CheckBox#checkedEvent failed. */
    const char* const errConstructingUnCheckedGameEvent;
    /**< Will be displayed when constructing CheckBox#unCheckedEvent failed. */
    const char* const errFocusedSoundRes;
    /**< Will be displayed when #TextParser have no CheckBoxSceneNode_parserStrings#focusedSoundRes. */
    const char* const errCheckedSoundRes;
    /**< Will be displayed when #TextParser have no CheckBoxSceneNode_parserStrings#checkedSoundRes. */
    const char* const errUnCheckedSoundRes;
    /**< Will be displayed when #TextParser have no CheckBoxSceneNode_parserStrings#unCheckedSoundRes. */
    const char* const errSpriteRes;
    /**< Will be displayed when #TextParser have no CheckBoxSceneNode_parserStrings#spriteRes. */
    const char* const errSpriteNoEnoughAnimations;
    /**< Will be displayed when constructed #Sprite doesn't have enough animations for #CheckBox. */
}CheckBoxSceneNode_errorMessages = {
    "CheckBox_loadEventsResources: focusedEventResource string haven't found!",
    "CheckBox_loadEventsResources: checkedEventResource string haven't found!",
    "CheckBox_loadEventsResources: unCheckedEventResource string haven't found!",
    "CheckBox_changeFocusedEventResource: constructing GameEvent failed!",
    "CheckBox_changeCheckedEventResource: constructing GameEvent failed!",
    "CheckBox_changeUnCheckedEventResource: constructing GameEvent failed!",
    "CheckBox_loadSoundResources: focusedSoundResource string haven't found!",
    "CheckBox_loadSoundResources: checkedSoundResource string haven't found!",
    "CheckBox_loadSoundResources: unCheckedSoundResource string haven't found!",
    "CheckBox_loadSpriteResource: spriteResource string haven't found!",
    "CheckBox_loadSpriteResource: loaded Sprite doesn't contain 4 animations!"};

    
/**
 * @brief Function for loading and constructing CheckBox#sprite.
 * @param checkBox Pointer to a #CheckBox where to construct CheckBox#sprite.
 * @param resourceManager Pointer to a #ResourceManager for loading #Sprite resource.
 * @param renderer Pointer to a #Renderer for constructing CheckBox#sprite.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing CheckBox#sprite.
 * @param textParser Pointer to a #TextParser, where function will get 
 * CheckBoxSceneNode_parserStrings#spriteRes.
 * @return #CheckBoxSceneNode_errors value.
 * @see #CheckBox
 * @see #Sprite
 * @see #ResourceManager
 * @see #Renderer
 * @see #SceneNodeTypesRegistrar
 * @see #TextParser
 * @see #CheckBoxSceneNode_parserStrings
 * @see #CheckBoxSceneNode_errors
 */
static enum CheckBoxSceneNode_errors CheckBox_loadSpriteResource(struct CheckBox* checkBox,
                                                 struct ResourceManager* const resourceManager,
                                                 struct Renderer* const renderer,
                                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                 struct TextParser* const textParser) {
    assert(checkBox);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    const char* tempResId = TextParser_getString(textParser, CheckBoxSceneNode_parserString.spriteRes, 0);
    if (!tempResId) {
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errSpriteRes);
        return CHECK_BOX_ERR_NO_SPRITE_RES;
    }
    checkBox->sprite = (struct Sprite*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                                  renderer,
                                                                                  sceneNodeTypesRegistrar,
                                                                                  tempResId,
                                                                                  SpriteSceneNode_parserStrings.type);
    if (!checkBox->sprite)
        return CHECK_BOX_ERR_CONSTRUCTIG_SPRITE;
    if (checkBox->sprite->animationsCount < 4) {
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errSpriteNoEnoughAnimations);
        return CHECK_BOX_ERR_NOT_ENOUGH_ANIMATIONS;
    }
    return CHECK_BOX_NO_ERRORS;
}

/**
 * @brief Function for loading and constructing CheckBox#focusedSoundResource,
 * CheckBox#checkedSoundResource and CheckBox#unCheckedSoundResource.
 * @param checkBox Pointer to a #CheckBox where to construct sound resources.
 * @param resourceManager Pointer to a #ResourceManager for loading sound resources.
 * @param textParser Pointer to a #TextParser, where function will get 
 * CheckBoxSceneNode_parserStrings#focusedSoundRes, 
 * CheckBoxSceneNode_parserStrings#checkedSoundRes and 
 * CheckBoxSceneNode_parserStrings#unCheckedSoundRes.
 * @see #CheckBox
 * @see #SoundResource
 * @see #ResourceManager
 * @see #TextParser
 * @see #CheckBoxSceneNode_parserStrings
 */
static void CheckBox_loadSoundResources(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                               struct TextParser* textParser) {
    assert(checkBox);
    assert(resourceManager);
    assert(textParser);
    const char* tempFocusedSoundResourceString = TextParser_getString(textParser,
                                                        CheckBoxSceneNode_parserString.focusedSoundRes, 0);
    if (!tempFocusedSoundResourceString)
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errFocusedSoundRes);
    checkBox->focusedSoundResource = ResourceManager_loadSoundResource(resourceManager,
                                                                       tempFocusedSoundResourceString);
    const char* tempCheckedSoundResourceString = TextParser_getString(textParser,
                                                        CheckBoxSceneNode_parserString.checkedSoundRes, 0);
    if (!tempCheckedSoundResourceString)
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errCheckedSoundRes);
    checkBox->checkedSoundResource = ResourceManager_loadSoundResource(resourceManager,
                                                                       tempCheckedSoundResourceString);
    const char* tempUnCheckedSoundResourceString = TextParser_getString(textParser,
                                                        CheckBoxSceneNode_parserString.unCheckedSoundRes, 0);
    if (!tempUnCheckedSoundResourceString)
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errUnCheckedSoundRes);
    checkBox->unCheckedSoundResource = ResourceManager_loadSoundResource(resourceManager,
                                                                         tempUnCheckedSoundResourceString);
}

/**
 * @brief Function for loading and constructing CheckBox#focusedEvent, 
 * CheckBox#checkedEvent and CheckBox#unChecnedEvent.
 * Also, it will update CheckBox#focusedEventResource, CheckBox#checkedEventResource 
 * and CheckBox#unCheckedEventResource.
 * @param checkBox Pointer to a #CheckBox where to construct events.
 * @param resourceManager Pointer to a #ResourceManager for loading #GameEvent resources.
 * @param textParser Pointer to a #TextParser, where function will get 
 * CheckBoxSceneNode_parserStrings#focusedEventRes, 
 * CheckBoxSceneNode_parserStrings#checkedEventRes 
 * and CheckBoxSceneNode_parserStrings#unCheckedEventRes.
 * @see #CheckBox
 * @see #GameEvent
 * @see #ResourceManager
 * @see #TextParser
 * @see #CheckBoxSceneNode_parserStrings
 */
static void CheckBox_loadEventsResources(struct CheckBox* checkBox, struct ResourceManager* resourceManager, 
                                struct TextParser* textParser){
    assert(checkBox);
    assert(resourceManager);
    assert(textParser);
    const char* tempFocusedEventResourceString = TextParser_getString(textParser,
                                                          CheckBoxSceneNode_parserString.focusedEventRes, 0);
    if (!tempFocusedEventResourceString)
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errFocusedGameEventRes);
    const char* tempCheckedEventResourceString = TextParser_getString(textParser,
                                                          CheckBoxSceneNode_parserString.checkedEventRes, 0);
    if (!tempCheckedEventResourceString)
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errCheckedGameEventRes);
    const char* tempUnCheckedEventResourceString = TextParser_getString(textParser,
                                                          CheckBoxSceneNode_parserString.unCheckedEventRes, 0);
    if (!tempUnCheckedEventResourceString)
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errUnCheckedGameEventRes);
    CheckBox_changeFocusedEventResource(checkBox, resourceManager, tempFocusedEventResourceString);
    CheckBox_changeCheckedEventResource(checkBox, resourceManager, tempCheckedEventResourceString);
    CheckBox_changeCheckedEventResource(checkBox, resourceManager, tempUnCheckedEventResourceString);
}

/**
 * @brief Function for loading settings and initializing #CheckBox from #TextParser.
 * @param checkBox Pointer to a #CheckBox which will be initialized.
 * @param resourceManager Pointer to a #ResourceManager for loading required resources.
 * @param renderer Pointer to a #Renderer for constructing CheckBox#sprite.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing CheckBox#sprite.
 * @param textParser Pointer to a #TextParser with data strings.
 * @return #CheckBoxSceneNode_errors value.
 * @see #CheckBox
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #CheckBoxSceneNode_parserStrings
 * @see #CheckBoxSceneNode_errors
 */
static enum CheckBoxSceneNode_errors CheckBox_tryGetSettingsFromTextParser(struct CheckBox* checkBox,
                                                           struct ResourceManager* const resourceManager,
                                                           struct Renderer* const renderer,
                                                           struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                           struct TextParser* const textParser) {
    assert(checkBox);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    CheckBox_loadEventsResources(checkBox, resourceManager, textParser);
    CheckBox_loadSoundResources(checkBox, resourceManager, textParser);
    return CheckBox_loadSpriteResource(checkBox, resourceManager, renderer,
                                          sceneNodeTypesRegistrar, textParser);
}
        
struct SceneNode* CheckBox_construct(struct ResourceManager* const resourceManager,
                                     struct Renderer* const renderer,
                                     struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                     struct TextParser* const textParser) {
    if (!resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return NULL;
    struct CheckBox* checkBox = NULL;
    checkBox = (struct CheckBox*)calloc(1, sizeof(struct CheckBox));
    if (!checkBox)
        return NULL;
    SceneNode_init(&(checkBox->sceneNode));
    if (CheckBox_tryGetSettingsFromTextParser(checkBox, resourceManager, renderer,
                                                sceneNodeTypesRegistrar, textParser)) {
        CheckBox_destruct((struct SceneNode*)checkBox);
        return NULL;
    }
    checkBox->sceneNode.control = CheckBox_control;
    checkBox->sceneNode.update = CheckBox_update;
    checkBox->sceneNode.render = CheckBox_render;
    checkBox->sceneNode.sound = CheckBox_sound;
    checkBox->sceneNode.destruct = CheckBox_destruct;
    checkBox->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(CheckBoxSceneNode_parserString.type) + 1));
    if (!checkBox->sceneNode.type) {
        CheckBox_destruct((struct SceneNode*)checkBox);
        return NULL;
    }
    strcpy(checkBox->sceneNode.type, CheckBoxSceneNode_parserString.type);
    checkBox->isGeometryChanged = true;
    return (struct SceneNode*)checkBox;
}

void CheckBox_destruct(struct SceneNode* checkBox) {
    if (!checkBox)
        return;
    struct CheckBox* tempCheckBox = (struct CheckBox*)checkBox;
    if (tempCheckBox->sprite)
        Sprite_destruct((struct SceneNode*)tempCheckBox->sprite);
    TextResource_decreasePointersCounter(tempCheckBox->focusedEventResource);
    SoundResource_decreasePointersCounter(tempCheckBox->focusedSoundResource);
    if (tempCheckBox->focusedEvent)
        tempCheckBox->focusedEvent->isNeeded = false;
    TextResource_decreasePointersCounter(tempCheckBox->checkedEventResource);
    SoundResource_decreasePointersCounter(tempCheckBox->checkedSoundResource);
    if (tempCheckBox->checkedEvent)
        tempCheckBox->checkedEvent->isNeeded = false;
    TextResource_decreasePointersCounter(tempCheckBox->unCheckedEventResource);
    SoundResource_decreasePointersCounter(tempCheckBox->unCheckedSoundResource);
    if (tempCheckBox->unCheckedEvent)
        tempCheckBox->unCheckedEvent->isNeeded = false;
    TextResource_decreasePointersCounter(checkBox->sceneNodeTextResource);
    if (checkBox->type)
        free(checkBox->type);
    free(checkBox);
}

enum CheckBoxSceneNode_errors CheckBox_changeFocusedEventResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                                                const char* const focusedEventResId) {
    if (!checkBox || !resourceManager || !focusedEventResId)
        return CHECK_BOX_ERR_NULL_ARGUMENT;
    struct TextResource* newGameEventTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                                     focusedEventResId, 0);
    if (!newGameEventTextResource)
        return CHECK_BOX_ERR_NO_FOCUSED_EVENT_RES;
    struct TextParser* textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                                         newGameEventTextResource);
    if (!textParser) {
        TextResource_decreasePointersCounter(newGameEventTextResource);
        return CHECK_BOX_ERR_CONSTRUCTIG_TEXT_PARSER;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)checkBox);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errFocusedGameEventRes);
        TextResource_decreasePointersCounter(newGameEventTextResource);
        TextParser_destruct(textParser);
        return CHECK_BOX_ERR_CONSTRUCTIG_FOCUSED_EVENT;
    }
    if (checkBox->focusedEvent)
        checkBox->focusedEvent->isNeeded = false;
    TextResource_decreasePointersCounter(checkBox->focusedEventResource);
    checkBox->focusedEventResource = newGameEventTextResource;
    checkBox->focusedEvent = newGameEvent;
    return CHECK_BOX_NO_ERRORS;
}

enum CheckBoxSceneNode_errors CheckBox_changeCheckedEventResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                                                const char* const checkedEventResId) {
    if (!checkBox || !resourceManager || !checkedEventResId)
        return CHECK_BOX_ERR_NULL_ARGUMENT;
    struct TextResource* newGameEventTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                                     checkedEventResId, 0);
    if (!newGameEventTextResource)
        return CHECK_BOX_ERR_NO_CHECKED_EVENT_RES;
    struct TextParser* textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                                         newGameEventTextResource);
    if (!textParser) {
        TextResource_decreasePointersCounter(newGameEventTextResource);
        return CHECK_BOX_ERR_CONSTRUCTIG_TEXT_PARSER;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)checkBox);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errCheckedGameEventRes);
        TextResource_decreasePointersCounter(newGameEventTextResource);
        TextParser_destruct(textParser);
        return CHECK_BOX_ERR_CONSTRUCTIG_CHECKED_EVENT;
    }
    if (checkBox->checkedEvent)
        checkBox->checkedEvent->isNeeded = false;
    TextResource_decreasePointersCounter(checkBox->checkedEventResource);
    checkBox->checkedEventResource = newGameEventTextResource;
    checkBox->checkedEvent = newGameEvent;
    return CHECK_BOX_NO_ERRORS;
}

enum CheckBoxSceneNode_errors CheckBox_changeUnCheckedEventResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                                                const char* const unCheckedEventResId) {
    if (!checkBox || !resourceManager || !unCheckedEventResId)
        return CHECK_BOX_ERR_NULL_ARGUMENT;
    struct TextResource* newGameEventTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                                     unCheckedEventResId, 0);
    if (!newGameEventTextResource)
        return CHECK_BOX_ERR_NO_UNCHECKED_EVENT_RES;
    struct TextParser* textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                                         newGameEventTextResource);
    if (!textParser) {
        TextResource_decreasePointersCounter(newGameEventTextResource);
        return 3;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)checkBox);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errConstructingUnCheckedGameEvent);
        TextResource_decreasePointersCounter(newGameEventTextResource);
        TextParser_destruct(textParser);
        return 4;
    }
    if (checkBox->unCheckedEvent)
        checkBox->unCheckedEvent->isNeeded = false;
    TextResource_decreasePointersCounter(checkBox->unCheckedEventResource);
    checkBox->unCheckedEventResource = newGameEventTextResource;
    checkBox->unCheckedEvent = newGameEvent;
    return CHECK_BOX_NO_ERRORS;
}

enum CheckBoxSceneNode_errors CheckBox_save(const struct CheckBox* const checkBox, struct ResourceManager* const resourceManager, 
                          const char* const checkBoxResId) {
    if (!checkBox || !resourceManager || !checkBoxResId)
        return CHECK_BOX_ERR_NULL_ARGUMENT;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return CHECK_BOX_ERR_CONSTRUCTIG_TEXT_PARSER;
    result += (TextParser_addString(textParser, TextParser_standartTypeString, CheckBoxSceneNode_parserString.type) != 0);
    result += (TextParser_addString(textParser, CheckBoxSceneNode_parserString.spriteRes,
                                   checkBox->sprite->dynamicSceneNode.sceneNode.sceneNodeTextResource->id) != 0);
    result += (TextParser_addString(textParser, CheckBoxSceneNode_parserString.focusedEventRes,
                                   checkBox->focusedEventResource->id) != 0);
    result += (TextParser_addString(textParser, CheckBoxSceneNode_parserString.checkedEventRes,
                                   checkBox->checkedEventResource->id) != 0);
    result += (TextParser_addString(textParser, CheckBoxSceneNode_parserString.unCheckedEventRes,
                                   checkBox->unCheckedEventResource->id) != 0);
    result += (TextParser_addString(textParser, CheckBoxSceneNode_parserString.focusedSoundRes,
                                   checkBox->focusedSoundResource->id) != 0);
    result += (TextParser_addString(textParser, CheckBoxSceneNode_parserString.checkedSoundRes,
                                   checkBox->checkedSoundResource->id) != 0);
    result += (TextParser_addString(textParser, CheckBoxSceneNode_parserString.unCheckedSoundRes,
                                   checkBox->unCheckedSoundResource->id) != 0);
    char* tempString = TextParser_convertToText(textParser);
    result += (textParser->lastError != 0);
    result += (TextResource_updateContent(checkBox->sceneNode.sceneNodeTextResource, tempString) != 0);
    result += (ResourceManager_saveTextResource(resourceManager,
                                               checkBox->sceneNode.sceneNodeTextResource, checkBoxResId) != 0);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    if (result)
        return CHECK_BOX_ERR_SAVING;
    return CHECK_BOX_NO_ERRORS;
}

void CheckBox_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {
    if (!sceneNode || !eventManager)
        return;
    struct CheckBox* checkBox = (struct CheckBox*)sceneNode;
    size_t i;
    checkBox->isStateChanged = false;
    for (i = 0; i < eventManager->sdlEventsCount; i++)
        if (eventManager->sdlEventsList[i].type == SDL_MOUSEMOTION 
            || eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONDOWN 
            || eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONUP) {
            SDL_Point mouseCoordinates;
            SDL_GetMouseState(&mouseCoordinates.x, &mouseCoordinates.y);
            if (mouseCoordinates.x >= checkBox->sprite->dstRect.x 
                && mouseCoordinates.x < (checkBox->sprite->dstRect.x + checkBox->sprite->dstRect.w) 
                && mouseCoordinates.y >= checkBox->sprite->dstRect.y 
                && mouseCoordinates.y < (checkBox->sprite->dstRect.y + checkBox->sprite->dstRect.h)) {
                if (checkBox->state == CHECK_BOX_STATE_UNCHECKED) {
                    checkBox->state = CHECK_BOX_STATE_FOCUSED_UNCHEKED;
                    checkBox->isStateChanged = true;
                    EventManager_addEvent(eventManager, checkBox->focusedEvent);
                }
                if (checkBox->state == CHECK_BOX_STATE_CHECKED) {
                    checkBox->state = CHECK_BOX_STATE_FOCUSED_CHECKED;
                    checkBox->isStateChanged = true;
                    EventManager_addEvent(eventManager, checkBox->focusedEvent);
                }
                if (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONDOWN) {
                    if (checkBox->state == CHECK_BOX_STATE_FOCUSED_UNCHEKED) {
                        checkBox->state = CHECK_BOX_STATE_CHECKING;
                        checkBox->isStateChanged = true;
                        EventManager_removeEvent(eventManager, checkBox->unCheckedEvent);
                        EventManager_addEvent(eventManager, checkBox->checkedEvent);
                    } else if (checkBox->state == CHECK_BOX_STATE_FOCUSED_CHECKED) {
                        checkBox->state = CHECK_BOX_STATE_UNCHECKING;
                        checkBox->isStateChanged = true;
                        
                        EventManager_removeEvent(eventManager, checkBox->checkedEvent);
                        EventManager_addEvent(eventManager, checkBox->unCheckedEvent);
                    }
                }
                if (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONUP) {
                   if (checkBox->state == CHECK_BOX_STATE_UNCHECKING) {
                    checkBox->state = CHECK_BOX_STATE_FOCUSED_UNCHEKED;
                    checkBox->isStateChanged = true;
                    EventManager_addEvent(eventManager, checkBox->focusedEvent);
                    }
                    if (checkBox->state == CHECK_BOX_STATE_CHECKING) {
                        checkBox->state = CHECK_BOX_STATE_FOCUSED_CHECKED;
                        checkBox->isStateChanged = true;
                        EventManager_addEvent(eventManager, checkBox->focusedEvent);
                    }
                }
            } else {
                    if (checkBox->state == CHECK_BOX_STATE_FOCUSED_UNCHEKED
                        || checkBox->state == CHECK_BOX_STATE_UNCHECKING)
                        checkBox->state = CHECK_BOX_STATE_UNCHECKED;
                    if (checkBox->state == CHECK_BOX_STATE_FOCUSED_CHECKED
                        || checkBox->state == CHECK_BOX_STATE_CHECKING)
                        checkBox->state = CHECK_BOX_STATE_CHECKED;
                    checkBox->isStateChanged = true;
                    EventManager_removeEvent(eventManager, checkBox->focusedEvent);
                }
        }
}

void CheckBox_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager || !renderer)
        return;
    struct CheckBox* checkBox = (struct CheckBox*)sceneNode;
    if (checkBox->isStateChanged) {
        if (checkBox->state == CHECK_BOX_STATE_UNCHECKED || checkBox->state == CHECK_BOX_STATE_UNCHECKING)
            checkBox->sprite->currentAnimation = 0;
        if (checkBox->state == CHECK_BOX_STATE_FOCUSED_UNCHEKED)
            checkBox->sprite->currentAnimation = 1;
        if (checkBox->state == CHECK_BOX_STATE_CHECKED || checkBox->state == CHECK_BOX_STATE_CHECKING)
            checkBox->sprite->currentAnimation = 2;
        if (checkBox->state == CHECK_BOX_STATE_FOCUSED_CHECKED)
            checkBox->sprite->currentAnimation = 3;
        checkBox->sprite->currentFrame = 0;
        checkBox->sprite->renderingsCounter = 0;
    }
    if (checkBox->isGeometryChanged) {
        checkBox->sprite->dynamicSceneNode.sceneNode.coordinates = checkBox->sceneNode.coordinates;
        checkBox->isGeometryChanged = false;
    }
    Sprite_update((struct SceneNode*)checkBox->sprite, eventManager, renderer);
}

void CheckBox_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct CheckBox* checkBox = (struct CheckBox*)sceneNode;
    Sprite_render((struct SceneNode*)checkBox->sprite, renderer);
}

void CheckBox_sound(struct SceneNode* sceneNode, struct Musican* musican){
    if (!sceneNode || !musican)
        return;
    struct CheckBox* checkBox = (struct CheckBox*)sceneNode;
    if (checkBox->isStateChanged) {
        if (checkBox->state == CHECK_BOX_STATE_FOCUSED_UNCHEKED 
            || checkBox->state == CHECK_BOX_STATE_FOCUSED_CHECKED)
            Musican_playSound(musican, checkBox->focusedSoundResource, 0);
        if (checkBox->state == CHECK_BOX_STATE_CHECKING)
            Musican_playSound(musican, checkBox->checkedSoundResource, 0);
        if (checkBox->state == CHECK_BOX_STATE_UNCHECKING)
            Musican_playSound(musican, checkBox->unCheckedSoundResource, 0);
    }
}
