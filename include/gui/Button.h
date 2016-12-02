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
 * @file Button.h
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing #Button and its stuff.
 */
#ifndef ALONE_BUTTON_H
#define ALONE_BUTTON_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "sprite/Sprite.h"
#include "text/Text.h"

/**
 * @brief Strings which are used for constructing #Button from #TextParser.
 * @see Button_construct
 * @note Button#sprite, constructed from ButtonSceneNode_parserStrings#spriteRes should have 3 animations.
 */
static const struct ButtonSceneNode_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #Button resource from others. */
    const char* const focusedEventRes;
    /**< Focused #GameEvent resource string, which is used to construct Button#focusedEvent. */
    const char* const pressedEventRes;
    /**< Pressed #GameEvent resource string, which is used to construct Button#pressedEvent. */
    const char* const focusedSoundRes;
    /**< Focused #SoundResource string, which is used to construct Button#focusedSoundResource. */
    const char* const pressedSoundRes;
    /**< Pressed #SoundResource string, which is used to construct Button#pressedSoundResource. */
    const char* const spriteRes;
    /**< #Sprite resource string, which is used to construct Button#sprite. */
    const char* const textRes;
    /**< #Text resource string, which is used to construct Button#label. */
    const char* const labelOffset;
    /**< Offset string, which is used to set Y offset for Button#label. */
}ButtonSceneNode_parserStrings = {
    "Button",
    "focusedEventResource",
    "pressedEventResource",
    "focusedSoundResource",
    "pressedSoundResource",
    "spriteResource",
    "textResource",
    "labelOffset"};

/**
 * @brief Error codes for #Button.
 */
enum ButtonSceneNode_errors {
    BUTTON_NO_ERRORS = 0,
    /**< All right, no errors. */
    BUTTON_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    BUTTON_ERR_NO_SPRITE_RES = 2,
    /**< No ButtonSceneNode_parserStrings#spriteRes string found in #TextParser. */
    BUTTON_ERR_CONSTRUCTIG_SPRITE = 3,
    /**< Constructing Button#sprite failed. */
    BUTTON_ERR_NOT_ENOUGH_ANIMATIONS = 4,
    /**< Constructed Button#sprite doesn't have enough animations. */
    BUTTON_ERR_NO_TEXT_RES = 5,
    /**< No ButtonSceneNode_parserStrings#textRes string found in #TextParser. */
    BUTTON_ERR_CONSTRUCTIG_TEXT = 6,
    /**< Constructing Button#label failed. */
    BUTTON_ERR_LOADING_SETTINGS = 7,
    /**< Loading required settings from #TextParser failed. */
    BUTTON_ERR_NO_FOCUSED_EVENT_RES = 8,
    /**< Loading new Button#focusedEventResource from #ResourceManager failed. */
    BUTTON_ERR_NO_PRESSED_EVENT_RES = 9,
    /**< Loading new Button#pressedEventResource from #ResourceManager failed. */
    BUTTON_ERR_CONSTRUCTIG_FOCUSED_EVENT = 10,
    /**< Constructing new Button#focusedEvent failed. */
    BUTTON_ERR_CONSTRUCTIG_PRESSED_EVENT = 11,
    /**< Constructing new Button#pressedEvent failed. */
    BUTTON_ERR_CONSTRUCTIG_TEXT_PARSER = 12,
    /**< Constructing new #TextParser for some required reason failed. */
    BUTTON_ERR_SAVING = 13
    /**< Saving #Button failed due to internal reason. */
};
    
/**
 * @brief Possible #Button states.
 * @see Button_control()
 * @see Button_update()
 * @see Button_render()
 * @see Button_sound()
 */
enum ButtonState {
    BUTTON_STATE_NORMAL = 0,
    /**< #Button in normal state. No one tryies to iteract with it. */
    BUTTON_STATE_FOCUSED = 1,
    /**< #Button now have a focus. User moves cursor on it. */
    BUTTON_STATE_PRESSED = 2
    /**< #Button have been pressed and still holds. User clicked on it and holds mouse button. */
};

/**
 * @brief Standart Gui element to be clicked without holding its state through the frames.
 * Has some text on itself, focused and pressed #GameEvent and #SoundResource.
 * User can click the #Button, and some things will happen (such a #GameEvent or sound playing).
 * Also it's useful for other Gui elements, which are needed to be clicked without saving state.
 * @warning #Button is an inheritor of the SceneNode.
 * It means, that other code is use <B>pointer casting</B> to #Button from #SceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct SceneNode blablaNode;</tt> at the begining of #Button struct, 
 * if you want code to work with #Button like with a #SceneNode. 
 * More, you <B>SHOULD</B> initialize function pointers in <tt>blablaNode</tt> to NULL (by calling SceneNode_init()) 
 * or to your function implementation. 
 * Don't forget to add this warning comment to your own new #SceneNode inheritors.
 * @see Button_control()
 * @see Button_update()
 * @see Button_render()
 * @see Button_sound()
 */
