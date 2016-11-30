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
 * @file Button.c
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing implementation of #Button.
 */
#include <assert.h>
#include "gui/Button.h"
#include "textParser/TextParser.h"

/**
 * @brief Error message strings for #Button.
 */
static const struct ButtonSceneNode_errorMessages {
    const char* const errFocusedGameEventRes;
    /**< Will be displayed when #TextParser have no ButtonSceneNode_parserStrings#focusedEventRes. */
    const char* const errPressedGameEventRes;
    /**< Will be displayed when #TextParser have no ButtonSceneNode_parserStrings#pressedEventRes. */
    const char* const errConstructingFocusedGameEvent;
    /**< Will be displayed when constructing Button#focusedEvent failed. */
    const char* const errConstructingPressedGameEvent;
    /**< Will be displayed when constructing Button#pressedEvent failed. */
    const char* const errFocusedSoundRes;
    /**< Will be displayed when #TextParser have no ButtonSceneNode_parserStrings#focusedSoundRes. */
    const char* const errPressedSoundRes;
    /**< Will be displayed when #TextParser have no ButtonSceneNode_parserStrings#presedSoundRes. */
    const char* const errSpriteRes;
    /**< Will be displayed when #TextParser have no ButtonSceneNode_parserStrings#spriteRes. */
    const char* const errSpriteNoEnoughAnimations;
    /**< Will be displayed when constructed #Sprite doesn't have enough animations for #Button. */
    const char* const errTextRes;
    /**< Will be displayed when #TextParser have no ButtonSceneNode_parserStrings#textRes. */
}ButtonSceneNode_errorMessages = {
    "Button_loadEventsResources: focusedEventResource string haven't found!",
    "Button_loadEventsResources: pressedEventResource string haven't found!",
    "Button_changeFocusedEventResource: constructing GameEvent failed!",
    "Button_changePressedEventResource: constructing GameEvent failed!",
    "Button_loadSoundResources: focusedSoundResource string haven't found!",
    "Button_loadSoundResources: pressedSoundResource string haven't found!",
    "Button_loadSpriteResource: spriteResource string haven't found!",
    "Button_loadSpriteResource: loaded Sprite doesn't contain 3 animations!",
    "Button_loadTextResource: textResource string haven't found!"};

/**
 * @brief Function for loading and constructing Button#label.
 * @param button Pointer to a #Button where to construct Button#label. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager for loading #Text resource. Can be NULL.
 * @param renderer Pointer to a #Renderer for constructing Button#label. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing Button#label. Can be NULL.
 * @param textParser Pointer to a #TextParser, where function will get 
 * ButtonSceneNode_parserStrings#textRes. Can be NULL.
 * @return #ButtonSceneNode_errors value.
 * @see #Button
 * @see #Text
 * @see #ResourceManager
 * @see #Renderer
 * @see #SceneNodeTypesRegistrar
 * @see #TextParser
 * @see #ButtonSceneNode_parserStrings
 * @see #ButtonSceneNode_errors
 */
static enum ButtonSceneNode_errors Button_loadTextResource(struct Button* button,
                                             struct ResourceManager* resourceManager,
                                             struct Renderer* renderer,
                                             struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                             struct TextParser* textParser) {
    assert(button);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    const char* const tempResId = TextParser_getString(textParser, ButtonSceneNode_parserStrings.textRes, 0);
    if (!tempResId) {
        Logger_log(resourceManager->logger, ButtonSceneNode_errorMessages.errTextRes);
        return BUTTON_ERR_NO_TEXT_RES;
    }
    button->label = (struct Text*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                               renderer,
                                                               sceneNodeTypesRegistrar,
                                                               tempResId,
                                                               TextSceneNode_parserStrings.type);
    if (!button->label)
        return BUTTON_ERR_CONSTRUCTIG_TEXT;
    return BUTTON_NO_ERRORS;
}

/**
 * @brief Function for loading and constructing Button#sprite.
 * @param button Pointer to a #Button where to construct Button#sprite. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager for loading #Sprite resource. Can be NULL.
 * @param renderer Pointer to a #Renderer for constructing Button#sprite. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing Button#sprite. Can be NULL.
 * @param textParser Pointer to a #TextParser, where function will get 
 * ButtonSceneNode_parserStrings#spriteRes. Can be NULL.
 * @return #ButtonSceneNode_errors value.
 * @see #Button
 * @see #Sprite
 * @see #ResourceManager
 * @see #Renderer
 * @see #SceneNodeTypesRegistrar
 * @see #TextParser
 * @see #ButtonSceneNode_parserStrings
 * @see #ButtonSceneNode_errors
 */
