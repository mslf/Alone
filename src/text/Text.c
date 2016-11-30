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
 * @file Text.c
 * @author mslf
 * @date 11 Aug 2016
 * @brief File containing implementation of #Text.
 */
#include <stdbool.h>
#include "text/Text.h"
#include "textParser/TextParser.h"

/**
 * @brief Error message strings for #Text.
 */
static const struct TextSceneNode_errorMessages {
    const char* const errNoFontPath;
    /**< Will be displayed when #TextParser have no TextSceneNode_parserStrings#fontPath. */
    const char* const errNoText;
    /**< Will be displayed when #TextParser have no TextSceneNode_parserStrings#text. */
    const char* const errNoSize;
    /**< Will be displayed when #TextParser have no TextSceneNode_parserStrings#size or
     * it doesn't contain suitable number. */
    const char* const errNoColor;
    /**< Will be displayed when #TextParser have no TextSceneNode_parserStrings#color or
     * it doesn't contain suitable number. */
}TextSceneNode_errorMessages = {
    "Text_tryGetSettingsFromTextParser: fontPath string haven't found! Using default.",
    "Text_tryGetSettingsFromTextParser: text string haven't found! Using default.",
    "Text_tryGetSettingsFromTextParser: size haven't found! Using default.",
    "Text_tryGetSettingsFromTextParser: color haven't found! Using default."};

/**
 * @brief Loads #Text settings from #TextParser and calls Text_regenerateTexture() with them.
 * @param text Pointer to a #Text, which will be initialized. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager for passing to Text_regenerateTexture(). Can be NULL.
 * @param renderer Pointer to a #Renderer for passing to Text_regenerateTexture(). Can be NULL.
 * @param textParser Pointer to a #TextParser with data strings for initializing #Text. Can be NULL.
 * @param logFlag Poinnter to a bool flag. Will be set if sobe log happened in this function.
 * @return TextSceneNode_errors value.
 * @see #TextSceneNode_errors
 * @see #TextSceneNode_parserStrings
 * @see Text_regenerateTexture()
 * @see #TextParser
 */
static enum TextSceneNode_errors Text_tryGetSettingsFromTextParser(struct Text* text,
                                                                   struct ResourceManager* const resourceManager,
                                                                   struct Renderer* const renderer,
                                                                   struct TextParser* textParser,
                                                                   bool* logFlag) {
    if (!text || !resourceManager || !renderer || !textParser || !logFlag)
        return TEXT_ERR_NULL_ARGUMENT;
    const char* tempFontPath = TextParser_getString(textParser, TextSceneNode_parserStrings.fontPath, 0);
    if (!tempFontPath && ++(*logFlag)) {
        Logger_log(renderer->logger, TextSceneNode_errorMessages.errNoFontPath);
        tempFontPath = TextSceneNode_default.fontPath;
    }
    const char* tempTextString = TextParser_getString(textParser, TextSceneNode_parserStrings.text, 0);
    if (!tempTextString && ++(*logFlag)) {
        Logger_log(renderer->logger, TextSceneNode_errorMessages.errNoText);
        tempTextString = TextSceneNode_default.text;
    }
    size_t tempSize = (int)TextParser_getInt(textParser, TextSceneNode_parserStrings.size, 0);
    if (tempSize <= 0 && ++(*logFlag)) {
        Logger_log(renderer->logger, TextSceneNode_errorMessages.errNoSize);
        tempSize = TextSceneNode_default.size;
    }
    SDL_Color tempColor;
    tempColor.r = (Uint8)TextParser_getInt(textParser, TextSceneNode_parserStrings.color, 0);
    if (textParser->lastError && ++(*logFlag)) {
        Logger_log(renderer->logger, TextSceneNode_errorMessages.errNoColor);
        tempColor = TextSceneNode_default.color;
    }
    tempColor.g = (Uint8)TextParser_getInt(textParser, TextSceneNode_parserStrings.color, 1);
    if (textParser->lastError && ++(*logFlag))
        tempColor.g = TextSceneNode_default.color.g;
    tempColor.b = (Uint8)TextParser_getInt(textParser, TextSceneNode_parserStrings.color, 2);
    if (textParser->lastError && ++(*logFlag))
        tempColor.b = TextSceneNode_default.color.b;
    tempColor.a = (Uint8)TextParser_getInt(textParser, TextSceneNode_parserStrings.color, 3);
    if (textParser->lastError && ++(*logFlag))
        tempColor.a = TextSceneNode_default.color.a;
    return Text_regenerateTexture(text, resourceManager, renderer, tempTextString, tempFontPath, tempSize, tempColor);
}

