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
const char* const CONTEXT_MENU_SCENENODE_ERR_EXIST_OPTION = 
            "ContextMenu_addMenuOption: menu option with that label is already exist in list!";
const char* const CONTEXT_MENU_SCENENODE_ERR_CONSTRUCTING_OPTION = 
            "ContextMenu_updateMenuOption: constructing menuOption failed!";
const char* const CONTEXT_MENU_SCENENODE_ERR_UPDATING_OPTION = 
            "ContextMenu_updateMenuOption: updating menuOption failed!";

static unsigned char ContextMenu_constructMenuOptions(struct ContextMenu* contextMenu,
                                                       struct ResourceManager* resourceManager,
                                                       struct Renderer* renderer,
                                                       struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                       struct TextParser* textParser) {
    if (!contextMenu || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    size_t i;
    size_t menuOptionsCount = TextParser_getItemsCount(textParser, CONTEXT_MENU_SCENENODE_PARSER_OPTIONS_LIST);
    if (textParser->lastError) {
        Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_PARSER_ERR_NO_OPTIONS);
        return 2;
    }
    for (i = 0; i < menuOptionsCount; i++) {
        const char* tempMenuOptionsNameString = TextParser_getString(textParser,
                                                               CONTEXT_MENU_SCENENODE_PARSER_OPTIONS_LIST, i);
        if (TextParser_getItemsCount(textParser, tempMenuOptionsNameString) == 0) {
            char tempErrString[600];
            sprintf(tempErrString, "%s Name: <%s>", CONTEXT_MENU_SCENENODE_PARSER_ERR_NO_OPTION_DEF,
                    tempMenuOptionsNameString);
            Logger_log(resourceManager->logger, tempErrString);
        }
        const char* tempLabelString = TextParser_getString(textParser, tempMenuOptionsNameString, 0);
        const char* tempPressedResourceString = TextParser_getString(textParser, tempMenuOptionsNameString, 1);
        const char* tempFocusedResourceString = TextParser_getString(textParser, tempMenuOptionsNameString, 2);
        ContextMenu_addMenuOption(contextMenu, resourceManager, renderer, 
                                  sceneNodeTypesRegistrar, tempFocusedResourceString,
                                  tempPressedResourceString, tempLabelString);
    }
    return 0;
}

static unsigned char ContextMenu_loadButtonPrototypes(struct ContextMenu* contextMenu,
                                                       struct ResourceManager* resourceManager,
                                                       struct Renderer* renderer, struct TextParser* textParser) {
    if (!contextMenu || !resourceManager || !renderer || !textParser)
        return 1;
    const char* tempString = NULL;
    tempString = TextParser_getString(textParser, CONTEXT_MENU_SCENENODE_PARSER_PROTOTYPES, 0);
    if (!tempString) {
        Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_PARSER_ERR_NO_PROTOTYPES);
        return 2;
    }
    contextMenu->onlyOneMenuOptionPrototype = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
    if (!contextMenu->onlyOneMenuOptionPrototype) {
        Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_PARSER_ERR_MENU_OPTION_PROTOTYPE_ALLOC);
        return 3;
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

