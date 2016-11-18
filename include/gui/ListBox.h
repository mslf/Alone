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
 * @file ListBox.h
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing #ListBox and it's stuff.
 */
#ifndef ALONE_LISTBOX_H
#define ALONE_LISTBOX_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "gui/TextBox.h"
#include "gui/Button.h"
#include "gui/ContextMenu.h"

/**
 * @brief Strings which are used for constructing #ListBox from #TextParser.
 * @see ListBox_construct()
 */
static const struct ListBoxSceneNode_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #ListBox resource from other. */
    const char* const textBoxRes;
    /**< #TextBox resource string, which is used to construct ListBox#textBox. */
    const char* const buttonRes;
    /**< #Button resoruce string, which is used to construct ListBox#button. */
    const char* const contextMenuRes;
    /**< #ContextMenu resource string, which is used to construct ListBox#contextMenu. */
}ListBoxSceneNode_parserStrings = {
    "ListBox",
    "textBoxResource",
    "buttonResource",
    "contextMenuResource"};

/**
 * @brief Error codes for #ListBox.
 */
enum ListBoxSceneNode_errors {
    LIST_BOX_NO_ERRORS = 0,
    /**< All right, no errors. */
    LIST_BOX_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    LIST_BOX_ERR_LOADING_SETTINGS = 2,
    /**< Loading required settings from #TextParser failed. */
    LIST_BOX_ERR_NO_TEXT_BOX_RES = 3,
    /**< No ListBoxSceneNode_parserString#textBoxRes string found in #TextParser. */
    LIST_BOX_ERR_TEXT_BOX_CONSTRUCTING = 4,
    /**< Constructing ListBox#textBox failed. */
    LIST_BOX_ERR_NO_BUTTON_RES = 5,
    /**< No ListBoxSceneNode_parserString#buttonRes string found in #TextParser. */
    LIST_BOX_ERR_BUTTON_CONSTRUCTING = 6,
    /**< Constructing ListBox#button failed. */
    LIST_BOX_ERR_NO_CONTEXT_MENU_RES = 7,
    /**< No ListBoxSceneNode_parserString#contextMenuRes string found in #TextParser. */
    LIST_BOX_ERR_CONTEXT_MENU_CONSTRUCTING = 8,
    /**< Constructing ListBox#contextMenu failed. */
    LIST_BOX_ERR_CONTEXT_MENU_LABEL_LENGTH_ABOVE_MAX = 9,
    /**< Some of ListBox#contextMenu menuOption's label string length is above ListBox#textBox#maxLength. */
    LIST_BOX_ERR_CONSTRUCTIG_TEXT_PARSER = 10,
    /**< Constructing new #TextParser for some required reason failed. */
    LIST_BOX_ERR_SAVING = 11
    /**< Saving #ListBox failed due to internal reason. */
};
    
/**
 * @brief Standart Gui element for list box.
 * Normally draws ListBox#textBox and ListBox#button.
 * If user clicks ListBox#button, then ListBox#contextMenu will be displayed.
 * User can choose some menu option, and it will appears in ListBox#textBox.
 * @warning #ListBox is an inheritor of the SceneNode.
 * It means, that other code is use <B>pointer casting</B> to #ListBox from #SceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct SceneNode blablaNode;</tt> at the begining of #ListBox struct, 
 * if you want code to work with #ListBox like with a #SceneNode. 
 * More, you <B>SHOULD</B> initialize function pointers in <tt>blablaNode</tt> to NULL (by calling SceneNode_init()) 
 * or to your function implementation. 
 * Don't forget to add this warning comment to your own new #SceneNode inheritors.
 * @see #TextBox
 * @see #Button
 * @see #ContextMenu
 * @see ListBox_control()
 * @see ListBox_update()
 * @see ListBox_render()
 * @see ListBox_sound()
 */
