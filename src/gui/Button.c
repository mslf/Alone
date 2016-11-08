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
#include "gui/Button.h"
#include "textParser/TextParser.h"

const char* const BUTTON_SCENENODE_ERR_FOCUSED_GAME_EVENT_RES = 
        "Button_loadEventsResources: focusedEventResource string haven't found!";
const char* const BUTTON_SCENENODE_ERR_PRESSED_GAME_EVENT_RES = 
        "Button_loadEventsResources: pressedEventResource string haven't found!";
const char* const BUTTON_SCENENODE_ERR_CONSTRUCTING_FOCUSED_GAME_EVENT = 
        "Button_changeFocusedEventResource: constructing GameEvent failed!";
const char* const BUTTON_SCENENODE_ERR_CONSTRUCTING_PRESSED_GAME_EVENT = 
        "Button_changePressedEventResource: constructing GameEvent failed!";
const char* const BUTTON_SCENENODE_ERR_FOCUSED_SOUND_RES = 
        "Button_loadSoundResources: focusedSoundResource string haven't found!";
const char* const BUTTON_SCENENODE_ERR_PRESSED_SOUND_RES = 
        "Button_loadSoundResources: pressedSoundResource string haven't found!";
const char* const BUTTON_SCENENODE_ERR_SPRITE_RES = 
        "Button_loadSpriteResource: spriteResource string haven't found!";
const char* const BUTTON_SCENENODE_ERR_SPRITE_NO_3_ANIMATIONS = 
        "Button_loadSpriteResource: loaded Sprite doesn't contain 3 animations!";
const char* const BUTTON_SCENENODE_ERR_TEXT_RES = 
        "Button_loadTextResource: textResource string haven't found!";
        

static unsigned char Button_loadTextResource(struct Button* button,
                                             struct ResourceManager* resourceManager,
                                             struct Renderer* renderer,
                                             struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                             struct TextParser* textParser) {
    if (!button || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    char* tempResId = TextParser_getString(textParser, BUTTON_SCENENODE_PARSER_TEXT_RES_STRING, 0);
    if (!tempResId) {
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_TEXT_RES);
        return 2;
    }
    button->label = (struct Text*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                               renderer,
                                                               sceneNodeTypesRegistrar,
                                                               tempResId,
                                                               TEXT_SCENENODE_PARSER_TYPE_STRING);
    if (!button->label)
        return 3;
    return 0;
}

static unsigned char Button_loadSpriteResource(struct Button* button,
                                               struct ResourceManager* resourceManager,
                                               struct Renderer* renderer,
                                               struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                               struct TextParser* textParser) {
    if (!button || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    char* tempResId = TextParser_getString(textParser, BUTTON_SCENENODE_PARSER_SPRITE_RES_STRING, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, BUTTON_SCENENODE_ERR_SPRITE_RES);
        return 2;
    }
    button->sprite = (struct Sprite*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                renderer,
                                                                sceneNodeTypesRegistrar,
                                                                tempResId,
                                                                SPRITE_SCENENODE_PARSER_TYPE_STRING);
    if (!button->sprite)
        return 3;
    if (button->sprite->animationsCount < 3) {
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_SPRITE_NO_3_ANIMATIONS);
        button->sprite->sceneNode.destruct((struct SceneNode*)button->sprite);
        button->sprite = NULL;
        return 4;
    }
    return 0;
}

static void Button_loadSoundResources(struct Button* button, struct ResourceManager* resourceManager,
                               struct TextParser* textParser) {
    if (!button || !resourceManager || !textParser)
        return;
    char* tempFocusedSoundResourceString = TextParser_getString(textParser,
                                                                BUTTON_SCENENODE_PARSER_FOCUSED_SOUND_RES_STRING, 0);
    if (!tempFocusedSoundResourceString)
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_FOCUSED_SOUND_RES);
    button->focusedSoundResource = ResourceManager_loadSoundResource(resourceManager, tempFocusedSoundResourceString);
    char* tempPressedSoundResourceString = TextParser_getString(textParser,
                                                                BUTTON_SCENENODE_PARSER_PRESSED_SOUND_RES_STRING, 0);
    if (!tempPressedSoundResourceString)
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_PRESSED_SOUND_RES);
    button->pressedSoundResource = ResourceManager_loadSoundResource(resourceManager, tempPressedSoundResourceString);
}