static unsigned char ContextMenu_tryGetSettingsFromTextParser(struct ContextMenu* contextMenu,
                                                       struct ResourceManager* resourceManager,
                                                       struct Renderer* renderer,
                                                       struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                       struct TextParser* textParser) {
    if (!contextMenu || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    unsigned char result = 0;
    result += ContextMenu_loadButtonPrototypes(contextMenu, resourceManager, renderer, textParser);
    result += ContextMenu_constructMenuOptions(contextMenu, resourceManager, renderer,
                                               sceneNodeTypesRegistrar, textParser);
    return result;
}

struct SceneNode* ContextMenu_construct(struct ResourceManager* const resourceManager,
                                        struct Renderer* const renderer,
                                        struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                        struct TextParser* const textParser) {
    if (!resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return NULL;
    struct ContextMenu* contextMenu = NULL;
    contextMenu = (struct ContextMenu*)calloc(1, sizeof(struct ContextMenu));
    if (!contextMenu)
        return NULL;
    SceneNode_init(&(contextMenu->sceneNode));
    contextMenu->menuOptionsList = (struct Button**)malloc(sizeof(struct Button*) * 
                                                    CONTEXT_MENU_SCENENODE_INITIAL_NUMBER_ALLOCATED_MENU_OPTIONS);
    if (!contextMenu->menuOptionsList) {
        ContextMenu_destruct((struct SceneNode*)contextMenu);
        return NULL;
    }
    contextMenu->allocatedMenuOptions = CONTEXT_MENU_SCENENODE_INITIAL_NUMBER_ALLOCATED_MENU_OPTIONS;
    if (ContextMenu_tryGetSettingsFromTextParser(contextMenu, resourceManager, renderer,
                                                    sceneNodeTypesRegistrar, textParser)) {
        ContextMenu_destruct((struct SceneNode*)contextMenu);
        return NULL;
    }
    contextMenu->sceneNode.control = ContextMenu_control;
    contextMenu->sceneNode.update = ContextMenu_update;
    contextMenu->sceneNode.render = ContextMenu_render;
    contextMenu->sceneNode.sound = ContextMenu_sound;
    contextMenu->sceneNode.destruct = ContextMenu_destruct;
    contextMenu->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(CONTEXT_MENU_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!contextMenu->sceneNode.type) {
        ContextMenu_destruct((struct SceneNode*)contextMenu);
        return NULL;
    }
    strcpy(contextMenu->sceneNode.type, CONTEXT_MENU_SCENENODE_PARSER_TYPE_STRING);
    return (struct SceneNode*)contextMenu;
}

void ContextMenu_destruct(struct SceneNode* contextMenu) {
    if (!contextMenu)
        return;
    struct ContextMenu* tempContextMenu = (struct ContextMenu*)contextMenu;
    if (tempContextMenu->onlyOneMenuOptionPrototype)
        free(tempContextMenu->onlyOneMenuOptionPrototype);
    if (tempContextMenu->topMenuOptionPrototype)
        free(tempContextMenu->topMenuOptionPrototype);
    if (tempContextMenu->middleMenuOptionPrototype)
        free(tempContextMenu->middleMenuOptionPrototype);
    if (tempContextMenu->lowerMenuOptionPrototype)
        free(tempContextMenu->lowerMenuOptionPrototype);
    if (tempContextMenu->menuOptionsList) {
        size_t i;
        for (i = 0; i < tempContextMenu->menuOptionsCount; i++)
            tempContextMenu->menuOptionsList[i]->sceneNode.destruct((struct SceneNode*)tempContextMenu->menuOptionsList[i]);
        free(tempContextMenu->menuOptionsList);
    }
    if (contextMenu->sceneNodeTextResource)
        contextMenu->sceneNodeTextResource->pointersCount--;
    if (contextMenu->type)
        free(contextMenu->type);
    free(contextMenu);
}

static unsigned char ContextMenu_updateMenuOption(struct ContextMenu* contextMenu,
                                                  struct ResourceManager* const resourceManager,
                                                  struct Renderer* renderer,
                                                  struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                  size_t index,
                                                  const char* const newButtonResId) {
    if (!contextMenu || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !newButtonResId)
        return 1;
    if (index >= contextMenu->menuOptionsCount)
        return 2;
    unsigned char result = 0;
    struct Button* tempCopyFromButton = contextMenu->menuOptionsList[index];
    // We won't update menuOption if it already has suitable prototype
    if (strcmp(tempCopyFromButton->sceneNode.sceneNodeTextResource->id, newButtonResId) != 0) {
        struct Button* tempButton =(struct Button*)SceneNodeTypesRegistrar_constructSceneNode(
                                                    resourceManager,
                                                    renderer,
                                                    sceneNodeTypesRegistrar,
                                                    newButtonResId,
                                                    ButtonSceneNode_parserStrings.type);
        if (!tempButton) {
            char tempErrString[600];
            sprintf(tempErrString, "%s MenuOption resource ID: %s",
                    CONTEXT_MENU_SCENENODE_ERR_CONSTRUCTING_OPTION, newButtonResId);
            Logger_log(renderer->logger, tempErrString);
            return 3;
        }
        result += Button_changeFocusedEventResource(tempButton, resourceManager,
                                                    tempCopyFromButton->focusedEventResource->id);
        result += Button_changePressedEventResource(tempButton, resourceManager,
                                                    tempCopyFromButton->pressedEventResource->id);
        result += Text_regenerateTexture(tempButton->label, resourceManager, renderer,
                                         tempCopyFromButton->label->text,
                                         tempCopyFromButton->label->fontPath,
                                         tempCopyFromButton->label->size,
                                         tempCopyFromButton->label->color);
        if (result) {
            Button_destruct((struct SceneNode*)tempButton);
            char tempErrString[600];
            sprintf(tempErrString, "%s MenuOption resource ID: %s",
                    CONTEXT_MENU_SCENENODE_ERR_UPDATING_OPTION, newButtonResId);
            Logger_log(renderer->logger, tempErrString);
            return 4;
        }
        Button_destruct((struct SceneNode*)tempCopyFromButton);
        contextMenu->menuOptionsList[index] = tempButton;
    }
    return 0;
    
}

static unsigned char ContextMenu_realloccateMenuOptionsList(struct ContextMenu* contextMenu) {
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

static unsigned char CotextMenu_updateMenuOptionsList(struct ContextMenu* contextMenu,
                                                      struct ResourceManager* const resourceManager,
                                                      struct Renderer* const renderer,
                                                      struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar) {
    if (!contextMenu || !resourceManager || !renderer)
        return 1;
    size_t i;
    switch (contextMenu->menuOptionsCount) {
        case 0:
            break;
        case 1:
            if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, sceneNodeTypesRegistrar, 0,
                                             contextMenu->onlyOneMenuOptionPrototype))
                return 2;
            break;
        case 2:
            if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, sceneNodeTypesRegistrar, 0,
                                             contextMenu->topMenuOptionPrototype)) {
                Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_ERR_TRY_DEFAULT_PROTOTYPE);
                if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, sceneNodeTypesRegistrar, 0,
                                             contextMenu->onlyOneMenuOptionPrototype))
                    return 3;
            }
            if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, sceneNodeTypesRegistrar, 1,
                                             contextMenu->lowerMenuOptionPrototype)) {
                Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_ERR_TRY_DEFAULT_PROTOTYPE);
                if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, sceneNodeTypesRegistrar, 1,
                                             contextMenu->onlyOneMenuOptionPrototype))
                    return 4;
            }
            break;
        default:
            for (i = 1; i < contextMenu->menuOptionsCount - 1; i++)
                if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, sceneNodeTypesRegistrar, i,
                                             contextMenu->middleMenuOptionPrototype)) {
                    Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_ERR_TRY_DEFAULT_PROTOTYPE);
                    if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, sceneNodeTypesRegistrar, i,
                                                contextMenu->onlyOneMenuOptionPrototype))
                        return 5;
                }
             if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, sceneNodeTypesRegistrar,
                                             contextMenu->menuOptionsCount - 1,
                                             contextMenu->lowerMenuOptionPrototype)) {
                Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_ERR_TRY_DEFAULT_PROTOTYPE);
                if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer, sceneNodeTypesRegistrar,
                                             contextMenu->menuOptionsCount - 1,
                                             contextMenu->onlyOneMenuOptionPrototype))
                    return 6;
            }
            break;
    }
    return 0;
}

