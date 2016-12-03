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
 * @file StandardUser.h
 * @author mslf
 * @date 28 Sep 2016
 * @brief File containing #StandardUser and its stuff.
 */
#ifndef ALONE_STANDARD_USER_H
#define ALONE_STANDARD_USER_H

#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "body/Body.h"
#include "resourceManager/SoundResource.h"
#include "module/section/MacroSection.h"

/**
 * @brief Strings which are used for constructing #StandardUser from #TextParser.
 * @note StandardUser_parserStrings#iventoryItemsList doesn't contain actual
 * settings (such an ID or count) of the constructed #InventoryItem.
 * It contains array of #InventoryItem names, which will be found in #TextParser, so they will contain actual data.
 * @note StandardUser_parserStrings#eventsList doesn't contain actual
 * settings (such an ID or trigger scancode string) of the constructed #TriggeredGameEvent.
 * It contains array of #TriggeredGameEvent names, which will be found in #TextParser, so they will contain actual data.
 * @note StandardUser_parserStrings#nanoSections doesn't contain actual
 * settings (such an ID or valueString) of the constructed #NanoSection.
 * It contains array of #NanoSection names, which will be found in #TextParser, so they will contain actual data.
 * @see StandardUser_construct
 */
static const struct StandardUser_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #StandardUser resource from others. */
    const char* const bodyRes;
    /**< #Body resource string, which is used to construct StandardUser#body. */
    const char* const inventoryItemsList;
    /**< List of strings with name of #InventoryItem, which #StandardUser will find in #TextParser
     * to init StandardUser#inventoryItemsList. */
    const char* const soundsList;
    /**< List of #SoundResource to init StandardUser#soundResourcesList. */
    const char* const triggeredEventsList;
    /**< List of strings with name of #nanoSections, which #StandardUser will find in #TextParser
     * to init StandardUser#triggeredEventsList. */
    const char* const nanoSections;
    /**< List of strings with name of #NanoSection, which #StandardUser will find in #TextParser
     * to init StandardUser#microSection. */
}StandardUser_parserStrings = {
        "StandardUser",
        "bodyResource",
        "inventoryItemsList",
        "soundResourcesList",
        "triggeredEventsList",
        "nanoSectionResourcesList"};

/**
 * @brief Error codes for #StandardUser.
 */
enum StandardUserSceneNode_errors {
    STD_USER_NO_ERRORS = 0,
    /**< All right, no errors. */
    STD_USER_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    STD_USER_ERR_NO_SPRITE_RES = 2,
    /**< No StandardUserSceneNode_parserStrings#spriteRes string found in #TextParser. */
    STD_USER_ERR_CONSTRUCTING_SPRITE = 3,
    /**< Constructing StandardUser#sprite failed. */
    STD_USER_ERR_CONSTRUCTIG_TEXT_PARSER = 4,
    /**< Constructing new #TextParser for some required reason failed. */
    STD_USER_ERR_SAVING = 5
    /**< Saving #StandardUser failed due to internal reason. */
};

/**
 * @brief Represents an item in #StandardUser inventory.
 */
struct InventoryItem {
    struct TextResource* textResource;
    /**< #TextResource from which #InventoryItem was constructed. */
    const char* name;
    /**< Displaying name string of the #InventoryItem. */
    const char* itemResource;
    /**< #Item resource ID, which will be created when #InventoryItem
     * dropped from #StandardUser inventory. */
    struct GameEvent* usedEvent;
    /**< #GameEvent, which will happened when #InventoryItem
     * dropped from #StandardUser inventory.*/
    size_t count;
    /**< Count of such #InventoryItem in the #StandardUser inventory. */
};

/**
 * @brief Keyboard triggered #GameEvent for #StandardUser.
 */
struct TriggeredGameEvent {
    struct TextResource* textResource;
    /**< #GameEvent resource. */
    struct GameEvent* gameEvent;
    /**< #GameEvent, which will happen, when user press appropriate key. */
    const char* triggerScanCodeString;
    /**< Trigger scancode in human-readable scancode name. */
    SDL_Scancode triggerScanCode;
    /**< Trigger scancode. */
};

/**
 * @brief Standard #PhysicalSceneNode inheritor which is #Body with some number of #SoundResource,
 * input-triggered #GameEvent, #InventoryItem and #MicroSection.
 * @warning #StandardUser is an inheritor of the #PhysicalSceneNode.
 * It means, that other code is use <B>pointer casting</B> to #StandardUser from #PhysicalSceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct PhysicalSceneNode blablaNode;</tt> at the begining of #StandardUser struct, 
 * if you want code to work with #StandardUser like with a #PhysicalSceneNode. 
 * More, you <B>SHOULD</B> initialize <tt>blablaNode</tt> by calling SceneNode_initPhysical().
 * Also function pointers in PhysicalSceneNode#sceneNode <B>SHOULD</B> be initialized to your function implementation.
 * Don't forget to add this warning comment to your own new #PhysicalSceneNode inheritors.
 * @see StandardUser_control()
 * @see StandardUser_update()
 * @see StandardUser_render()
 * @see StandardUser_sound()
 */
