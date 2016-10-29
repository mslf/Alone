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
#include "gui/ContextMenu.h"
#include "textParser/TextParser.h"

const char* const CONTEXT_MENU_SCENENODE_PARSER_ERR_NO_PROTOTYPES = 
            "ContextMenu_loadButtonProototypes: optionPrototypeButtonResources string haven't found!";
const char* const CONTEXT_MENU_SCENENODE_PARSER_ERR_MENU_OPTION_PROTOTYPE_ALLOC = 
            "ContextMenu_loadButtonPrototypes: allocating memory for menuOptionPrototype string failed!";
const char* const CONTEXT_MENU_SCENENODE_PARSER_ERR_NO_OPTIONS = 
            "ContextMenu_constructMenuOptions: menuOptions string haven't found!";
const char* const CONTEXT_MENU_SCENENODE_PARSER_ERR_NO_OPTION_DEF = 
            "ContextMenu_constructMenuOptions: definition of MenuOption haven't found!";
const char* const CONTEXT_MENU_SCENENODE_ERR_TRY_DEFAULT_PROTOTYPE = 
            "ContextMenu_addMenuOption: trying onlyOneMenuOptionPrototype!";
const char* const CONTEXT_MENU_SCENENODE_ERR_CONSTRUCTING_OPTION = 
            "ContextMenu_updateMenuOption: constructing menuOption failed!";
const char* const CONTEXT_MENU_SCENENODE_ERR_UPDATING_OPTION = 
            "ContextMenu_updateMenuOption: updating menuOption failed!";

unsigned char ContextMenu_constructMenuOptions(struct ContextMenu* contextMenu,
                                                       struct ResourceManager* resourceManager,
                                                       struct Renderer* renderer, struct TextParser* textParser) {
    size_t i;
    size_t menuOptionsCount = TextParser_getItemsCount(textParser, CONTEXT_MENU_SCENENODE_PARSER_OPTIONS_LIST);
    if (textParser->lastError) {
        Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_PARSER_ERR_NO_OPTIONS);
        return 1;
    }
    for (i = 0; i < menuOptionsCount; i++) {
        char* tempMenuOptionsNameString = TextParser_getString(textParser,
                                                               CONTEXT_MENU_SCENENODE_PARSER_OPTIONS_LIST, i);
        if (TextParser_getItemsCount(textParser, tempMenuOptionsNameString) == 0) {
            char tempErrString[600];
            sprintf(tempErrString, "%s Name: <%s>", CONTEXT_MENU_SCENENODE_PARSER_ERR_NO_OPTION_DEF,
                    tempMenuOptionsNameString);
            Logger_log(resourceManager->logger, tempErrString);
        }
        char* tempLabelString = TextParser_getString(textParser, tempMenuOptionsNameString, 0);
        char* tempPressedResourceString = TextParser_getString(textParser, tempMenuOptionsNameString, 1);
        char* tempFocusedResourceString = TextParser_getString(textParser, tempMenuOptionsNameString, 2);
        ContextMenu_addMenuOption(contextMenu, resourceManager, renderer, tempFocusedResourceString,
                                  tempPressedResourceString, tempLabelString);
    }
    return 0;
}

unsigned char ContextMenu_loadButtonPrototypes(struct ContextMenu* contextMenu,
                                                       struct ResourceManager* resourceManager,
                                                       struct Renderer* renderer, struct TextParser* textParser) {
    char* tempString = NULL;
    tempString = TextParser_getString(textParser, CONTEXT_MENU_SCENENODE_PARSER_PROTOTYPES, 0);
    if (!tempString) {
        Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_PARSER_ERR_NO_PROTOTYPES);
        return 1;
    }
    contextMenu->onlyOneMenuOptionPrototype = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
    if (!contextMenu->onlyOneMenuOptionPrototype) {
        Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_PARSER_ERR_MENU_OPTION_PROTOTYPE_ALLOC);
        return 2;
    }
    strcpy (contextMenu->onlyOneMenuOptionPrototype, tempString);
    // Other menuOptionPrototypes are not mandatory, so we will ignore errors
    tempString = TextParser_getString(textParser, CONTEXT_MENU_SCENENODE_PARSER_PROTOTYPES, 1);
    if (tempString) {
        contextMenu->topMenuOptionPrototype = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
        if (contextMenu->topMenuOptionPrototype)
            strcpy (contextMenu->topMenuOptionPrototype, tempString);
        else
            Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_PARSER_ERR_MENU_OPTION_PROTOTYPE_ALLOC);
    }
    tempString = TextParser_getString(textParser, CONTEXT_MENU_SCENENODE_PARSER_PROTOTYPES, 2);
    if (tempString) {
        contextMenu->middleMenuOptionPrototype = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
        if (contextMenu->middleMenuOptionPrototype)
            strcpy (contextMenu->middleMenuOptionPrototype, tempString);
        else
            Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_PARSER_ERR_MENU_OPTION_PROTOTYPE_ALLOC);
    }
    tempString = TextParser_getString(textParser, CONTEXT_MENU_SCENENODE_PARSER_PROTOTYPES, 3);
    if (tempString) {
        contextMenu->lowerMenuOptionPrototype = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
        if (contextMenu->lowerMenuOptionPrototype)
            strcpy (contextMenu->lowerMenuOptionPrototype, tempString);
        else
            Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_PARSER_ERR_MENU_OPTION_PROTOTYPE_ALLOC);
    }
    return 0; 
}

