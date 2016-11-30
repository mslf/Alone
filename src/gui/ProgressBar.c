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
 * @file ProgressBar.c
 * @author mslf
 * @date 13 Aug 2016
 * @brief File containing implementation of #ProgressBar.
 */
#include <assert.h>
#include "gui/ProgressBar.h"
#include "textParser/TextParser.h"

/**
 * @brief Error message strings for #ProgressBar.
 */
static const struct ProgressBarSceneNode_errorMessages {
    const char* const errNoSpriteRes;
     /**< Will be displayed when #TextParser have no ProgressBarSceneNode_parserStrings#spriteRes. */
    const char* const errNoEnoughAnimations;
    /**< Will be displayed when constructed #Sprite doesn't have enough animations for #ProgressBar. */
}ProgressBarSceneNode_errorMessages = {
    "ProgressBar_loadSpritesResource: spriteResource string haven't found!",
    "ProgressBar_loadSpritesResource: loaded Sprite doesn't contain 2 animations!"};

/**
 * @brief Function for loading and constructing ProgressBar#spriteBase and ProgressBar#spriteBar.
 * @param progressBar Pointer to a #ProgressBar where to construct ProgressBar#spriteBase 
 * and ProgressBar#spriteBar.
 * @param resourceManager Pointer to a #ResourceManager for loading #Sprite resource.
 * @param renderer Pointer to a #Renderer for constructing ProgressBar#spriteBase 
 * and ProgressBar#spriteBar.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing ProgressBar#spriteBase and ProgressBar#spriteBar.
 * @param textParser Pointer to a #TextParser, where function will get 
 * ProgressBarSceneNode_parserStrings#spriteRes.
 * @return #ProgressBarSceneNode_errors value.
 * @see #ProgressBar
 * @see #Sprite
 * @see #ResourceManager
 * @see #Renderer
 * @see #SceneNodeTypesRegistrar
 * @see #TextParser
 * @see #ProgressBarSceneNode_parserStrings
 * @see #ProgressBarSceneNode_errors
 */
static enum ProgressBarSceneNode_errors ProgressBar_loadSpritesResource(struct ProgressBar* progressBar,
                                                     struct ResourceManager* const resourceManager,
                                                     struct Renderer* const renderer,
                                                     struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                     struct TextParser* const textParser) {
    assert(progressBar);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    const char* const tempResId = TextParser_getString(textParser, ProgressBarSceneNode_parserStrings.spriteRes, 0);
    if (!tempResId) {
        Logger_log(resourceManager->logger, ProgressBarSceneNode_errorMessages.errNoSpriteRes);
        return PROGRESS_BAR_ERR_NO_SPRITE_RES;
    }
    progressBar->spriteBase = (struct Sprite*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                         renderer,
                                                                         sceneNodeTypesRegistrar,
                                                                         tempResId,
                                                                         SPRITE_SCENENODE_PARSER_TYPE_STRING);
    if (!progressBar->spriteBase)
        return PROGRESS_BAR_ERR_CONSTRUCTIG_SPRITE;
    if (progressBar->spriteBase->animationsCount < 2) {
        Logger_log(resourceManager->logger, ProgressBarSceneNode_errorMessages.errNoEnoughAnimations);
        return PROGRESS_BAR_ERR_NOT_ENOUGH_ANIMATIONS;
    }
    progressBar->spriteBar = (struct Sprite*)SceneNodeTypesRegistrar_constructSceneNode(resourceManager,
                                                                         renderer,
                                                                         sceneNodeTypesRegistrar,
                                                                         tempResId,
                                                                         SPRITE_SCENENODE_PARSER_TYPE_STRING);
    if (!progressBar->spriteBar)
        return PROGRESS_BAR_ERR_CONSTRUCTIG_SPRITE;
    progressBar->spriteBar->currentAnimation = 1;
    return PROGRESS_BAR_NO_ERRORS;
}