struct ListBox {
    struct SceneNode sceneNode;
    /**< Inheritance stuff. */
    struct TextBox* textBox;
    /**< #TextBox, where user can input some text. 
     *It will indicates if user input string, which there is in ListBox#contextMenu. */
    struct Button* button;
    /**< #Button to be clicked by user for displaying ListBox#contextMenu. */
    struct ContextMenu* contextMenu;
    /**< #ContextMenu with possible data strings for #ListBox. */
    bool isContextMenuShown;
    /**< Flag, which is answers "Do we need to draw ListBox#contextMenu right now?". */
    bool isGeometryChanged;
    /**< Flag, which is need to be changed when someone tried to move #ListBox. */
    bool isStringExistInList;
    /**< Flag, which is indicates existing current ListBox#textBox#text 
     *string in some label in ListBox#contextMenu. */
};

/**
 * @brief Constructs #ListBox and inits it from a #TextParser.
 * @param resourceManager Pointer to a #ResourceManager which is used to load additional 
 * resources, needed for #ListBox. Can be NULL.
 * @param renderer Pointer to a #Renderer which is used to construct ListBox#button,
 * ListBox#textBox and ListBox#contextMenu. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar which is used to 
 * construct ListBox#button, ListBox#textBox and ListBox#contextMenu. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #ListBox. Can be NULL.
 * @return Pointer to an initialized #ListBox, casted to #SceneNode, or NULL if constructing failed.
 * @note Use only ListBox_destruct() or ListBox#sceneNode#destruct() to destruct returned #SceneNode.
 * @see #ListBox
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #ListBoxSceneNode_parserString
 */
struct SceneNode* ListBox_construct(struct ResourceManager* const resourceManager,
                                    struct Renderer* const renderer,
                                    struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                    struct TextParser* const textParser);

/**
 * @brief Destructs #ListBox and frees memory, used by it.
 * Pointer to that function should be at ListBox#sceneNode#destruct.
 * @param listBox Pointer to a #ListBox, casted to #SceneNode. Can be not fully initialized. Can be NULL.
 * @warning Use this function only in pair with ListBox_construct()!
 * @see ListBox_construct()
 */
void ListBox_destruct(struct SceneNode* listBox);

/**
 * @brief Saves #ListBox to the filesystem via #ResourceManager.
 * Before saving, it updates ListBox#sceneNode#sceneNodeTextResource with the latest changes in #ListBox.
 * @param listBox Pointer to a #ListBox which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save ListBox#sceneNode#sceneNodeTextResource. Can be NULL.
 * @param listBoxResId Path string, where #ResourceManager will 
 * save ListBox#sceneNode#sceneNodeTextResource. Can be NULL.
 * @return #ListBoxSceneNode_errors value.
 * @see #ListBox
 * @see #SceneNode
 * @see #ResourceManager
 * @see #ListBoxSceneNode_errors
 */
enum ListBoxSceneNode_errors ListBox_save(const struct ListBox* const listBox,
                                          struct ResourceManager* const resourceManager,
                                          const char* const listBoxResId);

/**
 * @brief Function to control user input events to #ListBox.
 * Pointer to that function should be at ListBox#sceneNode#control.
 * @param sceneNode Pointer to a #ListBox, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will get user input events. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #ListBox is not accepted!
 */
void ListBox_control(struct SceneNode* sceneNode, struct EventManager* eventManager);

/**
 * @brief Function to update #ListBox and interacting with #EventManager.
 * Pointer to that function should be at ListBox#sceneNode#update.
 * @param sceneNode Pointer to a #ListBox, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will place it's events. Can be NULL.
 * @param renderer Pointer to a #Renderer for screen-depend coordinates converting. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #ListBox is not accepted!
 * @note Don't use this function for drawing.
 */
void ListBox_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);

/**
 * @brief Function to render #ListBox on the screen.
 * Pointer to that function should be at ListBox#sceneNode#render.
 * @param sceneNode Pointer to a #ListBox, casted to #SceneNode. Can be NULL.
 * @param renderer Pointer to a #Renderer for drawing #ListBox. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #ListBox is not accepted!
 */
void ListBox_render(struct SceneNode* sceneNode, struct Renderer* renderer);

/**
 * @brief Function to play sounds of a #ListBox.
 * Pointer to that function should be at ListBox#sceneNode#sound.
 * @param sceneNode Pointer to a #ListBox, casted to #SceneNode. Can be NULL.
 * @param musican Pointer to a #Musican for playing audio of a #ListBox. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #ListBox is not accepted!
 */
void ListBox_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_LISTBOX_H