static enum ButtonSceneNode_errors Button_loadSpriteResource(struct Button* button,
                                               struct ResourceManager* resourceManager,
                                               struct Renderer* renderer,
                                               struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                               struct TextParser* textParser) {
    assert(button);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    const char* const tempResId = TextParser_getString(textParser, ButtonSceneNode_parserStrings.spriteRes, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, ButtonSceneNode_errorMessages.errSpriteRes);
        return BUTTON_ERR_NO_SPRITE_RES;
    }
    button->sprite = (struct Sprite*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                renderer,
                                                                sceneNodeTypesRegistrar,
                                                                tempResId,
                                                                SPRITE_SCENENODE_PARSER_TYPE_STRING);
    if (!button->sprite)
        return BUTTON_ERR_CONSTRUCTIG_SPRITE;
    if (button->sprite->animationsCount < 3) {
        Logger_log(resourceManager->logger, ButtonSceneNode_errorMessages.errSpriteNoEnoughAnimations);
        return BUTTON_ERR_NOT_ENOUGH_ANIMATIONS;
    }
    return BUTTON_NO_ERRORS;
}

/**
 * @brief Function for loading and constructing Button#focusedSoundResource and Button#pressedSoundResource.
 * @param button Pointer to a #Button where to construct Button#focusedSoundResource 
 * and Button#pressedSoundResource. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager for loading sound resources. Can be NULL.
 * @param textParser Pointer to a #TextParser, where function will get 
 * ButtonSceneNode_parserStrings#focusedSoundRes 
 * and ButtonSceneNode_parserStrings#pressedSoundRes. Can be NULL.
 * @see #Button
 * @see #SoundResource
 * @see #ResourceManager
 * @see #TextParser
 * @see #ButtonSceneNode_parserStrings
 */
static void Button_loadSoundResources(struct Button* button,
                                      struct ResourceManager* resourceManager,
                                      struct TextParser* textParser) {
    assert(button);
    assert(resourceManager);
    assert(textParser);
    const char* const tempFocusedSoundResourceString = TextParser_getString(textParser,
                                                                ButtonSceneNode_parserStrings.focusedSoundRes, 0);
    if (!tempFocusedSoundResourceString)
        Logger_log(resourceManager->logger, ButtonSceneNode_errorMessages.errFocusedSoundRes);
    button->focusedSoundResource = ResourceManager_loadSoundResource(resourceManager, tempFocusedSoundResourceString);
    const char* const tempPressedSoundResourceString = TextParser_getString(textParser,
                                                                ButtonSceneNode_parserStrings.pressedSoundRes, 0);
    if (!tempPressedSoundResourceString)
        Logger_log(resourceManager->logger, ButtonSceneNode_errorMessages.errPressedSoundRes);
    button->pressedSoundResource = ResourceManager_loadSoundResource(resourceManager, tempPressedSoundResourceString);
}

/**
 * @brief Function for loading and constructing Button#focusedEvent and Button#pressedEvent.
 * Also, it will update Button#focusedEventResource and Button#pressedEventResource.
 * @param button Pointer to a #Button where to construct events. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager for loading #GameEvent resources. Can be NULL.
 * @param textParser Pointer to a #TextParser, where function will get 
 * ButtonSceneNode_parserStrings#focusedEventRes
 * and ButtonSceneNode_parserStrings#pressedEventRes. Can be NULL.
 * @see #Button
 * @see #GameEvent
 * @see #ResourceManager
 * @see #TextParser
 * @see #ButtonSceneNode_parserStrings
 */
static void Button_loadEventsResources(struct Button* button,
                                       struct ResourceManager* resourceManager,
                                       struct TextParser* textParser){
    assert(button);
    assert(resourceManager);
    assert(textParser);
    const char* const tempFocusedEventResourceString = TextParser_getString(textParser,
                                                          ButtonSceneNode_parserStrings.focusedEventRes, 0);
    if (!tempFocusedEventResourceString)
        Logger_log(resourceManager->logger, ButtonSceneNode_errorMessages.errFocusedGameEventRes);
    const char* const tempPressedEventResourceString = TextParser_getString(textParser,
                                                          ButtonSceneNode_parserStrings.pressedEventRes, 0);
    if (!tempPressedEventResourceString)
        Logger_log(resourceManager->logger, ButtonSceneNode_errorMessages.errPressedGameEventRes);
    Button_changeFocusedEventResource(button, resourceManager, tempFocusedEventResourceString);
    Button_changePressedEventResource(button, resourceManager, tempPressedEventResourceString);
}

/**
 * @brief Function for loading settings and initializing #Button from #TextParser.
 * @param button Pointer to a #Button which will be initialized. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager for loading required resources. Can be NULL.
 * @param renderer Pointer to a #Renderer for constructing Button#sprite and Button#label. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing Button#sprite and Button#label. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings. Can be NULL.
 * @return #ButtonSceneNode_errors value.
 * @see #Button
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #ButtonSceneNode_parserStrings
 * @see #ButtonSceneNode_errors
 */
