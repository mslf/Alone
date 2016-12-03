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
 * @file Slider.c
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing implementation of #Slider.
 */
#include <assert.h>
#include "gui/Slider.h"

/**
 * @brief Error message strings for #Slider.
 */
static const struct SliderSceneNode_errorMessages {
    const char* const errNoButtonRes;
    /**< Will be displayed when #TextParser have no SliderSceneNode_parserStrings#buttonRes. */
    const char* const errNoProgressBarRes;
    /**< Will be displayed when #TextParser have no SliderSceneNode_parserStrings#progressBarRes. */
}SliderSceneNode_errorMessages = {
    "Slider_loadButton: buttonResource string haven't found!",
    "Slider_loadBase: progressBarResource string haven't found!"};

/**
 * @brief Function for loading and constructing Slider#base.
 * Also sets Slider#step and Slider#base#value from #TextParser.
 * @param slider Pointer to a #Slider where to construct Slider#base.
 * @param resourceManager Pointer to a #ResourceManager for loading #ProgressBar resource.
 * @param renderer Pointer to a #Renderer for constructing Slider#base.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing Slider#base.
 * @param textParser Pointer to a #TextParser, where function will get 
 * SliderSceneNode_parserStrings#progressBarRes, SliderSceneNode_parserStrings#value 
 * and SliderSceneNode_parserStrings#step.
 * @return #SliderSceneNode_errors value.
 * @see #Slider
 * @see #Button
 * @see #ResourceManager
 * @see #Renderer
 * @see #SceneNodeTypesRegistrar
 * @see #TextParser
 * @see #SliderSceneNode_parserStrings
 * @see #SliderSceneNode_errors
 */
static enum SliderSceneNode_errors Slider_loadBase(struct Slider* slider,
                                     struct ResourceManager* const resourceManager,
                                     struct Renderer* const renderer,
                                     struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                     struct TextParser* const textParser) {
    assert(slider);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    const char* tempResId = NULL;
    tempResId = TextParser_getString(textParser, SliderSceneNode_parserStrings.progressBarRes, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, SliderSceneNode_errorMessages.errNoProgressBarRes);
        return SLIDER_ERR_NO_PROGRESS_BAR_RES;
    }
    slider->base = (struct ProgressBar*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                            renderer,
                                                                            sceneNodeTypesRegistrar,
                                                                            tempResId,
                                                                            ProgressBarSceneNode_parserStrings.type);
    if (!slider->base)
        return SLIDER_ERR_CONSTRUCTIG_PROGRESS_BAR;
    slider->base->value = (unsigned char)TextParser_getInt(textParser, SliderSceneNode_parserStrings.value, 0);
    if (slider->base->value > 100)
        slider->base->value = 100;
    slider->step = (unsigned char)TextParser_getInt(textParser, SliderSceneNode_parserStrings.step, 0);
    if (slider->step > 100 || slider->step == 0)
        slider->step = 100;
    return SLIDER_NO_ERRORS;
}

/**
 * @brief Function for loading and constructing Slider#button.
 * Also sets Slider#buttonAllignY from #TextParser.
 * @param slider Pointer to a #Slider where to construct Slider#button.
 * @param resourceManager Pointer to a #ResourceManager for loading #Button resource.
 * @param renderer Pointer to a #Renderer for constructing Slider#button.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing Slider#button.
 * @param textParser Pointer to a #TextParser, where function will get 
 * SliderSceneNode_parserStrings#buttonRes and SliderSceneNode_parserStrings#allign.
 * @return #SliderSceneNode_errors value.
 * @see #Slider
 * @see #SliderSceneNode_buttonAllignY
 * @see #Button
 * @see #ResourceManager
 * @see #Renderer
 * @see #SceneNodeTypesRegistrar
 * @see #TextParser
 * @see #SliderSceneNode_parserStrings
 * @see #SliderSceneNode_errors
 */
static enum SliderSceneNode_errors Slider_loadButton(struct Slider* slider,
                                       struct ResourceManager* const resourceManager,
                                       struct Renderer* const renderer,
                                       struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                       struct TextParser* const textParser) {
    assert(slider);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    const char* tempResId = NULL;
    tempResId = TextParser_getString(textParser, SliderSceneNode_parserStrings.buttonRes, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, SliderSceneNode_errorMessages.errNoButtonRes);
        return SLIDER_ERR_NO_BUTTON_RES;
    }
    slider->button = (struct Button*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                                renderer,
                                                                                sceneNodeTypesRegistrar,
                                                                                tempResId,
                                                                                ButtonSceneNode_parserStrings.type);
    if (!slider->button)
        return SLIDER_ERR_CONSTRUCTIG_BUTTON;
    slider->buttonAllignY = (enum SliderSceneNode_buttonAllignY)TextParser_getInt(textParser,
                                                                                  SliderSceneNode_parserStrings.allign, 0);
    if (slider->buttonAllignY > (enum SliderSceneNode_buttonAllignY)3)
        slider->buttonAllignY = SLIDER_ALLIGN_Y_CENTER;
    return SLIDER_NO_ERRORS; 
}

