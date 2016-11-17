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
#include "gui/TextBox.h"

const char* const TEXT_BOX_SCENENODE_ERR_BUTTON_RES = 
        "TextBox_tryGetSettingsFromTextParser: buttonResource string haven't found!";
const char* const TEXT_BOX_SCENENODE_ERR_LENGTH = 
        "TextBox_tryGetSettingsFromTextParser: default string length > MAX_LENGTH!";

static unsigned char TextBox_tryGetSettingsFromTextParser(struct TextBox* textBox,
                                                         struct ResourceManager* resourceManager, struct Renderer* renderer,
                                                         struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                         struct TextParser* textParser) {
    if (!textBox || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    const char* tempResId = TextParser_getString(textParser, TEXT_BOX_SCENENODE_PARSER_BUTTON_RES_STRING, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, TEXT_BOX_SCENENODE_ERR_BUTTON_RES);
        return 2;
    }
    textBox->box = (struct Button*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                renderer,
                                                                sceneNodeTypesRegistrar,
                                                                tempResId,
                                                                ButtonSceneNode_parserStrings.type);
    if (!textBox->box)
        return 3;
    if (strlen(textBox->box->label->text) > TEXT_BOX_SCEENODE_MAX_LENGTH) {
        Logger_log(renderer->logger, TEXT_BOX_SCENENODE_ERR_LENGTH);
        return 4;
    }
    textBox->maxLength = TextParser_getInt(textParser, TEXT_BOX_SCENENODE_PARSER_MAX_LENGTH, 0);
    if (textBox->maxLength == 0)
        textBox->maxLength = TEXT_BOX_SCEENODE_MAX_LENGTH;
    if (strlen(textBox->box->label->text) < textBox->maxLength) {
        textBox->string = (char*)malloc(sizeof(char) * (strlen(textBox->box->label->text) + 1));
        if (!textBox->string) {
            return 5;
        }
        textBox->allocatedChars = strlen(textBox->box->label->text) + 1;
        strcpy(textBox->string, textBox->box->label->text);
        textBox->stringLength = strlen(textBox->box->label->text);
    }
    return 0;
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
    textBox->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(TEXT_BOX_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!textBox->sceneNode.type) {
        TextBox_destruct((struct SceneNode*)textBox);
        return NULL;
    }
    strcpy(textBox->sceneNode.type, TEXT_BOX_SCENENODE_PARSER_TYPE_STRING);
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

unsigned char TextBox_save(
        const struct TextBox* const textBox, struct ResourceManager* const resourceManager,
        const char* const textBoxResId) {
    if (!textBox || !resourceManager || !textBoxResId)
        return 1;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return 2;
    result += TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING, TEXT_BOX_SCENENODE_PARSER_TYPE_STRING);
    result += TextParser_addString(textParser, TEXT_BOX_SCENENODE_PARSER_BUTTON_RES_STRING,
                                   textBox->box->sceneNode.sceneNodeTextResource->id);
    result += TextParser_addInt(textParser, TEXT_BOX_SCENENODE_PARSER_MAX_LENGTH, textBox->maxLength);
    char* tempString = TextParser_convertToText(textParser);
    result += textParser->lastError;
    result += TextResource_updateContent(textBox->sceneNode.sceneNodeTextResource, tempString);
    result += ResourceManager_saveTextResource(resourceManager,
                                               textBox->sceneNode.sceneNodeTextResource, textBoxResId);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    return result;
}

static unsigned char TextBox_reallocateString(struct TextBox* textBox, size_t requiredLength) {
    if (!textBox)
        return 1;
    char* newText = NULL;
    size_t newSize;
    if (requiredLength)
        newSize = requiredLength;
    else
        newSize = textBox->allocatedChars + TEXT_BOX_SCENENODE_REALLOC_STRING_LENGTH_STEP;
    newText = (char*)malloc(sizeof(char) * (newSize + 1));
    if (!newText)
        return 2;
    strcpy(newText, textBox->string);
    free(textBox->string);
    textBox->string = newText;
    textBox->allocatedChars = newSize;
    return 0;
}

unsigned char TextBox_changeString (struct TextBox* textBox, const char* const newString) {
    if (!textBox || !newString)
        return 1;
    if (strlen(newString) <= textBox->maxLength) {
        if (strlen(newString) >= textBox->allocatedChars)
            if (TextBox_reallocateString(textBox, strlen(newString)))
                return 2;
        strcpy(textBox->string, newString);
        textBox->string[strlen(newString)] = 0;
        textBox->stringLength = strlen(newString);
        textBox->isStringChanged = true;
    }
    return 0;
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
