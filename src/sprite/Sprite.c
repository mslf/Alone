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
 * @file Sprite.c
 * @author mslf
 * @date 11 Aug 2016
 * @brief File containing implementation of #Sprite.
 */
#include <assert.h>
#include <stdbool.h>
#include <textParser/TextParser.h>
#include "sprite/Sprite.h"

static const struct SpriteSceneNode_errorMessages {
    const char* const warnNoVirtualSize;
    const char* const errNoTextureRes;
    const char* const errAnimationsAlloc;
    const char* const warnAnimationsCount;
    const char* const warnFramesCount;
    const char* const warnFrameDuration;
    const char* const warnFramesNotFitH;
    const char* const errFramesNotFitH;
    const char* const warnFramesNotFitV;
    const char* const errFramesNotFitV;
}SpriteSceneNode_errorMessages = {
    "Sprite_initAnimations: virtual size of sprite haven't found. Using present texture size for virtual size.",
    "Sprite_tryGetSettingsFromTextParser: TextureResource constructing failed!",
    "Sprite_initAnimations: allocating memory for animations failed!",
    "Sprite_initAnimations: animations count haven't properly defined or ignored. Assuming there is one animation.",
    "Sprite_initAnimations: framesCount haven't properly defined or ignored. Assuming there is one frame.",
    "Sprite_initAnimations: frameDuration haven't properly defined or ignored. Assuming it is equal to 0.",
    "Sprite_initAnimations: frame size is not fit in texture size horizontally. Trying to divide it by 2.",
    "Sprite_initAnimations: divided frame size is not fit in texture size horizontally!",
    "Sprite_initAnimations: frame size is not fit in texture size vertically. Trying to divide it by 2.",
    "Sprite_initAnimations: divided frame size is not fit in texture size vertically!"};

/**
 * @brief Checks and inits Sprite#frameSize and Sprite#virtualSize from #TextParser.
 * Sprite#frameSize.x * maxFramesCount should be less (or equal) then width of Sprite#textureResource.
 * Sprite#frameSize.y * Sprite#animationsCount should be less (or equal) then height of Sprite#textureResource.
 * If it fails with given sizes, then it will try to divide them by 2.
 * @param sprite Pointer to a #Sprite, where Sprite#frameSize and Sprite#virtualSize will be set.
 * @param logger Pointer to a #Logger for logging purpose. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for creating #Sprite.
 * @param usePresentForVirtual If that flag is set, then Sprite#virtualSize will be set 
 * to the real size of Sprite#textureResource.
 * @param maxFramesCount Given maximum number of frames for that #Sprite.
 * @return #SpriteSceneNode_errors value.
 * @see #SpriteSceneNode_errors
 * @see #TextureResource
 * @see #Animation
 * @see #Sprite
 */
static enum SpriteSceneNode_errors Sprite_initFrameSize(struct Sprite* sprite,
                                                        struct Logger* logger,
                                                        struct TextParser* textParser,
                                                        bool usePresentForVirtual,
                                                        size_t maxFramesCount) {
    assert(sprite);
    assert(textParser);
    int textureW;
    int textureH;
    if (SDL_QueryTexture(sprite->textureResource->texture, NULL, NULL, &textureW, &textureH))
        return SPRITE_ERR_GETTING_TEXTURE_SIZE;
    sprite->frameSize.x = (int)TextParser_getInt(textParser, SpriteSceneNode_parserStrings.frameSize, 0);
    if (sprite->frameSize.x <= 0 || sprite->frameSize.x * maxFramesCount > (size_t)textureW) {
        Logger_log(logger, SpriteSceneNode_errorMessages.warnFramesNotFitH);
        if (maxFramesCount == 1) {
            sprite->frameSize.x = textureW;
        }
        else {
            sprite->frameSize.x /= 2;
            if (sprite->frameSize.x <= 0 || sprite->frameSize.x * maxFramesCount > (size_t)textureW) {
                Logger_log(logger, SpriteSceneNode_errorMessages.errFramesNotFitH);
                return SPRITE_ERR_FRAME_SIZE_NOT_FIT_H;
            }
        }
    }
    sprite->frameSize.y = (int)TextParser_getInt(textParser, SpriteSceneNode_parserStrings.frameSize, 1);
    if (sprite->frameSize.y <= 0 || sprite->frameSize.y * sprite->animationsCount > (size_t)textureH) {
        Logger_log(logger, SpriteSceneNode_errorMessages.warnFramesNotFitV);
        if (sprite->animationsCount == 1) {
            sprite->frameSize.y = textureH;
        }
        else {
            sprite->frameSize.y /= 2;
            if (sprite->frameSize.y <= 0 || sprite->frameSize.y * sprite->animationsCount > (size_t)textureH) {
                Logger_log(logger, SpriteSceneNode_errorMessages.errFramesNotFitV);
                return SPRITE_ERR_FRAME_SIZE_NOT_FIT_V;
            }
        }
    }
    if (usePresentForVirtual) {
        sprite->virtualSize.x = textureW;
        sprite->virtualSize.y = textureH;
    }
    return SPRITE_NO_ERRORS;
}

