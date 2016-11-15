//
// Created by mslf on 8/13/16.
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
#include "gui/CheckBox.h"
#include "textParser/TextParser.h"

static const struct CheckBoxSceneNode_errorMessages {
    const char* const errFocusedGameEventRes;
    const char* const errCheckedGameEventRes;
    const char* const errUnCheckedGameEventRes;
    const char* const errConstructingFocusedGameEvent;
    const char* const errConstructingCheckedGameEvent;
    const char* const errConstructingUnCheckedGameEvent;
    const char* const errFocusedSoundRes;
    const char* const errCheckedSoundRes;
    const char* const errUnCheckedSoundRes;
    const char* const errSpriteRes;
    const char* const errSpriteNoEnoughAnimations;
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

static enum CheckBoxSceneNode_errors CheckBox_loadSpriteResource(struct CheckBox* checkBox,
                                                 struct ResourceManager* const resourceManager,
                                                 struct Renderer* const renderer,
                                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                 struct TextParser* const textParser) {
    if (!checkBox || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return CHECK_BOX_ERR_NULL_ARGUMENT;
    const char* tempResId = TextParser_getString(textParser, CheckBoxSceneNode_parserString.spriteRes, 0);
    if (!tempResId) {
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errSpriteRes);
        return CHECK_BOX_ERR_NO_SPRITE_RES;
    }
    checkBox->sprite = (struct Sprite*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                                  renderer,
                                                                                  sceneNodeTypesRegistrar,
                                                                                  tempResId,
                                                                                  SPRITE_SCENENODE_PARSER_TYPE_STRING);
    if (!checkBox->sprite)
        return CHECK_BOX_ERR_CONSTRUCTIG_SPRITE;
    if (checkBox->sprite->animationsCount < 4) {
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errSpriteNoEnoughAnimations);
        return CHECK_BOX_ERR_NOT_ENOUGH_ANIMATIONS;
    }
    return CHECK_BOX_NO_ERRORS;
}

