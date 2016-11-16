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
 * @file CheckBox.h
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing #CheckBox and it's stuff.
 */
#ifndef ALONE_CHECKBOX_H
#define ALONE_CHECKBOX_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "sprite/Sprite.h"

/**
 * @brief Strings which are used for constructing #CheckBox from #TextParser.
 * @see CheckBox_construct
 * @note CheckBox#sprite, constructed from CheckBoxSceneNode_parserString#spriteRes should have 4 animations.
 */
static const struct CheckBoxSceneNode_parserString {
    const char* const type;
    /**< Type string, which is used to distinquish #CheckBox resource from other. */
    const char* const focusedEventRes;
    /**< Focused #GameEvent resource string, which is used to construct CheckBox#focusedEvent. */
    const char* const checkedEventRes;
    /**< Focused #GameEvent resource string, which is used to construct CheckBox#checkedEvent. */
    const char* const unCheckedEventRes;
    /**< Focused #GameEvent resource string, which is used to construct CheckBox#unChecedEvent. */
    const char* const focusedSoundRes;
    /**< Focused #SoundResource string, which is used to construct CheckBox#focusedSoundResource. */
    const char* const checkedSoundRes;
    /**< Focused #SoundResource string, which is used to construct CheckBox#checkedSoundResource. */
    const char* const unCheckedSoundRes;
    /**< Focused #SoundResource string, which is used to construct CheckBox#unCheckedSoundResource. */
    const char* const spriteRes;
    /**< #Sprite resource string, which is used to construct CheckBox#sprite. */
}CheckBoxSceneNode_parserString = {
    "CheckBox",
    "focusedEventResource",
    "checkedEventResource",
    "unCheckedEventResource",
    "focusedSoundResource",
    "checkedSoundResource",
    "unCheckedSoundResource",
    "spriteResource"};

/**
 * @brief Error codes for #CheckBox.
 */
enum CheckBoxSceneNode_errors {
    CHECK_BOX_NO_ERRORS = 0,
    /**< All right, no errors. */
    CHECK_BOX_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    CHECK_BOX_ERR_NO_SPRITE_RES = 2,
    /**< No CheckBoxSceneNode_parserStrings#spriteRes string found in #TextParser. */
    CHECK_BOX_ERR_CONSTRUCTIG_SPRITE = 3,
    /**< Constructing CheckBox#sprite failed. */
    CHECK_BOX_ERR_NOT_ENOUGH_ANIMATIONS = 4,
    /**< Constructed CheckBox#sprite doesn't have enough animations. */
    CHECK_BOX_ERR_NO_FOCUSED_EVENT_RES = 5,
    /**< Loading new CheckBox#focusedEventResource from #ResourceManager failed. */
    CHECK_BOX_ERR_NO_CHECKED_EVENT_RES = 6,
    /**< Loading new CheckBox#checkedEventResource from #ResourceManager failed. */
    CHECK_BOX_ERR_NO_UNCHECKED_EVENT_RES = 7,
    /**< Loading new CheckBox#unCheckedEventResource from #ResourceManager failed. */
    CHECK_BOX_ERR_CONSTRUCTIG_FOCUSED_EVENT = 8,
    /**< Constructing new CheckBox#focusedEvent failed. */
    CHECK_BOX_ERR_CONSTRUCTIG_CHECKED_EVENT = 9,
    /**< Constructing new CheckBox#checkedEvent failed. */
    CHECK_BOX_ERR_CONSTRUCTIG_UNCHECKED_EVENT = 10,
    /**< Constructing new CheckBox#unChecedEvent failed. */
    CHECK_BOX_ERR_CONSTRUCTIG_TEXT_PARSER = 11,
    /**< Constructing new #TextParser for some required reason failed. */
    CHECK_BOX_ERR_SAVING = 12
    /**< Saving #CheckBox failed due to internal reason. */
};
    
/**
 * @brief Possible CheckBox states.
 * @see CheckBox_control()
 * @see CheckBox_update()
 * @see CheckBox_render()
 * @see CheckBox_sound()
 */
enum CheckBoxState {
    CHECK_BOX_STATE_UNCHECKED = 0,
    /**< #CheckBox is unchecked. */
    CHECK_BOX_STATE_FOCUSED_UNCHEKED = 1,
    /**< #CheckBox is unchecked and user moves cursor on it. */
    CHECK_BOX_STATE_CHECKED = 2,
    /**< #CheckBox is checked. */
    CHECK_BOX_STATE_FOCUSED_CHECKED = 3,
    /**< #CheckBox is checked and user moves cursor on it. */
    CHECK_BOX_STATE_CHECKING = 4,
    /**< User presses mouse button on the #CheckBox while it
     * was CheckBoxState#CHECK_BOX_STATE_UNCHECKED and still holds it. */
    CHECK_BOX_STATE_UNCHECKING = 5
    /**< User presses mouse button on the #CheckBox while it
     * was CheckBoxState#CHECK_BOX_STATE_CHECKED and still holds it. */
};