unsigned char ContextMenu_addMenuOption(struct ContextMenu* contextMenu,
                                        struct ResourceManager* const resourceManager,
                                        struct Renderer* const renderer,
                                        struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                        const char* const focusedEventRes,
                                        const char* const pressedEventRes,
                                        const char* const labelText) {
    // Don't care about NULL *EventRes and label strings, because we can add default menuOption
    if (!contextMenu || !resourceManager || !renderer || !sceneNodeTypesRegistrar)
        return 1;
    if (contextMenu->menuOptionsCount >= contextMenu->allocatedMenuOptions)
        if (ContextMenu_realloccateMenuOptionsList(contextMenu))
            return 2;
    size_t i;
    unsigned char result = 0;
    struct Button* tempButtonToAdd = NULL;
    tempButtonToAdd = (struct Button*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                            renderer,
                                                                            sceneNodeTypesRegistrar,
                                                                            contextMenu->onlyOneMenuOptionPrototype,
                                                                            ButtonSceneNode_parserStrings.type);
    if (!tempButtonToAdd)
        return 3;
    result += Button_changeFocusedEventResource(tempButtonToAdd, resourceManager, focusedEventRes);
    result += Button_changePressedEventResource(tempButtonToAdd, resourceManager, pressedEventRes);
    result += Text_regenerateTexture(tempButtonToAdd->label, resourceManager, renderer, labelText,
                            tempButtonToAdd->label->fontPath, tempButtonToAdd->label->size,
                            tempButtonToAdd->label->color);
    if (result)
        Logger_log(renderer->logger, CONTEXT_MENU_SCENENODE_ERR_UPDATING_OPTION);
    for (i = 0; i < contextMenu->menuOptionsCount; i++)
        if (strcmp(tempButtonToAdd->label->text, contextMenu->menuOptionsList[i]->label->text) == 0) {
            char tempErrString[600];
            sprintf(tempErrString, "%s Label: <%s>", CONTEXT_MENU_SCENENODE_ERR_EXIST_OPTION,
                    tempButtonToAdd->label->text);
            Logger_log(renderer->logger, tempErrString);
            Button_destruct((struct SceneNode*)tempButtonToAdd);
            return 4;
        }
    contextMenu->isGeometryChanged = true;
    contextMenu->menuOptionsList[contextMenu->menuOptionsCount] = tempButtonToAdd;
    contextMenu->menuOptionsCount++;
    if (CotextMenu_updateMenuOptionsList(contextMenu, resourceManager, renderer, sceneNodeTypesRegistrar)) {
        Button_destruct((struct SceneNode*)contextMenu->menuOptionsList[contextMenu->menuOptionsCount - 1]);
        contextMenu->menuOptionsCount--;
        return 5;
    }
    return 0;
}

