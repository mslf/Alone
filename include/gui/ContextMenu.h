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
 * @file ContextMenu.h
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing #ContextMenu and it's stuff.
 */
#ifndef ALONE_CONTEXTMENU_H
#define ALONE_CONTEXTMENU_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "gui/Button.h"

/**
 * @brief Strings which are used for constructing #ContextMenu from #TextParser.
 * @see ContextMenu_construct()
 * @see ContextMenu_constructMenuOptions()
 * @note ContextMenuSceneNode_parserStrings#optionsList doesn't contain actual 
 * settings (label text, focusedEventRes, pressedEventRes) of the constructed #Button for new menuOption.
 * It contains array of menuOption names, which will be found in #TextParser, so they will contain actual data. 
 */
static const struct ContextMenuSceneNode_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #ContextMenu resource from other. */
    const char* const prototypes;
    /**< #Button resource strings, which are used to construct ContextMenu#menuOptionsList.
     * Note, this string should have 1 #Button resource (if you want every menuOption to be equal)
     * or 4 #Button resources (first - for only one menuOption (or alone), second - for top menuOption,
     * third - for middle menuOption (between top and lower) and fourth - for lower menuOption. */
    const char* const optionsList;
    /**< List of strings with name of menuOptions, which #ContextMenu will find in #TextParser. */
}ContextMenuSceneNode_parserStrings = {
    "ContextMenu",
    "optionPrototypeButtonResources",
    "menuOptions"};

/**
 * @brief Error codes for #ContextMenu.
 */
enum ContextMenuSceneNode_errors {
    CONTEXT_MENU_NO_ERRORS = 0,
    /**< All right, no errors. */
    CONTEXT_MENU_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    CONTEXT_MENU_ERR_LOADING_SETTINGS = 2,
    /**< Loading required settings from #TextParser failed. */
    CONTEXT_MENU_ERR_NO_PROTOTYPES = 3,
    /**< No ContextMenuSceneNode_parserStrings#prototypes string found in #TextParser. */
    CONTEXT_MENU_ERR_ALLOC_PROTOTYPE_STRING = 4,
    /**< Allocating memory for ContextMenu#onlyOneMenuOptionPrototype string failed. */
    CONTEXT_MENU_ERR_SAVING = 5,
    /**< Saving #ContextMenu failed due to internal reason. */
    CONTEXT_MENU_ERR_REALLOC = 6,
    /**< Reallocating ContextMenu#menuOptionsList failed. */
    CONTEXT_MENU_ERR_BUTTON_CONSTRUCTING = 7,
    /**< Constructing #Button failed. */
    CONTEXT_MENU_ERR_SAME_LABEL_EXISTS = 8,
    /**< Adding new menuOption #Button failed because #Button with the same 
     *label already exists in ContextMenu#menuOptionsList. */
    CONTEXT_MENU_ERR_LIST_UPDATING = 9,
    /**< Updating ContextMenu#menuOptionsList failed. */
    CONTEXT_MENU_ERR_MENU_OPTION_UPDATING = 10,
    /**< Updating one #Button from ContextMenu#menuOptionsList failed. */
    CONTEXT_MENU_ERR_INDEX_OUT_OF_RANGE = 11,
    /**< Some index, needed by function is out of range. */
    CONTEXT_MENU_ERR_NO_OPTIONS = 12,
    /**< No ContextMenuSceneNode_parserStrings#optionsList string found in #TextParser. */
    CONTEXT_MENU_ERR_CONSTRUCTIG_TEXT_PARSER = 13
    /**< Constructing new #TextParser for some required reason failed. */
};
    
/**
 * @brief Some initial constants for #ContextMeu.
 */
enum ContextMenuSceneNode_constants{
    CONTEXT_MENU_SCENENODE_INITIAL_NUMBER_ALLOCATED_MENU_OPTIONS = 3
    /**< Init alocating number and reallocating step for ContextMenu#menuOptionsList. */
};

