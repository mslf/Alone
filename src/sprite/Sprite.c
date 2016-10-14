//
// Created by mslf on 8/11/16.
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
#include <textParser/TextParser.h>
#include "sprite/Sprite.h"

const char* const SPRITE_SCENENODE_WARN_VIRTUAL_SIZE_NOT_FOUND =
        "Sprite_initAnimations: virtual size of sprite haven't found. Using present texture size for virtual size.";
const char* const SPRITE_SCENENODE_ERR_TEXTURE_RES_NOT_FOUND =
        "Sprite_tryGetSettingsFromTextParser: TextureResource constructing failed! Trying default.";
const char* const SPRITE_SCENENODE_ERR_DEFAULT_TEXTURE_RES_NOT_FOUND =
        "Sprite_tryGetSettingsFromTextParser: default TextureResource constructing failed!";
const char* const SPRITE_SCENENODE_ERR_ANIMATIONS_ALLOC =
        "Sprite_initAnimations: allocating memory for animations failed!";
const char* const SPRITE_SCENENODE_WARN_ANIMATIONS_COUNT =
        "Sprite_initAnimations: animations count haven't properly defined or ignored. Using default.";
const char* const SPRITE_SCENENODE_WARN_FRAMES_COUNT =
        "Sprite_initAnimations: framesCount haven't properly defined or ignored. Using default.";
const char* const SPRITE_SCENENODE_WARN_ONE_FRAME_DURATION =
        "Sprite_initAnimations: oneFrameDuration haven't properly defined or ignored. Using default.";
const char* const SPRITE_SCENENODE_WARN_FRAMES_NOT_FIT_W =
        "Sprite_initAnimations: frame size is not fit in texture size horizontally.";
const char* const SPRITE_SCENENODE_WARN_FRAMES_NOT_FIT_W_DEFAULT =
        "Sprite_initAnimations: using present frame width.";
const char* const SPRITE_SCENENODE_ERR_FRAMES_NOT_FIT_W =
        "Sprite_initAnimations: divided frame size is not fit in texture size horizontally.";
const char* const SPRITE_SCENENODE_WARN_FRAMES_NOT_FIT_H =
        "Sprite_initAnimations: frame size is not fit in texture size vertically.";
const char* const SPRITE_SCENENODE_WARN_FRAMES_NOT_FIT_H_DEFAULT =
        "Sprite_initAnimations: using present frame width.";
const char* const SPRITE_SCENENODE_ERR_FRAMES_NOT_FIT_H =
        "Sprite_initAnimations: divided frame size is not fit in texture size vertically.";

unsigned char Sprite_initFrameSize(struct Sprite* sprite, struct ResourceManager* resourceManager,
                                   struct TextParser* textParser, unsigned char usePresentForVirtual,
                                   unsigned char useDefaultTexture, size_t maxFramesCount) {
    int textureW;
    int textureH;
    if (SDL_QueryTexture(sprite->textureResource->texture, NULL, NULL, &textureW, &textureH))
        return 1;
    sprite->frameSize.x = (int)TextParser_getInt(textParser, SPRITE_SCENENODE_PARSER_FRAME_WIDTH, 0);
    if (sprite->frameSize.x <= 0 || sprite->frameSize.x * maxFramesCount > (size_t)textureW) {
        Logger_log(resourceManager->logger, SPRITE_SCENENODE_WARN_FRAMES_NOT_FIT_W);
        if (useDefaultTexture || maxFramesCount == 1) {
            Logger_log(resourceManager->logger, SPRITE_SCENENODE_WARN_FRAMES_NOT_FIT_W_DEFAULT);
            sprite->frameSize.x = textureW;
        }
        else {
            sprite->frameSize.x /= SPRITE_SCENENODE_DEFAULT_FRAME_WIDTH_DIVIDER;
            if (sprite->frameSize.x <= 0 || sprite->frameSize.x * maxFramesCount > (size_t)textureW) {
                Logger_log(resourceManager->logger, SPRITE_SCENENODE_ERR_FRAMES_NOT_FIT_W);
                return 2;
            }
        }
    }
    sprite->frameSize.y = (int)TextParser_getInt(textParser, SPRITE_SCENENODE_PARSER_FRAME_HEIGHT, 0);
    if (sprite->frameSize.y <= 0 || sprite->frameSize.y * sprite->animationsCount > (size_t)textureH) {
        Logger_log(resourceManager->logger, SPRITE_SCENENODE_WARN_FRAMES_NOT_FIT_H);
        if (useDefaultTexture || sprite->animationsCount == 1) {
            Logger_log(resourceManager->logger, SPRITE_SCENENODE_WARN_FRAMES_NOT_FIT_H_DEFAULT);
            sprite->frameSize.y = textureH;
        }
        else {
            sprite->frameSize.y /= SPRITE_SCENENODE_DEFAULT_FRAME_HEIGHT_DIVIDER;
            if (sprite->frameSize.y <= 0 || sprite->frameSize.y * sprite->animationsCount > (size_t)textureH) {
                Logger_log(resourceManager->logger, SPRITE_SCENENODE_ERR_FRAMES_NOT_FIT_W);
                return 3;
            }
        }
    }
    if (usePresentForVirtual) {
        sprite->virtualSize.x = textureW;
        sprite->virtualSize.y = textureH;
    }
    return 0;
}

