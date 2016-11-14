//
// Created by mslf on 8/13/16.
//
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
#include "gui/Slider.h"

const char* const SLIDER_SCENENODE_PARSER_ERR_NO_BUTTON = 
            "Slider_loadButton: buttonResource string haven't found!";
const char* const SLIDER_SCENENODE_PARSER_ERR_NO_PROGRESS_BAR =
            "Slider_loadBase: progressBarResource string haven't found!";

static unsigned char Slider_loadBase(struct Slider* slider,
                                     struct ResourceManager* const resourceManager,
                                     struct Renderer* const renderer,
                                     struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                     struct TextParser* const textParser) {
    if (!slider || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    const char* tempResId = NULL;
    tempResId = TextParser_getString(textParser, SLIDER_SCENENODE_PARSER_PROGRESS_BAR_RESOURCE_STRING, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, SLIDER_SCENENODE_PARSER_ERR_NO_PROGRESS_BAR);
        return 2;
    }
    slider->base = (struct ProgressBar*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                            renderer,
                                                                            sceneNodeTypesRegistrar,
                                                                            tempResId,
                                                                            PROGRESS_BAR_SCENENODE_PARSER_TYPE_STRING);
    if (!slider->base)
        return 3;
    slider->base->value = (unsigned char)TextParser_getInt(textParser, SLIDER_SCENENODE_PARSER_VALUE, 0);
    if (slider->base->value > 100)
        slider->base->value = 100;
    slider->step = (unsigned char)TextParser_getInt(textParser, SLIDER_SCENENODE_PARSER_STEP, 0);
    if (slider->step > 100 || slider->step == 0)
        slider->step = 100;
    return 0;
}

static unsigned char Slider_loadButton(struct Slider* slider,
                                       struct ResourceManager* const resourceManager,
                                       struct Renderer* const renderer,
                                       struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                       struct TextParser* const textParser) {
    if (!slider || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    const char* tempResId = NULL;
    tempResId = TextParser_getString(textParser, SLIDER_SCENENODE_PARSER_BUTTON_RESOURCE_STRING, 0);
    if (!tempResId) {
        Logger_log(renderer->logger, SLIDER_SCENENODE_PARSER_ERR_NO_BUTTON);
        return 2;
    }
    slider->button = (struct Button*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                                renderer,
                                                                                sceneNodeTypesRegistrar,
                                                                                tempResId,
                                                                                ButtonSceneNode_parserStrings.type);
    if (!slider->button)
        return 3;
    slider->buttonAllignY = (enum AllignY)TextParser_getInt(textParser, SLIDER_SCENENODE_PARSER_ALLIGN_STRING, 0);
    if (slider->buttonAllignY > (enum AllignY)3)
        slider->buttonAllignY = AllignY_center;
    return 0; 
}

static unsigned char Slider_tryGetSettingsFromTextParser(struct Slider* slider,
                                       struct ResourceManager* const resourceManager,
                                       struct Renderer* const renderer,
                                       struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                       struct TextParser* const textParser) {
    if (!slider || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    unsigned char result = 0;
    result += Slider_loadButton(slider, resourceManager, renderer, sceneNodeTypesRegistrar, textParser);
    result += Slider_loadBase(slider, resourceManager, renderer, sceneNodeTypesRegistrar, textParser);
    slider->isGeometryChanged = true;
    return result;
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
    if (Slider_tryGetSettingsFromTextParser(slider, resourceManager, renderer,
                                            sceneNodeTypesRegistrar, textParser)) {
        Slider_destruct((struct SceneNode*)slider);
        return NULL;
    }
    slider->sceneNode.control = Slider_control;
    slider->sceneNode.update = Slider_update;
    slider->sceneNode.render = Slider_render;
    slider->sceneNode.sound = Slider_sound;
    slider->sceneNode.destruct = Slider_destruct;
    slider->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(SLIDER_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!slider->sceneNode.type) {
        Slider_destruct((struct SceneNode*)slider);
        return NULL;
    }
    strcpy(slider->sceneNode.type, SLIDER_SCENENODE_PARSER_TYPE_STRING);
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
    if (slider->sceneNodeTextResource)
        slider->sceneNodeTextResource->pointersCount--;
    if (slider->type)
        free(slider->type);
    free(slider);
}

unsigned char Slider_save(
        const struct Slider* const slider, struct ResourceManager* const resourceManager,
        const char* const sliderResId) {
    if (!slider || !resourceManager || !sliderResId)
        return 1;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return 2;
    result += TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING, SLIDER_SCENENODE_PARSER_TYPE_STRING);
    result += TextParser_addString(textParser, SLIDER_SCENENODE_PARSER_PROGRESS_BAR_RESOURCE_STRING,
                                   slider->base->sceneNode.sceneNodeTextResource->id);
    result += TextParser_addString(textParser, SLIDER_SCENENODE_PARSER_BUTTON_RESOURCE_STRING,
                                   slider->button->sceneNode.sceneNodeTextResource->id);
    result += TextParser_addInt(textParser, SLIDER_SCENENODE_PARSER_STEP, (long)slider->step);
    result += TextParser_addInt(textParser, SLIDER_SCENENODE_PARSER_VALUE, (long)slider->base->value);
    char* tempString = TextParser_convertToText(textParser);
    result += textParser->lastError;
    result += TextResource_updateContent(slider->sceneNode.sceneNodeTextResource, tempString);
    result += ResourceManager_saveTextResource(resourceManager,
                                               slider->sceneNode.sceneNodeTextResource, sliderResId);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    return result;
}

void Slider_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {
    if (!sceneNode || !eventManager)
        return;
    struct Slider* slider = (struct Slider*)sceneNode;
    Button_control((struct SceneNode*)slider->button, eventManager);
    if (slider->button->state == BUTTON_STATE_PRESSED) {
        SDL_Point mouseCoordinates;
        SDL_GetMouseState(&mouseCoordinates.x, &mouseCoordinates.y);
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
        slider->base->sceneNode.flip = slider->sceneNode.flip;
        slider->base->sceneNode.scaleX = slider->sceneNode.scaleX;
        slider->base->sceneNode.scaleY = slider->sceneNode.scaleY;
        slider->base->isGeometryChanged = true;
        ProgressBar_update((struct SceneNode*)slider->base, eventManager, renderer);
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
                case AllignY_center:
                    slider->button->sceneNode.coordinates.y -= slider->button->sprite->virtualSize.y / 2;
                    slider->button->sceneNode.coordinates.y += slider->base->spriteBase->virtualSize.y / 2;
                    break;
                case AllignY_top:
                    slider->button->sceneNode.coordinates.y += slider->base->spriteBase->virtualSize.y;
                    break;
                case AllignY_bottom:
                    slider->button->sceneNode.coordinates.y -= slider->button->sprite->virtualSize.y;
                    break;
                case AllignY_none:
                    break;
            }
        slider->button->sceneNode.flip = slider->sceneNode.flip;
        slider->button->sceneNode.scaleX = slider->sceneNode.scaleX;
        slider->button->sceneNode.scaleY = slider->sceneNode.scaleY;
        slider->button->isGeometryChanged = true;
        slider->isGeometryChanged = false;
    }
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
