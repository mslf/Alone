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
                                                         struct ResourceManager* resourceManager, 
                                                         struct Renderer* renderer, struct TextParser* textParser) {
    if (!slider || !resourceManager || !renderer || !textParser)
        return 1;
    char* tempProgressBarResourceString = NULL;
    tempProgressBarResourceString = TextParser_getString(textParser,
                                                         SLIDER_SCENENODE_PARSER_PROGRESS_BAR_RESOURCE_STRING, 0);
    if (!tempProgressBarResourceString) {
        Logger_log(renderer->logger, SLIDER_SCENENODE_PARSER_ERR_NO_PROGRESS_BAR);
        return 2;
    }
    slider->base = ProgressBar_construct(resourceManager, renderer, tempProgressBarResourceString);
    if (!slider->base)
        return 3;
    slider->base->value = (unsigned char)TextParser_getInt(textParser, SLIDER_SCENENODE_PARSER_VALUE, 0);
    if (slider->base->value > 100)
        slider->base->value = 100;
    slider->step = (unsigned char)TextParser_getInt(textParser, SLIDER_SCENENODE_PARSER_STEP, 0);
    if (slider->step > 100)
        slider->step = 100;
    return 0;
}

static unsigned char Slider_loadButton(struct Slider* slider, 
                                                         struct ResourceManager* resourceManager, 
                                                         struct Renderer* renderer, struct TextParser* textParser) {
    if (!slider || !resourceManager || !renderer || !textParser)
        return 1;
    char* tempButtonResourceString = NULL;
    tempButtonResourceString = TextParser_getString(textParser, SLIDER_SCENENODE_PARSER_BUTTON_RESOURCE_STRING, 0);
    if (!tempButtonResourceString) {
        Logger_log(renderer->logger, SLIDER_SCENENODE_PARSER_ERR_NO_BUTTON);
        return 2;
    }
    slider->button = Button_construct(resourceManager, renderer, tempButtonResourceString);
    if (!slider->button)
        return 3;
    slider->buttonAllignX = (enum AllignX)TextParser_getInt(textParser, SLIDER_SCENENODE_PARSER_ALLIGN_STRING, 0);
    if (slider->buttonAllignX > (enum AllignX)2)
        slider->buttonAllignX = AllignX_center;
    slider->buttonAllignY = (enum AllignY)TextParser_getInt(textParser, SLIDER_SCENENODE_PARSER_ALLIGN_STRING, 1);
    if (slider->buttonAllignY > (enum AllignY)3)
        slider->buttonAllignY = AllignY_center;
    return 0; 
}

static unsigned char Slider_tryGetSettingsFromTextParser(struct Slider* slider, 
                                                         struct ResourceManager* resourceManager, 
                                                         struct Renderer* renderer, struct TextParser* textParser) {
    if (!slider || !resourceManager || !renderer || !textParser)
        return 1;
    unsigned char result = 0;
    result += Slider_loadButton(slider, resourceManager, renderer, textParser);
    result += Slider_loadBase(slider, resourceManager, renderer, textParser);
    slider->isGeometryChanged = true;
    return result;
}

struct Slider* Slider_construct(struct ResourceManager* const resourceManager,
                                struct Renderer* renderer, const char* const sliderResId) {
    if (!resourceManager || !renderer || !sliderResId)
        return NULL;
    struct Slider* slider = NULL;
    slider = (struct Slider*)calloc(1, sizeof(struct Slider));
    if (!slider)
        return NULL;
    SceneNode_init(&(slider->sceneNode));
    slider->sceneNode.sceneNodeTextResource = ResourceManager_loadTextResource(resourceManager, sliderResId, 0);
    if (!slider->sceneNode.sceneNodeTextResource) {
        Slider_destruct(slider);
        return NULL;
    }
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                      slider->sceneNode.sceneNodeTextResource);
    if (!textParser) {
        Slider_destruct(slider);
        return NULL;
    }
    if (Slider_tryGetSettingsFromTextParser(slider, resourceManager, renderer, textParser)) {
        TextParser_destruct(textParser);
        Slider_destruct(slider);
        return NULL;
    }
    TextParser_destruct(textParser);
    slider->sceneNode.control = Slider_control;
    slider->sceneNode.update = Slider_update;
    slider->sceneNode.render = Slider_render;
    slider->sceneNode.sound = Slider_sound;
    slider->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(SLIDER_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!slider->sceneNode.type) {
        Slider_destruct(slider);
        return NULL;
    }
    strcpy(slider->sceneNode.type, SLIDER_SCENENODE_PARSER_TYPE_STRING);
    return slider;
}