static enum ButtonSceneNode_errors Button_tryGetSettingsFromTextParser(struct Button* button,
                                                         struct ResourceManager* resourceManager,
                                                         struct Renderer* renderer,
                                                         struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                         struct TextParser* textParser) {
    assert(button);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    enum ButtonSceneNode_errors loadingSpriteResult = BUTTON_NO_ERRORS;
    enum ButtonSceneNode_errors loadingTextResult = BUTTON_NO_ERRORS;
    Button_loadEventsResources(button, resourceManager, textParser);
    Button_loadSoundResources(button, resourceManager, textParser);
    loadingSpriteResult = Button_loadSpriteResource(button, resourceManager, renderer,
                                                    sceneNodeTypesRegistrar, textParser);
    loadingTextResult = Button_loadTextResource(button, resourceManager, renderer,
                                                sceneNodeTypesRegistrar, textParser);
    button->labelOffset.x = (int)TextParser_getInt(textParser, ButtonSceneNode_parserStrings.labelOffset, 0);
    button->labelOffset.y = (int)TextParser_getInt(textParser, ButtonSceneNode_parserStrings.labelOffset, 1);
    if (loadingSpriteResult || loadingTextResult)
        return BUTTON_ERR_LOADING_SETTINGS;
    return BUTTON_NO_ERRORS;
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
    button->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(ButtonSceneNode_parserStrings.type) + 1));
    if (!button->sceneNode.type) {
        Button_destruct((struct SceneNode*)button);
        return NULL;
    }
    strcpy(button->sceneNode.type, ButtonSceneNode_parserStrings.type);
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
    TextResource_decreasePointersCounter(tempButton->focusedEventResource);
    SoundResource_decreasePointersCounter(tempButton->focusedSoundResource);
    TextResource_decreasePointersCounter(tempButton->pressedEventResource);
    SoundResource_decreasePointersCounter(tempButton->pressedSoundResource);
    if (tempButton->focusedEvent)
        tempButton->focusedEvent->isNeeded = false;
    if (tempButton->pressedEvent)
        tempButton->pressedEvent->isNeeded = false;
    TextResource_decreasePointersCounter(button->sceneNodeTextResource);
    if (button->type)
        free(button->type);
    free(button);
}

enum ButtonSceneNode_errors Button_changePressedEventResource(struct Button* button, struct ResourceManager* resourceManager,
                                                const char* const pressedEventResId) {
    if (!button || !resourceManager || !pressedEventResId)
        return BUTTON_ERR_NULL_ARGUMENT;
    struct TextResource* newGameEventTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                                     pressedEventResId, 0);
    if (!newGameEventTextResource)
        return BUTTON_ERR_NO_PRESSED_EVENT_RES;
    struct TextParser* textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                                         newGameEventTextResource);
    if (!textParser) {
        TextResource_decreasePointersCounter(newGameEventTextResource);
        return BUTTON_ERR_CONSTRUCTIG_TEXT_PARSER;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)button);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, ButtonSceneNode_errorMessages.errConstructingPressedGameEvent);
        TextResource_decreasePointersCounter(newGameEventTextResource);
        TextParser_destruct(textParser);
        return BUTTON_ERR_CONSTRUCTIG_PRESSED_EVENT;
    }
    TextParser_destruct(textParser);
    if (button->pressedEvent)
        button->pressedEvent->isNeeded = false;
    TextResource_decreasePointersCounter(button->pressedEventResource);
    button->pressedEventResource = newGameEventTextResource;
    button->pressedEvent = newGameEvent;
    return BUTTON_NO_ERRORS;
}

enum ButtonSceneNode_errors Button_changeFocusedEventResource(struct Button* button, struct ResourceManager* resourceManager,
                                                const char* const focusedEventResId) {
    if (!button || !resourceManager || !focusedEventResId)
        return BUTTON_ERR_NULL_ARGUMENT;
    struct TextResource* newGameEventTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                                     focusedEventResId, 0);
    if (!newGameEventTextResource)
        return BUTTON_ERR_NO_FOCUSED_EVENT_RES;
    struct TextParser* textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                                         newGameEventTextResource);
    if (!textParser) {
        TextResource_decreasePointersCounter(newGameEventTextResource);
        return BUTTON_ERR_CONSTRUCTIG_TEXT_PARSER;
    }
    struct GameEvent* newGameEvent = GameEvent_constructFromTextParser(textParser, (struct SceneNode*)button);
    if (!newGameEvent) {
        Logger_log(resourceManager->logger, ButtonSceneNode_errorMessages.errConstructingFocusedGameEvent);
        TextResource_decreasePointersCounter(newGameEventTextResource);
        TextParser_destruct(textParser);
        return BUTTON_ERR_CONSTRUCTIG_FOCUSED_EVENT;
    }
    TextParser_destruct(textParser);
    if (button->focusedEvent)
        button->focusedEvent->isNeeded = false;
    TextResource_decreasePointersCounter(button->focusedEventResource);
    button->focusedEventResource = newGameEventTextResource;
    button->focusedEvent = newGameEvent;
    return BUTTON_NO_ERRORS;
}

