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

const char* const CHECK_BOX_SCENENODE_ERR_FOCUSED_GAME_EVENT_RES = 
        "CheckBox_loadEventsResources: focusedEventResource string haven't found!";
const char* const CHECK_BOX_SCENENODE_ERR_CHECKED_GAME_EVENT_RES = 
        "CheckBox_loadEventsResources: checkedEventResource string haven't found!";
const char* const CHECK_BOX_SCENENODE_ERR_UNCHECKED_GAME_EVENT_RES = 
        "CheckBox_loadEventsResources: unCheckedEventResource string haven't found!";
const char* const CHECK_BOX_SCENENODE_ERR_CONSTRUCTING_FOCUSED_GAME_EVENT = 
        "CheckBox_changeFocusedEventResource: constructing GameEvent failed!";
const char* const CHECK_BOX_SCENENODE_ERR_CONSTRUCTING_CHECKED_GAME_EVENT = 
        "CheckBox_changeCheckedEventResource: constructing GameEvent failed!";
const char* const CHECK_BOX_SCENENODE_ERR_CONSTRUCTING_UNCHECKED_GAME_EVENT = 
        "CheckBox_changeUnCheckedEventResource: constructing GameEvent failed!";
const char* const CHECK_BOX_SCENENODE_ERR_FOCUSED_SOUND_RES = 
        "CheckBox_loadSoundResources: focusedSoundResource string haven't found!";
const char* const CHECK_BOX_SCENENODE_ERR_CHECKED_SOUND_RES = 
        "CheckBox_loadSoundResources: checkedSoundResource string haven't found!";
const char* const CHECK_BOX_SCENENODE_ERR_UNCHECKED_SOUND_RES = 
        "CheckBox_loadSoundResources: unCheckedSoundResource string haven't found!";
const char* const CHECK_BOX_SCENENODE_ERR_SPRITE_RES = 
        "CheckBox_loadSpriteResource: spriteResource string haven't found!";
const char* const CHECK_BOX_SCENENODE_ERR_SPRITE_NO_TYPE = 
        "CheckBox_loadSpriteResource: type string haven't found!";
const char* const CHECK_BOX_SCENENODE_ERR_SPRITE_TYPE = 
        "CheckBox_loadSpriteResource: suitable Sprite type string haven't detected!";
const char* const CHECK_BOX_SCENENODE_ERR_SPRITE_NO_4_ANIMATIONS = 
        "CheckBox_loadSpriteResource: loaded Sprite doesn't contain 4 animations!";
        
unsigned char CheckBox_loadSpriteResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                               struct Renderer* renderer, struct TextParser* textParser) {
    char* tempSpriteResourceString = TextParser_getString(textParser, CHECK_BOX_SCENENODE_PARSER_SPRITE_RES_STRING, 0);
    if (!tempSpriteResourceString) {
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_SPRITE_RES);
        return 1;
    }
    struct TextResource* spriteTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                               tempSpriteResourceString, 0);
    if (!spriteTextResource)
       return 2;
    struct TextParser* spriteTextParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                                               spriteTextResource);
    if (!spriteTextParser) {
        spriteTextResource->pointersCount--;
        return 3;
    }
    char* tempSpriteTypeString = TextParser_getString(spriteTextParser, TEXT_PARSER_TYPE_STRING, 0);
    if (!tempSpriteTypeString) {
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_SPRITE_NO_TYPE);
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 4;
    }
    if (strcmp(tempSpriteTypeString, SPRITE_SCENENODE_PARSER_TYPE_STRING) != 0) {
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_SPRITE_TYPE);
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 5;
    }
    checkBox->sprite = Sprite_construct(resourceManager, renderer, tempSpriteResourceString);
    if (!checkBox->sprite) {
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 6;
    }
    if (checkBox->sprite->animationsCount < 4) {
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_SPRITE_NO_4_ANIMATIONS);
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 7;
    }
    TextParser_destruct(spriteTextParser);
    spriteTextResource->pointersCount--;
    return 0;
}