void ContextMenu_removeMenuOption(struct ContextMenu* contextMenu,
                                  struct ResourceManager* const resourceManager,
                                  struct Renderer* const renderer,
                                  struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                  const char* const label) {
    if (!contextMenu || !label || !resourceManager || !renderer || !sceneNodeTypesRegistrar)
        return;
    size_t i;
    bool found = false;
    size_t foundIndex = 0;
    for (i = 0; i < contextMenu->menuOptionsCount; i++)
        if (strcmp(label, contextMenu->menuOptionsList[i]->label->text) == 0) {
            found = true;
            foundIndex = i;
            break;
        }
    if (!found)
        return;
    struct Button* tempButton = contextMenu->menuOptionsList[foundIndex];
    for (i = foundIndex; i < contextMenu->menuOptionsCount - 1; i++)
        contextMenu->menuOptionsList[i] = contextMenu->menuOptionsList[i + 1];
    contextMenu->menuOptionsCount--;
    if (CotextMenu_updateMenuOptionsList(contextMenu, resourceManager, renderer, sceneNodeTypesRegistrar)) {
        // Reverting all menuOptions back
        for (i = contextMenu->menuOptionsCount; i > foundIndex; i--)
            contextMenu->menuOptionsList[i] = contextMenu->menuOptionsList[i - 1];
        contextMenu->menuOptionsList[foundIndex] = tempButton;
        contextMenu->menuOptionsCount++;
        return;
    }
    Button_destruct((struct SceneNode*)tempButton);
}

unsigned char ContextMenu_save(
        const struct ContextMenu* const contextMenu, struct ResourceManager* const resourceManager,
        const char* const contextMenuResId) {
    if (!contextMenu || !resourceManager || !contextMenuResId)
        return 1;
    size_t i;
    unsigned char result = 0;
    struct TextParser* textParser = TextParser_constructEmpty();
    if (!textParser)
        return 2;
    result += TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING,
                                   CONTEXT_MENU_SCENENODE_PARSER_TYPE_STRING);
    result += TextParser_addString(textParser, CONTEXT_MENU_SCENENODE_PARSER_PROTOTYPES,
                         contextMenu->onlyOneMenuOptionPrototype);
    if (contextMenu->topMenuOptionPrototype 
        && contextMenu->middleMenuOptionPrototype 
        && contextMenu->lowerMenuOptionPrototype) {
        result += TextParser_addString(textParser, CONTEXT_MENU_SCENENODE_PARSER_PROTOTYPES,
                            contextMenu->topMenuOptionPrototype);
        result += TextParser_addString(textParser, CONTEXT_MENU_SCENENODE_PARSER_PROTOTYPES,
                            contextMenu->middleMenuOptionPrototype);
        result += TextParser_addString(textParser, CONTEXT_MENU_SCENENODE_PARSER_PROTOTYPES,
                            contextMenu->lowerMenuOptionPrototype);
    }
    for (i = 0; i < contextMenu->menuOptionsCount; i++) {
        char tempMenuOptionName[600];
        sprintf(tempMenuOptionName, "%ld", i);
        result += TextParser_addString(textParser, CONTEXT_MENU_SCENENODE_PARSER_OPTIONS_LIST, tempMenuOptionName);
        result += TextParser_addString(textParser, tempMenuOptionName, contextMenu->menuOptionsList[i]->label->text);
        if (contextMenu->menuOptionsList[i]->pressedEventResource->id) {
            result += TextParser_addString(textParser, tempMenuOptionName,
                                 contextMenu->menuOptionsList[i]->pressedEventResource->id);
            // Next is optional
            TextParser_addString(textParser, tempMenuOptionName,
                                 contextMenu->menuOptionsList[i]->focusedEventResource->id);
        }
    }
    char* tempString = TextParser_convertToText(textParser);
    result += textParser->lastError;
    result += TextResource_updateContent(contextMenu->sceneNode.sceneNodeTextResource, tempString);
    result += ResourceManager_saveTextResource(resourceManager, contextMenu->sceneNode.sceneNodeTextResource,
                                               contextMenuResId);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    return result;
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
        contextMenu->isGeometryChanged = false;
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