/**
 * @brief Function for loading settings and initializing #Slider from #TextParser.
 * @param slider Pointer to a #Slider which will be initialized.
 * @param resourceManager Pointer to a #ResourceManager for loading required resources.
 * @param renderer Pointer to a #Renderer for constructing Slider#base and Slider#button.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing Slider#base and Slider#button.
 * @param textParser Pointer to a #TextParser with data strings.
 * @return #SliderSceneNode_errors value.
 * @see #Slider
 * @see #Button
 * @see #ProgressBar
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #SliderSceneNode_parserStrings
 * @see #SliderSceneNode_errors
 */
static enum SliderSceneNode_errors Slider_tryGetSettingsFromTextParser(struct Slider* slider,
                                       struct ResourceManager* const resourceManager,
                                       struct Renderer* const renderer,
                                       struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                       struct TextParser* const textParser) {
    assert(slider);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    enum SliderSceneNode_errors loadingButtonResult = SLIDER_NO_ERRORS;
    enum SliderSceneNode_errors loadingBaseResult = SLIDER_NO_ERRORS;
    loadingButtonResult = Slider_loadButton(slider, resourceManager, renderer, sceneNodeTypesRegistrar, textParser);
    loadingBaseResult = Slider_loadBase(slider, resourceManager, renderer, sceneNodeTypesRegistrar, textParser);
    slider->isGeometryChanged = true;
    if (loadingBaseResult || loadingButtonResult)
        return SLIDER_ERR_LOADING_SETTINGS;
    return SLIDER_NO_ERRORS;
}

struct SceneNode* Slider_construct(struct ResourceManager* const resourceManager,
                                   struct Renderer* const renderer,
                                   struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                   struct TextParser* const textParser) {
    if (!resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return NULL;
    struct Slider* slider = NULL;
    slider = (struct Slider*)calloc(1, sizeof(struct Slider));
    if (!slider)
        return NULL;
    SceneNode_init(&(slider->sceneNode));
    Logger_saveUsedFlagAndSetToFalse(renderer->logger);
    if (Slider_tryGetSettingsFromTextParser(slider, resourceManager, renderer,
                                            sceneNodeTypesRegistrar, textParser)) {
        if (renderer->logger->wasUsed)
            Logger_log(renderer->logger, "\tin file: %s", textParser->file);
        Logger_revertUsedFlag(renderer->logger);
        Slider_destruct((struct SceneNode*)slider);
        return NULL;
    }
    if (renderer->logger->wasUsed)
        Logger_log(renderer->logger, "\tin file: %s", textParser->file);
    Logger_revertUsedFlag(renderer->logger);
    slider->sceneNode.control = Slider_control;
    slider->sceneNode.update = Slider_update;
    slider->sceneNode.render = Slider_render;
    slider->sceneNode.sound = Slider_sound;
    slider->sceneNode.destruct = Slider_destruct;
    slider->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(SliderSceneNode_parserStrings.type) + 1));
    if (!slider->sceneNode.type) {
        Slider_destruct((struct SceneNode*)slider);
        return NULL;
    }
    strcpy(slider->sceneNode.type, SliderSceneNode_parserStrings.type);
    return (struct SceneNode*)slider;
}

void Slider_destruct(struct SceneNode* slider) {
    if (!slider)
        return;
    struct Slider* tempSlider = (struct Slider*)slider;
    if (tempSlider->base)
        ProgressBar_destruct((struct SceneNode*)tempSlider->base);
    if (tempSlider->button)
        Button_destruct((struct SceneNode*)tempSlider->button);
    TextResource_decreasePointersCounter(slider->sceneNodeTextResource);
    if (slider->type)
        free(slider->type);
    free(slider);
}

enum SliderSceneNode_errors Slider_save(const struct Slider* const slider,
                                        struct ResourceManager* const resourceManager,
                                        const char* const sliderResId) {
    if (!slider || !resourceManager || !sliderResId)
        return SLIDER_ERR_NULL_ARGUMENT;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return SLIDER_ERR_CONSTRUCTIG_TEXT_PARSER;
    result += (TextParser_addString(textParser, TextParser_standartTypeString, SliderSceneNode_parserStrings.type) != 0);
    result += (TextParser_addString(textParser, SliderSceneNode_parserStrings.progressBarRes,
                                   slider->base->sceneNode.sceneNodeTextResource->id) != 0);
    result += (TextParser_addString(textParser, SliderSceneNode_parserStrings.buttonRes,
                                   slider->button->sceneNode.sceneNodeTextResource->id) != 0);
    result += (TextParser_addInt(textParser, SliderSceneNode_parserStrings.step, (long)slider->step) != 0);
    result += (TextParser_addInt(textParser, SliderSceneNode_parserStrings.value, (long)slider->base->value) != 0);
    result += (TextParser_addInt(textParser, SliderSceneNode_parserStrings.allign, (long)slider->buttonAllignY) != 0);
    char* tempString = TextParser_convertToText(textParser);
    result += (textParser->lastError != 0);
    result += (TextResource_updateContent(slider->sceneNode.sceneNodeTextResource, tempString) != 0);
    result += (ResourceManager_saveTextResource(resourceManager,
                                               slider->sceneNode.sceneNodeTextResource, sliderResId) != 0);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    if (result)
        return SLIDER_ERR_SAVING;
    return SLIDER_NO_ERRORS;
}