struct SceneNode* Text_construct(struct ResourceManager* const resourceManager,
                                                 struct Renderer* const renderer,
                                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                 struct TextParser* const textParser) {
    // Text doesn't need sceneNodeTypesRegistrar, so we ignore it
    if (!resourceManager || !renderer || !textParser)
        return NULL;
    struct Text* text = NULL;
    text = (struct Text*)calloc(1, sizeof(struct Text));
    if (!text)
        return NULL;
    (*text) = TextSceneNode_default;
    // We don't want to free static const later, yeah?
    text->fontPath = NULL;
    text->text = NULL;
    SceneNode_initDynamic(&(text->dynamicSceneNode));
    bool logFlag = 0;
    if (Text_tryGetSettingsFromTextParser(text, resourceManager, renderer, textParser, &logFlag)) {
        Text_destruct((struct SceneNode*)text);
        return NULL;
    }
    if (logFlag) {
        /*char tempString[600];
        sprintf(tempString, "\tin ResourceID: %s", textResId);*/
        // FIXME: correct error message displaying
        Logger_log(resourceManager->logger, "NOT_ENOUGH_DATA_TO_DISPLAY_ERROR_MESSAGE");
    }
    text->dynamicSceneNode.sceneNode.update = Text_update;
    text->dynamicSceneNode.sceneNode.render = Text_render;
    text->dynamicSceneNode.sceneNode.destruct = Text_destruct;
    text->dynamicSceneNode.sceneNode.type = (char*)malloc(sizeof(char) * (strlen(TextSceneNode_parserStrings.type) + 1));
    if (!text->dynamicSceneNode.sceneNode.type) {
        Text_destruct((struct SceneNode*)text);
        return NULL;
    }
    strcpy(text->dynamicSceneNode.sceneNode.type, TextSceneNode_parserStrings.type);
    return (struct SceneNode*)text;
}

void Text_destruct(struct SceneNode* text) {
    if (!text)
        return;
    struct Text* tempText = (struct Text*)text;
    if (tempText->fontPath)
        free(tempText->fontPath);
    if (tempText->text)
        free(tempText->text);
    TextureResource_decreasePointersCounter(tempText->textureResource);
    TextResource_decreasePointersCounter(text->sceneNodeTextResource);
    if (text->type)
        free(text->type);
    free(text);
}

/**
 * @brief Updates Text#fontPath, Text#text, Text#size and Text#color with new values.
 * @param text Poinnter to a #Text, where fields will be changed. Can be NULL.
 * @param textString String with text rendered in Text#textureResource. Can be NULL.
 * @param fontPath String with path to the ttf font. Can be NULL.
 * @param size Vertical size of text in Upixels, which rendered in Text#textureResource.
 * @param color RGBA color of text, which rendered in Text#textureResource.
 * @return #TextSceneNode_errors value.
 * @see #TextSceneNode_errors
 */
static enum TextSceneNode_errors Text_updateFields(struct Text* text,
                                                    const char* const textString,
                                                    const char* const fontPath,
                                                    int size,
                                                    SDL_Color color) {
    if (!text || !textString || !fontPath || size <= 0)
        return TEXT_ERR_NULL_ARGUMENT;
    char* tempTextString = NULL;
    tempTextString = (char*)malloc(sizeof(char) * (strlen(textString) + 1));
    if (!tempTextString)
        return TEXT_ERR_STRING_ALLOC;
    char* tempFontPath = NULL;
    tempFontPath = (char*)malloc(sizeof(char) * (strlen(fontPath) + 1));
    if (!tempFontPath) {
        free(tempTextString);
        return TEXT_ERR_STRING_ALLOC;
    }
    /*
     * Order of strcpy and free matters!
     */
    strcpy(tempTextString, textString);
    free(text->text);
    text->text = tempTextString;
    strcpy(tempFontPath, fontPath);
    free(text->fontPath); 
    text->fontPath = tempFontPath;
    text->size = size;
    text->color = color;
    return TEXT_NO_ERRORS;
}

