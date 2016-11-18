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
 * @file ContextMenu.c
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing implementation of #ContextMenu.
 */
#include "gui/ContextMenu.h"
#include "textParser/TextParser.h"

/**
 * @brief Error message strings for #ContextMenu.
 */
static const struct ContextMenuScenNode_errorMessages {
    const char* const errNoPrototypes;
    /**< Will be displayed when #TextParser have no ContextMenuSceneNode_parserStrings#prototypes. */
    const char* const errAllocPrototypeString;
    /**< Will be displayed when allocating memory for menuOptionPrototype string failed. */
    const char* const errNoOptions;
    /**< Will be displayed when #TextParser have no ContextMenuSceneNode_parserStrings#optionsList. */
    const char* const errNoOptionDefinition;
    /**< Will be displayed when #TextParser have no string, specified in ContextMenuSceneNode_parserStrings#optionsList. */
    const char* const warnTryingDefault;
    /**< Will be displayed when constructing #Button with required prototype (top, middlem lower) failed.
     * Now, #ContextMenu will try to construct #Button with ContxextMenu#onlyOneMenuOptionPrototype. */
    const char* const errSameLabelExists;
    /**< Will be displayed when exists #Button in ContextMenu#menuOptionsList with the same label text. 
     * Labels should be unique. */
    const char* const errConstructingMenuOption;
    /**< Will be displayed when constructing #Button failed and there is no alternatives. */
    const char* const errUpdatingOption;
    /**< Will be displayed when updating #Button failed. */
}ContextMenuScenNode_errorMessages = {
    "ContextMenu_loadButtonProototypes: optionPrototypeButtonResources string haven't found!",
    "ContextMenu_loadButtonPrototypes: allocating memory for menuOptionPrototype string failed!",
    "ContextMenu_constructMenuOptions: menuOptions string haven't found!",
    "ContextMenu_constructMenuOptions: definition of MenuOption haven't found!",
    "ContextMenu_addMenuOption: trying onlyOneMenuOptionPrototype!",
    "ContextMenu_addMenuOption: menu option with that label is already exist in list!",
    "ContextMenu_updateMenuOption: constructing menuOption failed!",
    "ContextMenu_updateMenuOption: updating menuOption failed!"};

/**
 * @brief Function for initialization ContextMenu#menuOptionsList from #TextParser.
 * @param contextMenu Pointer to a #ContextMenu, where to set prototype strings. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager for loading #Button resources. Can be NULL.
 * @param renderer Pointer to a #Renderer for constructing #Button. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar for constructing #Button. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings. Can be NULL.
 * @return #ContextMenuSceneNode_errors value.
 * @see #ContextMenu
 * @see #ResourceManager
 * @see #Renderer
 * @see #SceneNodeTypesRegistrar
 * @see #TextParser
 * @see #ContextMenuSceneNode_parserStrings
 * @see #ContextMenuSceneNode_errors
 */