/**
 * @brief Inits Sprite#animations list and inits Sprite#frameSize and Sprite#virtualSize from #TextParser.
 * @param sprite Pointer to a #Sprite, where Sprite#animations, Sprite#frameSize and Sprite#virtualSize will be set.
 * @param logger Pointer to a #Logger for logging purpose. Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for creating #Sprite.
 * @param usePresentForVirtual If that flag is set, then Sprite#virtualSize will be set 
 * to the real size of Sprite#textureResource.
 * @return #SpriteSceneNode_errors value.
 * @see #SpriteSceneNode_errors
 * @see #TextureResource
 * @see #Animation
 * @see #Sprite
 */
static enum SpriteSceneNode_errors Sprite_initAnimations(struct Sprite* sprite,
                                                         struct Logger* logger,
                                                         struct TextParser* textParser,
                                                         bool usePresentForVirtual) {
    assert(sprite);
    assert(textParser);
    sprite->animationsCount = (size_t)TextParser_getItemsCount(textParser, SpriteSceneNode_parserStrings.frameCountsList);
    if (sprite->animationsCount == 0) {
        Logger_log(logger, SpriteSceneNode_errorMessages.warnAnimationsCount);
        sprite->animationsCount = 1;
    }
    sprite->animations = (struct Animation*)calloc(sprite->animationsCount, sizeof(struct Animation));
    if (!sprite->animations) {
        Logger_log(logger, SpriteSceneNode_errorMessages.errAnimationsAlloc);
        return SPRITE_ERR_ANIMATIONS_ALLOC;
    }
    size_t maxFramesCount = 0;
    for (size_t i = 0; i < sprite->animationsCount; i++) {
        sprite->animations[i].framesCount = (size_t)TextParser_getInt(textParser,
                                                                      SpriteSceneNode_parserStrings.frameCountsList, i);
        if (textParser->lastError) {
            Logger_log(logger, SpriteSceneNode_errorMessages.warnAnimationsCount);
            sprite->animations[i].framesCount = 1;
        }
        if (sprite->animations[i].framesCount > maxFramesCount)
            maxFramesCount = sprite->animations[i].framesCount;
        sprite->animations[i].frameDuration = (size_t)TextParser_getInt(textParser,
                                                                        SpriteSceneNode_parserStrings.frameDurationsList, i);
        if (textParser->lastError) {
            Logger_log(logger, SpriteSceneNode_errorMessages.warnFrameDuration);
            sprite->animations[i].frameDuration = 0;
        }
    }
    enum SpriteSceneNode_errors result = Sprite_initFrameSize(sprite, logger, textParser,
                                                              usePresentForVirtual, maxFramesCount);
    if (result)
        return result;
    return SPRITE_NO_ERRORS;
}

/**
 * @brief Inits #Sprite from #TextParser.
 * @param sprite Pointer to a #Sprite to be initialized.
 * @param resourceManager Pointer to a #ResourceManager to load Sprite#textureResource.
 * @param renderer Pointer to a #Renderer to create Sprite#textureResource.
 * @param textParser Pointer to a #TextParser with data strings for creating #Sprite.
 * @return #SpriteSceneNode_errors value.
 * @see #SpriteSceneNode_errors
 * @see #TextureResource
 * @see #Animation
 * @see #Sprite
 */
static enum SpriteSceneNode_errors Sprite_tryGetSettingsFromTextParser(struct Sprite* sprite,
                                                                       struct ResourceManager* resourceManager,
                                                                       struct Renderer* renderer,
                                                                       struct TextParser* textParser) {
    assert(sprite);
    assert(resourceManager);
    assert(renderer);
    assert(textParser);
    bool usePresentForVirtual = 0;
    sprite->virtualSize.x = (int)TextParser_getInt(textParser, SpriteSceneNode_parserStrings.virtualSize, 0);
    sprite->virtualSize.y = (int)TextParser_getInt(textParser, SpriteSceneNode_parserStrings.virtualSize, 1);
    if (sprite->virtualSize.x <= 0 || sprite->virtualSize.y <= 0) {
        Logger_log(resourceManager->logger, SpriteSceneNode_errorMessages.warnNoVirtualSize);
        usePresentForVirtual = true;
    }
    const char* tempTextureResourceId = TextParser_getString(textParser, SpriteSceneNode_parserStrings.textureRes, 0);
    sprite->textureResource = ResourceManager_loadTextureResource(resourceManager, renderer, tempTextureResourceId);
    if (!sprite->textureResource) {
        Logger_log(resourceManager->logger, SpriteSceneNode_errorMessages.errNoTextureRes);
        return SPRITE_ERR_CONSTRUCTING_TEXTURE_RES;
    }
    enum SpriteSceneNode_errors result = Sprite_initAnimations(sprite, renderer->logger, textParser, usePresentForVirtual);
    sprite->percentsToRender.x = 100;
    sprite->percentsToRender.y = 100;
    if (result)
        return result;
    return SPRITE_NO_ERRORS;
}

