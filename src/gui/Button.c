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
        "Button_loadSoundResources: focusedEventResource string haven't found!";
const char* const BUTTON_SCENENODE_ERR_PRESSED_GAME_EVENT_RES = 
        "Button_loadSoundResources: pressedEventResource string haven't found!";
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
const char* const BUTTON_SCENENODE_ERR_SPRITE_NO_TYPE = 
        "Button_loadSpriteResource: type string haven't found!";
const char* const BUTTON_SCENENODE_ERR_SPRITE_TYPE = 
        "Button_loadSpriteResource: suitable Sprite type string haven't detected!";
const char* const BUTTON_SCENENODE_ERR_SPRITE_NO_3_ANIMATIONS = 
        "Button_loadSpriteResource: loaded Sprite doesn't contain 3 animations!";
const char* const BUTTON_SCENENODE_ERR_TEXT_RES = 
        "Button_loadTextResource: textResource string haven't found!";
const char* const BUTTON_SCENENODE_ERR_TEXT_NO_TYPE = 
        "Button_loadTextResource: type string haven't found!";
const char* const BUTTON_SCENENODE_ERR_TEXT_TYPE = 
        "Button_loadTextResource: suitable Text type string haven't detected!";
        

unsigned char Button_loadTextResource(struct Button* button, struct ResourceManager* resourceManager,
                               struct Renderer* renderer, struct TextParser* textParser) {
    char* tempTextSceneNodeResourceString = TextParser_getString(textParser,
                                                                 BUTTON_SCENENODE_PARSER_TEXT_RES_STRING, 0);
    if (!tempTextSceneNodeResourceString) {
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_TEXT_RES);
        return 1;
    }
    struct TextResource* textSceneNodeTextResource = 
                        ResourceManager_loadTextResource(resourceManager, tempTextSceneNodeResourceString, 0);
    if (!textSceneNodeTextResource)
       return 2;
    struct TextParser* textSceneNodeTextParser = TextParser_constructFromTextResource(resourceManager->logger, 
                                                                                      textSceneNodeTextResource);
    if (!textSceneNodeTextParser) {
        textSceneNodeTextResource->pointersCount--;
        return 3;
    }
    char* tempTextSceneNodeTypeString = TextParser_getString(textSceneNodeTextParser, TEXT_PARSER_TYPE_STRING, 0);
    if (!tempTextSceneNodeTypeString) {
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_TEXT_NO_TYPE);
        textSceneNodeTextResource->pointersCount--;
        TextParser_destruct(textSceneNodeTextParser);
        return 4;
    }
    if (strcmp(tempTextSceneNodeTypeString, TEXT_SCENENODE_PARSER_TYPE_STRING) != 0) {
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_TEXT_TYPE);
        textSceneNodeTextResource->pointersCount--;
        TextParser_destruct(textSceneNodeTextParser);
        return 5;
    }
    button->label = Text_construct(resourceManager, renderer, tempTextSceneNodeResourceString);
    if (!button->label) {
        textSceneNodeTextResource->pointersCount--;
        TextParser_destruct(textSceneNodeTextParser);
        return 6;
    }
    TextParser_destruct(textSceneNodeTextParser);
    textSceneNodeTextResource->pointersCount--;
    return 0;
}

unsigned char Button_loadSpriteResource(struct Button* button, struct ResourceManager* resourceManager,
                               struct Renderer* renderer, struct TextParser* textParser) {
    char* tempSpriteResourceString = TextParser_getString(textParser, BUTTON_SCENENODE_PARSER_SPRITE_RES_STRING, 0);
    if (!tempSpriteResourceString) {
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_SPRITE_RES);
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
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_SPRITE_NO_TYPE);
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 4;
    }
    if (strcmp(tempSpriteTypeString, SPRITE_SCENENODE_PARSER_TYPE_STRING) != 0) {
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_SPRITE_TYPE);
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 5;
    }
    button->sprite = Sprite_construct(resourceManager, renderer, tempSpriteResourceString);
    if (!button->sprite) {
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 6;
    }
    if (button->sprite->animationsCount < 3) {
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_SPRITE_NO_3_ANIMATIONS);
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 7;
    }
    TextParser_destruct(spriteTextParser);
    spriteTextResource->pointersCount--;
    return 0;
}

void Button_loadSoundResources(struct Button* button, struct ResourceManager* resourceManager,
                               struct TextParser* textParser) {
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

void Button_loadEventsResources(struct Button* button, struct ResourceManager* resourceManager, 
                                struct TextParser* textParser){
    char* tempFocusedEventResourceString = TextParser_getString(textParser,
                                                          BUTTON_SCENENODE_PARSER_FOCUSED_EVENT_RES_STRING, 0);
    if (!tempFocusedEventResourceString)
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_FOCUSED_GAME_EVENT_RES);
    char* tempPresedEventResourceString = TextParser_getString(textParser,
                                                          BUTTON_SCENENODE_PARSER_PRESSED_EVENT_RES_STRING, 0);
    if (!tempPresedEventResourceString)
        Logger_log(resourceManager->logger, BUTTON_SCENENODE_ERR_PRESSED_GAME_EVENT_RES);
    Button_changeFocusedEventResource(button, resourceManager, tempFocusedEventResourceString);
    Button_changePressedEventResource(button, resourceManager, tempPresedEventResourceString);
}