unsigned char ContextMenu_tryGetSettingsFromTextParser(struct ContextMenu* contextMenu,
                                                       struct ResourceManager* resourceManager,
                                                       struct Renderer* renderer, struct TextParser* textParser) {
    unsigned char result = 0;
    result += ContextMenu_loadButtonPrototypes(contextMenu, resourceManager, renderer, textParser);
    result += ContextMenu_constructMenuOptions(contextMenu, resourceManager, renderer, textParser);
    if (result)
        return result;
    return 0;
}

struct ContextMenu* ContextMenu_construct(struct ResourceManager* const resourceManager, struct Renderer* renderer,
                                          const char* const contextMenuResId) {
    struct ContextMenu* contextMenu = NULL;
    contextMenu = (struct ContextMenu*)calloc(1, sizeof(struct ContextMenu));
    if (!contextMenu)
        return NULL;
    SceneNode_init(&(contextMenu->sceneNode));
    contextMenu->sceneNode.sceneNodeTextResource = ResourceManager_loadTextResource(resourceManager, contextMenuResId, 0);
    if (!contextMenu->sceneNode.sceneNodeTextResource) {
        ContextMenu_destruct(contextMenu);
        return NULL;
    }
    contextMenu->menuOptionsList = (struct Button**)malloc(sizeof(struct Button*) * 
                                                    CONTEXT_MENU_SCENENODE_INITIAL_NUMBER_ALLOCATED_MENU_OPTIONS);
    if (!contextMenu->menuOptionsList) {
        ContextMenu_destruct(contextMenu);
        return NULL;
    }
    contextMenu->allocatedMenuOptions = CONTEXT_MENU_SCENENODE_INITIAL_NUMBER_ALLOCATED_MENU_OPTIONS;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                      contextMenu->sceneNode.sceneNodeTextResource);
    if (!textParser) {
        ContextMenu_destruct(contextMenu);
        return NULL;
    }
    if (ContextMenu_tryGetSettingsFromTextParser(contextMenu, resourceManager, renderer, textParser)) {
        TextParser_destruct(textParser);
        ContextMenu_destruct(contextMenu);
        return NULL;
    }
    TextParser_destruct(textParser);
    contextMenu->sceneNode.control = ContextMenu_control;
    contextMenu->sceneNode.update = ContextMenu_update;
    contextMenu->sceneNode.render = ContextMenu_render;
    contextMenu->sceneNode.sound = ContextMenu_sound;
    contextMenu->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(CONTEXT_MENU_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!contextMenu->sceneNode.type) {
        ContextMenu_destruct(contextMenu);
        return NULL;
    }
    strcpy(contextMenu->sceneNode.type, CONTEXT_MENU_SCENENODE_PARSER_TYPE_STRING);
    return contextMenu;
}

void ContextMenu_destruct(struct ContextMenu* contextMenu) {
    if (!contextMenu)
        return;
    if (contextMenu->onlyOneMenuOptionPrototype)
        free(contextMenu->onlyOneMenuOptionPrototype);
    if (contextMenu->topMenuOptionPrototype)
        free(contextMenu->topMenuOptionPrototype);
    if (contextMenu->middleMenuOptionPrototype)
        free(contextMenu->middleMenuOptionPrototype);
    if (contextMenu->lowerMenuOptionPrototype)
        free(contextMenu->lowerMenuOptionPrototype);
    if (contextMenu->menuOptionsList) {
        size_t i;
        for (i = 0; i < contextMenu->menuOptionsCount; i++)
            Button_destruct(contextMenu->menuOptionsList[i]);
        free(contextMenu->menuOptionsList);
    }
    if (contextMenu->sceneNode.sceneNodeTextResource)
        contextMenu->sceneNode.sceneNodeTextResource->pointersCount--;
    if (contextMenu->sceneNode.type)
        free(contextMenu->sceneNode.type);
}