static enum ContextMenuSceneNode_errors ContextMenu_constructMenuOptions(struct ContextMenu* contextMenu,
                                                       struct ResourceManager* resourceManager,
                                                       struct Renderer* renderer,
                                                       struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                       struct TextParser* textParser) {
    if (!contextMenu || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return CONTEXT_MENU_ERR_NULL_ARGUMENT;
    size_t i;
    size_t menuOptionsCount = TextParser_getItemsCount(textParser, ContextMenuSceneNode_parserString.optionsList);
    if (textParser->lastError) {
        Logger_log(renderer->logger, ContextMenuScenNode_errorMessages.errNoOptions);
        return CONTEXT_MENU_ERR_NO_OPTIONS;
    }
    for (i = 0; i < menuOptionsCount; i++) {
        const char* tempMenuOptionsNameString = TextParser_getString(textParser,
                                                               ContextMenuSceneNode_parserString.optionsList, i);
        if (TextParser_getItemsCount(textParser, tempMenuOptionsNameString) == 0) {
            char tempErrString[600];
            sprintf(tempErrString, "%s Name: <%s>", ContextMenuScenNode_errorMessages.errNoOptionDefinition,
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
    return CONTEXT_MENU_NO_ERRORS;
}

/**
 * @brief Function for loading #Button prototype strings.
 * Sets ContextMenu#onlyOneMenuOptionPrototype, ContextMenu#topMenuOptionPrototype, 
 * ContextMenu#middleMenuOptionPrototype and ContextMenu#lowerMenuOptionPrototype strings from #TextParser.
 * @param contextMenu Pointer to a #ContextMenu, where to set prototype strings. Can be NULL.
 * @param logger Pointer to a #Logger for logging. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings. Can be NULL.
 * @return #ContextMenuSceneNode_errors value.
 * @see #ContextMenu
 * @see #Logger
 * @see #TextParser
 * @see #ContextMenuSceneNode_parserStrings
 * @see #ContextMenuSceneNode_errors
 */
static enum ContextMenuSceneNode_errors ContextMenu_loadButtonPrototypes(struct ContextMenu* contextMenu,
                                                                         struct Logger* logger,
                                                                         struct TextParser* textParser) {
    // Don't care, if logger is NULL
    if (!contextMenu || !textParser)
        return CONTEXT_MENU_ERR_NULL_ARGUMENT;
    const char* tempString = NULL;
    tempString = TextParser_getString(textParser, ContextMenuSceneNode_parserString.prototypes, 0);
    if (!tempString) {
        Logger_log(logger, ContextMenuScenNode_errorMessages.errNoPrototypes);
        return CONTEXT_MENU_ERR_NO_PROTOTYPES;
    }
    contextMenu->onlyOneMenuOptionPrototype = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
    if (!contextMenu->onlyOneMenuOptionPrototype) {
        Logger_log(logger, ContextMenuScenNode_errorMessages.errAllocPrototypeString);
        return CONTEXT_MENU_ERR_ALLOC_PROTOTYPE_STRING;
    }
    strcpy (contextMenu->onlyOneMenuOptionPrototype, tempString);
    // Other menuOptionPrototypes are not mandatory, so we will ignore errors
    tempString = TextParser_getString(textParser, ContextMenuSceneNode_parserString.prototypes, 1);
    if (tempString) {
        contextMenu->topMenuOptionPrototype = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
        if (contextMenu->topMenuOptionPrototype)
            strcpy (contextMenu->topMenuOptionPrototype, tempString);
        else
            Logger_log(logger, ContextMenuScenNode_errorMessages.errAllocPrototypeString);
    }
    tempString = TextParser_getString(textParser, ContextMenuSceneNode_parserString.prototypes, 2);
    if (tempString) {
        contextMenu->middleMenuOptionPrototype = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
        if (contextMenu->middleMenuOptionPrototype)
            strcpy (contextMenu->middleMenuOptionPrototype, tempString);
        else
            Logger_log(logger, ContextMenuScenNode_errorMessages.errAllocPrototypeString);
    }
    tempString = TextParser_getString(textParser, ContextMenuSceneNode_parserString.prototypes, 3);
    if (tempString) {
        contextMenu->lowerMenuOptionPrototype = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
        if (contextMenu->lowerMenuOptionPrototype)
            strcpy (contextMenu->lowerMenuOptionPrototype, tempString);
        else
            Logger_log(logger, ContextMenuScenNode_errorMessages.errAllocPrototypeString);
    }
    return CONTEXT_MENU_NO_ERRORS; 
}

/**
 * @brief Function for loading settings and initializing #ContextMenu from #TextParser.
 * @param contextMenu Pointer to a #ContextMenu which will be initialized. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager for loading required resources. Can be NULL.
 * @param renderer Pointer to a #Renderer for constructing #Button for menuOptions. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing #Button for menuOptions. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings. Can be NULL.
 * @return #ContextMenuSceneNode_errors value.
 * @see #ContextMenu
 * @see #Button
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #ContextMenuSceneNode_parserStrings
 * @see #ContextMenuSceneNode_errors
 */
static enum ContextMenuSceneNode_errors ContextMenu_tryGetSettingsFromTextParser(struct ContextMenu* contextMenu,
                                                       struct ResourceManager* resourceManager,
                                                       struct Renderer* renderer,
                                                       struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                       struct TextParser* textParser) {
    if (!contextMenu || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return CONTEXT_MENU_ERR_NULL_ARGUMENT;
    enum ContextMenuSceneNode_errors loadingPrototypesError = CONTEXT_MENU_NO_ERRORS;
    enum ContextMenuSceneNode_errors constructingOptionsError = CONTEXT_MENU_NO_ERRORS;
    loadingPrototypesError = ContextMenu_loadButtonPrototypes(contextMenu, renderer->logger, textParser);
    constructingOptionsError = ContextMenu_constructMenuOptions(contextMenu, resourceManager, renderer,
                                               sceneNodeTypesRegistrar, textParser);
    if (loadingPrototypesError || constructingOptionsError)
        return CONTEXT_MENU_ERR_LOADING_SETTINGS;
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
    contextMenu->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(ContextMenuSceneNode_parserString.type) + 1));
    if (!contextMenu->sceneNode.type) {
        ContextMenu_destruct((struct SceneNode*)contextMenu);
        return NULL;
    }
    strcpy(contextMenu->sceneNode.type, ContextMenuSceneNode_parserString.type);
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
    TextResource_decreasePointersCounter(contextMenu->sceneNodeTextResource);
    if (contextMenu->type)
        free(contextMenu->type);
    free(contextMenu);
}

/**
 * @brief Updates given #Button in by index ContextMenu#optionsList to the specified prototype.
 * Tryies to construct specified prototype and then uses Button_changeFocusedEventResource(), 
 * Button_changePressedEventResource() and Text_regenerateTexture() on that #Button (and its label).
 * If succeed, destructs old #Button in ContextMenu#optionsList and changes it to new.
 * @param contextMenu Pointer to a #ContextMenu, where #Button will be updated. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager for loading #Button resources. Can be NULL.
 * @param renderer Pointer to a #Renderer for constructing #Button. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar for constructing #Button. Can be NULL.
 * @param index Index of #Button in ContextMenu#optionsList to be updated. Can be anything (there is checking).
 * @param newButtonResId String with ID of new #Button prototype to load via #ResourceManager. Can be NULL.
 * @return #ContextMenuSceneNode_errors value.
 * @see #ContextMenu
 * @see #Button
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #ContextMenuSceneNode_parserStrings
 * @see #ContextMenuSceneNode_errors
 */
static enum ContextMenuSceneNode_errors ContextMenu_updateMenuOption(struct ContextMenu* contextMenu,
                                                  struct ResourceManager* const resourceManager,
                                                  struct Renderer* renderer,
                                                  struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                  size_t index,
                                                  const char* const newButtonResId) {
    if (!contextMenu || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !newButtonResId)
        return CONTEXT_MENU_ERR_NULL_ARGUMENT;
    if (index >= contextMenu->menuOptionsCount)
        return CONTEXT_MENU_ERR_INDEX_OUT_OF_RANGE;
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
                    ContextMenuScenNode_errorMessages.errConstructingMenuOption, newButtonResId);
            Logger_log(renderer->logger, tempErrString);
            return CONTEXT_MENU_ERR_BUTTON_CONSTRUCTING;
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
                    ContextMenuScenNode_errorMessages.errUpdatingOption, newButtonResId);
            Logger_log(renderer->logger, tempErrString);
            return CONTEXT_MENU_ERR_MENU_OPTION_UPDATING;
        }
        Button_destruct((struct SceneNode*)tempCopyFromButton);
        contextMenu->menuOptionsList[index] = tempButton;
    }
    return CONTEXT_MENU_NO_ERRORS;
    
}