void CheckBox_loadSoundResources(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                               struct TextParser* textParser) {
    char* tempFocusedSoundResourceString = TextParser_getString(textParser,
                                                        CHECK_BOX_SCENENODE_PARSER_FOCUSED_SOUND_RES_STRING, 0);
    if (!tempFocusedSoundResourceString)
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_FOCUSED_SOUND_RES);
    checkBox->focusedSoundResource = ResourceManager_loadSoundResource(resourceManager,
                                                                       tempFocusedSoundResourceString);
    char* tempCheckedSoundResourceString = TextParser_getString(textParser,
                                                        CHECK_BOX_SCENENODE_PARSER_CHECKED_SOUND_RES_STRING, 0);
    if (!tempCheckedSoundResourceString)
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_CHECKED_SOUND_RES);
    checkBox->checkedSoundResource = ResourceManager_loadSoundResource(resourceManager,
                                                                       tempCheckedSoundResourceString);
    char* tempUnCheckedSoundResourceString = TextParser_getString(textParser,
                                                        CHECK_BOX_SCENENODE_PARSER_UNCHECKED_SOUND_RES_STRING, 0);
    if (!tempUnCheckedSoundResourceString)
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_UNCHECKED_SOUND_RES);
    checkBox->unCheckedSoundResource = ResourceManager_loadSoundResource(resourceManager,
                                                                         tempUnCheckedSoundResourceString);
}

void CheckBox_loadEventsResources(struct CheckBox* checkBox, struct ResourceManager* resourceManager, 
                                struct TextParser* textParser){
    char* tempFocusedEventResourceString = TextParser_getString(textParser,
                                                          CHECK_BOX_SCENENODE_PARSER_FOCUSED_EVENT_RES_STRING, 0);
    if (!tempFocusedEventResourceString)
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_FOCUSED_GAME_EVENT_RES);
    char* tempCheckedEventResourceString = TextParser_getString(textParser,
                                                          CHECK_BOX_SCENENODE_PARSER_CHECKED_EVENT_RES_STRING, 0);
    if (!tempCheckedEventResourceString)
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_CHECKED_GAME_EVENT_RES);
    char* tempUnCheckedEventResourceString = TextParser_getString(textParser,
                                                          CHECK_BOX_SCENENODE_PARSER_UNCHECKED_EVENT_RES_STRING, 0);
    if (!tempUnCheckedEventResourceString)
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_UNCHECKED_GAME_EVENT_RES);
    CheckBox_changeFocusedEventResource(checkBox, resourceManager, tempFocusedEventResourceString);
    CheckBox_changeCheckedEventResource(checkBox, resourceManager, tempCheckedEventResourceString);
    CheckBox_changeCheckedEventResource(checkBox, resourceManager, tempUnCheckedEventResourceString);
}
        
unsigned char CheckBox_tryGetSettingsFromTextParser(struct CheckBox* checkBox, struct ResourceManager* resourceManager, 
                                                  struct Renderer* renderer, struct TextParser* textParser) {
    unsigned char result = 0;
    CheckBox_loadEventsResources(checkBox, resourceManager, textParser);
    CheckBox_loadSoundResources(checkBox, resourceManager, textParser);
    result += CheckBox_loadSpriteResource(checkBox, resourceManager, renderer, textParser);
    if (result)
        return result;
    return 0;
}
        
struct CheckBox* CheckBox_construct(struct ResourceManager* const resourceManager, struct Renderer* renderer,
                                const char* const checkBoxResId) {
    struct CheckBox* checkBox = NULL;
    checkBox = (struct CheckBox*)calloc(1, sizeof(struct CheckBox));
    if (!checkBox)
        return NULL;
    SceneNode_init(&(checkBox->sceneNode));
    checkBox->sceneNode.sceneNodeTextResource = ResourceManager_loadTextResource(resourceManager, checkBoxResId, 0);
    if (!checkBox->sceneNode.sceneNodeTextResource) {
        CheckBox_destruct(checkBox);
        return NULL;
    }
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructFromTextResource(resourceManager->logger, checkBox->sceneNode.sceneNodeTextResource);
    if (!textParser) {
        CheckBox_destruct(checkBox);
        return NULL;
    }
    if (CheckBox_tryGetSettingsFromTextParser(checkBox, resourceManager, renderer, textParser)) {
        TextParser_destruct(textParser);
        CheckBox_destruct(checkBox);
        return NULL;
    }
    TextParser_destruct(textParser);
    checkBox->sceneNode.control = CheckBox_control;
    checkBox->sceneNode.update = CheckBox_update;
    checkBox->sceneNode.render = CheckBox_render;
    checkBox->sceneNode.sound = CheckBox_sound;
    checkBox->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(CHECK_BOX_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!checkBox->sceneNode.type) {
        CheckBox_destruct(checkBox);
        return NULL;
    }
    strcpy(checkBox->sceneNode.type, CHECK_BOX_SCENENODE_PARSER_TYPE_STRING);
    checkBox->isGeometryChanged = true;
    return checkBox;
}

