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
 * @brief File containing CheckBox and it's stuff.
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
 * Has focused, checked and unCHecked #GameEvent and #SoundResource.
 * User can click the #CheckBox, and some things will happen (such a #GameEvent or sound playing).
 * Also it's useful for other Gui elements, which are needed to be clicked with saving state.
 * @warning #CheckBox is an inheritor of the SceneNode.
 * It means, that other code is use <B>pointer casting</B> to #CheckBox from #SceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct SceneNode blablaNode;</tt> at the begining of #CheckBox struct, 
 * if you want code to work with #CheckBox like with a #SceneNode. 
 * More, you <B>SHOULD</B> initialize function pointers in <tt>blablaNode</tt> to NULL (by calling SceneNode_init()) 
 * or to your function implementation. 
 * Don't forget to add this warning comment to your own new #SceneNode inheritors.
 */
struct CheckBox {
    struct SceneNode sceneNode;
    struct Sprite* sprite; // animation: 0 - normal, 1 - focusedNormal, 2 - checked, 3 - focusedChecked
    struct SoundResource* focusedSoundResource;
    struct SoundResource* checkedSoundResource;
    struct SoundResource* unCheckedSoundResource;
    struct TextResource* focusedEventResource;
    struct TextResource* checkedEventResource;
    struct TextResource* unCheckedEventResource;
    struct GameEvent* focusedEvent;
    struct GameEvent* checkedEvent;
    struct GameEvent* unCheckedEvent;
    bool isGeometryChanged;
    bool isStateChanged;
    enum CheckBoxState state;
};

struct SceneNode* CheckBox_construct(struct ResourceManager* const resourceManager,
                                     struct Renderer* const renderer,
                                     struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                     struct TextParser* const textParser);
void CheckBox_destruct(struct SceneNode* checkBox);

unsigned char CheckBox_changeFocusedEventResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                                                const char* const focusedEventResId);
unsigned char CheckBox_changeCheckedEventResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                                                const char* const checkedEventResId);
unsigned char CheckBox_changeUnCheckedEventResource(struct CheckBox* checkBox, struct ResourceManager* resourceManager,
                                                const char* const unCheckeEventResId);
unsigned char CheckBox_save(const struct CheckBox* const checkBox, struct ResourceManager* const resourceManager, 
                          const char* const checkBoxResId);
void CheckBox_control(struct SceneNode* sceneNode, struct EventManager* eventManager);
void CheckBox_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);
void CheckBox_render(struct SceneNode* sceneNode, struct Renderer* renderer);
void CheckBox_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_CHECKBOX_H