struct SceneNode* Sprite_construct(struct ResourceManager* const resourceManager,
                                                 struct Renderer* const renderer,
                                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                 struct TextParser* const textParser) {
    // Sprite doesn't need sceneNodeTypesRegistrar, so we ignore it
    if (!resourceManager || !renderer || !textParser)
        return NULL;
    struct Sprite* sprite = NULL;
    sprite = (struct Sprite*)calloc(1, sizeof(struct Sprite));
    if (!sprite)
        return NULL;
    SceneNode_initDynamic(&(sprite->dynamicSceneNode));
    Logger_saveUsedFlagAndSetToFalse(renderer->logger);
    if (Sprite_tryGetSettingsFromTextParser(sprite, resourceManager, renderer, textParser)) {
        if (renderer->logger->wasUsed)
            Logger_log(renderer->logger, "\tin file: %s", textParser->file);
        Logger_revertUsedFlag(renderer->logger);
        Sprite_destruct((struct SceneNode*)sprite);
        return NULL;
    }
    if (renderer->logger->wasUsed)
        Logger_log(renderer->logger, "\tin file: %s", textParser->file);
    Logger_revertUsedFlag(renderer->logger);
    sprite->dynamicSceneNode.sceneNode.update = Sprite_update;
    sprite->dynamicSceneNode.sceneNode.render = Sprite_render;
    sprite->dynamicSceneNode.sceneNode.destruct = Sprite_destruct;
    sprite->dynamicSceneNode.sceneNode.type = (char*)malloc(sizeof(char) * (strlen(SpriteSceneNode_parserStrings.type) + 1));
    if (!sprite->dynamicSceneNode.sceneNode.type) {
        Sprite_destruct((struct SceneNode*)sprite);
        return NULL;
    }
    strcpy(sprite->dynamicSceneNode.sceneNode.type, SpriteSceneNode_parserStrings.type);
    return (struct SceneNode*)sprite;
}

void Sprite_destruct(struct SceneNode* sprite) {
    if (!sprite)
        return;
    struct Sprite* tempSprite = (struct Sprite*)sprite;
    if (tempSprite->animations)
        free(tempSprite->animations);
    TextureResource_decreasePointersCounter(tempSprite->textureResource);
    TextResource_decreasePointersCounter(sprite->sceneNodeTextResource);
    if (sprite->type)
        free(sprite->type);
    free(sprite);
}

enum SpriteSceneNode_errors Sprite_save(const struct Sprite* const sprite,
                                        struct ResourceManager* const resourceManager,
                                        const char* const spriteResId) {
    if (!sprite || !resourceManager || !spriteResId)
        return SPRITE_ERR_NULL_ARGUMENT;
    unsigned char result = 0;
    struct TextParser* textParser = TextParser_constructEmpty();
    if (!textParser)
        return SPRITE_ERR_CONSTRUCTIG_TEXT_PARSER;
    result += (TextParser_addString(textParser, TextParser_standartTypeString, SpriteSceneNode_parserStrings.type) != 0);
    result += (TextParser_addString(textParser, SpriteSceneNode_parserStrings.textureRes, sprite->textureResource->id) != 0);
    result += (TextParser_addInt(textParser, SpriteSceneNode_parserStrings.frameSize, sprite->frameSize.x) != 0);
    result += (TextParser_addInt(textParser, SpriteSceneNode_parserStrings.frameSize, sprite->frameSize.y) != 0);
    result += (TextParser_addInt(textParser, SpriteSceneNode_parserStrings.virtualSize, sprite->virtualSize.x) != 0);
    result += (TextParser_addInt(textParser, SpriteSceneNode_parserStrings.virtualSize, sprite->virtualSize.y) != 0);
    for (size_t i = 0; i < sprite->animationsCount; i++) {
        result += (TextParser_addInt(textParser, SpriteSceneNode_parserStrings.frameCountsList,
                                    sprite->animations[i].framesCount) != 0);
        result += (TextParser_addInt(textParser, SpriteSceneNode_parserStrings.frameDurationsList,
                                    sprite->animations[i].frameDuration) != 0);
    }
    char* tempString = TextParser_convertToText(textParser);
    result += (textParser->lastError != 0);
    result += (TextResource_updateContent(sprite->dynamicSceneNode.sceneNode.sceneNodeTextResource, tempString) != 0);
    result += (ResourceManager_saveTextResource(resourceManager, sprite->dynamicSceneNode.sceneNode.sceneNodeTextResource,
                                                spriteResId) != 0);
    TextParser_destruct(textParser);
    if (tempString)
        free(tempString);
    if (result)
        return SPRITE_ERR_SAVING;
    return SPRITE_NO_ERRORS;
}