/**
 * @brief Standart Gui element to be clicked with holding it's state through the frames.
 * Has focused, checked and unChecked #GameEvent and #SoundResource.
 * User can click the #CheckBox, and some things will happen (such a #GameEvent or sound playing).
 * Also it's useful for other Gui elements, which are needed to be clicked with saving state.
 * @warning #CheckBox is an inheritor of the SceneNode.
 * It means, that other code is use <B>pointer casting</B> to #CheckBox from #SceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct SceneNode blablaNode;</tt> at the begining of #CheckBox struct, 
 * if you want code to work with #CheckBox like with a #SceneNode. 
 * More, you <B>SHOULD</B> initialize function pointers in <tt>blablaNode</tt> to NULL (by calling SceneNode_init()) 
 * or to your function implementation. 
 * Don't forget to add this warning comment to your own new #SceneNode inheritors.
 * @see CheckBox_control()
 * @see CheckBox_update()
 * @see CheckBox_render()
 * @see CheckBox_sound()
 */
struct CheckBox {
    struct SceneNode sceneNode;
    /**< Inheritance stuff. */
    struct Sprite* sprite;
    /**< #Sprite to be displayed on the screen. 
     * Should have 4 animations (unchecked, focusedUnchecked, checked, focusedChecked). */
    struct SoundResource* focusedSoundResource;
    /**< Sound to be played when the #CheckBox receives focus. */
    struct SoundResource* checkedSoundResource;
    /**< Sound to be played when the #CheckBox checked. */
    struct SoundResource* unCheckedSoundResource;
    /**< Sound to be played when the #CheckBox unchecked. */
    struct TextResource* focusedEventResource;
    /**< #TextResource to construct CheckBox#focusedEvent. */
    struct TextResource* checkedEventResource;
    /**< #TextResource to construct CheckBox#checkedEvent. */
    struct TextResource* unCheckedEventResource;
    /**< #TextResource to construct CheckBox#unCheckedEvent. */
    struct GameEvent* focusedEvent;
    /**< #GameEvent to be added to the #EventManager when the #CheckBox receives focus. */
    struct GameEvent* checkedEvent;
    /**< #GameEvent to be added to the #EventManager when the #CheckBox checked. */
    struct GameEvent* unCheckedEvent;
    /**< #GameEvent to be added to the #EventManager when the #CheckBox unchecked. */
    bool isGeometryChanged;
    /**< Flag, which is need to be changed when someone tried to move #CheckBox, or update CheckBox#SceneNode. */
    bool isStateChanged;
    /**< Flag, which is need to be changed when someone tried to change CheckBox#state. */
    enum CheckBoxState state;
    /**< Current #CheckBox state. */
};

/**
 * @brief Constructs #CheckBox and inits it from a #TextParser.
 * @param resourceManager Pointer to a #ResourceManager which is used to load additional 
 * resources, needed for #CheckBox. Can be NULL.
 * @param renderer Pointer to a #Renderer which is used to construct CheckBox#sprite. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar which is used to 
 * construct CheckBox#sprite. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #CheckBox. Can be NULL.
 * @return Pointer to an initialized #CheckBox, casted to #SceneNode, or NULL if constructing failed.
 * @note Use only CheckBox_destruct() or CheckBox#sceneNode#destruct() to destruct returned #SceneNode.
 * @see #CheckBox
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #CheckBoxSceneNode_parserString
 */
struct SceneNode* CheckBox_construct(struct ResourceManager* const resourceManager,
                                     struct Renderer* const renderer,
                                     struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                     struct TextParser* const textParser);

/**
 * @brief Destructs #CheckBox and frees memory, used by it.
 * Pointer to that function should be at CheckBox#sceneNode#destruct.
 * @param checkBox Pointer to a #CheckBox, casted to #SceneNode. Can be not fully initialized. Can be NULL.
 * @warning Use this function only in pair with CheckBox_construct()!
 * @see CheckBox_construct()
 */
void CheckBox_destruct(struct SceneNode* checkBox);

/**
 * @brief Changes CheckBox#focusedEventResource and CheckBox#focusedEvent.
 * Sets old CheckBox#focusedEvent#isNeeded to false, if exists.
 * Decreases old CheckBox#focusedEventResource#pointersCount if exists.
 * @param checkBox Pointer to a #CheckBox. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to load 
 * new CheckBox#focusedEventResource. Can be NULL.
 * @param focusedEventResId String with ID of new #GameEvent to load via #ResourceManager. Can be NULL.
 * @return #CheckBoxSceneNode_errors value.
 * @see #CheckBox
 * @see #GameEvent
 * @see #ResourceManager
 * @see #CheckBoxSceneNode_errors
 */