void CheckBox_destruct(struct CheckBox* checkBox) {
    if (!checkBox)
        return;
    if (checkBox->sprite)
        Sprite_destruct(checkBox->sprite);
    if (checkBox->focusedEventResource)
        checkBox->focusedEventResource->pointersCount--;
    if (checkBox->focusedSoundResource)
        checkBox->focusedSoundResource->pointersCount--;
    if (checkBox->focusedEvent)
        checkBox->focusedEvent->isNeeded = false;
    if (checkBox->checkedEventResource)
        checkBox->checkedEventResource->pointersCount--;
    if (checkBox->checkedSoundResource)
        checkBox->checkedSoundResource->pointersCount--;
    if (checkBox->checkedEvent)
        checkBox->checkedEvent->isNeeded = false;
    if (checkBox->unCheckedEventResource)
        checkBox->unCheckedEventResource->pointersCount--;
    if (checkBox->unCheckedSoundResource)
        checkBox->unCheckedSoundResource->pointersCount--;
    if (checkBox->unCheckedEvent)
        checkBox->unCheckedEvent->isNeeded = false;
    if (checkBox->sceneNode.sceneNodeTextResource)
        checkBox->sceneNode.sceneNodeTextResource->pointersCount--;
    if (checkBox->sceneNode.type)
        free(checkBox->sceneNode.type);
}

unsigned char CheckBox_changeFocusedEventResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                                                const char* const focusedEventResId) {
    if (!checkBox || !resourceManager || !focusedEventResId)
        return 1;
    struct TextResource* newGameEventTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                                     focusedEventResId, 0);
    if (!newGameEventTextResource)
        return 2;
    struct TextParser* textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                                         newGameEventTextResource);
    if (!textParser) {
        newGameEventTextResource->pointersCount--;
        return 3;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)checkBox);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_CONSTRUCTING_FOCUSED_GAME_EVENT);
        newGameEventTextResource->pointersCount--;
        TextParser_destruct(textParser);
        return 4;
    }
    if (checkBox->focusedEvent)
        checkBox->focusedEvent->isNeeded = false;
    if (checkBox->focusedEventResource)
        checkBox->focusedEventResource->pointersCount--;
    checkBox->focusedEventResource = newGameEventTextResource;
    checkBox->focusedEvent = newGameEvent;
    return 0;
}

unsigned char CheckBox_changeCheckedEventResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                                                const char* const checkedEventResId) {
    if (!checkBox || !resourceManager || !checkedEventResId)
        return 1;
    struct TextResource* newGameEventTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                                     checkedEventResId, 0);
    if (!newGameEventTextResource)
        return 2;
    struct TextParser* textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                                         newGameEventTextResource);
    if (!textParser) {
        newGameEventTextResource->pointersCount--;
        return 3;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)checkBox);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_CONSTRUCTING_CHECKED_GAME_EVENT);
        newGameEventTextResource->pointersCount--;
        TextParser_destruct(textParser);
        return 4;
    }
    if (checkBox->checkedEvent)
        checkBox->checkedEvent->isNeeded = false;
    if (checkBox->checkedEventResource)
        checkBox->checkedEventResource->pointersCount--;
    checkBox->checkedEventResource = newGameEventTextResource;
    checkBox->checkedEvent = newGameEvent;
    return 0;
}

unsigned char CheckBox_changeUnCheckedEventResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                                                const char* const unCheckeEventResId) {
    if (!checkBox || !resourceManager || !unCheckeEventResId)
        return 1;
    struct TextResource* newGameEventTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                                     unCheckeEventResId, 0);
    if (!newGameEventTextResource)
        return 2;
    struct TextParser* textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                                         newGameEventTextResource);
    if (!textParser) {
        newGameEventTextResource->pointersCount--;
        return 3;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)checkBox);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, CHECK_BOX_SCENENODE_ERR_CONSTRUCTING_UNCHECKED_GAME_EVENT);
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
    return 0;
}