static void Button_loadEventsResources(struct Button* button, struct ResourceManager* resourceManager, 
                                struct TextParser* textParser){
    if (!button || !resourceManager || !textParser)
        return;
    char* tempFocusedEventResourceString = TextParser_getString(textParser,
                                                          BUTTON_SCENENODE_PARSER_FOCUSED_EVENT_RES_STRING, 0);
    if (!tempFocusedEventResourceString)
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_FOCUSED_GAME_EVENT_RES);
    char* tempPressedEventResourceString = TextParser_getString(textParser,
                                                          BUTTON_SCENENODE_PARSER_PRESSED_EVENT_RES_STRING, 0);
    if (!tempPressedEventResourceString)
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_PRESSED_GAME_EVENT_RES);
    Button_changeFocusedEventResource(button, resourceManager, tempFocusedEventResourceString);
    Button_changePressedEventResource(button, resourceManager, tempPressedEventResourceString);
}

static unsigned char Button_tryGetSettingsFromTextParser(struct Button* button,
                                                         struct ResourceManager* resourceManager, struct Renderer* renderer,
                                                         struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                         struct TextParser* textParser) {
    if (!button || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    unsigned char result = 0;
    Button_loadEventsResources(button, resourceManager, textParser);
    Button_loadSoundResources(button, resourceManager, textParser);
    result += Button_loadSpriteResource(button, resourceManager, renderer, sceneNodeTypesRegistrar, textParser);
    result += Button_loadTextResource(button, resourceManager, renderer, sceneNodeTypesRegistrar, textParser);
    button->labelOffset.x = (int)TextParser_getInt(textParser, BUTTON_SCENENODE_PARSER_LABEL_OFFSET_STRING, 0);
    button->labelOffset.y = (int)TextParser_getInt(textParser, BUTTON_SCENENODE_PARSER_LABEL_OFFSET_STRING, 1);
    return result;
}

struct SceneNode* Button_construct(struct ResourceManager* const resourceManager,
                                                 struct Renderer* const renderer,
                                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                 struct TextParser* const textParser) {
    if (!resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return NULL;
    struct Button* button = NULL;
    button = (struct Button*)calloc(1, sizeof(struct Button));
    if (!button)
        return NULL;
    SceneNode_init(&(button->sceneNode));
    if (Button_tryGetSettingsFromTextParser(button, resourceManager, renderer, sceneNodeTypesRegistrar, textParser)) {
        Button_destruct((struct SceneNode*)button);
        return NULL;
    }
    button->sceneNode.control = Button_control;
    button->sceneNode.update = Button_update;
    button->sceneNode.render = Button_render;
    button->sceneNode.sound = Button_sound;
    button->sceneNode.destruct = Button_destruct;
    button->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(BUTTON_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!button->sceneNode.type) {
        Button_destruct((struct SceneNode*)button);
        return NULL;
    }
    strcpy(button->sceneNode.type, BUTTON_SCENENODE_PARSER_TYPE_STRING);
    button->isGeometryChanged = true;
    return (struct SceneNode*)button;
}

void Button_destruct(struct SceneNode* button) {
    if (!button)
        return;
    struct Button* tempButton = (struct Button*)button;
    if (tempButton->sprite)
        Sprite_destruct((struct SceneNode*)tempButton->sprite);
    if (tempButton->label)
        Text_destruct((struct SceneNode*)tempButton->label);
    if (tempButton->focusedEventResource)
        tempButton->focusedEventResource->pointersCount--;
    if (tempButton->focusedSoundResource)
        tempButton->focusedSoundResource->pointersCount--;
    if (tempButton->pressedEventResource)
        tempButton->pressedEventResource->pointersCount--;
    if (tempButton->pressedSoundResource)
        tempButton->pressedSoundResource->pointersCount--;
    if (tempButton->focusedEvent)
        tempButton->focusedEvent->isNeeded = false;
    if (tempButton->pressedEvent)
        tempButton->pressedEvent->isNeeded = false;
    if (button->sceneNodeTextResource)
        button->sceneNodeTextResource->pointersCount--;
    if (button->type)
        free(button->type);
}

unsigned char Button_changePressedEventResource(struct Button* button, struct ResourceManager* resourceManager,
                                                const char* const pressedEventResId) {
    if (!button || !resourceManager || !pressedEventResId)
        return 1;
    struct TextResource* newGameEventTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                                     pressedEventResId, 0);
    if (!newGameEventTextResource)
        return 2;
    struct TextParser* textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                                         newGameEventTextResource);
    if (!textParser) {
        newGameEventTextResource->pointersCount--;
        return 3;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)button);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_CONSTRUCTING_PRESSED_GAME_EVENT);
        newGameEventTextResource->pointersCount--;
        TextParser_destruct(textParser);
        return 4;
    }
    if (button->pressedEvent)
        button->pressedEvent->isNeeded = false;
    if (button->pressedEventResource)
        button->pressedEventResource->pointersCount--;
    button->pressedEventResource = newGameEventTextResource;
    button->pressedEvent = newGameEvent;
    return 0;
}