struct Button {
    struct SceneNode sceneNode;
    /**< Inheritance stuff. */
    struct Sprite* sprite;
    /**< #Sprite to be displayed on the screen. Should have 3 animations (normal, focused, pressed). */
    struct Text* label;
    /**< Some #Text to be displayed on the screen as a label of the #Button. */
    struct SoundResource* focusedSoundResource;
    /**< Sound to be played when the #Button receives focus. */
    struct SoundResource* pressedSoundResource;
    /**< Sound to be played when the #Button clicked. */
    struct TextResource* focusedEventResource;
    /**< #TextResource to construct Button#focusedEvent. */
    struct TextResource* pressedEventResource;
    /**< #TextResource to construct Button#pressedEvent. */
    struct GameEvent* focusedEvent;
    /**< #GameEvent to be added to the #EventManager when the #Button receives focus. */
    struct GameEvent* pressedEvent;
    /**< #GameEvent to be added to the #EventManager when the #Button clicked. */
    SDL_Point labelOffset;
    /**< Offset to draw the Button#label in properly place. */ 
    bool isGeometryChanged;
    /**< Flag, which is need to be changed when someone tried to move #Button, or update Button#SceneNode. */
    bool isStateChanged;
    /**< Flag, which is need to be changed when someone tried to change Button#state. */
    enum ButtonState state;
    /**< Curent #ButtonState. */
};

/**
 * @brief Constructs #Button and inits it from a #TextParser.
 * @param resourceManager Pointer to a #ResourceManager which is used to load additional 
 * resources, needed for #Button. Can be NULL.
 * @param renderer Pointer to a #Renderer which is used to construct Button#sprite and Button#label. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar which is used to 
 * construct Button#sprite and Button#label. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #Button. Can be NULL.
 * @return Pointer to an initialized #Button, casted to #SceneNode, or NULL if constructing failed.
 * @note Use only Button_destruct() or Button#sceneNode#destruct() to destruct returned #SceneNode.
 * @see #Button
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #ButtonSceneNode_parserStrings
 */
struct SceneNode* Button_construct(struct ResourceManager* const resourceManager,
                                                 struct Renderer* const renderer,
                                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                 struct TextParser* const textParser);

/**
 * @brief Destructs #Button and frees memory, used by it.
 * Pointer to that function should be at Button#sceneNode#destruct.
 * @param button Pointer to a #Button, casted to #SceneNode. Can be not fully initialized. Can be NULL.
 * @warning Use this function only in pair with Button_construct()!
 * @see Button_construct()
 */
void Button_destruct(struct SceneNode* button);

/**
 * @brief Changes Button#focusedEventResource and Button#focusedEvent.
 * Sets old Button#focusedEvent#isNeeded to false, if exists.
 * Decreases old Button#focusedEventResource#pointersCount if exists.
 * @param button Pointer to a #Button. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to load 
 * new Button#focusedEventResource. Can be NULL.
 * @param focusedEventResId String with ID of new #GameEvent to load via #ResourceManager. Can be NULL.
 * @return #ButtonSceneNode_errors value.
 * @see #Button
 * @see #GameEvent
 * @see #ResourceManager
 * @see #ButtonSceneNode_errors
 */
enum ButtonSceneNode_errors Button_changeFocusedEventResource(struct Button* button,
                                                     struct ResourceManager* resourceManager,
                                                     const char* const focusedEventResId);

/**
 * @brief Changes Button#pressedEventResource and Button#pressedEvent.
 * Sets old Button#pressedEvent#isNeeded to false, if exists.
 * Decreases old Button#pressedEventResource#pointersCount if exists.
 * @param button Pointer to a #Button. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to load 
 * new Button#pressedEventResource. Can be NULL.
 * @param pressedEventResId String with ID of new #GameEvent to load via #ResourceManager. Can be NULL.
 * @return #ButtonSceneNode_errors value.
 * @see #Button
 * @see #GameEvent
 * @see #ResourceManager
 * @see #ButtonSceneNode_errors
 */
enum ButtonSceneNode_errors Button_changePressedEventResource(struct Button* button,
                                                     struct ResourceManager* resourceManager,
                                                     const char* const pressedEventResId);

/**
 * @brief Saves #Button to the filesystem via #ResourceManager.
 * Before saving, it updates Button#sceneNode#sceneNodeTextResource with the latest changes in #Button.
 * @param button Pointer to a #Button which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save Button#sceneNode#sceneNodeTextResource. Can be NULL.
 * @param buttonResId Path string, where #ResourceManager will 
 * save Button#sceneNode#sceneNodeTextResource. Can be NULL.
 * @return #ButtonSceneNode_errors value.
 * @see #Button
 * @see #SceneNode
 * @see #ResourceManager
 * @see #ButtonSceneNode_errors
 */
enum ButtonSceneNode_errors Button_save(const struct Button* const button,
                               struct ResourceManager* const resourceManager,
                               const char* const buttonResId);

/**
 * @brief Function to control user input events to #Button.
 * Pointer to that function should be at Button#sceneNode#control.
 * @param sceneNode Pointer to a #Button, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will get user input events. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Button is not accepted!
 */
void Button_control(struct SceneNode* sceneNode, struct EventManager* eventManager);

/**
 * @brief Function to update #Button and interacting with #EventManager.
 * Pointer to that function should be at Button#sceneNode#update.
 * @param sceneNode Pointer to a #Button, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will place it's events. Can be NULL.
 * @param renderer Pointer to a #Renderer for screen-depend coordinates converting. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Button is not accepted!
 * @note Don't use this function for drawing.
 */
void Button_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);

/**
 * @brief Function to render #Button on the screen.
 * Pointer to that function should be at Button#sceneNode#render.
 * @param sceneNode Pointer to a #Button, casted to #SceneNode. Can be NULL.
 * @param renderer Pointer to a #Renderer for drawing #Button. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Button is not accepted!
 */
void Button_render(struct SceneNode* sceneNode, struct Renderer* renderer);

/**
 * @brief Function to play sounds of a #Button.
 * Pointer to that function should be at Button#sceneNode#sound.
 * @param sceneNode Pointer to a #Button, casted to #SceneNode. Can be NULL.
 * @param musican Pointer to a #Musican for playing audio of a #Button. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Button is not accepted!
 */
void Button_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_BUTTON_H