static void CheckBox_loadSoundResources(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                               struct TextParser* textParser) {
    if (!checkBox || !resourceManager || !textParser)
        return;
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

static void CheckBox_loadEventsResources(struct CheckBox* checkBox, struct ResourceManager* resourceManager, 
                                struct TextParser* textParser){
    if (!checkBox || !resourceManager || !textParser)
        return;
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
        
static enum CheckBoxSceneNode_errors CheckBox_tryGetSettingsFromTextParser(struct CheckBox* checkBox,
                                                           struct ResourceManager* const resourceManager,
                                                           struct Renderer* const renderer,
                                                           struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                           struct TextParser* const textParser) {
    if (!checkBox || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return CHECK_BOX_ERR_NULL_ARGUMENT;
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
    if (tempCheckBox->focusedEventResource)
        tempCheckBox->focusedEventResource->pointersCount--;
    if (tempCheckBox->focusedSoundResource)
        tempCheckBox->focusedSoundResource->pointersCount--;
    if (tempCheckBox->focusedEvent)
        tempCheckBox->focusedEvent->isNeeded = false;
    if (tempCheckBox->checkedEventResource)
        tempCheckBox->checkedEventResource->pointersCount--;
    if (tempCheckBox->checkedSoundResource)
        tempCheckBox->checkedSoundResource->pointersCount--;
    if (tempCheckBox->checkedEvent)
        tempCheckBox->checkedEvent->isNeeded = false;
    if (tempCheckBox->unCheckedEventResource)
        tempCheckBox->unCheckedEventResource->pointersCount--;
    if (tempCheckBox->unCheckedSoundResource)
        tempCheckBox->unCheckedSoundResource->pointersCount--;
    if (tempCheckBox->unCheckedEvent)
        tempCheckBox->unCheckedEvent->isNeeded = false;
    if (checkBox->sceneNodeTextResource)
        checkBox->sceneNodeTextResource->pointersCount--;
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
        newGameEventTextResource->pointersCount--;
        return CHECK_BOX_ERR_CONSTRUCTIG_TEXT_PARSER;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)checkBox);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errFocusedGameEventRes);
        newGameEventTextResource->pointersCount--;
        TextParser_destruct(textParser);
        return CHECK_BOX_ERR_CONSTRUCTIG_FOCUSED_EVENT;
    }
    if (checkBox->focusedEvent)
        checkBox->focusedEvent->isNeeded = false;
    if (checkBox->focusedEventResource)
        checkBox->focusedEventResource->pointersCount--;
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
        newGameEventTextResource->pointersCount--;
        return CHECK_BOX_ERR_CONSTRUCTIG_TEXT_PARSER;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)checkBox);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errCheckedGameEventRes);
        newGameEventTextResource->pointersCount--;
        TextParser_destruct(textParser);
        return CHECK_BOX_ERR_CONSTRUCTIG_CHECKED_EVENT;
    }
    if (checkBox->checkedEvent)
        checkBox->checkedEvent->isNeeded = false;
    if (checkBox->checkedEventResource)
        checkBox->checkedEventResource->pointersCount--;
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
        newGameEventTextResource->pointersCount--;
        return 3;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)checkBox);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, CheckBoxSceneNode_errorMessages.errConstructingUnCheckedGameEvent);
        newGameEventTextResource->pointersCount--;
        TextParser_destruct(textParser);
        return 4;
    }
    if (checkBox->unCheckedEvent)
        checkBox->unCheckedEvent->isNeeded = false;
    if (checkBox->unCheckedEventResource)
        checkBox->unCheckedEventResource->pointersCount--;
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
    result += TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING, CheckBoxSceneNode_parserString.type);
    result += TextParser_addString(textParser, CheckBoxSceneNode_parserString.spriteRes,
                                   checkBox->sprite->sceneNode.sceneNodeTextResource->id);
    result += TextParser_addString(textParser, CheckBoxSceneNode_parserString.focusedEventRes,
                                   checkBox->focusedEventResource->id);
    result += TextParser_addString(textParser, CheckBoxSceneNode_parserString.checkedEventRes,
                                   checkBox->checkedEventResource->id);
    result += TextParser_addString(textParser, CheckBoxSceneNode_parserString.unCheckedEventRes,
                                   checkBox->unCheckedEventResource->id);
    result += TextParser_addString(textParser, CheckBoxSceneNode_parserString.focusedSoundRes,
                                   checkBox->focusedSoundResource->id);
    result += TextParser_addString(textParser, CheckBoxSceneNode_parserString.checkedSoundRes,
                                   checkBox->checkedSoundResource->id);
    result += TextParser_addString(textParser, CheckBoxSceneNode_parserString.unCheckedSoundRes,
                                   checkBox->unCheckedSoundResource->id);
    char* tempString = TextParser_convertToText(textParser);
    result += textParser->lastError;
    result += TextResource_updateContent(checkBox->sceneNode.sceneNodeTextResource, tempString);
    result += ResourceManager_saveTextResource(resourceManager,
                                               checkBox->sceneNode.sceneNodeTextResource, checkBoxResId);
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
                        || checkBox->state == CHECK_BOX_STATE_FOCUSED_UNCHEKED)
                        checkBox->state = CHECK_BOX_STATE_FOCUSED_UNCHEKED;
                    if (checkBox->state == CHECK_BOX_STATE_FOCUSED_CHECKED
                        || checkBox->state == CHECK_BOX_STATE_CHECKING)
                        checkBox->state = CHECK_BOX_STATE_FOCUSED_CHECKED;
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
        if (checkBox->state == CHECK_BOX_STATE_FOCUSED_UNCHEKED || checkBox->state == CHECK_BOX_STATE_UNCHECKING)
            checkBox->sprite->currentAnimation = 0;
        if (checkBox->state == CHECK_BOX_STATE_FOCUSED_UNCHEKED)
            checkBox->sprite->currentAnimation = 1;
        if (checkBox->state == CHECK_BOX_STATE_FOCUSED_CHECKED || checkBox->state == CHECK_BOX_STATE_CHECKING)
            checkBox->sprite->currentAnimation = 2;
        if (checkBox->state == CHECK_BOX_STATE_FOCUSED_CHECKED)
            checkBox->sprite->currentAnimation = 3;
        checkBox->sprite->currentFrame = 0;
        checkBox->sprite->renderingsCounter = 0;
    }
    if (checkBox->isGeometryChanged) {
        checkBox->sprite->sceneNode.angle = checkBox->sceneNode.angle;
        checkBox->sprite->sceneNode.coordinates = checkBox->sceneNode.coordinates;
        checkBox->sprite->sceneNode.flip = checkBox->sceneNode.flip;
        checkBox->sprite->sceneNode.rotatePointCoordinates = checkBox->sceneNode.rotatePointCoordinates;
        checkBox->sprite->sceneNode.scaleX = checkBox->sceneNode.scaleX;
        checkBox->sprite->sceneNode.scaleY = checkBox->sceneNode.scaleY;
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