enum TextSceneNode_errors Text_regenerateTexture(struct Text* text,
                                                 struct ResourceManager* resourceManager,
                                                 struct Renderer* renderer,
                                                 const char* const textString,
                                                 const char* const fontPath,
                                                 int size,
                                                 SDL_Color color) {
    // Don't check resourceManager to NULL, because it's okey
    if (!text || !renderer || !textString || !fontPath || size <= 0)
        return TEXT_ERR_NULL_ARGUMENT;
    struct TextureResource* textureResource = ResourceManager_loadTextureResourceFromText(resourceManager, renderer, 
                                                                                          textString, fontPath, size, color);
    if (!textureResource)
        return TEXT_ERR_GENERATING_TEXTURE;
    int textureW;
    int textureH;
    if (SDL_QueryTexture(textureResource->texture, NULL, NULL, &textureW, &textureH)) {
        TextureResource_decreasePointersCounter(textureResource);
        return TEXT_ERR_GETTING_TEXTURE_SIZE;
    }
    if (Text_updateFields(text, textString, fontPath, size, color)) {
        TextureResource_decreasePointersCounter(textureResource);
        return TEXT_ERR_STRING_ALLOC;
    }
    if (text->textureResource) {
        TextureResource_decreasePointersCounter(text->textureResource);
        // See WARNING in 'ResourceManager_loadTextureResourceFromText'
        if (!text->textureResource->isCreatedWithResourceManager)
            TextureResource_destruct(text->textureResource);
    }
    text->textureResource = textureResource;
    text->srcRect.w = textureW;
    text->srcRect.h = textureH;
    return TEXT_NO_ERRORS;
}

enum TextSceneNode_errors Text_save(const struct  Text* const text,
                                    struct ResourceManager* const resourceManager,
                                    const char* const textResId) {
     if (!text || !resourceManager || !textResId)
        return TEXT_ERR_NULL_ARGUMENT;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return TEXT_ERR_CONSTRUCTIG_TEXT_PARSER;
    unsigned char result = 0;
    result += (TextParser_addString(textParser, TextParser_standartTypeString, TextSceneNode_parserStrings.type) != 0);
    result += (TextParser_addString(textParser, TextSceneNode_parserStrings.fontPath, text->fontPath) != 0);
    result += (TextParser_addString(textParser, TextSceneNode_parserStrings.text, text->text) != 0);
    result += (TextParser_addInt(textParser, TextSceneNode_parserStrings.size, (size_t)text->size) != 0);
    result += (TextParser_addInt(textParser, TextSceneNode_parserStrings.color, (size_t)text->color.r) != 0);
    result += (TextParser_addInt(textParser, TextSceneNode_parserStrings.color, (size_t)text->color.g) != 0);
    result += (TextParser_addInt(textParser, TextSceneNode_parserStrings.color, (size_t)text->color.b) != 0);
    result += (TextParser_addInt(textParser, TextSceneNode_parserStrings.color, (size_t)text->color.a) != 0);
    char* newText = NULL;
    newText = TextParser_convertToText(textParser);
    result += (textParser->lastError != 0);
    result += (TextResource_updateContent(text->dynamicSceneNode.sceneNode.sceneNodeTextResource, newText) != 0);
    result += (ResourceManager_saveTextResource(resourceManager, text->dynamicSceneNode.sceneNode.sceneNodeTextResource,
                                                textResId) != 0);
    TextParser_destruct(textParser);
    if (newText)
        free(newText);
    if (result)
        return TEXT_ERR_SAVING;
    return TEXT_NO_ERRORS;
}

void Text_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct Text* text = (struct Text*)sceneNode;
    SDL_Point coordinates = Renderer_convertCoordinates(renderer, text->dynamicSceneNode.sceneNode.coordinates);
    text->dstRect.x = coordinates.x;
    text->dstRect.y = coordinates.y;
    SDL_Point size;
    size.x = text->srcRect.w;
    size.y = text->srcRect.h;
    SDL_Point convertedSize = Renderer_convertCoordinatesA(renderer, size);
    text->dstRect.w = convertedSize.x * text->dynamicSceneNode.scaleX;
    text->dstRect.h = convertedSize.y * text->dynamicSceneNode.scaleY;
}

void Text_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct Text* text = (struct Text*)sceneNode;
    SDL_RenderCopyEx(renderer->renderer, text->textureResource->texture, &text->srcRect, &text->dstRect,
                     text->dynamicSceneNode.angle, &text->dynamicSceneNode.rotatePointCoordinates, text->dynamicSceneNode.flip);
}