/**
 * @brief Reallocates memory for the ContextMenu#optionsList.
 * ContextMenu#allocatedMenuOptions will be increased by #CONTEXT_MENU_SCENENODE_INITIAL_NUMBER_ALLOCATED_MENU_OPTIONS.
 * @param contextMenu Pointer to a #ContextMenu. Can be NULL.
 * @return #ContextMenuSceneNode_errors value.
 * @see #ContextMenuSceneNode_errors
 */
static enum ContextMenuSceneNode_errors ContextMenu_realloccateMenuOptionsList(struct ContextMenu* contextMenu) {
    if (!contextMenu)
        return CONTEXT_MENU_ERR_NULL_ARGUMENT;
    struct Button** menuOptionsList = NULL;
    size_t newSize = contextMenu->allocatedMenuOptions + CONTEXT_MENU_SCENENODE_INITIAL_NUMBER_ALLOCATED_MENU_OPTIONS;
    menuOptionsList = (struct Button**)realloc(contextMenu->menuOptionsList, sizeof(struct Button*) * newSize);
    if (!menuOptionsList)
        return CONTEXT_MENU_ERR_REALLOC;
    contextMenu->menuOptionsList = menuOptionsList;
    contextMenu->allocatedMenuOptions = newSize;
    return CONTEXT_MENU_NO_ERRORS;
}