unsigned char Button_changeFocusedEventResource(struct Button* button, struct ResourceManager* resourceManager,
                                                const char* const focusedEventResId) {
    if (!button || !resourceManager || !focusedEventResId)
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
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)button);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_CONSTRUCTING_FOCUSED_GAME_EVENT);
        newGameEventTextResource->pointersCount--;
        TextParser_destruct(textParser);
        return 4;
    }
    if (button->focusedEvent)
        button->focusedEvent->isNeeded = false;
    if (button->focusedEventResource)
        button->focusedEventResource->pointersCount--;
    button->focusedEventResource = newGameEventTextResource;
    button->focusedEvent = newGameEvent;
    return 0;
}

unsigned char Button_save(const struct Button* const button, struct ResourceManager* const resourceManager, 
                          const char* const buttonResId) {
    if (!button || !resourceManager || !buttonResId)
        return 1;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return 2;
    result += TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING, BUTTON_SCENENODE_PARSER_TYPE_STRING);
    result += TextParser_addString(textParser, BUTTON_SCENENODE_PARSER_SPRITE_RES_STRING,
                                   button->sprite->sceneNode.sceneNodeTextResource->id);
    result += TextParser_addString(textParser, BUTTON_SCENENODE_PARSER_TEXT_RES_STRING,
                                   button->label->sceneNode.sceneNodeTextResource->id);
    result += TextParser_addString(textParser, BUTTON_SCENENODE_PARSER_FOCUSED_EVENT_RES_STRING,
                                   button->focusedEventResource->id);
    result += TextParser_addString(textParser, BUTTON_SCENENODE_PARSER_PRESSED_EVENT_RES_STRING,
                                   button->pressedEventResource->id);
    result += TextParser_addString(textParser, BUTTON_SCENENODE_PARSER_FOCUSED_SOUND_RES_STRING,
                                   button->focusedSoundResource->id);
    result += TextParser_addString(textParser, BUTTON_SCENENODE_PARSER_PRESSED_SOUND_RES_STRING,
                                   button->pressedSoundResource->id);
    result += TextParser_addInt(textParser, BUTTON_SCENENODE_PARSER_LABEL_OFFSET_STRING,
                                   button->labelOffset.x);
    result += TextParser_addInt(textParser, BUTTON_SCENENODE_PARSER_LABEL_OFFSET_STRING,
                                   button->labelOffset.y);
    char* tempString = TextParser_convertToText(textParser);
    result += textParser->lastError;
    result += TextResource_updateContent(button->sceneNode.sceneNodeTextResource, tempString);
    result += ResourceManager_saveTextResource(resourceManager,
                                               button->sceneNode.sceneNodeTextResource, buttonResId);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    return result;
}