/**
 * @brief Standart Gui element for drawing menu, builded from #Button.
 * All menuOptions are have the similar to theirs prototypes design, but 
 * events and labels can be unique for each menuOption.
 * User can click the #ContextMenu, and some things will happen 
 * (such a #GameEvent or sound playing in appropriate #Button).
 * @warning #ContextMenu is an inheritor of the SceneNode.
 * It means, that other code is use <B>pointer casting</B> to #ContextMenu from #SceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct SceneNode blablaNode;</tt> at the begining of #ContextMenu struct, 
 * if you want code to work with #ContextMenu like with a #SceneNode. 
 * More, you <B>SHOULD</B> initialize function pointers in <tt>blablaNode</tt> to NULL (by calling SceneNode_init()) 
 * or to your function implementation. 
 * Don't forget to add this warning comment to your own new #SceneNode inheritors.
 * @see ContextMenu_control()
 * @see ContextMenu_update()
 * @see ContextMenu_render()
 * @see ContextMenu_sound()
 */
struct ContextMenu {
    struct SceneNode sceneNode;
    /**< Inheritance stuff. */
    char* onlyOneMenuOptionPrototype;
    /**< Path string to the only one menuOption (alone) #Button resource. */
    char* topMenuOptionPrototype;
    /**< Path string to the top #Button resource. */
    char* middleMenuOptionPrototype;
    /**< Path string to the middle #Button resource. */
    char* lowerMenuOptionPrototype;
    /**< Path string to the lower #Button resource. */
    struct Button** menuOptionsList;
    /**< List of #Button, which are represents menuOptions. */
    size_t allocatedMenuOptions;
    /**< Allocated number of #Button in the ContextMenu#menuOptionsList. */
    size_t menuOptionsCount;
    /**< Current number of existing #Button in the ContextMenu#menuOptionsList */
    bool isGeometryChanged;
    /**< Flag, which is need to be changed when someone tried to move #ContextMenu, 
     *update ContextMenu#sceneNode or change ContextMenu#menuOptionsList. */
};

/**
 * @brief Constructs #ContextMenu and inits it from a #TextParser.
 * @param resourceManager Pointer to a #ResourceManager which is used to load additional 
 * resources, needed for #ContextMenu. Can be NULL.
 * @param renderer Pointer to a #Renderer which is used to construct #Button for menuOptions. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar which is used to 
 * construct #Button for menuOption. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #ContextMenu. Can be NULL.
 * @return Pointer to an initialized #ContextMenu, casted to #SceneNode, or NULL if constructing failed.
 * @note Use only ContextMenu_destruct() or ContextMenu#sceneNode#destruct() to destruct returned #SceneNode.
 * @see #ContextMenu
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #ContextMenuSceneNode_parserStrings
 */
struct SceneNode* ContextMenu_construct(struct ResourceManager* const resourceManager,
                                        struct Renderer* const renderer,
                                        struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                        struct TextParser* const textParser);

/**
 * @brief Destructs #ContextMenu and frees memory, used by it.
 * Pointer to that function should be at ContextMenu#sceneNode#destruct.
 * @param contextMenu Pointer to a #ContextMenu, casted to #SceneNode. Can be not fully initialized. Can be NULL.
 * @warning Use this function only in pair with ContextMenu_construct()!
 * @see ContextMenu_construct()
 */
void ContextMenu_destruct(struct SceneNode* contextMenu);

/**
 * @brief Adds new menuOption to the ContextMenu#menuOptionsList with specified data.
 * Also, it increases ContextMenu#menuOptionsCount and changes ContextMenu#allocatedMenuOptions, if needed.
 * @param contextMenu Pointer to a #ContetMenu, where to add new menuOption. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to load #Button resoures 
 * for constructing new menuOption. Can be NULL.
 * @param renderer Pointer to a #Renderer which is used to construct #Button 
 * for constructing new menuOption. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar which is used to 
 * construct #Button for constructing new menuOption. Can be NULL.
 * @param focusedEventRes String with ID of new #GameEvent, which will be set to new 
 * menuOption Button#focusedEventResource and Button#focusedGameEvent.
 * @param pressedEventRes String with ID of new #GameEvent, which will be set to new 
 * menuOption Button#pressedEventResource and Button#pressedGameEvent.
 * @param labelText String to set new menuOption Button#label#text. 
 * Should be unique in the ContextMenu#menuOptionsList.
 * @return ContextMenuSceneNode_errors value.
 * @note This function causes CotextMenu_updateMenuOptionsList().
 * @see #ContextMenu
 * @see #ResourceManager
 * @see #Renderer
 * @see #SceneNodeTypesRegistrar
 * @see #GameEvent
 * @see #Button
 * @see #ContextMenuSceneNode_errors
 */