void Slider_destruct(struct Slider* slider) {
    if (!slider)
        return;
    if (slider->base)
        ProgressBar_destruct(slider->base);
    if (slider->button)
        Button_destruct(slider->button);
    if (slider->sceneNode.sceneNodeTextResource)
        slider->sceneNode.sceneNodeTextResource->pointersCount--;
    if (slider->sceneNode.type)
        free(slider->sceneNode.type);
}

void Slider_save(
        const struct Slider* const slider, struct ResourceManager* const resourceManager,
        const char* const sliderResId) {

}

void Slider_control(struct SceneNode* sceneNode, struct EventManager* eventManager) {
    if (!sceneNode || !eventManager)
        return;
    struct Slider* slider = (struct Slider*)sceneNode;
    Button_control((struct SceneNode*)slider->button, eventManager);
    if (slider->button->state == ButtonState_Pressed) {
        SDL_Point mouseCoordinates;
        SDL_GetMouseState(&mouseCoordinates.x, &mouseCoordinates.y);
        mouseCoordinates.x -= slider->base->spriteBase->dstRect.x;
        mouseCoordinates.x = mouseCoordinates.x / slider->base->spriteBase->dstRect.w * 100;
        // mouseCoordinates.x now contain ofsset in % from begining slider->base->spriteBase
        if (mouseCoordinates.x % slider->step == 0) {
            slider->base->value = mouseCoordinates.x;
            slider->isGeometryChanged = true;
        }
    }
}

void Slider_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager || !renderer)
        return;
    struct Slider* slider = (struct Slider*)sceneNode;
    if (slider->isGeometryChanged) {
        slider->base->sceneNode.coordinates = slider->sceneNode.coordinates;
        slider->base->sceneNode.flip = slider->sceneNode.flip;
        slider->base->sceneNode.scaleX = slider->sceneNode.scaleX;
        slider->base->sceneNode.scaleY = slider->sceneNode.scaleY;
        ProgressBar_update((struct SceneNode*)slider->base, eventManager, renderer);
        slider->button->sceneNode.coordinates = slider->sceneNode.coordinates;
        slider->button->sceneNode.coordinates.x += slider->base->spriteBase->virtualSize.x * slider->base->value / 100;
        switch (slider->buttonAllignX) {
            case AllignX_center:
                slider->button->sceneNode.coordinates.x -= slider->button->sprite->virtualSize.x / 2;
                break;
            case AllignX_right:
                slider->button->sceneNode.coordinates.x -= slider->button->sprite->virtualSize.x;
                break;
            case AllignX_left:
                break;
        }
        switch (slider->buttonAllignY) {
            case AllignY_center:
                slider->button->sceneNode.coordinates.y -= slider->button->sprite->virtualSize.y / 2;
                slider->button->sceneNode.coordinates.y += slider->base->spriteBase->virtualSize.y / 2;
                break;
            case AllignY_top:
                slider->button->sceneNode.coordinates.x += slider->base->spriteBase->virtualSize.y;
                break;
            case AllignY_bottom:
                slider->button->sceneNode.coordinates.x -= slider->button->sprite->virtualSize.y;
                break;
            case AllignY_none:
                break;
        }
        slider->button->sceneNode.flip = slider->sceneNode.flip;
        slider->button->sceneNode.scaleX = slider->sceneNode.scaleX;
        slider->button->sceneNode.scaleY = slider->sceneNode.scaleY;
        Button_update((struct SceneNode*)slider->button, eventManager, renderer);
        slider->isGeometryChanged = false;
    }
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
