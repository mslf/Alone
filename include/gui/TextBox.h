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
 * @file TextBox.h
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing #TextBox and its stuff.
 */
#ifndef ALONE_TEXTBOX_H
#define ALONE_TEXTBOX_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "gui/Button.h"

/**
 * @brief Strings which are used for constructing #TextBox from #TextParser.
 * @see TextBox_construct
 * @note TextBoxSceneNode_parserStrings#maxLength value contains maximum possible 
 * number of <B>bytes</B> in TextBox#string.
 */
static const struct TextBoxSceneNode_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #TextBox resource from other. */
    const char* const buttonRes;
    /**< #Button resource string, which is used to construct TextBox#box. */
    const char* const maxLength;
    /**< String with number to set TextBox#maxLength. */
}TextBoxSceneNode_parserStrings = {
    "TextBox",
    "buttonResource",
    "maxLength"};

/**
 * @brief Error codes for #TextBox.
 */
enum TextBoxSceneNode_errors {
    TEXT_BOX_NO_ERRORS = 0,
    /**< All right, no errors. */
    TEXT_BOX_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    TEXT_BOX_ERR_NO_BUTTON_RES = 2,
    /**< No TextBoxSceneNode_parserStrings#buttonRes string found in #TextParser. */
    TEXT_BOX_ERR_CONSTRUCTIG_BUTTON = 3,
    /**< Constructing TextBox#box failed. */
    TEXT_BOX_ERR_LENGTH_ABOVE_MAX = 4,
    /**< TextBox#box#label#text string or TextBox#string length is more than TextBox#maxLength. */
    TEXT_BOX_ERR_ALLOC_STRING = 5,
    /**< Allocating or reallocating memory for TextBox#string failed. */
    TEXT_BOX_ERR_CONSTRUCTIG_TEXT_PARSER = 6,
    /**< Constructing new #TextParser for some required reason failed. */
    TEXT_BOX_ERR_SAVING = 7
    /**< Saving #TextBox failed due to internal reason. */
};    
    
/**
 * @brief Some initial constants for #TextBox.
 */
enum TextBoxSceneNode_constants{
    TEXT_BOX_SCENENODE_REALLOC_STRING_LENGTH_STEP = 500,
    /**< Init alocating number and reallocating step for TextBox#string. */
    TEXT_BOX_SCENENODE_MAX_LENGTH = 10
    /**< Default TextBox#maxLength. */
};

/**
 * @brief Standard Gui element for some text editing, builded from #Button.
 * User can input, delete, copy and paste text strings in it.
 * User can click the #TextBox, and some things will happen 
 * (such a #GameEvent or sound playing in TextBox#box).
 * @warning #TextBox is an inheritor of the SceneNode.
 * It means, that other code is use <B>pointer casting</B> to #TextBox from #SceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct SceneNode blablaNode;</tt> at the begining of #TextBox struct, 
 * if you want code to work with #TextBox like with a #SceneNode. 
 * More, you <B>SHOULD</B> initialize function pointers in <tt>blablaNode</tt> to NULL (by calling SceneNode_init()) 
 * or to your function implementation. 
 * Don't forget to add this warning comment to your own new #SceneNode inheritors.
 * @see TextBox_control()
 * @see TextBox_update()
 * @see TextBox_render()
 * @see TextBox_sound()
 */
struct TextBox {
    struct SceneNode sceneNode;
    /**< Inheritance stuff. */
    struct Button* box;
    /**< #Button which is represents #TextBox. TextBox#box#label will be editable. */
    char* string;
    /**< Buffer string for editing. */
    size_t allocatedChars;
    /**< Allocated number of bytes in TextBox#string. */
    size_t stringLength;
    /**< Current position of last symbol. */
    size_t maxLength;
    /**< Maximum possible length of TextBox#string. */
    bool haveFocus;
    /**< Flag, which is indicates has user focus by #TextBox. */
    bool isGeometryChanged;
    /**< Flag, which is need to be changed when someone tried to move #TextBox. */
    bool isStringChanged;
    /**< Flag, which is need to be changed when someone tried to change TextBox#string and 
     * Button#label need to be regenerated. */
};