/**
 * @brief Updates ContextMenu#optionsList in some way.
 * Update method depends on ContextMenu#menuOptionsCount, so here is example:
 * O - onlyOneMenuOptionPrototype, T - topMenuOptionPrototype, M - middleMenuOptionPrototype, L - lowerMenuOptionPrototype.
 * So, if ContextMenu#menuOptionsCount == 1 then #ContextMenu will looks like this [0. < O >].
 * If ContextMenu#menuOptionsCount == 2 then #ContextMenu will looks like this [0. < T >], [1. < L >].
 * If ContextMenu#menuOptionsCount == 3 then #ContextMenu will looks like this [0. < T >], [1. < M >], [2. < L >].
 * And so on. If some error happened than < O > prototype will be used.
 * @param contextMenu Pointer to a #ContextMenu, where ContextMenu#optionsList will be updated. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager for loading needed 
 * by ContextMenu_updateMenuOption() resources. Can be NULL.
 * @param renderer Pointer to a #Renderer for constructing #Button by ContextMenu_updateMenuOption(). Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar for constructing #Button 
 * by ContextMenu_updateMenuOption(). Can be NULL.
 * @return #ContextMenuSceneNode_errors value.
 * @see #ContextMenu
 * @see #Button
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see ContextMenu_updateMenuOption()
 * @see #ContextMenuSceneNode_parserStrings
 * @see #ContextMenuSceneNode_errors
 */
static enum ContextMenuSceneNode_errors CotextMenu_updateMenuOptionsList(struct ContextMenu* contextMenu,
                                                      struct ResourceManager* const resourceManager,
                                                      struct Renderer* const renderer,
                                                      struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar) {
    if (!contextMenu || !resourceManager || !renderer)
        return CONTEXT_MENU_ERR_NULL_ARGUMENT;
    size_t i;
    switch (contextMenu->menuOptionsCount) {
        case 0:
            break;
        case 1:
            if (ContextMenu_updateMenuOption(contextMenu, resourceManager,
                                             renderer, sceneNodeTypesRegistrar, 0,
                                             contextMenu->onlyOneMenuOptionPrototype))
                return CONTEXT_MENU_ERR_MENU_OPTION_UPDATING;
            break;
        case 2:
            if (ContextMenu_updateMenuOption(contextMenu, resourceManager,
                                             renderer, sceneNodeTypesRegistrar, 0,
                                             contextMenu->topMenuOptionPrototype)) {
                Logger_log(renderer->logger, ContextMenuScenNode_errorMessages.warnTryingDefault);
                if (ContextMenu_updateMenuOption(contextMenu, resourceManager,
                                                renderer, sceneNodeTypesRegistrar, 0,
                                                contextMenu->onlyOneMenuOptionPrototype))
                    return CONTEXT_MENU_ERR_MENU_OPTION_UPDATING;
            }
            if (ContextMenu_updateMenuOption(contextMenu, resourceManager, renderer,
                                             sceneNodeTypesRegistrar, 1,
                                             contextMenu->lowerMenuOptionPrototype)) {
                Logger_log(renderer->logger, ContextMenuScenNode_errorMessages.warnTryingDefault);
                if (ContextMenu_updateMenuOption(contextMenu, resourceManager,
                                                renderer, sceneNodeTypesRegistrar, 1,
                                                contextMenu->onlyOneMenuOptionPrototype))
                    return CONTEXT_MENU_ERR_MENU_OPTION_UPDATING;
            }
            break;
        default:
            for (i = 1; i < contextMenu->menuOptionsCount - 1; i++)
                if (ContextMenu_updateMenuOption(contextMenu, resourceManager,
                                                renderer, sceneNodeTypesRegistrar, i,
                                                contextMenu->middleMenuOptionPrototype)) {
                    Logger_log(renderer->logger, ContextMenuScenNode_errorMessages.warnTryingDefault);
                    if (ContextMenu_updateMenuOption(contextMenu, resourceManager,
                                                    renderer, sceneNodeTypesRegistrar, i,
                                                    contextMenu->onlyOneMenuOptionPrototype))
                        return CONTEXT_MENU_ERR_MENU_OPTION_UPDATING;
                }
             if (ContextMenu_updateMenuOption(contextMenu, resourceManager,
                                             renderer, sceneNodeTypesRegistrar,
                                             contextMenu->menuOptionsCount - 1,
                                             contextMenu->lowerMenuOptionPrototype)) {
                Logger_log(renderer->logger, ContextMenuScenNode_errorMessages.warnTryingDefault);
                if (ContextMenu_updateMenuOption(contextMenu, resourceManager,
                                                renderer, sceneNodeTypesRegistrar,
                                                contextMenu->menuOptionsCount - 1,
                                                contextMenu->onlyOneMenuOptionPrototype))
                    return CONTEXT_MENU_ERR_MENU_OPTION_UPDATING;
            }
            break;
    }
    return CONTEXT_MENU_NO_ERRORS;
}