void Slider_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {
    if (!sceneNode || !eventManager)
        return;
    struct Slider* slider = (struct Slider*)sceneNode;
    Button_control((struct SceneNode*)slider->button, eventManager);
    if (slider->button->state == BUTTON_STATE_PRESSED) {
        SDL_Point mouseCoordinates;
        SDL_GetMouseState(&mouseCoordinates.x, &mouseCoordinates.y);
        //FIXME leftBorder and rightBorder should be calculated via Renderer_convertCoordinatesBack
        //Right now, if you want to move camera position, slider will works bad.
        int leftBorder = slider->base->spriteBase->dstRect.x;
        int rightBorder = slider->base->spriteBase->dstRect.x + slider->base->spriteBase->dstRect.w;
        if (mouseCoordinates.x > leftBorder && mouseCoordinates.x < rightBorder) {
            slider->lastMouseCoordinates = mouseCoordinates;
            mouseCoordinates.x -= slider->base->spriteBase->dstRect.x;
            rightBorder -= slider->base->spriteBase->dstRect.x;
            mouseCoordinates.x = mouseCoordinates.x * 100 / rightBorder;
            // mouseCoordinates.x now contain ofsset in % from begining slider->base->spriteBase
            slider->base->value = mouseCoordinates.x;
            slider->isGeometryChanged = true;
            slider->isButtonMoving = true;
        } else {
            if (mouseCoordinates.x < leftBorder)
                slider->base->value = 0;
            if (mouseCoordinates.x > rightBorder)
                slider->base->value = 100;
            slider->isGeometryChanged = true;
        }
    } else
        if (slider->isButtonMoving) {
            slider->isGeometryChanged = true;
            slider->isButtonMoving = false;
        }
}

void Slider_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager || !renderer)
        return;
    struct Slider* slider = (struct Slider*)sceneNode;
    if (slider->isGeometryChanged) {
        if (slider->base->value % slider->step != 0)
            slider->base->value = slider->base->value / slider->step * slider->step;
        slider->base->sceneNode.coordinates = slider->sceneNode.coordinates;
        slider->base->isGeometryChanged = true;
        slider->button->sceneNode.coordinates.y = slider->sceneNode.coordinates.y;
        if (slider->isButtonMoving) {
            SDL_Point mouseVirtualCoordinates = Renderer_convertCoordinatesBack(renderer, slider->lastMouseCoordinates);
            slider->button->sceneNode.coordinates.x = mouseVirtualCoordinates.x;
        } else {
            slider->button->sceneNode.coordinates.x = slider->sceneNode.coordinates.x;
            slider->button->sceneNode.coordinates.x += slider->base->spriteBase->virtualSize.x * slider->base->value / 100;
        }
        slider->button->sceneNode.coordinates.x -= slider->button->sprite->virtualSize.x / 2;
            switch (slider->buttonAllignY) {
                case SLIDER_ALLIGN_Y_CENTER:
                    slider->button->sceneNode.coordinates.y -= slider->button->sprite->virtualSize.y / 2;
                    slider->button->sceneNode.coordinates.y += slider->base->spriteBase->virtualSize.y / 2;
                    break;
                case SLIDER_ALLIGN_Y_TOP:
                    slider->button->sceneNode.coordinates.y += slider->base->spriteBase->virtualSize.y;
                    break;
                case SLIDER_ALLIGN_Y_BOTTOM:
                    slider->button->sceneNode.coordinates.y -= slider->button->sprite->virtualSize.y;
                    break;
                case SLIDER_ALLIGN_Y_NONE:
                    break;
            }
        slider->button->isGeometryChanged = true;
        slider->isGeometryChanged = false;
    }
    ProgressBar_update((struct SceneNode*)slider->base, eventManager, renderer);
    Button_update((struct SceneNode*)slider->button, eventManager, renderer);
}

void Slider_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct Slider* slider = (struct Slider*)sceneNode;
    ProgressBar_render((struct SceneNode*)slider->base, renderer);
    Button_render((struct SceneNode*)slider->button, renderer);
}

void Slider_sound(struct SceneNode* sceneNode, struct Musican* musican) {
    if (!sceneNode || !musican)
        return;
    struct Slider* slider = (struct Slider*)sceneNode;
    Button_sound((struct SceneNode*)slider->button, musican);
}
