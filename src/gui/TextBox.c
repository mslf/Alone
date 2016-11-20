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
 * @file TextBox.c
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing implementation of #TextBox.
 */
#include "gui/TextBox.h"

/**
 * @brief Error message strings for #TextBox.
 */
static const struct TextBoxSceneNode_errorMessages {
    const char* const errNoButtonRes;
    /**< Will be displayed when #TextParser have no string, specified in TextBoxSceneNode_parserStrings#buttonRes. */
    const char* const errDefButtonLabelStringLengthAboveMax;
    /**< Will be displayed when TextBox#box#label#text string length is more than length, 
     *specified in ContextMenuSceneNode_parserStringss#maxLength. */
}TextBoxSceneNode_errorMessages = {
    "TextBox_tryGetSettingsFromTextParser: buttonResource string haven't found!",
    "TextBox_tryGetSettingsFromTextParser: default label string length > MAX_LENGTH!"};

/**
 * @brief Function for loading settings and initializing #TextBox from #TextParser.
 * @param textBox Pointer to a #TextBox which will be initialized. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager for loading required resources. Can be NULL.
 * @param renderer Pointer to a #Renderer for constructing TextBox#box. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing TextBox#box. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings. Can be NULL.
 * @return #TextBoxSceneNode_errors value.
 * @see #TextBox
 * @see #Button
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #TextBoxSceneNode_parserStrings
 * @see #TextBoxSceneNode_errors
 */
static enum TextBoxSceneNode_errors TextBox_tryGetSettingsFromTextParser(struct TextBox* textBox,
                                                         struct ResourceManager* resourceManager, struct Renderer* renderer,
                                                         struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                         struct TextParser* textParser) {
    if (!textBox || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return TEXT_BOX_ERR_NULL_ARGUMENT;
    const char* tempResId = TextParser_getString(textParser, TextBoxSceneNode_parserStrings.buttonRes, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, TextBoxSceneNode_errorMessages.errNoButtonRes);
        return TEXT_BOX_ERR_NO_BUTTON_RES;
    }
    textBox->box = (struct Button*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                renderer,
                                                                sceneNodeTypesRegistrar,
                                                                tempResId,
                                                                ButtonSceneNode_parserStrings.type);
    if (!textBox->box)
        return TEXT_BOX_ERR_CONSTRUCTIG_BUTTON;
    textBox->maxLength = TextParser_getInt(textParser, TextBoxSceneNode_parserStrings.maxLength, 0);
    if (textBox->maxLength == 0)
        textBox->maxLength = TEXT_BOX_SCENENODE_MAX_LENGTH;
     if (strlen(textBox->box->label->text) > textBox->maxLength) {
        Logger_log(renderer->logger, TextBoxSceneNode_errorMessages.errDefButtonLabelStringLengthAboveMax);
        return TEXT_BOX_ERR_LENGTH_ABOVE_MAX;
    }
    if (strlen(textBox->box->label->text) < textBox->maxLength) {
        textBox->string = (char*)malloc(sizeof(char) * (strlen(textBox->box->label->text) + 1));
        if (!textBox->string) {
            return TEXT_BOX_ERR_ALLOC_STRING;
        }
        textBox->allocatedChars = strlen(textBox->box->label->text) + 1;
        strcpy(textBox->string, textBox->box->label->text);
        textBox->stringLength = strlen(textBox->box->label->text);
    }
    return TEXT_BOX_NO_ERRORS;
}

struct SceneNode* TextBox_construct(struct ResourceManager* const resourceManager,
                                    struct Renderer* const renderer,
                                    struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                    struct TextParser* const textParser) {
    if (!resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return NULL;
    struct TextBox* textBox = NULL;
    textBox = (struct TextBox*)calloc(1, sizeof(struct TextBox));
    if (!textBox)
        return NULL;
    SceneNode_init(&(textBox->sceneNode));
    if (TextBox_tryGetSettingsFromTextParser(textBox, resourceManager, renderer, sceneNodeTypesRegistrar, textParser)) {
        TextBox_destruct((struct SceneNode*)textBox);
        return NULL;
    }
    textBox->sceneNode.control = TextBox_control;
    textBox->sceneNode.update = TextBox_update;
    textBox->sceneNode.render = TextBox_render;
    textBox->sceneNode.sound = TextBox_sound;
    textBox->sceneNode.destruct = TextBox_destruct;
    textBox->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(TextBoxSceneNode_parserStrings.type) + 1));
    if (!textBox->sceneNode.type) {
        TextBox_destruct((struct SceneNode*)textBox);
        return NULL;
    }
    strcpy(textBox->sceneNode.type, TextBoxSceneNode_parserStrings.type);
    textBox->isGeometryChanged = true;
    return (struct SceneNode*)textBox;
}

