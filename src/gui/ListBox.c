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
#include "gui/ListBox.h"

const char* const LIST_BOX_SCENENODE_ERR_TEXT_BOX_RES = 
        "ListBox_loadTextBoxResource: textBoxResource string haven't found!";
const char* const LIST_BOX_SCENENODE_ERR_BUTTON_RES = 
        "ListBox_loadButtonResource: buttonResource string haven't found!";
const char* const LIST_BOX_SCENENODE_ERR_CONTEXT_MENU_RES = 
        "ListBox_loadContextMenuResource: contextMenuResource string haven't found!";
const char* const LIST_BOX_SCENENODE_ERR_CONTEXT_MENU_LENGTH_RES = 
        "ListBox_loadContextMenuResource: menuOption string length in created ContextMenu > max string length in created TextBox!";

static unsigned char ListBox_loadContextMenuResource(struct ListBox* listBox,
                                               struct ResourceManager* resourceManager,
                                               struct Renderer* renderer,
                                               struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                               struct TextParser* textParser) {
    if (!listBox || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    const char* tempResId = TextParser_getString(textParser, LIST_BOX_SCENENODE_PARSER_CONTEXT_MENU_RES_STRING, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, LIST_BOX_SCENENODE_ERR_CONTEXT_MENU_RES);
        return 2;
    }
    listBox->contextMenu = (struct ContextMenu*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                renderer,
                                                                sceneNodeTypesRegistrar,
                                                                tempResId,
                                                                ContextMenuSceneNode_parserString.type);
    if (!listBox->contextMenu)
        return 3;
    if (listBox->textBox) {
        size_t i;
        for (i = 0; i < listBox->contextMenu->menuOptionsCount; i++)
            if (strlen(listBox->contextMenu->menuOptionsList[i]->label->text) > listBox->textBox->maxLength) {
                char tempErrString[600];
                sprintf(tempErrString, "%s MenuOption string: %s",
                        LIST_BOX_SCENENODE_ERR_CONTEXT_MENU_LENGTH_RES,
                        listBox->contextMenu->menuOptionsList[i]->label->text);
                Logger_log(renderer->logger, tempErrString);
                return 4;
            }
    }
    return 0;
}            

static unsigned char ListBox_loadButtonResource(struct ListBox* listBox,
                                               struct ResourceManager* resourceManager,
                                               struct Renderer* renderer,
                                               struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                               struct TextParser* textParser) {
    if (!listBox || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    const char* tempResId = TextParser_getString(textParser, LIST_BOX_SCENENODE_PARSER_BUTTON_RES_STRING, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, LIST_BOX_SCENENODE_ERR_BUTTON_RES);
        return 2;
    }
    listBox->button = (struct Button*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                renderer,
                                                                sceneNodeTypesRegistrar,
                                                                tempResId,
                                                                ButtonSceneNode_parserStrings.type);
    if (!listBox->button)
        return 3;
    return 0;
}        
        
static unsigned char ListBox_loadTextBoxResource(struct ListBox* listBox,
                                               struct ResourceManager* resourceManager,
                                               struct Renderer* renderer,
                                               struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                               struct TextParser* textParser) {
    if (!listBox || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    const char* tempResId = TextParser_getString(textParser, LIST_BOX_SCENENODE_PARSER_TEXT_BOX_RES_STRING, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, LIST_BOX_SCENENODE_ERR_TEXT_BOX_RES);
        return 2;
    }
    listBox->textBox = (struct TextBox*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                renderer,
                                                                sceneNodeTypesRegistrar,
                                                                tempResId,
                                                                TEXT_BOX_SCENENODE_PARSER_TYPE_STRING);
    if (!listBox->textBox)
        return 3;
    return 0;
}