unsigned char Sprite_initAnimations(struct Sprite* sprite, struct ResourceManager* resourceManager,
                                    struct Renderer* renderer, struct TextParser* textParser,
                                    unsigned char usePresentForVirtual, unsigned char useDefaultTexture) {
    size_t i = 0;
    sprite->animationsCount = (size_t)TextParser_getItemsCount(textParser, SPRITE_SCENENODE_PARSER_FRAMES_COUNT);
    if (sprite->animationsCount == 0 || useDefaultTexture) {
        Logger_log(resourceManager->logger, SPRITE_SCENENODE_WARN_ANIMATIONS_COUNT);
        sprite->animationsCount = SPRITE_SCENENODE_DEFAULT_ANIMATIONS_COUNT;
    }
    sprite->animations = (struct Animation*)calloc(sprite->animationsCount, sizeof(struct Animation));
    if (!sprite->animations) {
        Logger_log(resourceManager->logger, SPRITE_SCENENODE_ERR_ANIMATIONS_ALLOC);
        return 1;
    }
    size_t maxFramesCount = 0;
    for (i = 0; i < sprite->animationsCount; i++) {
        sprite->animations[i].framesCount = (size_t)TextParser_getInt(textParser, SPRITE_SCENENODE_PARSER_FRAMES_COUNT, i);
        if (sprite->animations[i].framesCount == 0 || useDefaultTexture) {
            Logger_log(resourceManager->logger, SPRITE_SCENENODE_WARN_FRAMES_COUNT);
            sprite->animations[i].framesCount = SPRITE_SCENENODE_DEFAULT_FRAMES_COUNT;
        }
        if (sprite->animations[i].framesCount > maxFramesCount)
            maxFramesCount = sprite->animations[i].framesCount;
        sprite->animations[i].oneFrameDuration = (size_t)TextParser_getInt(textParser,
                                                                           SPRITE_SCENENODE_PARSER_ONE_FRAME_DURATION, i);
        if (useDefaultTexture || textParser->lastError) {
            Logger_log(resourceManager->logger, SPRITE_SCENENODE_WARN_ONE_FRAME_DURATION);
            sprite->animations[i].oneFrameDuration = SPRITE_SCENENODE_DEFAULT_ONE_FRAME_DURATION;
        }
    }
    unsigned char result = Sprite_initFrameSize(sprite, resourceManager, textParser, usePresentForVirtual,
                                                useDefaultTexture, maxFramesCount);
    if (result)
        return result + 1;
    return 0;
}

unsigned char Sprite_tryGetSettingsFromTextParser(struct Sprite* sprite, struct ResourceManager* resourceManager,
                                                  struct Renderer* renderer, struct TextParser* textParser) {
    unsigned char useDefaultTexture = 0;
    unsigned char usePresentForVirtual = 0;
    sprite->virtualSize.x = (int)TextParser_getInt(textParser, SPRITE_SCENENODE_PARSER_VIRTUAL_WIDTH, 0);
    sprite->virtualSize.y = (int)TextParser_getInt(textParser, SPRITE_SCENENODE_PARSER_VIRTUAL_HEIGHT, 0);
    if (sprite->virtualSize.x <= 0 || sprite->virtualSize.y <= 0) {
        Logger_log(resourceManager->logger, SPRITE_SCENENODE_WARN_VIRTUAL_SIZE_NOT_FOUND);
        usePresentForVirtual = 1;
    }
    char* tempTextureResourceId = TextParser_getString(textParser, SPRITE_SCENENODE_PARSER_TEXTURE_RESOURCE, 0);
    sprite->textureResource = ResourceManager_loadTextureResource(resourceManager, renderer, tempTextureResourceId);
    if (!sprite->textureResource) {
        Logger_log(resourceManager->logger, SPRITE_SCENENODE_ERR_TEXTURE_RES_NOT_FOUND);
        useDefaultTexture = 1;
        if (!(sprite->textureResource = ResourceManager_loadTextureResource(resourceManager, renderer,
                                                                            SPRITE_SCENENODE_DEFAULT_TEXTURE))) {
            Logger_log(resourceManager->logger, SPRITE_SCENENODE_ERR_DEFAULT_TEXTURE_RES_NOT_FOUND);
            return 1;
        }

    }
    unsigned char result = Sprite_initAnimations(sprite, resourceManager, renderer, textParser,
                                                 usePresentForVirtual, useDefaultTexture);
    if (result)
        return result + 1;
    return 0;
}