void Sprite_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct Sprite* sprite = (struct Sprite*)sceneNode;
    sprite->srcRect.x = sprite->currentFrame * sprite->frameSize.x;
    sprite->srcRect.y = sprite->currentAnimation * sprite->frameSize.y;
    sprite->srcRect.w = sprite->frameSize.x * sprite->percentsToRender.x / 100;
    sprite->srcRect.h = sprite->frameSize.y * sprite->percentsToRender.y / 100;
    SDL_Point coordinates = Renderer_convertCoordinates(renderer, sceneNode->coordinates, sceneNode->parallax);
    sprite->dstRect.x = coordinates.x;
    sprite->dstRect.y = coordinates.y;
    SDL_Point size = Renderer_convertCoordinatesA(renderer, sprite->virtualSize);
    sprite->dstRect.w = size.x * sprite->dynamicSceneNode.scaleX * sprite->percentsToRender.x / 100;
    sprite->dstRect.h = size.y * sprite->dynamicSceneNode.scaleY * sprite->percentsToRender.y / 100;
}

void Sprite_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct Sprite* sprite = (struct Sprite*)sceneNode;
    if (sprite->renderingsCounter > sprite->animations[sprite->currentAnimation].frameDuration) {
        sprite->currentFrame++;
        sprite->renderingsCounter = 0;
    }
    if (sprite->currentFrame > sprite->animations[sprite->currentAnimation].framesCount - 1)
        sprite->currentFrame = 0;
    SDL_Point convertedRotatePoint = Renderer_convertCoordinatesA(renderer, sprite->dynamicSceneNode.rotatePointCoordinates);
    SDL_RenderCopyEx(renderer->renderer, sprite->textureResource->texture, &sprite->srcRect, &sprite->dstRect,
                     sprite->dynamicSceneNode.angle, &convertedRotatePoint, sprite->dynamicSceneNode.flip);
    sprite->renderingsCounter++;
}

enum SpriteSceneNode_errors Sprite_changeTextureResource(struct Sprite* sprite,
                                                         struct ResourceManager* resourceManager,
                                                         struct Renderer* renderer,
                                                         const char* const textureResId) {
    if (!sprite || !resourceManager || !renderer || !textureResId)
        return SPRITE_ERR_NULL_ARGUMENT;
    struct TextureResource* textureResource = NULL;
    textureResource = ResourceManager_loadTextureResource(resourceManager, renderer, textureResId);
    if (!textureResource)
        return SPRITE_ERR_CONSTRUCTING_TEXTURE_RES;
    int textureW;
    int textureH;
    size_t maxFramesCount = 0;
    if (SDL_QueryTexture(textureResource->texture, NULL, NULL, &textureW, &textureH)) {
        TextureResource_decreasePointersCounter(textureResource);
        return SPRITE_ERR_GETTING_TEXTURE_SIZE;
    }
    for (size_t i = 0; i < sprite->animationsCount; i++)
        if (sprite->animations[i].framesCount > maxFramesCount)
            maxFramesCount = sprite->animations[i].framesCount;
    if (sprite->frameSize.x * maxFramesCount > (size_t)textureW) {
        Logger_log(resourceManager->logger, SpriteSceneNode_errorMessages.errFramesNotFitH);
        TextureResource_decreasePointersCounter(textureResource);
        return SPRITE_ERR_FRAME_SIZE_NOT_FIT_H;
    }
    if (sprite->frameSize.y * sprite->animationsCount > (size_t)textureH) {
        Logger_log(resourceManager->logger, SpriteSceneNode_errorMessages.errFramesNotFitV);
        TextureResource_decreasePointersCounter(textureResource);
        return SPRITE_ERR_FRAME_SIZE_NOT_FIT_V;
    }
    TextureResource_decreasePointersCounter(sprite->textureResource);
    sprite->textureResource = textureResource;
    return SPRITE_NO_ERRORS;
}