enum ContextMenuSceneNode_errors ContextMenu_addMenuOption(struct ContextMenu* contextMenu,
                                        struct ResourceManager* const resourceManager,
                                        struct Renderer* const renderer,
                                        struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                        const char* const focusedEventRes,
                                        const char* const pressedEventRes,
                                        const char* const labelText);

/**
 * @brief Removes menuOption from the ContextMenu#menuOptionsList with specified label text.
 * Also, it decreases ContextMenu#menuOptionsCount.
 * @param contextMenu Pointer to a #ContetMenu, where to add new menuOption. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to update 
 * ContextMenu#menuOptionsList. Can be NULL.
 * @param renderer Pointer to a #Renderer which is used to to update 
 * ContextMenu#menuOptionsList. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar which is used to update 
 * ContextMenu#menuOptionsList. Can be NULL.
 * @param label String to find menuOption in the ContextMenu#menuOptionsList (Button#label#text). 
 * @note This function causes CotextMenu_updateMenuOptionsList().
 * @see #ContextMenu
 * @see #ResourceManager
 * @see #Renderer
 * @see #SceneNodeTypesRegistrar
 * @see #Button
 */
void ContextMenu_removeMenuOption(struct ContextMenu* contextMenu,
                                                              struct ResourceManager* const resourceManager,
                                                              struct Renderer* const renderer,
                                                              struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                              const char* const label);

/**
 * @brief Saves #ContextMenu to the filesystem via #ResourceManager.
 * Before saving, it updates ContextMenu#sceneNode#sceneNodeTextResource with the latest changes in #ContextMenu.
 * @param contextMenu Pointer to a #ContextMenu which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save ContextMenu#sceneNode#sceneNodeTextResource. Can be NULL.
 * @param contextMenuResId Path string, where #ResourceManager will 
 * save ContextMenu#sceneNode#sceneNodeTextResource. Can be NULL.
 * @return #ContextMenuSceneNode_errors value.
 * @see #ContextMenu
 * @see #SceneNode
 * @see #ResourceManager
 * @see #ContextMenuSceneNode_errors
 */
enum ContextMenuSceneNode_errors ContextMenu_save(const struct ContextMenu* const contextMenu,
                                                  struct ResourceManager* const resourceManager,
                                                  const char* const contextMenuResId);

/**
 * @brief Function to control user input events to #ContextMenu.
 * Pointer to that function should be at ContextMenu#sceneNode#control.
 * @param sceneNode Pointer to a #ContextMenu, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will get user input events. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #ContextMenu is not accepted!
 */
void ContextMenu_control(struct SceneNode* sceneNode, struct EventManager* eventManager);

/**
 * @brief Function to update #ContextMenu and interacting with #EventManager.
 * Pointer to that function should be at ContextMenu#sceneNode#update.
 * @param sceneNode Pointer to a #ContextMenu, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will place it's events. Can be NULL.
 * @param renderer Pointer to a #Renderer for screen-depend coordinates converting. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #ContextMenu is not accepted!
 * @note Don't use this function for drawing.
 */
void ContextMenu_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);

/**
 * @brief Function to render #ContextMenu on the screen.
 * Pointer to that function should be at ContextMenu#sceneNode#render.
 * @param sceneNode Pointer to a #ContextMenu, casted to #SceneNode. Can be NULL.
 * @param renderer Pointer to a #Renderer for drawing #ContextMenu. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #ContextMenu is not accepted!
 */
void ContextMenu_render(struct SceneNode* sceneNode, struct Renderer* renderer);

/**
 * @brief Function to play sounds of a #ContextMenu.
 * Pointer to that function should be at ContextMenu#sceneNode#sound.
 * @param sceneNode Pointer to a #ContextMenu, casted to #SceneNode. Can be NULL.
 * @param musican Pointer to a #Musican for playing audio of a #ContextMenu. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #ContextMenu is not accepted!
 */
void ContextMenu_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_CONTEXTMENU_H