enum ContextMenuSceneNode_errors ContextMenu_addMenuOption(struct ContextMenu* contextMenu,
                                        struct ResourceManager* const resourceManager,
                                        struct Renderer* const renderer,
                                        struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                        const char* const focusedEventRes,
                                        const char* const pressedEventRes,
                                        const char* const labelText) {
    // Don't care about NULL *EventRes and label strings, because we can add default menuOption
    if (!contextMenu || !resourceManager || !renderer || !sceneNodeTypesRegistrar)
        return CONTEXT_MENU_ERR_NULL_ARGUMENT;
    if (contextMenu->menuOptionsCount >= contextMenu->allocatedMenuOptions)
        if (ContextMenu_realloccateMenuOptionsList(contextMenu))
            return CONTEXT_MENU_ERR_REALLOC;
    size_t i;
    unsigned char result = 0;
    struct Button* tempButtonToAdd = NULL;
    tempButtonToAdd = (struct Button*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                            renderer,
                                                                            sceneNodeTypesRegistrar,
                                                                            contextMenu->onlyOneMenuOptionPrototype,
                                                                            ButtonSceneNode_parserStrings.type);
    if (!tempButtonToAdd)
        return CONTEXT_MENU_ERR_BUTTON_CONSTRUCTING;
    result += Button_changeFocusedEventResource(tempButtonToAdd, resourceManager, focusedEventRes);
    result += Button_changePressedEventResource(tempButtonToAdd, resourceManager, pressedEventRes);
    result += Text_regenerateTexture(tempButtonToAdd->label, resourceManager, renderer, labelText,
                            tempButtonToAdd->label->fontPath, tempButtonToAdd->label->size,
                            tempButtonToAdd->label->color);
    if (result)
        Logger_log(renderer->logger, ContextMenuScenNode_errorMessages.errUpdatingOption);
    for (i = 0; i < contextMenu->menuOptionsCount; i++)
        if (strcmp(tempButtonToAdd->label->text, contextMenu->menuOptionsList[i]->label->text) == 0) {
            char tempErrString[600];
            sprintf(tempErrString, "%s Label: <%s>", ContextMenuScenNode_errorMessages.errSameLabelExists,
                    tempButtonToAdd->label->text);
            Logger_log(renderer->logger, tempErrString);
            Button_destruct((struct SceneNode*)tempButtonToAdd);
            return CONTEXT_MENU_ERR_SAME_LABEL_EXISTS;
        }
    contextMenu->isGeometryChanged = true;
    contextMenu->menuOptionsList[contextMenu->menuOptionsCount] = tempButtonToAdd;
    contextMenu->menuOptionsCount++;
    if (CotextMenu_updateMenuOptionsList(contextMenu, resourceManager, renderer, sceneNodeTypesRegistrar)) {
        Button_destruct((struct SceneNode*)contextMenu->menuOptionsList[contextMenu->menuOptionsCount - 1]);
        contextMenu->menuOptionsCount--;
        return CONTEXT_MENU_ERR_LIST_UPDATING;
    }
    return CONTEXT_MENU_NO_ERRORS;
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

enum ContextMenuSceneNode_errors ContextMenu_save(
        const struct ContextMenu* const contextMenu, struct ResourceManager* const resourceManager,
        const char* const contextMenuResId) {
    if (!contextMenu || !resourceManager || !contextMenuResId)
        return CONTEXT_MENU_ERR_NULL_ARGUMENT;
    size_t i;
    unsigned char result = 0;
    struct TextParser* textParser = TextParser_constructEmpty();
    if (!textParser)
        return CONTEXT_MENU_ERR_CONSTRUCTIG_TEXT_PARSER;
    result += TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING,
                                   ContextMenuSceneNode_parserString.type);
    result += TextParser_addString(textParser, ContextMenuSceneNode_parserString.prototypes,
                         contextMenu->onlyOneMenuOptionPrototype);
    if (contextMenu->topMenuOptionPrototype 
        && contextMenu->middleMenuOptionPrototype 
        && contextMenu->lowerMenuOptionPrototype) {
        result += TextParser_addString(textParser, ContextMenuSceneNode_parserString.prototypes,
                            contextMenu->topMenuOptionPrototype);
        result += TextParser_addString(textParser, ContextMenuSceneNode_parserString.prototypes,
                            contextMenu->middleMenuOptionPrototype);
        result += TextParser_addString(textParser, ContextMenuSceneNode_parserString.prototypes,
                            contextMenu->lowerMenuOptionPrototype);
    }
    for (i = 0; i < contextMenu->menuOptionsCount; i++) {
        char tempMenuOptionName[600];
        sprintf(tempMenuOptionName, "%ld", i);
        result += TextParser_addString(textParser, ContextMenuSceneNode_parserString.optionsList, tempMenuOptionName);
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
    if (result)
        return CONTEXT_MENU_ERR_SAVING;
    return CONTEXT_MENU_NO_ERRORS;
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