unsigned char ContextMenu_updateMenuOption(struct ContextMenu* contextMenu, struct ResourceManager* const resourceManager,
                                           struct Renderer* renderer, size_t index, const char* const newButtonResId) {
    unsigned char result = 0;
    /* TO FIX: Assigned value can be undefined */
    struct Button* tempCopyFromButton = contextMenu->menuOptionsList[index];
    struct Button* tempButton = Button_construct(resourceManager, renderer, newButtonResId);
    if (!tempButton) {
        char tempErrString[600];
        sprintf(tempErrString, "%s MenuOption resource ID: %s",
                CONTEXT_MENU_SCENENODE_ERR_CONSTRUCTING_OPTION, newButtonResId);
        Logger_log(renderer->logger, tempErrString);
        return 1;
    }
    result += Button_changeFocusedEventResource(tempButton, resourceManager,
                                                tempCopyFromButton->focusedEventResource->id);
    result += Button_changePressedEventResource(tempButton, resourceManager,
                                                tempCopyFromButton->pressedEventResource->id);
    result += Text_regenerateTexture(tempButton->label, resourceManager, renderer, tempCopyFromButton->label->text,
                            tempCopyFromButton->label->fontPath, tempCopyFromButton->label->size,
                            tempCopyFromButton->label->color);
    if (result) {
        Button_destruct(tempButton);
        char tempErrString[600];
        sprintf(tempErrString, "%s MenuOption resource ID: %s",
                CONTEXT_MENU_SCENENODE_ERR_UPDATING_OPTION, newButtonResId);
        Logger_log(renderer->logger, tempErrString);
        return 2;
    }
    Button_destruct(tempCopyFromButton);
    contextMenu->menuOptionsList[index] = tempButton;
    return 0;
    
}

unsigned char ContextMenu_realloccateMenuOptionsList(struct ContextMenu* contextMenu) {
    if (!contextMenu)
        return 1;
    struct Button** menuOptionsList = NULL;
    size_t i;
    size_t newSize = contextMenu->allocatedMenuOptions + CONTEXT_MENU_SCENENODE_INITIAL_NUMBER_ALLOCATED_MENU_OPTIONS;
    if (!(menuOptionsList = (struct Button**)malloc(sizeof(struct Button*) * newSize)))
        return 2;
    for (i = 0; i < contextMenu->menuOptionsCount; i++)
        menuOptionsList[i] = contextMenu->menuOptionsList[i]; /* TO FIX: Assigned value can be undefined */
    free(contextMenu->menuOptionsList);
    contextMenu->menuOptionsList = menuOptionsList;
    contextMenu->allocatedMenuOptions = newSize;
    return 0;
}

