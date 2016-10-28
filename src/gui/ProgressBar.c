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
const char* const PROGRESS_BAR_SCENENODE_ERR_SPRITE_NO_TYPE = 
        "ProgressBar_loadSpritesResource: type string haven't found!";
const char* const PROGRESS_BAR_SCENENODE_ERR_SPRITE_TYPE = 
        "ProgressBar_loadSpritesResource: suitable Sprite type string haven't detected!";
const char* const PROGRESS_BAR_SCENENODE_ERR_SPRITE_NO_2_ANIMATIONS = 
        "ProgressBar_loadSpritesResource: loaded Sprite doesn't contain 2 animations!";

unsigned char ProgressBar_loadSpritesResource(struct ProgressBar* progressBar, struct ResourceManager* resourceManager,
                               struct Renderer* renderer, struct TextParser* textParser) {
    char* tempSpriteResourceString = TextParser_getString(textParser, PROGRESS_BAR_SCENENODE_PARSER_SPRITE_RES_STRING, 0);
    if (!tempSpriteResourceString) {
        Logger_log(resourceManager->logger, PROGRESS_BAR_SCENENODE_ERR_SPRITE_RES);
        return 1;
    }
    struct TextResource* spriteTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                               tempSpriteResourceString, 0);
    if (!spriteTextResource)
       return 2;
    struct TextParser* spriteTextParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                                               spriteTextResource);
    if (!spriteTextParser) {
        spriteTextResource->pointersCount--;
        return 3;
    }
    char* tempSpriteTypeString = TextParser_getString(spriteTextParser, TEXT_PARSER_TYPE_STRING, 0);
    if (!tempSpriteTypeString) {
        Logger_log(resourceManager->logger, PROGRESS_BAR_SCENENODE_ERR_SPRITE_NO_TYPE);
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 4;
    }
    if (strcmp(tempSpriteTypeString, SPRITE_SCENENODE_PARSER_TYPE_STRING) != 0) {
        Logger_log(resourceManager->logger, PROGRESS_BAR_SCENENODE_ERR_SPRITE_TYPE);
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 5;
    }
    progressBar->spriteBase = Sprite_construct(resourceManager, renderer, tempSpriteResourceString);
    if (!progressBar->spriteBase) {
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 6;
    }
    if (progressBar->spriteBase->animationsCount < 2) {
        Logger_log(resourceManager->logger, PROGRESS_BAR_SCENENODE_ERR_SPRITE_NO_2_ANIMATIONS);
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 7;
    }
    progressBar->spriteBar = Sprite_construct(resourceManager, renderer, tempSpriteResourceString);
    if (!progressBar->spriteBar) {
        spriteTextResource->pointersCount--;
        TextParser_destruct(spriteTextParser);
        return 8;
    }
    progressBar->spriteBar->currentAnimation = 1;
    TextParser_destruct(spriteTextParser);
    spriteTextResource->pointersCount--;
    return 0;
}

unsigned char ProgressBar_tryGetSettingsFromTextParser(struct ProgressBar* progressBar,
                                                       struct ResourceManager* resourceManager, 
                                                       struct Renderer* renderer, struct TextParser* textParser) {
    unsigned char result = 0;
    unsigned char value = 0;
    result += ProgressBar_loadSpritesResource(progressBar, resourceManager, renderer, textParser);
    value = (unsigned char)TextParser_getInt(textParser, PROGRESS_BAR_SCENENODE_PARSER_INIT_VALUE_STRING, 0);
    if (value <= 100)
        progressBar->value = value;
    if (result)
        return result;
    return 0;
}

struct ProgressBar* ProgressBar_construct(struct ResourceManager* const resourceManager,
                                          struct Renderer* renderer, const char* const progressBarResId) {
    struct ProgressBar* progressBar = NULL;
    progressBar = (struct ProgressBar*)calloc(1, sizeof(struct ProgressBar));
    if (!progressBar)
        return NULL;
    SceneNode_init(&(progressBar->sceneNode));
    progressBar->sceneNode.sceneNodeTextResource = ResourceManager_loadTextResource(resourceManager,
                                                                                    progressBarResId, 0);
    if (!progressBar->sceneNode.sceneNodeTextResource) {
        ProgressBar_destruct(progressBar);
        return NULL;
    }
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                      progressBar->sceneNode.sceneNodeTextResource);
    if (!textParser) {
        ProgressBar_destruct(progressBar);
        return NULL;
    }
    if (ProgressBar_tryGetSettingsFromTextParser(progressBar, resourceManager, renderer, textParser)) {
        TextParser_destruct(textParser);
        ProgressBar_destruct(progressBar);
        return NULL;
    }
    TextParser_destruct(textParser);
    progressBar->sceneNode.update = ProgressBar_update;
    progressBar->sceneNode.render = ProgressBar_render;
    progressBar->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(PROGRESS_BAR_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!progressBar->sceneNode.type) {
        ProgressBar_destruct(progressBar);
        return NULL;
    }
    strcpy(progressBar->sceneNode.type, PROGRESS_BAR_SCENENODE_PARSER_TYPE_STRING);
    progressBar->isGeometryChanged = true;
    return progressBar;
}

void ProgressBar_destruct(struct ProgressBar* progressBar) {
    if (!progressBar)
        return;
    if (progressBar->spriteBase)
        Sprite_destruct(progressBar->spriteBase);
    if (progressBar->spriteBar)
        Sprite_destruct(progressBar->spriteBar);
    if (progressBar->sceneNode.sceneNodeTextResource)
        progressBar->sceneNode.sceneNodeTextResource->pointersCount--;
    if (progressBar->sceneNode.type)
        free(progressBar->sceneNode.type);
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
    result += TextParser_addInt(textParser, PROGRESS_BAR_SCENENODE_PARSER_INIT_VALUE_STRING, progressBar->value);
    char* tempString = TextParser_convertToText(textParser);
    if (!tempString)
        result++;
    if (TextResource_updateContent(progressBar->sceneNode.sceneNodeTextResource, tempString))
        result++;
    result += ResourceManager_saveTextResource(resourceManager,
                                               progressBar->sceneNode.sceneNodeTextResource, progressBarResId);
    if (result) {
        TextParser_destruct(textParser);
        return 3;
    }
    TextParser_destruct(textParser);
    return 0;
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