unsigned char Button_tryGetSettingsFromTextParser(struct Button* button, struct ResourceManager* resourceManager,
                                                  struct Renderer* renderer, struct TextParser* textParser) {
    unsigned char result = 0;
    Button_loadEventsResources(button, resourceManager, textParser);
    Button_loadSoundResources(button, resourceManager, textParser);
    result += Button_loadSpriteResource(button, resourceManager, renderer, textParser);
    result += Button_loadTextResource(button, resourceManager, renderer, textParser);
    if (result)
        return result;
    return 0;
}

struct Button* Button_construct(struct ResourceManager* const resourceManager, struct Renderer* renderer,
                                const char* const buttonResId) {
    struct Button* button = NULL;
    button = (struct Button*)calloc(1, sizeof(struct Button));
    if (!button)
        return NULL;
    SceneNode_init(&(button->sceneNode));
    button->sceneNode.sceneNodeTextResource = ResourceManager_loadTextResource(resourceManager, buttonResId, 0);
    if (!button->sceneNode.sceneNodeTextResource) {
        Button_destruct(button);
        return NULL;
    }
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructFromTextResource(resourceManager->logger, button->sceneNode.sceneNodeTextResource);
    if (!textParser) {
        Button_destruct(button);
        return NULL;
    }
    if (Button_tryGetSettingsFromTextParser(button, resourceManager, renderer, textParser)) {
        TextParser_destruct(textParser);
        Button_destruct(button);
        return NULL;
    }
    TextParser_destruct(textParser);
    button->sceneNode.control = Button_control;
    button->sceneNode.update = Button_update;
    button->sceneNode.render = Button_render;
    button->sceneNode.sound = Button_sound;
    button->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(BUTTON_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!button->sceneNode.type) {
        Button_destruct(button);
        return NULL;
    }
    strcpy(button->sceneNode.type, BUTTON_SCENENODE_PARSER_TYPE_STRING);
    button->isGeometryChanged = true;
    return button;
}

void Button_destruct(struct Button* button) {
    if (!button)
        return;
    if (button->sprite)
        Sprite_destruct(button->sprite);
    if (button->label)
        Text_destruct(button->label);
    if (button->focusedEventResource)
        button->focusedEventResource->pointersCount--;
    if (button->focusedSoundResource)
        button->focusedSoundResource->pointersCount--;
    if (button->pressedEventResource)
        button->pressedEventResource->pointersCount--;
    if (button->pressedSoundResource)
        button->pressedSoundResource->pointersCount--;
    if (button->focusedEvent)
        GameEvent_destruct(button->focusedEvent);
    if (button->pressedEvent)
        GameEvent_destruct(button->pressedEvent);
    if (button->sceneNode.sceneNodeTextResource)
        button->sceneNode.sceneNodeTextResource->pointersCount--;
    if (button->sceneNode.type)
        free(button->sceneNode.type);
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
    if (button->pressedEvent) {
         // TODO: EventManager_removeMeFromList(button->pressedEvent);
        GameEvent_destruct(button->pressedEvent);
    }
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
    if (button->focusedEvent) {
        // TODO: EventManager_removeMeFromList(button->focusedEvent);
        GameEvent_destruct(button->focusedEvent);
    }
    if (button->focusedEventResource)
        button->focusedEventResource->pointersCount--;
    button->focusedEventResource = newGameEventTextResource;
    button->focusedEvent = newGameEvent;
    return 0;
}

unsigned char Button_save(const struct Button* const button, struct ResourceManager* const resourceManager, 
                          const char* const buttonResId) {

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
                && mouseCoordinates.x <= (button->sprite->dstRect.x + button->sprite->dstRect.w) 
                && mouseCoordinates.y >= button->sprite->dstRect.y 
                && mouseCoordinates.y <= (button->sprite->dstRect.y + button->sprite->dstRect.h)) {
                if (!(button->state == Focused)) {
                    button->state = Focused;
                    button->isStateChanged = true;
                    EventManager_addEvent(eventManager, button->focusedEvent);
                }
                if (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONDOWN) {
                    button->state = Pressed;
                    button->isStateChanged = true;
                    EventManager_addEvent(eventManager, button->pressedEvent);
                }
                if (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONUP) {
                    button->state = Focused;
                    button->isStateChanged = true;
                }
            } else if (!(button->state == Normal)) {
                    button->state = Normal;
                    button->isStateChanged = true;
            }
        }
}

void Button_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager || !renderer)
        return;
    struct Button* button = (struct Button*)sceneNode;
    if (button->isStateChanged) {
        if (button->state == Normal)
            button->sprite->currentAnimation = 0;
        if (button->state == Focused)
            button->sprite->currentAnimation = 1;
        if (button->state == Pressed)
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
        button->label->sceneNode.coordinates = button->sceneNode.coordinates;
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
        if (button->state == Focused)
            Musican_playSound(musican, button->focusedSoundResource, 0);
        if (button->state == Pressed)
            Musican_playSound(musican, button->pressedSoundResource, 0);
    }
}
