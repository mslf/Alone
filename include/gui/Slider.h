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
 * @file Slider.h
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing #Slider and its stuff.
 */
#ifndef ALONE_SLIDER_H
#define ALONE_SLIDER_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "gui/ProgressBar.h"
#include "gui/Button.h"

/**
 * @brief Strings which are used for constructing #Slider from #TextParser.
 * @see Slider_construct
 * @note SliderSceneNode_parserStrings#value value should contain number beetwen 0 and 100 (includes 100).
 * @note SliderSceneNode_parserStrings#step value should contain number beetwen 1 and 100 (includes 100).
 * @note SliderSceneNode_parserStrings#allign value should contain number from #SliderSceneNode_buttonAllignY.
 */
static const struct SliderSceneNode_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #Button resource from others. */
    const char* const progressBarRes;
    /**< #ProgressBar resource string, which is used to construct Slider#base. */
    const char* const buttonRes;
    /**< #Button resource string, which is used to construct Slider#button. */
    const char* const step;
    /**< String with number to set Slider#step. */
    const char* const value;
    /**< String with number to set Slider#base#value. */
    const char* const allign;
    /**< String with number to set Slider#buttonAllignY. */
}SliderSceneNode_parserStrings = {
    "Slider",
    "progressBarResource",
    "buttonResource",
    "step",
    "value",
    "allign"};

/**
 * @brief Error codes for #Slider.
 */
enum SliderSceneNode_errors {
    SLIDER_NO_ERRORS = 0,
    /**< All right, no errors. */
    SLIDER_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    SLIDER_ERR_NO_PROGRESS_BAR_RES = 2,
    /**< No SliderSceneNode_parserStrings#progressBarRes string found in #TextParser. */
    SLIDER_ERR_CONSTRUCTIG_PROGRESS_BAR = 3,
    /**< Constructing Slider#base failed. */
    SLIDER_ERR_NO_BUTTON_RES = 4,
    /**< No SliderSceneNode_parserStrings#buttonRes string found in #TextParser. */
    SLIDER_ERR_CONSTRUCTIG_BUTTON = 5,
    /**< Constructing Slider#button failed. */
    SLIDER_ERR_LOADING_SETTINGS = 6,
    /**< Loading required settings from #TextParser failed. */
    SLIDER_ERR_CONSTRUCTIG_TEXT_PARSER = 12,
    /**< Constructing new #TextParser for some required reason failed. */
    SLIDER_ERR_SAVING = 13
    /**< Saving #Slider failed due to internal reason. */
};
    
/**
 * @brief Possible vertical allign positions of Slider#button.
 */
enum SliderSceneNode_buttonAllignY {
    SLIDER_ALLIGN_Y_CENTER = 0,
    /**< Slider#button CENTER Y is equal to center Y of Slider#base. */
    SLIDER_ALLIGN_Y_TOP = 1,
    /**< Slider#button TOP Y is equal to bottom Y of Slider#base. */
    SLIDER_ALLIGN_Y_BOTTOM = 2,
    /**< Slider#button BOTTOM Y is equal to top Y of Slider#base. */
    SLIDER_ALLIGN_Y_NONE = 3
    /**< Slider#button Y is equal to Y of Slider#base. */
};

/**
 * @brief Standard Gui element with some possible states.
 * Based on #ProgressBar and #Button. User can click and move #Button across the #ProgressBar and it will
 * indicates current state (if possible).
 * As user releases #Button then #Button will stops on last possible state.
 * @warning #Slider is an inheritor of the SceneNode.
 * It means, that other code is use <B>pointer casting</B> to #Slider from #SceneNode and vise versa.
 * You <B>SHOULD</B> include the <tt>struct SceneNode blablaNode;</tt> at the begining of #Slider struct, 
 * if you want code to work with #Slider like with a #SceneNode. 
 * More, you <B>SHOULD</B> initialize function pointers in <tt>blablaNode</tt> to NULL (by calling SceneNode_init()) 
 * or to your function implementation. 
 * Don't forget to add this warning comment to your own new #SceneNode inheritors.
 * @see Slider_control()
 * @see Slider_update()
 * @see Slider_render()
 * @see Slider_sound()
 */