void TextBox_destruct(struct SceneNode* textBox) {
    if (!textBox)
        return;
    struct TextBox* tempTextBox = (struct TextBox*)textBox;
    if (tempTextBox->box)
        Button_destruct((struct SceneNode*)tempTextBox->box);
    if (tempTextBox->string)
        free(tempTextBox->string);
    TextResource_decreasePointersCounter(textBox->sceneNodeTextResource);
    if (textBox->type)
        free(textBox->type);
    free(textBox);
}

enum TextBoxSceneNode_errors TextBox_save(
        const struct TextBox* const textBox, struct ResourceManager* const resourceManager,
        const char* const textBoxResId) {
    if (!textBox || !resourceManager || !textBoxResId)
        return TEXT_BOX_ERR_NULL_ARGUMENT;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return TEXT_BOX_ERR_CONSTRUCTIG_TEXT_PARSER;
    result += TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING, TextBoxSceneNode_parserStrings.type);
    result += TextParser_addString(textParser, TextBoxSceneNode_parserStrings.buttonRes,
                                   textBox->box->sceneNode.sceneNodeTextResource->id);
    result += TextParser_addInt(textParser, TextBoxSceneNode_parserStrings.maxLength, textBox->maxLength);
    char* tempString = TextParser_convertToText(textParser);
    result += textParser->lastError;
    result += TextResource_updateContent(textBox->sceneNode.sceneNodeTextResource, tempString);
    result += ResourceManager_saveTextResource(resourceManager,
                                               textBox->sceneNode.sceneNodeTextResource, textBoxResId);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    if (result)
        return TEXT_BOX_ERR_SAVING;
    return TEXT_BOX_NO_ERRORS;
}

/**
 * @brief Reallocates TextBox#string and sets TextBox#allocatedChars.
 * If requiredLength specified (not 0) then TextBox#allocatedCharsh will be it.
 * Else, TextBox#allocatedChars will be sum of old TextBox#allocatedChars 
 * and #TEXT_BOX_SCENENODE_REALLOC_STRING_LENGTH_STEP.
 * @param textBox Pointer to a #TextBox, where string will be reallocated. Can be NULL.
 * @param requiredLength Nes size of TextBox#string.
 * @return TextBoxSceneNode_errors value.
 * @see #TextBoxSceneNode_errors
 * @see #TextBoxSceneNode_constants.
 */
static enum TextBoxSceneNode_errors TextBox_reallocateString(struct TextBox* textBox, size_t requiredLength) {
    if (!textBox)
        return TEXT_BOX_ERR_NULL_ARGUMENT;
    char* newText = NULL;
    size_t newSize;
    if (requiredLength)
        newSize = requiredLength;
    else
        newSize = textBox->allocatedChars + TEXT_BOX_SCENENODE_REALLOC_STRING_LENGTH_STEP;
    newText = (char*)malloc(sizeof(char) * (newSize + 1));
    if (!newText)
        return TEXT_BOX_ERR_ALLOC_STRING;
    strcpy(newText, textBox->string);
    free(textBox->string);
    textBox->string = newText;
    textBox->allocatedChars = newSize;
    return TEXT_BOX_NO_ERRORS;
}

enum TextBoxSceneNode_errors TextBox_changeString (struct TextBox* textBox, const char* const newString) {
    if (!textBox || !newString)
        return TEXT_BOX_ERR_NULL_ARGUMENT;
    if (strlen(newString) <= textBox->maxLength) {
        if (strlen(newString) >= textBox->allocatedChars)
            if (TextBox_reallocateString(textBox, strlen(newString)))
                return TEXT_BOX_ERR_ALLOC_STRING;
        strcpy(textBox->string, newString);
        textBox->string[strlen(newString)] = 0;
        textBox->stringLength = strlen(newString);
        textBox->isStringChanged = true;
    }
    return TEXT_BOX_NO_ERRORS;
}