/**
 * @brief Constructs #TextBox and inits it from a #TextParser.
 * @param resourceManager Pointer to a #ResourceManager which is used to load additional 
 * resources, needed for #TextBox. Can be NULL.
 * @param renderer Pointer to a #Renderer which is used to construct TextBox#box. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar which is used to 
 * construct TextBox#box. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #TextBox. Can be NULL.
 * @return Pointer to an initialized #TextBox, casted to #SceneNode, or NULL if constructing failed.
 * @note Use only TextBox_destruct() or TextBox#sceneNode#destruct() to destruct returned #SceneNode.
 * @see #TextBox
 * @see #Button
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #TextBoxSceneNode_parserStrings
 */
struct SceneNode* TextBox_construct(struct ResourceManager* const resourceManager,
                                    struct Renderer* const renderer,
                                    struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                    struct TextParser* const textParser);

/**
 * @brief Destructs #TextBox and frees memory, used by it.
 * Pointer to that function should be at TextBox#sceneNode#destruct.
 * @param textBox Pointer to a #TextBox, casted to #SceneNode. Can be not fully initialized. Can be NULL.
 * @warning Use this function only in pair with TextBox_construct()!
 * @see TextBox_construct()
 */
void TextBox_destruct(struct SceneNode* textBox);

/**
 * @brief Changes TextBox#string, sets TextBox#isStringChanged and reallocates #TextBox#string, if needed.
 * @param textBox Pointer to a #TextBox, casted to #SceneNode. Can be NULL.
 * @param newString String, which will be setted to TextBox#string. Can be NULL.
 * @return #TextBoxSceneNode_errors value.
 * @see #TextBox
 * @see #TextBoxSceneNode_errors
 */
enum TextBoxSceneNode_errors TextBox_changeString (struct TextBox* textBox, const char* const newString);

/**
 * @brief Saves #TextBox to the filesystem via #ResourceManager.
 * Before saving, it updates TextBox#sceneNode#sceneNodeTextResource with the latest changes in #TextBox.
 * @param textBox Pointer to a #TextBox which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save TextBox#sceneNode#sceneNodeTextResource. Can be NULL.
 * @param textBoxResId Path string, where #ResourceManager will 
 * save TextBox#sceneNode#sceneNodeTextResource. Can be NULL.
 * @return #TextBoxSceneNode_errors value.
 * @see #TextBox
 * @see #SceneNode
 * @see #ResourceManager
 * @see #TextBoxSceneNode_errors
 */
enum TextBoxSceneNode_errors TextBox_save(const struct TextBox* const textBox,
                                          struct ResourceManager* const resourceManager,
                                          const char* const textBoxResId);

/**
 * @brief Function to control user input events to #TextBox.
 * Pointer to that function should be at TextBox#sceneNode#control.
 * @param sceneNode Pointer to a #TextBox, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will get user input events. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #TextBox is not accepted!
 */
void TextBox_control(struct SceneNode* sceneNode, struct EventManager* eventManager);

/**
 * @brief Function to update #TextBox and interacting with #EventManager.
 * Pointer to that function should be at TextBox#sceneNode#update.
 * @param sceneNode Pointer to a #TextBox, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will place it's events. Can be NULL.
 * @param renderer Pointer to a #Renderer for screen-depend coordinates converting. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #TextBox is not accepted!
 * @note Don't use this function for drawing.
 */
void TextBox_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);

/**
 * @brief Function to render #TextBox on the screen.
 * Pointer to that function should be at TextBox#sceneNode#render.
 * @param sceneNode Pointer to a #TextBox, casted to #SceneNode. Can be NULL.
 * @param renderer Pointer to a #Renderer for drawing #TextBox. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #TextBox is not accepted!
 */
void TextBox_render(struct SceneNode* sceneNode, struct Renderer* renderer);

/**
 * @brief Function to play sounds of a #TextBox.
 * Pointer to that function should be at TextBox#sceneNode#sound.
 * @param sceneNode Pointer to a #TextBox, casted to #SceneNode. Can be NULL.
 * @param musican Pointer to a #Musican for playing audio of a #TextBox. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #TextBox is not accepted!
 */
void TextBox_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_TEXTBOX_H