struct StandardUser {
    struct PhysicalSceneNode physicalSceneNode;
    /**< Inheritance stuff. */
    struct Body* body;
    /**< Represents physical and graphical part of #StandardUser. */
    struct TriggeredGameEvent* triggeredEventsList;
    /**< List of #TriggeredGameEvent, which can be triggered by keyboard or directly. */
    struct SoundResource** soundResourcesList;
    /**< List of #SoundResource, which are represents sounds of #StandardUser. */
    struct MicroSection* microSection;
    /**< #MicroSection, where #NanoSection of #StandardUser, where will be its resources for live simulation. */
    struct InventoryItem* inventoryItemsList;
    /**< List of #InventoryItem, which is represent inventory of #StandardUser. */
    size_t soundResourcesCount;
    /**< Current number of existing #SoundResource in the StandardUser#soundResourcesList. */
    size_t allocatedSoundResourcesCount;
    /**< Current number of allocated #SoundResource in the StandardUser#soundResourcesList. */
    size_t inventoryItemsCount;
    /**< Current number of existing #InventoryItem in the StandardUser#inventoryItemsList. */
    size_t allocatedInventoryItemsCount;
    /**< Current number of allocated #InventoryItem in the StandardUser#inventoryItemsList. */
    size_t triggeredEventsCount;
    /**< Current number of existing #TriggeredGameEvent in the StandardUser#triggeredEventsList. */
    size_t allocatedTriggeredEventsCount;
    /**< Current number of allocated #TriggeredGameEvent in the StandardUser#triggeredEventsList. */
};

/**
 * @brief Constructs and inits #StandardUser from #TextParser.
 * @param resourceManager Pointer to a #ResourceManager which is needed to create 
 * StandardUser#body and load other resources (sounds, events, inventory items). Can be NULL.
 * @param renderer Pointer to a #Renderer to to create
 * StandardUser#body and load other resources (sounds, events, inventory items). Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar to create a StandardUser#body. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #StandardUser. Can be NULL.
 * @return Pointer to an initialized #StandardUser, casted to #SceneNode or NULL, if failed.
 * @note Use only StandardUser_destruct() or StandardUser#physicalSceneNode#dynamicSceneNode#sceneNode#destruct
 * to destruct returned #SceneNode.
 * @see #SceneNodeTypesRegistrar
 * @see #StandardUser
 * @see #Body
 * @see #SoundResoure
 * @see #TriggeredGameEvent
 * @see #InventoryItem
 * @see #StandardUser_parserStrings
 * @see #ResourceManager
 * @see #Renderer
 */
struct SceneNode* StandardUser_construct(struct ResourceManager* const resourceManager,
                                         struct Renderer* const renderer,
                                         struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                         struct TextParser* const textParser);

/**
 * @brief Destructs #StandardUser and frees memory, used by it.
 * Pointer to that function should be at StandardUser#physicalSceneNode#dynamicSceneNode#sceneNode#destruct.
 * @param body Pointer to a #StandardUser, casted to #SceneNode. Can be not fully initialized. Can be NULL.
 * @warning Use this function only in pair with StandardUser_construct()!
 * @see StandardUser_construct()
 */
void StandardUser_destruct(struct SceneNode* standardUser);

/**
 * @brief Saves #StandardUser to the filesystem via #ResourceManager.
 * Before saving, it updates StandardUser#physicalSceneNode#dynamicSceneNode#sceneNode#sceneNodeTextResource
 * with the latest changes in #StandardUser.
 * @param body Pointer to a #StandardUser which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save StandardUser#physicalSceneNode#dynamicSceneNode#sceneNode#sceneNodeTextResource. Can be NULL.
 * @param spriteResId Path string, where #ResourceManager will 
 * save StandardUser#physicalSceneNode#dynamicSceneNode#sceneNode#sceneNodeTextResource. Can be NULL.
 * @return #StandardUserSceneNode_errors value.
 * @see #StandardUser
 * @see #SceneNode
 * @see #ResourceManager
 * @see #StandardUserSceneNode_errors
 */
enum StandardUserSceneNode_errors StandardUser_save(const struct StandardUser* const standardUser,
                                                    struct ResourceManager* const resourceManager,
                                                    const char* const userResId);

/**
 * @brief Function to control user input events to #StandardUser.
 * Pointer to that function should be at StandardUser#physicalSceneNode#dynamicSceneNode#sceneNode#control.
 * @param sceneNode Pointer to a #StandardUser, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will get user input events. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #StandardUser is not accepted!
 */
void StandartUser_control(struct SceneNode* sceneNode, struct EventManager* eventManager);

/**
 * @brief Function to update #StandardUser and interacting with #EventManager.
 * Pointer to that function should be at StandardUser#physicalSceneNode#dynamicSceneNode#sceneNode#update.
 * @param sceneNode Pointer to a #StandardUser, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will place it's events. Can be NULL.
 * @param renderer Pointer to a #Renderer for screen-depend coordinates converting. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #StandardUser is not accepted!
 * @note Don't use this function for drawing.
 */
void StandartUser_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);

/**
 * @brief Function to render #StandardUser on the screen.
 * Pointer to that function should be at StandardUser#physicalSceneNode#dynamicSceneNode#sceneNode#render.
 * @param sceneNode Pointer to a #StandardUser, casted to #SceneNode. Can be NULL.
 * @param renderer Pointer to a #Renderer for drawing #StandardUser. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #StandardUser is not accepted!
 */
void StandartUser_render(struct SceneNode* sceneNode, struct Renderer* renderer);

/**
 * @brief Function to play sounds of a #StandardUser.
 * Pointer to that function should be at StandardUser#physicalSceneNode#dynamicSceneNode#sceneNode#sound.
 * @param sceneNode Pointer to a #StandardUser, casted to #SceneNode. Can be NULL.
 * @param musican Pointer to a #Musican for playing audio of a #StandardUser. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #StandardUser is not accepted!
 */
void StandartUser_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_STANDARD_USER_H