enum CheckBoxSceneNode_errors CheckBox_changeFocusedEventResource(struct CheckBox* checkBox,
                                                                  struct ResourceManager* resourceManager,
                                                                  const char* const focusedEventResId);

/**
 * @brief Changes CheckBox#checkedEventResource and CheckBox#checkedEvent.
 * Sets old CheckBox#checkedEvent#isNeeded to false, if exists.
 * Decreases old CheckBox#checkedEventResource#pointersCount if exists.
 * @param checkBox Pointer to a #CheckBox. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to load 
 * new CheckBox#checkedEventResource. Can be NULL.
 * @param checkedEventResId String with ID of new #GameEvent to load via #ResourceManager. Can be NULL.
 * @return #CheckBoxSceneNode_errors value.
 * @see #CheckBox
 * @see #GameEvent
 * @see #ResourceManager
 * @see #CheckBoxSceneNode_errors
 */
enum CheckBoxSceneNode_errors CheckBox_changeCheckedEventResource(struct CheckBox* checkBox,
                                                                  struct ResourceManager* resourceManager,
                                                                  const char* const checkedEventResId);

/**
 * @brief Changes CheckBox#unCheckedEventResource and CheckBox#unCheckedEvent.
 * Sets old CheckBox#unCheckedEvent#isNeeded to false, if exists.
 * Decreases old CheckBox#unCheckedEventResource#pointersCount if exists.
 * @param checkBox Pointer to a #CheckBox. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to load 
 * new CheckBox#unCheckedEventResource. Can be NULL.
 * @param unCheckedEventResId String with ID of new #GameEvent to load via #ResourceManager. Can be NULL.
 * @return #CheckBoxSceneNode_errors value.
 * @see #CheckBox
 * @see #GameEvent
 * @see #ResourceManager
 * @see #CheckBoxSceneNode_errors
 */
enum CheckBoxSceneNode_errors CheckBox_changeUnCheckedEventResource(struct CheckBox* checkBox,
                                                                    struct ResourceManager* resourceManager,
                                                                    const char* const unCheckedEventResId);

/**
 * @brief Saves #CheckBox to the filesystem via #ResourceManager.
 * Before saving, it updates CheckBox#sceneNode#sceneNodeTextResource with the latest changes in #CheckBox.
 * @param checkBox Pointer to a #CheckBox which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save CheckBox#sceneNode#sceneNodeTextResource. Can be NULL.
 * @param checkBoxResId Path string, where #ResourceManager will 
 * save CheckBox#sceneNode#sceneNodeTextResource. Can be NULL.
 * @return #CheckBoxSceneNode_errors value.
 * @see #CheckBox
 * @see #SceneNode
 * @see #ResourceManager
 * @see #CheckBoxSceneNode_errors
 */
enum CheckBoxSceneNode_errors CheckBox_save(const struct CheckBox* const checkBox,
                                            struct ResourceManager* const resourceManager,
                                            const char* const checkBoxResId);

/**
 * @brief Function to control user input events to #CheckBox.
 * Pointer to that function should be at CheckBox#sceneNode#control.
 * @param sceneNode Pointer to a #CheckBox, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will get user input events. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #CheckBox is not accepted!
 */
void CheckBox_control(struct SceneNode* sceneNode, struct EventManager* eventManager);

/**
 * @brief Function to update #CheckBox and interacting with #EventManager.
 * Pointer to that function should be at CheckBox#sceneNode#update.
 * @param sceneNode Pointer to a #CheckBox, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will place it's events. Can be NULL.
 * @param renderer Pointer to a #Renderer for screen-depend coordinates converting. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #CheckBox is not accepted!
 * @note Don't use this function for drawing.
 */
void CheckBox_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);

/**
 * @brief Function to render #CheckBox on the screen.
 * Pointer to that function should be at CheckBox#sceneNode#render.
 * @param sceneNode Pointer to a #CheckBox, casted to #SceneNode. Can be NULL.
 * @param renderer Pointer to a #Renderer for drawing #CheckBox. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #CheckBox is not accepted!
 */
void CheckBox_render(struct SceneNode* sceneNode, struct Renderer* renderer);

/**
 * @brief Function to play sounds of a #CheckBox.
 * Pointer to that function should be at CheckBox#sceneNode#sound.
 * @param sceneNode Pointer to a #CheckBox, casted to #SceneNode. Can be NULL.
 * @param musican Pointer to a #Musican for playing audio of a #CheckBox. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #CheckBox is not accepted!
 */
void CheckBox_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_CHECKBOX_H