struct Slider {
   struct SceneNode sceneNode;
   /**< Inheritance stuff. */
   struct ProgressBar* base;
   /**< #ProgressBar which will indicates current state. */
   struct Button* button;
   /**< #Button to be clicked by user for changing Slider#base#value in possible states. */
   unsigned char step;
   /**<Number which is indicates how many states would have #Slider.
     * e.g. if Slider#step == 100, then only two (0 and 100) states will be available.
     * if Slider#step == 50, then only three (0, 50 and 100) states will be available. */
   enum SliderSceneNode_buttonAllignY buttonAllignY;
   /**< Enumeration which is sets position of Slider#button over the Slider#base. */
   bool isGeometryChanged;
   /**< Flag, which is need to be changed when someone tried to move (or change) #Slider, 
    * or Slider#button sceneNodes. */
   bool isButtonMoving;
   /**< Flag, which is answers "Are we moving Slider#button right now?". */
   SDL_Point lastMouseCoordinates;
   /**< Temporary place to store mouse coordinates. It is here only because we can't
    * convert screen-based coordinates in Slider_control() function. May be need to be fixed. */
};

/**
 * @brief Constructs #Slider and inits it from a #TextParser.
 * @param resourceManager Pointer to a #ResourceManager which is used to load additional 
 * resources, needed for #Slider. Can be NULL.
 * @param renderer Pointer to a #Renderer which is used to construct Slider#base and Slider#button. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar which is used to 
 * construct Slider#base and Slider#button. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #Slider. Can be NULL.
 * @return Pointer to an initialized #Slider, casted to #SceneNode, or NULL if constructing failed.
 * @note Use only Slider_destruct() or Slider#sceneNode#destruct() to destruct returned #SceneNode.
 * @see #Slider
 * @see #Button
 * @see #ProgressBar
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #SliderSceneNode_parserStrings
 */
struct SceneNode* Slider_construct(struct ResourceManager* const resourceManager,
                                   struct Renderer* const renderer,
                                   struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                   struct TextParser* const textParser);

/**
 * @brief Destructs #Slider and frees memory, used by it.
 * Pointer to that function should be at Slider#sceneNode#destruct.
 * @param slider Pointer to a #Slider, casted to #SceneNode. Can be not fully initialized. Can be NULL.
 * @warning Use this function only in pair with Slider_construct()!
 * @see Slider_construct()
 */
void Slider_destruct(struct SceneNode* slider);

/**
 * @brief Saves #Slider to the filesystem via #ResourceManager.
 * Before saving, it updates Slider#sceneNode#sceneNodeTextResource with the latest changes in #Slider.
 * @param slider Pointer to a #Slider which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save Slider#sceneNode#sceneNodeTextResource. Can be NULL.
 * @param sliderResId Path string, where #ResourceManager will 
 * save Slider#sceneNode#sceneNodeTextResource. Can be NULL.
 * @return #SliderSceneNode_errors value.
 * @see #Slider
 * @see #SceneNode
 * @see #ResourceManager
 * @see #SliderSceneNode_errors
 */
enum SliderSceneNode_errors Slider_save(const struct Slider* const slider,
                                        struct ResourceManager* const resourceManager,
                                        const char* const sliderResId);

/**
 * @brief Function to control user input events to #Slider.
 * Pointer to that function should be at Slider#sceneNode#control.
 * @param sceneNode Pointer to a #Slider, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will get user input events. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Slider is not accepted!
 */
void Slider_control(struct SceneNode* sceneNode, struct EventManager* eventManager);

/**
 * @brief Function to update #Slider and interacting with #EventManager.
 * Pointer to that function should be at Slider#sceneNode#update.
 * @param sceneNode Pointer to a #Slider, casted to #SceneNode. Can be NULL.
 * @param eventManager Pointer to an #EventManager, where that function will place it's events. Can be NULL.
 * @param renderer Pointer to a #Renderer for screen-depend coordinates converting. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Slider is not accepted!
 * @note Don't use this function for drawing.
 */
void Slider_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);

/**
 * @brief Function to render #Slider on the screen.
 * Pointer to that function should be at Slider#sceneNode#render.
 * @param sceneNode Pointer to a #Slider, casted to #SceneNode. Can be NULL.
 * @param renderer Pointer to a #Renderer for drawing #Slider. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Slider is not accepted!
 */
void Slider_render(struct SceneNode* sceneNode, struct Renderer* renderer);

/**
 * @brief Function to play sounds of a #Slider.
 * Pointer to that function should be at Slider#sceneNode#sound.
 * @param sceneNode Pointer to a #Slider, casted to #SceneNode. Can be NULL.
 * @param musican Pointer to a #Musican for playing audio of a #Slider. Can be NULL.
 * @warning Passing #SceneNode inheritors, which is not #Slider is not accepted!
 */
void Slider_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_SLIDER_H