struct Sprite* Sprite_construct(struct ResourceManager* const resourceManager, struct Renderer* renderer,
                                const char* const spriteResId) {
    struct Sprite* sprite = NULL;
    sprite = (struct Sprite*)calloc(1, sizeof(struct Sprite));
    if (!sprite)
        return NULL;
    SceneNode_init(&(sprite->sceneNode));
    sprite->sceneNode.sceneNodeTextResource = ResourceManager_loadTextResource(resourceManager, spriteResId, 0);
    if (!sprite->sceneNode.sceneNodeTextResource) {
        Sprite_destruct(sprite);
        return NULL;
    }
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructFromTextResource(resourceManager->logger,
                                                      sprite->sceneNode.sceneNodeTextResource);
    if (!textParser) {
        Sprite_destruct(sprite);
        return NULL;
    }
    if (Sprite_tryGetSettingsFromTextParser(sprite, resourceManager, renderer, textParser)) {
        TextParser_destruct(textParser);
        Sprite_destruct(sprite);
        return NULL;
    }
    TextParser_destruct(textParser);
    sprite->sceneNode.update = Sprite_update;
    sprite->sceneNode.render = Sprite_render;
    sprite->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(SPRITE_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!sprite->sceneNode.type) {
        Sprite_destruct(sprite);
        return NULL;
    }
    strcpy(sprite->sceneNode.type, SPRITE_SCENENODE_PARSER_TYPE_STRING);
    return sprite;
}

void Sprite_destruct(struct Sprite* sprite) {
    if (!sprite)
        return;
    if (sprite->animations)
        free(sprite->animations);
    if (sprite->textureResource)
        sprite->textureResource->pointersCount--;
    if (sprite->sceneNode.sceneNodeTextResource)
        sprite->sceneNode.sceneNodeTextResource->pointersCount--;
    if (sprite->sceneNode.type)
        free(sprite->sceneNode.type);
    free(sprite);
}

void Sprite_save(
        const struct Sprite* const sprite, struct ResourceManager* const resourceManager,
        const char* const spriteResId) {

}

void Sprite_update(struct SceneNode* sceneNode, struct EventManager* eventManager) {

}

void Sprite_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct Sprite* sprite = (struct Sprite*)sceneNode;
    if (sprite->renderingsCounter > sprite->animations[sprite->currentAnimation].oneFrameDuration) {
        sprite->currentFrame++;
        sprite->renderingsCounter = 0;
    }
    if (sprite->currentFrame > sprite->animations[sprite->currentAnimation].framesCount - 1)
        sprite->currentFrame = 0;
    sprite->srcRect.x = sprite->currentFrame * sprite->frameSize.x;
    sprite->srcRect.y = sprite->currentAnimation * sprite->frameSize.y;
    sprite->srcRect.w = sprite->frameSize.x;
    sprite->srcRect.h = sprite->frameSize.y;
    SDL_Point coordinates = Renderer_convertCoordinates(renderer, sprite->sceneNode.coordinates);
    sprite->dstRect.x = coordinates.x;
    sprite->dstRect.y = coordinates.y;
    SDL_Point size = Renderer_convertCoordinates(renderer, sprite->virtualSize);
    sprite->dstRect.w = size.x * sceneNode->scaleX;
    sprite->dstRect.h = size.y * sceneNode->scaleY;
    SDL_RenderCopyEx(renderer->renderer, sprite->textureResource->texture, &sprite->srcRect, &sprite->dstRect,
                     sprite->sceneNode.angle, &sprite->sceneNode.rotatePointCoordinates, sprite->sceneNode.flip);
    sprite->renderingsCounter++;
}