unsigned char ContextMenu_addMenuOption(struct ContextMenu* contextMenu, struct ResourceManager* const resourceManager,
                                        struct Renderer* renderer, const char* const focusedEventRes,
                                        const char* const pressedEventRes, const char* const labelText) {
    // Don't care about NULL *EventRes and label strings, because we can add default menuOption
    if (!contextMenu || !resourceManager)
        return 1;
    if (contextMenu->menuOptionsCount >= contextMenu->allocatedMenuOptions)
        if (ContextMenu_realloccateMenuOptionsList(contextMenu))
            return 2;
    unsigned char result = 0;
    struct Button* tempButtonToAdd = NULL;
    if (contextMenu->menuOptionsCount == 0)
        tempButtonToAdd = Button_construct(resourceManager, renderer, contextMenu->onlyOneMenuOptionPrototype);
    if (contextMenu->menuOptionsCount == 1) {
        if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, 0,
            contextMenu->topMenuOptionPrototype)) {
            Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_ERR_TRY_DEFAULT_PROTOTYPE);
            if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, 0,
            contextMenu->onlyOneMenuOptionPrototype))
                return 2;
        }
        tempButtonToAdd = Button_construct(resourceManager, renderer, contextMenu->lowerMenuOptionPrototype);
    }
    if (contextMenu->menuOptionsCount > 1) {
        if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, contextMenu->menuOptionsCount - 1,
            contextMenu->middleMenuOptionPrototype)) {
            Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_ERR_TRY_DEFAULT_PROTOTYPE);
            if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, contextMenu->menuOptionsCount - 1,
            contextMenu->onlyOneMenuOptionPrototype))
                return 3;
        }
        tempButtonToAdd = Button_construct(resourceManager, renderer, contextMenu->lowerMenuOptionPrototype);
    }
    if (!tempButtonToAdd) {
        Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_ERR_TRY_DEFAULT_PROTOTYPE);
        tempButtonToAdd = Button_construct(resourceManager, renderer, contextMenu->onlyOneMenuOptionPrototype);
            if (!tempButtonToAdd)
                return 4;
    }
    result += Button_changeFocusedEventResource(tempButtonToAdd, resourceManager, focusedEventRes);
    result += Button_changePressedEventResource(tempButtonToAdd, resourceManager, pressedEventRes);
    result += Text_regenerateTexture(tempButtonToAdd->label, resourceManager, renderer, labelText,
                            tempButtonToAdd->label->fontPath, tempButtonToAdd->label->size,
                            tempButtonToAdd->label->color);
    if (result)
        Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_ERR_UPDATING_OPTION);
    contextMenu->isGeometryChanged = true;
    contextMenu->menuOptionsList[contextMenu->menuOptionsCount] = tempButtonToAdd;
    contextMenu->menuOptionsCount++;
    return 0;
}

void ContextMenu_removeMenuOption(struct ContextMenu* contextMenu, const char* const label) {
    
}

unsigned char ContextMenu_save(
        const struct ContextMenu* const contextMenu, struct ResourceManager* const resourceManager,
        const char* const contextMenuResId) {
    
}

void ContextMenu_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {
    if (!sceneNode || !eventManager)
        return;
    struct ContextMenu* contextMenu = (struct ContextMenu*)sceneNode;
    size_t i;
    for (i = 0; i < contextMenu->menuOptionsCount; i++)
        Button_control((struct SceneNode*)contextMenu->menuOptionsList[i], eventManager);
}

void ContextMenu_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager)
        return;
    struct ContextMenu* contextMenu = (struct ContextMenu*)sceneNode;
    size_t i;
    if (contextMenu->isGeometryChanged && contextMenu->menuOptionsCount > 0) {
            contextMenu->menuOptionsList[0]->sceneNode.coordinates = contextMenu->sceneNode.coordinates;
            contextMenu->menuOptionsList[0]->sceneNode.flip = contextMenu->sceneNode.flip;
            contextMenu->menuOptionsList[0]->sceneNode.scaleX = contextMenu->sceneNode.scaleX;
            contextMenu->menuOptionsList[0]->sceneNode.scaleY = contextMenu->sceneNode.scaleY;
        for (i = 1; i < contextMenu->menuOptionsCount; i++) {
            contextMenu->menuOptionsList[i]->sceneNode.coordinates = contextMenu->sceneNode.coordinates;
            contextMenu->menuOptionsList[i]->sceneNode.coordinates.y =
                    contextMenu->menuOptionsList[i - 1]->sceneNode.coordinates.y
                    + contextMenu->menuOptionsList[i - 1]->sprite->virtualSize.y;
            contextMenu->menuOptionsList[i]->sceneNode.flip = contextMenu->sceneNode.flip;
            contextMenu->menuOptionsList[i]->sceneNode.scaleX = contextMenu->sceneNode.scaleX;
            contextMenu->menuOptionsList[i]->sceneNode.scaleY = contextMenu->sceneNode.scaleY;
        }
    }
    for (i = 0; i < contextMenu->menuOptionsCount; i++)
        Button_update((struct SceneNode*)contextMenu->menuOptionsList[i], eventManager, renderer);
}

void ContextMenu_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct ContextMenu* contextMenu = (struct ContextMenu*)sceneNode;
    size_t i;
    for (i = 0; i < contextMenu->menuOptionsCount; i++)
        Button_render((struct SceneNode*)contextMenu->menuOptionsList[i], renderer);
}

void ContextMenu_sound(struct SceneNode* sceneNode, struct Musican* musican) {
    if (!sceneNode || !musican)
        return;
    struct ContextMenu* contextMenu = (struct ContextMenu*)sceneNode;
    size_t i;
    for (i = 0; i < contextMenu->menuOptionsCount; i++)
        Button_sound((struct SceneNode*)contextMenu->menuOptionsList[i], musican);
}