void TextBox_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {
    if (!sceneNode || !eventManager)
        return;
    struct TextBox* textBox = (struct TextBox*)sceneNode;
    Button_control((struct SceneNode*)textBox->box, eventManager);
    SDL_Point mouseCoordinates;
    SDL_GetMouseState(&mouseCoordinates.x, &mouseCoordinates.y);
    if (textBox->box->state == BUTTON_STATE_PRESSED) {
        //SDL_StartTextInput();
        // FIXME Need to find properly place for that
        textBox->haveFocus = true;
    }
    size_t i;
    if (textBox->box->state == BUTTON_STATE_NORMAL)
        for (i = 0; i < eventManager->sdlEventsCount; i++)
            if (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONDOWN ) {
                if (!(mouseCoordinates.x >= textBox->box->sprite->dstRect.x 
                    && mouseCoordinates.x < (textBox->box->sprite->dstRect.x + textBox->box->sprite->dstRect.w) 
                    && mouseCoordinates.y >= textBox->box->sprite->dstRect.y 
                    && mouseCoordinates.y < (textBox->box->sprite->dstRect.y + textBox->box->sprite->dstRect.h))) {
                    textBox->haveFocus = false;
                    //SDL_StopTextInput();
                    // FIXME Need to find properly place for that
                }
            }
    if (textBox->haveFocus) {
        textBox->box->state = BUTTON_STATE_FOCUSED;
        for (i = 0; i < eventManager->sdlEventsCount; i++)
            if (eventManager->sdlEventsList[i].type == SDL_KEYDOWN) {
                if (eventManager->sdlEventsList[i].key.keysym.sym == SDLK_BACKSPACE && textBox->stringLength > 0) {
                    textBox->stringLength--;
                    if (textBox->stringLength == 0)
                        textBox->string[textBox->stringLength] = ' ';
                    else
                        textBox->string[textBox->stringLength] = 0;
                    textBox->isStringChanged = true;
                } else if (eventManager->sdlEventsList[i].key.keysym.scancode == SDL_SCANCODE_C
                            && SDL_GetModState() & KMOD_CTRL) {
                    SDL_SetClipboardText(textBox->string);
                } else if (eventManager->sdlEventsList[i].key.keysym.scancode == SDL_SCANCODE_V
                            && SDL_GetModState() & KMOD_CTRL) {
                    char* tempString = SDL_GetClipboardText();
                    if (strlen(tempString) <= textBox->maxLength) {
                        if (TextBox_changeString(textBox, tempString)) {
                            SDL_free(tempString);
                            return;
                        }
                        SDL_free(tempString);
                    }
                }
            } else if(eventManager->sdlEventsList[i].type == SDL_TEXTINPUT) {
                    if(!((eventManager->sdlEventsList[i].text.text[0] == 'c' 
                        || eventManager->sdlEventsList[i].text.text[0] == 'C') 
                        && (eventManager->sdlEventsList[i].text.text[0] == 'v' 
                        || eventManager->sdlEventsList[i].text.text[0] == 'V') 
                        && SDL_GetModState() & KMOD_CTRL) && textBox->stringLength < textBox->maxLength) {
                        size_t addingStringLength = strlen(eventManager->sdlEventsList[i].text.text);
                        if (textBox->stringLength + addingStringLength >= textBox->allocatedChars)
                            if (TextBox_reallocateString(textBox, 0))
                                return;
                        strcpy(&(textBox->string[textBox->stringLength]), eventManager->sdlEventsList[i].text.text);
                        textBox->stringLength += addingStringLength;
                        textBox->isStringChanged = true;
                    }
                }
    }
}

void TextBox_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager || !renderer)
        return;
    struct TextBox* textBox = (struct TextBox*)sceneNode;
    if (textBox->isStringChanged) {
        Text_regenerateTexture(textBox->box->label, NULL, renderer,
                               textBox->string, textBox->box->label->fontPath,
                               textBox->box->label->size, textBox->box->label->color);
        textBox->isStringChanged = false;
    }
    if (textBox->isGeometryChanged) {
        textBox->box->sceneNode.coordinates = textBox->sceneNode.coordinates;
        textBox->box->sceneNode.flip = textBox->sceneNode.flip;
        textBox->box->sceneNode.scaleX = textBox->sceneNode.scaleX;
        textBox->box->sceneNode.scaleY = textBox->sceneNode.scaleY;
        textBox->box->isGeometryChanged = true;
        textBox->isGeometryChanged = false;
    }
    Button_update((struct SceneNode*)textBox->box, eventManager, renderer);
}

void TextBox_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct TextBox* textBox = (struct TextBox*)sceneNode;
    Button_render((struct SceneNode*)textBox->box, renderer);
}

void TextBox_sound(struct SceneNode* sceneNode, struct Musican* musican) {
    if (!sceneNode || !musican)
        return;
    struct TextBox* textBox = (struct TextBox*)sceneNode;
    Button_sound((struct SceneNode*)textBox->box, musican);
}