unsigned char CheckBox_save(const struct CheckBox* const checkBox, struct ResourceManager* const resourceManager, 
                          const char* const checkBoxResId) {
    if (!checkBox || !resourceManager || !checkBoxResId)
        return 1;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return 2;
    result += TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING, CHECK_BOX_SCENENODE_PARSER_TYPE_STRING);
    result += TextParser_addString(textParser, CHECK_BOX_SCENENODE_PARSER_SPRITE_RES_STRING,
                                   checkBox->sprite->sceneNode.sceneNodeTextResource->id);
    result += TextParser_addString(textParser, CHECK_BOX_SCENENODE_PARSER_FOCUSED_EVENT_RES_STRING,
                                   checkBox->focusedEventResource->id);
    result += TextParser_addString(textParser, CHECK_BOX_SCENENODE_PARSER_CHECKED_EVENT_RES_STRING,
                                   checkBox->checkedEventResource->id);
    result += TextParser_addString(textParser, CHECK_BOX_SCENENODE_PARSER_UNCHECKED_EVENT_RES_STRING,
                                   checkBox->unCheckedEventResource->id);
    result += TextParser_addString(textParser, CHECK_BOX_SCENENODE_PARSER_FOCUSED_SOUND_RES_STRING,
                                   checkBox->focusedSoundResource->id);
    result += TextParser_addString(textParser, CHECK_BOX_SCENENODE_PARSER_CHECKED_SOUND_RES_STRING,
                                   checkBox->checkedSoundResource->id);
    result += TextParser_addString(textParser, CHECK_BOX_SCENENODE_PARSER_UNCHECKED_SOUND_RES_STRING,
                                   checkBox->unCheckedSoundResource->id);
    char* tempString = TextParser_convertToText(textParser);
    if (!tempString)
        result++;
    if (TextResource_updateContent(checkBox->sceneNode.sceneNodeTextResource, tempString))
        result++;
    result += ResourceManager_saveTextResource(resourceManager,
                                               checkBox->sceneNode.sceneNodeTextResource, checkBoxResId);
    if (result) {
        TextParser_destruct(textParser);
        return 3;
    }
    TextParser_destruct(textParser);
    return 0;
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
                && mouseCoordinates.x <= (checkBox->sprite->dstRect.x + checkBox->sprite->dstRect.w) 
                && mouseCoordinates.y >= checkBox->sprite->dstRect.y 
                && mouseCoordinates.y <= (checkBox->sprite->dstRect.y + checkBox->sprite->dstRect.h)) {
            
                if (checkBox->state == CheckBoxState_UnChecked) {
                    checkBox->state = CheckBoxState_FocusedUnChecked;
                    checkBox->isStateChanged = true;
                    EventManager_addEvent(eventManager, checkBox->focusedEvent);
                }
                if (checkBox->state == CheckBoxState_Checked) {
                    checkBox->state = CheckBoxState_FocusedChecked;
                    checkBox->isStateChanged = true;
                    EventManager_addEvent(eventManager, checkBox->focusedEvent);
                }
                if (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONDOWN) {
                    if (checkBox->state == CheckBoxState_FocusedUnChecked) {
                        checkBox->state = CheckBoxState_Checked;
                        checkBox->isStateChanged = true;
                        EventManager_removeEvent(eventManager, checkBox->unCheckedEvent);
                        EventManager_addEvent(eventManager, checkBox->checkedEvent);
                    }
                    if (checkBox->state == CheckBoxState_FocusedChecked) {
                        checkBox->state = CheckBoxState_UnChecked;
                        checkBox->isStateChanged = true;
                        EventManager_removeEvent(eventManager, checkBox->checkedEvent);
                    EventManager_addEvent(eventManager, checkBox->unCheckedEvent);
                    }
                }
                if (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONUP) {
                   if (checkBox->state == CheckBoxState_UnChecked) {
                    checkBox->state = CheckBoxState_FocusedUnChecked;
                    checkBox->isStateChanged = true;
                    EventManager_addEvent(eventManager, checkBox->focusedEvent);
                }
                if (checkBox->state == CheckBoxState_Checked) {
                    checkBox->state = CheckBoxState_FocusedChecked;
                    checkBox->isStateChanged = true;
                    EventManager_addEvent(eventManager, checkBox->focusedEvent);
                }
                }
            } else 
                if (checkBox->state != CheckBoxState_Checked && checkBox->state != CheckBoxState_UnChecked) {
                    if (checkBox->state == CheckBoxState_FocusedUnChecked)
                        checkBox->state = CheckBoxState_UnChecked;
                    if (checkBox->state == CheckBoxState_FocusedChecked)
                        checkBox->state = CheckBoxState_Checked;
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
        if (checkBox->state == CheckBoxState_UnChecked)
            checkBox->sprite->currentAnimation = 0;
        if (checkBox->state == CheckBoxState_FocusedUnChecked)
            checkBox->sprite->currentAnimation = 1;
        if (checkBox->state == CheckBoxState_Checked)
            checkBox->sprite->currentAnimation = 2;
        if (checkBox->state == CheckBoxState_FocusedChecked)
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
        if (checkBox->state == CheckBoxState_FocusedUnChecked 
            || checkBox->state == CheckBoxState_FocusedChecked)
            Musican_playSound(musican, checkBox->focusedSoundResource, 0);
        if (checkBox->state == CheckBoxState_Checked)
            Musican_playSound(musican, checkBox->checkedSoundResource, 0);
        if (checkBox->state == CheckBoxState_UnChecked)
            Musican_playSound(musican, checkBox->unCheckedSoundResource, 0);
    }
}