/**
 * @brief Function for loading settings and initializing #ProgressBar from #TextParser.
 * @param progressBar Pointer to a #ProgressBar which will be initialized.
 * @param resourceManager Pointer to a #ResourceManager for loading required resources.
 * @param renderer Pointer to a #Renderer for constructing ProgressBar#spriteBase 
 * and ProgressBar#spriteBar.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar 
 * for constructing ProgressBar#spriteBase and ProgressBar#spriteBar.
 * @param textParser Pointer to a #TextParser with data strings.
 * @return #ProgressBarSceneNode_errors value.
 * @see #ProgressBar
 * @see #Sprite
 * @see #SceneNodeTypesRegistrar
 * @see #ResourceManager
 * @see #Renderer
 * @see #TextParser
 * @see #ProgressBarSceneNode_parserStrings
 * @see #ProgressBarSceneNode_errors
 */
static enum ProgressBarSceneNode_errors ProgressBar_tryGetSettingsFromTextParser(struct ProgressBar* progressBar,
                                                              struct ResourceManager* const resourceManager,
                                                              struct Renderer* const renderer,
                                                              struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                              struct TextParser* const textParser) {
    assert(progressBar);
    assert(resourceManager);
    assert(renderer);
    assert(sceneNodeTypesRegistrar);
    assert(textParser);
    unsigned char value = 0;
    value = (unsigned char)TextParser_getInt(textParser, ProgressBarSceneNode_parserStrings.value, 0);
    if (value <= 100)
        progressBar->value = value;
    return (ProgressBar_loadSpritesResource(progressBar, resourceManager, renderer,
                                             sceneNodeTypesRegistrar, textParser));
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
    progressBar->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(ProgressBarSceneNode_parserStrings.type) + 1));
    if (!progressBar->sceneNode.type) {
        ProgressBar_destruct((struct SceneNode*)progressBar);
        return NULL;
    }
    strcpy(progressBar->sceneNode.type, ProgressBarSceneNode_parserStrings.type);
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
    TextResource_decreasePointersCounter(progressBar->sceneNodeTextResource);
    if (progressBar->type)
        free(progressBar->type);
    free(progressBar);
}

enum ProgressBarSceneNode_errors ProgressBar_save(const struct ProgressBar* const progressBar, 
                                                  struct ResourceManager* const resourceManager,
                                                  const char* const progressBarResId) {
    if (!progressBar || !resourceManager || !progressBarResId)
        return PROGRESS_BAR_ERR_NULL_ARGUMENT;
    unsigned char result = 0;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return PROGRESS_BAR_ERR_CONSTRUCTIG_TEXT_PARSER;
    result += (TextParser_addString(textParser, TextParser_standartTypeString, ProgressBarSceneNode_parserStrings.type) != 0);
    result += (TextParser_addString(textParser, ProgressBarSceneNode_parserStrings.spriteRes,
                                   progressBar->spriteBase->dynamicSceneNode.sceneNode.sceneNodeTextResource->id) != 0);
    result += (TextParser_addInt(textParser, ProgressBarSceneNode_parserStrings.value, (long)progressBar->value) != 0);
    char* tempString = TextParser_convertToText(textParser);
    result += (textParser->lastError != 0);
    result += (TextResource_updateContent(progressBar->sceneNode.sceneNodeTextResource, tempString) != 0);
    result += (ResourceManager_saveTextResource(resourceManager,
                                               progressBar->sceneNode.sceneNodeTextResource, progressBarResId) != 0);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    if (result)
        return PROGRESS_BAR_ERR_SAVING;
    return PROGRESS_BAR_NO_ERRORS;
}

void ProgressBar_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !eventManager || !renderer)
        return;
    struct ProgressBar* progressBar = (struct ProgressBar*)sceneNode;
    if (progressBar->isGeometryChanged) {
        progressBar->spriteBase->dynamicSceneNode.sceneNode.coordinates = progressBar->sceneNode.coordinates;
        progressBar->spriteBar->dynamicSceneNode.sceneNode.coordinates = progressBar->sceneNode.coordinates;
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