enum ButtonSceneNode_errors Button_save(const struct Button* const button, struct ResourceManager* const resourceManager, 
                          const char* const buttonResId) {
    if (!button || !resourceManager || !buttonResId)
        return BUTTON_ERR_NULL_ARGUMENT;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return BUTTON_ERR_CONSTRUCTIG_TEXT_PARSER;
    result += (TextParser_addString(textParser, TextParser_standartTypeString, ButtonSceneNode_parserStrings.type) != 0);
    result += (TextParser_addString(textParser, ButtonSceneNode_parserStrings.spriteRes,
                                   button->sprite->dynamicSceneNode.sceneNode.sceneNodeTextResource->id) != 0);
    result += (TextParser_addString(textParser, ButtonSceneNode_parserStrings.textRes,
                                   button->label->dynamicSceneNode.sceneNode.sceneNodeTextResource->id) != 0);
    result += (TextParser_addString(textParser, ButtonSceneNode_parserStrings.focusedEventRes,
                                   button->focusedEventResource->id) != 0);
    result += (TextParser_addString(textParser, ButtonSceneNode_parserStrings.pressedEventRes,
                                   button->pressedEventResource->id) != 0);
    result += (TextParser_addString(textParser, ButtonSceneNode_parserStrings.focusedSoundRes,
                                   button->focusedSoundResource->id) != 0);
    result += (TextParser_addString(textParser, ButtonSceneNode_parserStrings.pressedSoundRes,
                                   button->pressedSoundResource->id) != 0);
    result += (TextParser_addInt(textParser, ButtonSceneNode_parserStrings.labelOffset,
                                   button->labelOffset.x) != 0);
    result += (TextParser_addInt(textParser, ButtonSceneNode_parserStrings.labelOffset,
                                   button->labelOffset.y) != 0);
    char* tempString = TextParser_convertToText(textParser);
    result += (textParser->lastError != 0);
    result += (TextResource_updateContent(button->sceneNode.sceneNodeTextResource, tempString) != 0);
    result += (ResourceManager_saveTextResource(resourceManager,
                                               button->sceneNode.sceneNodeTextResource, buttonResId) != 0);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    if (result)
        return BUTTON_ERR_SAVING;
    return BUTTON_NO_ERRORS;
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
                if (button->state == BUTTON_STATE_NORMAL) {
                    button->state = BUTTON_STATE_FOCUSED;
                    button->isStateChanged = true;
                    EventManager_addEvent(eventManager, button->focusedEvent);
                }
                if (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONDOWN) {
                    button->state = BUTTON_STATE_PRESSED;
                    button->isStateChanged = true;
                    
                }
                if (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONUP) {
                    if (button->state == BUTTON_STATE_PRESSED)
                        EventManager_addEvent(eventManager, button->pressedEvent);
                    button->state = BUTTON_STATE_FOCUSED;
                    button->isStateChanged = true;
                }
            } else {
                if (button->state == BUTTON_STATE_FOCUSED) {
                    button->state = BUTTON_STATE_NORMAL;
                    button->isStateChanged = true;
                    EventManager_removeEvent(eventManager, button->focusedEvent);
                }
                if (button->state == BUTTON_STATE_PRESSED 
                    && (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONUP 
                        || eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONDOWN)) {
                    button->state = BUTTON_STATE_NORMAL;
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
        if (button->state == BUTTON_STATE_NORMAL)
            button->sprite->currentAnimation = 0;
        if (button->state == BUTTON_STATE_FOCUSED)
            button->sprite->currentAnimation = 1;
        if (button->state == BUTTON_STATE_PRESSED)
            button->sprite->currentAnimation = 2;
        button->sprite->currentFrame = 0;
        button->sprite->renderingsCounter = 0;
    }
    if (button->isGeometryChanged) {
        button->sprite->dynamicSceneNode.sceneNode.coordinates = button->sceneNode.coordinates;
        button->label->dynamicSceneNode.sceneNode.coordinates.x = button->sceneNode.coordinates.x + button->labelOffset.x;
        button->label->dynamicSceneNode.sceneNode.coordinates.y = button->sceneNode.coordinates.y + button->labelOffset.y;
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
        if (button->state == BUTTON_STATE_FOCUSED)
            Musican_playSound(musican, button->focusedSoundResource, 0);
        if (button->state == BUTTON_STATE_PRESSED)
            Musican_playSound(musican, button->pressedSoundResource, 0);
    }
}