void Button_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {
    if (!sceneNode || !eventManager)
        return;
    struct Button* button = (struct Button*)sceneNode;
    size_t i;
    button->isStateChanged = false;
    for (i = 0; i < eventManager->sdlEventsCount; i++)
        if (eventManager->sdlEventsList[i].type == SDL_MOUSEMOTION 
            || eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONDOWN 
            || eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONUP) {
            SDL_Point mouseCoordinates;
            SDL_GetMouseState(&mouseCoordinates.x, &mouseCoordinates.y);
            if (mouseCoordinates.x >= button->sprite->dstRect.x 
                && mouseCoordinates.x < (button->sprite->dstRect.x + button->sprite->dstRect.w) 
                && mouseCoordinates.y >= button->sprite->dstRect.y 
                && mouseCoordinates.y < (button->sprite->dstRect.y + button->sprite->dstRect.h)) {
                if (button->state == ButtonState_Normal) {
                    button->state = ButtonState_Focused;
                    button->isStateChanged = true;
                    EventManager_addEvent(eventManager, button->focusedEvent);
                }
                if (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONDOWN) {
                    button->state = ButtonState_Pressed;
                    button->isStateChanged = true;
                    
                }
                if (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONUP) {
                    if (button->state == ButtonState_Pressed)
                        EventManager_addEvent(eventManager, button->pressedEvent);
                    button->state = ButtonState_Focused;
                    button->isStateChanged = true;
                }
            } else {
                if (button->state == ButtonState_Focused) {
                    button->state = ButtonState_Normal;
                    button->isStateChanged = true;
                    EventManager_removeEvent(eventManager, button->focusedEvent);
                }
                if (button->state == ButtonState_Pressed 
                    && (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONUP 
                        || eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONDOWN)) {
                    button->state = ButtonState_Normal;
                    button->isStateChanged = true;
                    EventManager_removeEvent(eventManager, button->focusedEvent);
                }
            }
        }
}

void Button_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager || !renderer)
        return;
    struct Button* button = (struct Button*)sceneNode;
    if (button->isStateChanged) {
        if (button->state == ButtonState_Normal)
            button->sprite->currentAnimation = 0;
        if (button->state == ButtonState_Focused)
            button->sprite->currentAnimation = 1;
        if (button->state == ButtonState_Pressed)
            button->sprite->currentAnimation = 2;
        button->sprite->currentFrame = 0;
        button->sprite->renderingsCounter = 0;
    }
    if (button->isGeometryChanged) {
        button->sprite->sceneNode.angle = button->sceneNode.angle;
        button->sprite->sceneNode.coordinates = button->sceneNode.coordinates;
        button->sprite->sceneNode.flip = button->sceneNode.flip;
        button->sprite->sceneNode.rotatePointCoordinates = button->sceneNode.rotatePointCoordinates;
        button->sprite->sceneNode.scaleX = button->sceneNode.scaleX;
        button->sprite->sceneNode.scaleY = button->sceneNode.scaleY;
        button->label->sceneNode.angle = button->sceneNode.angle;
        button->label->sceneNode.coordinates.x = button->sceneNode.coordinates.x + button->labelOffset.x;
        button->label->sceneNode.coordinates.y = button->sceneNode.coordinates.y + button->labelOffset.y;
        button->label->sceneNode.flip = button->sceneNode.flip;
        button->label->sceneNode.rotatePointCoordinates = button->sceneNode.rotatePointCoordinates;
        button->label->sceneNode.scaleX = button->sceneNode.scaleX;
        button->label->sceneNode.scaleY = button->sceneNode.scaleY;
        button->isGeometryChanged = false;
    }
    Sprite_update((struct SceneNode*)button->sprite, eventManager, renderer);
    Text_update((struct SceneNode*)button->label, eventManager, renderer);
}

void Button_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct Button* button = (struct Button*)sceneNode;
    Sprite_render((struct SceneNode*)button->sprite, renderer);
    Text_render((struct SceneNode*)button->label, renderer);
}

void Button_sound(struct SceneNode* sceneNode, struct Musican* musican) {
    if (!sceneNode || !musican)
        return;
    struct Button* button = (struct Button*)sceneNode;
    if (button->isStateChanged) {
        if (button->state == ButtonState_Focused)
            Musican_playSound(musican, button->focusedSoundResource, 0);
        if (button->state == ButtonState_Pressed)
            Musican_playSound(musican, button->pressedSoundResource, 0);
    }
}
