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
#include "gui/ProgressBar.h"
#include "textParser/TextParser.h"

const char* const PROGRESS_BAR_SCENENODE_ERR_SPRITE_RES = 
        "ProgressBar_loadSpritesResource: spriteResource string haven't found!";
const char* const PROGRESS_BAR_SCENENODE_ERR_SPRITE_NO_2_ANIMATIONS = 
        "ProgressBar_loadSpritesResource: loaded Sprite doesn't contain 2 animations!";

static unsigned char ProgressBar_loadSpritesResource(struct ProgressBar* progressBar,
                                                     struct ResourceManager* const resourceManager,
                                                     struct Renderer* const renderer,
                                                     struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                     struct TextParser* const textParser) {
    if (!progressBar || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    char* tempResId = TextParser_getString(textParser, PROGRESS_BAR_SCENENODE_PARSER_SPRITE_RES_STRING, 0);
    if (!tempResId) {
        Logger_log(resourceManager->logger, PROGRESS_BAR_SCENENODE_ERR_SPRITE_RES);
        return 2;
    }
    progressBar->spriteBase = (struct Sprite*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                         renderer,
                                                                         sceneNodeTypesRegistrar,
                                                                         tempResId,
                                                                         SPRITE_SCENENODE_PARSER_TYPE_STRING);
    if (!progressBar->spriteBase)
        return 3;
    if (progressBar->spriteBase->animationsCount < 2) {
        Logger_log(resourceManager->logger, PROGRESS_BAR_SCENENODE_ERR_SPRITE_NO_2_ANIMATIONS);
        progressBar->spriteBase->sceneNode.destruct((struct SceneNode*)progressBar->spriteBase);
        progressBar->spriteBase = NULL;
        return 4;
    }
    progressBar->spriteBar = (struct Sprite*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                         renderer,
                                                                         sceneNodeTypesRegistrar,
                                                                         tempResId,
                                                                         SPRITE_SCENENODE_PARSER_TYPE_STRING);
    if (!progressBar->spriteBar) {
        progressBar->spriteBase->sceneNode.destruct((struct SceneNode*)progressBar->spriteBase);
        progressBar->spriteBase = NULL;
        return 5;
    }
    progressBar->spriteBar->currentAnimation = 1;
    return 0;
}

static unsigned char ProgressBar_tryGetSettingsFromTextParser(struct ProgressBar* progressBar,
                                                              struct ResourceManager* const resourceManager,
                                                              struct Renderer* const renderer,
                                                              struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                              struct TextParser* const textParser) {
    if (!progressBar || !resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return 1;
    unsigned char result = 0;
    unsigned char value = 0;
    result += ProgressBar_loadSpritesResource(progressBar, resourceManager, renderer,
                                              sceneNodeTypesRegistrar, textParser);
    value = (unsigned char)TextParser_getInt(textParser, PROGRESS_BAR_SCENENODE_PARSER_INIT_VALUE_STRING, 0);
    if (value <= 100)
        progressBar->value = value;
    return result;
}

struct SceneNode* ProgressBar_construct(struct ResourceManager* const resourceManager,
                                        struct Renderer* const renderer,
                                        struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                        struct TextParser* const textParser) {
    if (!resourceManager || !renderer || !sceneNodeTypesRegistrar || !textParser)
        return NULL;
    struct ProgressBar* progressBar = NULL;
    progressBar = (struct ProgressBar*)calloc(1, sizeof(struct ProgressBar));
    if (!progressBar)
        return NULL;
    SceneNode_init(&(progressBar->sceneNode));
    if (ProgressBar_tryGetSettingsFromTextParser(progressBar, resourceManager, renderer,
                                                            sceneNodeTypesRegistrar, textParser)) {
        ProgressBar_destruct((struct SceneNode*)progressBar);
        return NULL;
    }
    progressBar->sceneNode.update = ProgressBar_update;
    progressBar->sceneNode.render = ProgressBar_render;
    progressBar->sceneNode.destruct = ProgressBar_destruct;
    progressBar->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(PROGRESS_BAR_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!progressBar->sceneNode.type) {
        ProgressBar_destruct((struct SceneNode*)progressBar);
        return NULL;
    }
    strcpy(progressBar->sceneNode.type, PROGRESS_BAR_SCENENODE_PARSER_TYPE_STRING);
    progressBar->isGeometryChanged = true;
    return (struct SceneNode*)progressBar;
}

void ProgressBar_destruct(struct SceneNode* progressBar) {
    if (!progressBar)
        return;
    struct ProgressBar* tempProgressBar = (struct ProgressBar*)progressBar;
    if (tempProgressBar->spriteBase)
        Sprite_destruct((struct SceneNode*)tempProgressBar->spriteBase);
    if (tempProgressBar->spriteBar)
        Sprite_destruct((struct SceneNode*)tempProgressBar->spriteBar);
    if (progressBar->sceneNodeTextResource)
        progressBar->sceneNodeTextResource->pointersCount--;
    if (progressBar->type)
        free(progressBar->type);
}

unsigned char ProgressBar_save(
        const struct ProgressBar* const progressBar, struct ResourceManager* const resourceManager,
        const char* const progressBarResId) {
    if (!progressBar || !resourceManager || !progressBarResId)
        return 1;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return 2;
    result += TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING, PROGRESS_BAR_SCENENODE_PARSER_TYPE_STRING);
    result += TextParser_addString(textParser, PROGRESS_BAR_SCENENODE_PARSER_SPRITE_RES_STRING,
                                   progressBar->spriteBase->sceneNode.sceneNodeTextResource->id);
    result += TextParser_addInt(textParser, PROGRESS_BAR_SCENENODE_PARSER_INIT_VALUE_STRING, (long)progressBar->value);
    char* tempString = TextParser_convertToText(textParser);
    result += textParser->lastError;
    result += TextResource_updateContent(progressBar->sceneNode.sceneNodeTextResource, tempString);
    result += ResourceManager_saveTextResource(resourceManager,
                                               progressBar->sceneNode.sceneNodeTextResource, progressBarResId);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    return result;
}

void ProgressBar_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager || !renderer)
        return;
    struct ProgressBar* progressBar = (struct ProgressBar*)sceneNode;
    if (progressBar->isGeometryChanged) {
        progressBar->spriteBase->sceneNode.angle = progressBar->sceneNode.angle;
        progressBar->spriteBase->sceneNode.coordinates = progressBar->sceneNode.coordinates;
        progressBar->spriteBase->sceneNode.flip = progressBar->sceneNode.flip;
        progressBar->spriteBase->sceneNode.rotatePointCoordinates = progressBar->sceneNode.rotatePointCoordinates;
        progressBar->spriteBase->sceneNode.scaleX = progressBar->sceneNode.scaleX;
        progressBar->spriteBase->sceneNode.scaleY = progressBar->sceneNode.scaleY;
        progressBar->spriteBar->sceneNode.angle = progressBar->sceneNode.angle;
        progressBar->spriteBar->sceneNode.coordinates = progressBar->sceneNode.coordinates;
        progressBar->spriteBar->sceneNode.flip = progressBar->sceneNode.flip;
        progressBar->spriteBar->sceneNode.rotatePointCoordinates = progressBar->sceneNode.rotatePointCoordinates;
        progressBar->spriteBar->sceneNode.scaleX = progressBar->sceneNode.scaleX;
        progressBar->spriteBar->sceneNode.scaleY = progressBar->sceneNode.scaleY;
        progressBar->spriteBar->percentsToRender.x = progressBar->value;
        progressBar->isGeometryChanged = false;
    }
    Sprite_update((struct SceneNode*)progressBar->spriteBase, eventManager, renderer);
    Sprite_update((struct SceneNode*)progressBar->spriteBar, eventManager, renderer);
}

void ProgressBar_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct ProgressBar* progressBar = (struct ProgressBar*)sceneNode;
    Sprite_render((struct SceneNode*)progressBar->spriteBase, renderer);
    Sprite_render((struct SceneNode*)progressBar->spriteBar, renderer);
}