static unsigned char ListBox_tryGetSettingsFromTextParser(struct ListBox* listBox,
                                                struct ResourceManager* resourceManager, struct Renderer* renderer,
                                                struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                struct TextParser* textParser) {
    if (!listBox || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    unsigned char result = 0;
    result += ListBox_loadTextBoxResource(listBox, resourceManager, renderer, sceneNodeTypesRegistrar, textParser);
    result += ListBox_loadButtonResource(listBox, resourceManager, renderer, sceneNodeTypesRegistrar, textParser);
    result += ListBox_loadContextMenuResource(listBox, resourceManager, renderer, sceneNodeTypesRegistrar, textParser);
    return result;
}

struct SceneNode* ListBox_construct(struct ResourceManager* const resourceManager,
                                    struct Renderer* const renderer,
                                    struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                    struct TextParser* const textParser) {
    if (!resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return NULL;
    struct ListBox* listBox = NULL;
    listBox = (struct ListBox*)calloc(1, sizeof(struct ListBox));
    if (!listBox)
        return NULL;
    SceneNode_init(&(listBox->sceneNode));
    if (ListBox_tryGetSettingsFromTextParser(listBox, resourceManager, renderer, sceneNodeTypesRegistrar, textParser)) {
        ListBox_destruct((struct SceneNode*)listBox);
        return NULL;
    }
    listBox->sceneNode.control = ListBox_control;
    listBox->sceneNode.update = ListBox_update;
    listBox->sceneNode.render = ListBox_render;
    listBox->sceneNode.sound = ListBox_sound;
    listBox->sceneNode.destruct = ListBox_destruct;
    listBox->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(LIST_BOX_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!listBox->sceneNode.type) {
        ListBox_destruct((struct SceneNode*)listBox);
        return NULL;
    }
    strcpy(listBox->sceneNode.type, LIST_BOX_SCENENODE_PARSER_TYPE_STRING);
    listBox->isGeometryChanged = true;
    return (struct SceneNode*)listBox;
}

void ListBox_destruct(struct SceneNode* listBox) {
    if (!listBox)
        return;
    struct ListBox* tempListBox = (struct ListBox*)listBox;
    if (tempListBox->textBox)
        TextBox_destruct((struct SceneNode*)tempListBox->textBox);
    if (tempListBox->button)
        Button_destruct((struct SceneNode*)tempListBox->button);
    if (tempListBox->button)
        ContextMenu_destruct((struct SceneNode*)tempListBox->contextMenu);
    TextResource_decreasePointersCounter(listBox->sceneNodeTextResource);
    if (listBox->type)
        free(listBox->type);
    free(listBox);
}

unsigned char ListBox_save(
        const struct ListBox* const listBox, struct ResourceManager* const resourceManager,
        const char* const listBoxResId) {
    if (!listBox || !resourceManager || !listBoxResId)
        return 1;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return 2;
    result += TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING, LIST_BOX_SCENENODE_PARSER_TYPE_STRING);
    result += TextParser_addString(textParser, LIST_BOX_SCENENODE_PARSER_TEXT_BOX_RES_STRING,
                                   listBox->textBox->sceneNode.sceneNodeTextResource->id);
    result += TextParser_addString(textParser, LIST_BOX_SCENENODE_PARSER_BUTTON_RES_STRING,
                                   listBox->button->sceneNode.sceneNodeTextResource->id);
    result += TextParser_addString(textParser, LIST_BOX_SCENENODE_PARSER_CONTEXT_MENU_RES_STRING,
                                   listBox->contextMenu->sceneNode.sceneNodeTextResource->id);
    char* tempString = TextParser_convertToText(textParser);
    result += textParser->lastError;
    result += TextResource_updateContent(listBox->sceneNode.sceneNodeTextResource, tempString);
    result += ResourceManager_saveTextResource(resourceManager,
                                               listBox->sceneNode.sceneNodeTextResource, listBoxResId);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    return result;
}

void ListBox_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {
    if (!sceneNode || !eventManager || !eventManager)
        return;
    size_t i;
    struct ListBox* listBox = (struct ListBox*)sceneNode;
    TextBox_control((struct SceneNode*)listBox->textBox, eventManager);
    if (listBox->textBox->isStringChanged) {
        bool found = false;
        for (i = 0; i < listBox->contextMenu->menuOptionsCount; i++)
            if (strcmp(listBox->textBox->string, listBox->contextMenu->menuOptionsList[i]->label->text) == 0) {
                listBox->isStringExistInList = true;
                found = true;
                break;
            }
        if (!found) {
            listBox->isStringExistInList = false;
        }
    }
    if (listBox->textBox->haveFocus) {
        if (listBox->isStringExistInList) {
            listBox->textBox->box->state = BUTTON_STATE_PRESSED;
            listBox->textBox->box->isStateChanged = true;
        }
        else {
            listBox->textBox->box->state = BUTTON_STATE_FOCUSED;
            listBox->textBox->box->isStateChanged = true;
        }
    }
    if (listBox->isContextMenuShown) {
        ContextMenu_control((struct SceneNode*)listBox->contextMenu, eventManager);
        SDL_Point mouseCoordinates;
        SDL_GetMouseState(&mouseCoordinates.x, &mouseCoordinates.y);
        for (i = 0; i < listBox->contextMenu->menuOptionsCount; i++)
            if (listBox->contextMenu->menuOptionsList[i]->isStateChanged
                && listBox->contextMenu->menuOptionsList[i]->state == BUTTON_STATE_PRESSED) {
                char* newString = listBox->contextMenu->menuOptionsList[i]->label->text;
                if (TextBox_changeString(listBox->textBox, newString))
                    TextBox_changeString(listBox->textBox, "Err");
                    // FIXME Add checking ContextMenu's options labels length
                break;
            }
        for (i = 0; i < eventManager->sdlEventsCount; i++)
            // FIXME Rewrite this shit
            if (eventManager->sdlEventsList[i].type == SDL_MOUSEBUTTONDOWN && listBox->contextMenu->menuOptionsCount > 0) {
                if (!(mouseCoordinates.x >= listBox->contextMenu->menuOptionsList[0]->sprite->dstRect.x
                    && mouseCoordinates.x < (listBox->contextMenu->menuOptionsList[0]->sprite->dstRect.x 
                                                        + listBox->contextMenu->menuOptionsList[0]->sprite->dstRect.w) 
                    && mouseCoordinates.y >= listBox->contextMenu->menuOptionsList[0]->sprite->dstRect.y 
                    && mouseCoordinates.y < (listBox->contextMenu->menuOptionsList[0]->sprite->dstRect.x
                                                                + listBox->contextMenu->menuOptionsList[0]->sprite->dstRect.h 
                                                                            * (int)listBox->contextMenu->menuOptionsCount))) {
                    listBox->isContextMenuShown = false;
                }
            }
    }
    Button_control((struct SceneNode*)listBox->button, eventManager);
    if (listBox->button->isStateChanged && listBox->button->state == BUTTON_STATE_PRESSED) {
        if (listBox->isContextMenuShown)
            listBox->isContextMenuShown = false;
        else
            listBox->isContextMenuShown = true;
    }
}

void ListBox_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager || !renderer)
        return;
    struct ListBox* listBox = (struct ListBox*)sceneNode;
    if (listBox->isGeometryChanged) {
        listBox->textBox->sceneNode.coordinates = listBox->sceneNode.coordinates;
        listBox->textBox->sceneNode.flip = listBox->sceneNode.flip;
        listBox->textBox->sceneNode.scaleX = listBox->sceneNode.scaleX;
        listBox->textBox->sceneNode.scaleY = listBox->sceneNode.scaleY;
        listBox->textBox->isGeometryChanged = true;
        listBox->button->sceneNode.coordinates = listBox->sceneNode.coordinates;
        listBox->button->sceneNode.coordinates.x += listBox->textBox->box->sprite->virtualSize.x;
        listBox->button->sceneNode.flip = listBox->sceneNode.flip;
        listBox->button->sceneNode.scaleX = listBox->sceneNode.scaleX;
        listBox->button->sceneNode.scaleY = listBox->sceneNode.scaleY;
        listBox->button->isGeometryChanged = true;
        listBox->contextMenu->sceneNode.coordinates = listBox->sceneNode.coordinates;
        listBox->contextMenu->sceneNode.coordinates.y += listBox->textBox->box->sprite->virtualSize.y;
        listBox->contextMenu->sceneNode.flip = listBox->sceneNode.flip;
        listBox->contextMenu->sceneNode.scaleX = listBox->sceneNode.scaleX;
        listBox->contextMenu->sceneNode.scaleY = listBox->sceneNode.scaleY;
        listBox->contextMenu->isGeometryChanged = true;
        listBox->isGeometryChanged = false;
    }
    TextBox_update((struct SceneNode*)listBox->textBox, eventManager, renderer);
    Button_update((struct SceneNode*)listBox->button, eventManager, renderer);
    ContextMenu_update((struct SceneNode*)listBox->contextMenu, eventManager, renderer);
}

void ListBox_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct ListBox* listBox = (struct ListBox*)sceneNode;
    TextBox_render((struct SceneNode*)listBox->textBox, renderer);
    Button_render((struct SceneNode*)listBox->button, renderer);
    if (listBox->isContextMenuShown)
        ContextMenu_render((struct SceneNode*)listBox->contextMenu, renderer);
}

void ListBox_sound(struct SceneNode* sceneNode, struct Musican* musican) {
    if (!sceneNode || !musican)
        return;
    struct ListBox* listBox = (struct ListBox*)sceneNode;
    TextBox_sound((struct SceneNode*)listBox->textBox, musican);
    Button_sound((struct SceneNode*)listBox->button, musican);
    if (listBox->isContextMenuShown)
        ContextMenu_sound((struct SceneNode*)listBox->contextMenu, musican);
}
