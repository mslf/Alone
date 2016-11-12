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
#include "text/Text.h"
#include "textParser/TextParser.h"

const char* const TEXT_SCENENODE_ERR_NO_FONT_PATH =
        "Text_tryGetSettingsFromTextParser: fontPath string haven't found! Using default.";
const char* const TEXT_SCENENODE_ERR_NO_TEXT =
        "Text_tryGetSettingsFromTextParser: text string haven't found! Using default.";
const char* const TEXT_SCENENODE_ERR_NO_SIZE =
        "Text_tryGetSettingsFromTextParser: size haven't found! Using default.";
const char* const TEXT_SCENENODE_ERR_NO_COLOR_R =
        "Text_tryGetSettingsFromTextParser: color[0] haven't found! Using default.";
const char* const TEXT_SCENENODE_ERR_NO_COLOR_G =
        "Text_tryGetSettingsFromTextParser: color[1] haven't found! Using default.";
const char* const TEXT_SCENENODE_ERR_NO_COLOR_B =
        "Text_tryGetSettingsFromTextParser: color[2] haven't found! Using default.";
const char* const TEXT_SCENENODE_ERR_NO_COLOR_A =
        "Text_tryGetSettingsFromTextParser: color[3] haven't found! Using default.";

//? Unsigned char again ffs 
//? Also, try to refactor
static unsigned char Text_tryGetSettingsFromTextParser(struct Text* text, struct ResourceManager* resourceManager,
                                                struct TextParser* textParser, unsigned char* logFlag) {
    char* tempFontPath = TextParser_getString(textParser, TEXT_SCENENODE_PARSER_FONT_PATH, 0);
    if (!tempFontPath) {
        Logger_log(resourceManager->logger, TEXT_SCENENODE_ERR_NO_FONT_PATH);
        tempFontPath = TEXT_SCENENODE_DEFAULT_FONT_PATH;
        (*logFlag) = 1;
    }
    text->fontPath = (char*)malloc(sizeof(char) * (strlen(tempFontPath) + 1));
    if (!text->fontPath)
        return 2;
    strcpy(text->fontPath, tempFontPath);
    char* tempTextString = TextParser_getString(textParser, TEXT_SCENENODE_PARSER_TEXT, 0);
    if (!tempTextString) {
        Logger_log(resourceManager->logger, TEXT_SCENENODE_ERR_NO_TEXT);
        tempTextString = TEXT_SCENENODE_DEFAULT_TEXT;
        (*logFlag) = 1;
    }
    text->text = (char*)malloc(sizeof(char) * (strlen(tempTextString) + 1));
    if (!text->text)
        return 4;
    strcpy(text->text, tempTextString);
    text->size = (int)TextParser_getInt(textParser, TEXT_SCENENODE_PARSER_SIZE, 0);
    if (text->size <= 0) {
        Logger_log(resourceManager->logger, TEXT_SCENENODE_ERR_NO_SIZE);
        text->size = TEXT_SCENENODE_DEFAULT_SIZE;
        (*logFlag) = 1;
    }
    text->color.r = (Uint8)TextParser_getInt(textParser, TEXT_SCENENODE_PARSER_COLOR, 0);
    if (textParser->lastError) {
        Logger_log(resourceManager->logger, TEXT_SCENENODE_ERR_NO_COLOR_R);
        text->color.r = TEXT_SCENENODE_DEFAULT_COLOR_R;
        (*logFlag) = 1;
    }
    text->color.g = (Uint8)TextParser_getInt(textParser, TEXT_SCENENODE_PARSER_COLOR, 1);
    if (textParser->lastError) {
        Logger_log(resourceManager->logger, TEXT_SCENENODE_ERR_NO_COLOR_G);
        text->color.g = TEXT_SCENENODE_DEFAULT_COLOR_G;
        (*logFlag) = 1;
    }
    text->color.b = (Uint8)TextParser_getInt(textParser, TEXT_SCENENODE_PARSER_COLOR, 2);
    if (textParser->lastError) {
        Logger_log(resourceManager->logger, TEXT_SCENENODE_ERR_NO_COLOR_B);
        text->color.b = TEXT_SCENENODE_DEFAULT_COLOR_B;
        (*logFlag) = 1;
    }
    text->color.a = (Uint8)TextParser_getInt(textParser, TEXT_SCENENODE_PARSER_COLOR, 3);
    if (textParser->lastError) {
        Logger_log(resourceManager->logger, TEXT_SCENENODE_ERR_NO_COLOR_A);
        text->color.a = TEXT_SCENENODE_DEFAULT_COLOR_A;
        (*logFlag) = 1;
    }
    return 0;
}

static unsigned char Text_generateTexture(struct Text* text, struct ResourceManager* resourceManager,
                                   struct Renderer* renderer) {
    text->textureResource = ResourceManager_loadTextureResourceFromText(resourceManager, renderer, text->text,
                                                                        text->fontPath, text->size,
                                                                        text->color);
    if (!text->textureResource)
        return 1;
    int textureW;
    int textureH;
    if (SDL_QueryTexture(text->textureResource->texture, NULL, NULL, &textureW, &textureH))
        return 2;
    text->srcRect.w = textureW;
    text->srcRect.h = textureH;
    text->sceneNode.update = Text_update;
    text->sceneNode.render = Text_render;
    text->sceneNode.destruct = Text_destruct;
    text->sceneNode.type = (char*)malloc(sizeof(char) * (strlen(TEXT_SCENENODE_PARSER_TYPE_STRING) + 1));
    if (!text->sceneNode.type)
        return 3;
    return 0;
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
    SceneNode_init(&(text->sceneNode));
    unsigned char logFlag = 0;
    if (Text_tryGetSettingsFromTextParser(text, resourceManager, textParser, &logFlag)) {
        Text_destruct((struct SceneNode*)text);
        return NULL;
    }
    if (logFlag) {
        /*char tempString[600];
        sprintf(tempString, "\tin ResourceID: %s", textResId);*/
        // FIXME: correct error message displaying
        Logger_log(resourceManager->logger, "NOT_ENOUGH_DATA_TO_DISPLAY_ERROR_MESSAGE");
    }
    if (Text_generateTexture(text, resourceManager, renderer)) {
        Text_destruct((struct SceneNode*)text);
        return NULL;
    }
    strcpy(text->sceneNode.type, TEXT_SCENENODE_PARSER_TYPE_STRING);
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
    if (tempText->textureResource)
        tempText->textureResource->pointersCount--;
    if (text->sceneNodeTextResource)
        text->sceneNodeTextResource->pointersCount--;
    if (text->type)
        free(text->type);
    free(text);
}
//? try to refactor
unsigned char Text_regenerateTexture(struct Text* text, struct ResourceManager* resourceManager, struct Renderer* renderer,
                                     const char* const textString, const char* const fontPath, int size, SDL_Color color) {
    // Don't check resourceManager to NULL, because it's okey
    if (!text || !renderer || !textString || !fontPath || size <= 0)
        return 1;
    struct TextureResource* textureResource = ResourceManager_loadTextureResourceFromText(resourceManager, renderer, 
                                                                                          textString, fontPath, size, color);
    if (!textureResource)
        return 2;
    int textureW;
    int textureH;
    if (SDL_QueryTexture(textureResource->texture, NULL, NULL, &textureW, &textureH)) {
        textureResource->pointersCount--;
        return 3;
    }
    char* tempTextString = NULL;
    tempTextString = (char*)malloc(sizeof(char) * (strlen(textString) + 1));
    if (!tempTextString) {
        textureResource->pointersCount--;
        return 4;
    }
    char* tempFontPath = NULL;
    tempFontPath = (char*)malloc(sizeof(char) * (strlen(fontPath) + 1));
    if (!tempFontPath) {
        free(tempTextString);
        textureResource->pointersCount--;
        return 5;
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
    text->textureResource->pointersCount--;
    // See WARNING in 'ResourceManager_loadTextureResourceFromText'
    if (!text->textureResource->isCreatedWithResourceManager)
        TextureResource_destruct(text->textureResource);
    text->textureResource = textureResource;
    text->srcRect.w = textureW;
    text->srcRect.h = textureH;
    return 0;
}

//?unsigned char again, wtf
unsigned char Text_save(
        const struct  Text* const text, struct ResourceManager* const resourceManager,
        const char* const textResId) {
     if (!text || !resourceManager || !textResId)
        return 1;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return 2;
    unsigned char result = 0;
    result += TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING, TEXT_SCENENODE_PARSER_TYPE_STRING);
    result += TextParser_addString(textParser, TEXT_SCENENODE_PARSER_FONT_PATH, text->fontPath);
    result += TextParser_addString(textParser, TEXT_SCENENODE_PARSER_TEXT, text->text);
    result += TextParser_addInt(textParser, TEXT_SCENENODE_PARSER_SIZE, (size_t)text->size);
    result += TextParser_addInt(textParser, TEXT_SCENENODE_PARSER_COLOR, (size_t)text->color.r);
    result += TextParser_addInt(textParser, TEXT_SCENENODE_PARSER_COLOR, (size_t)text->color.g);
    result += TextParser_addInt(textParser, TEXT_SCENENODE_PARSER_COLOR, (size_t)text->color.b);
    result += TextParser_addInt(textParser, TEXT_SCENENODE_PARSER_COLOR, (size_t)text->color.a);
    char* newText = NULL;
    newText = TextParser_convertToText(textParser);
    result += textParser->lastError;
    result += TextResource_updateContent(text->sceneNode.sceneNodeTextResource, newText);
    result += ResourceManager_saveTextResource(resourceManager, text->sceneNode.sceneNodeTextResource, textResId);
    TextParser_destruct(textParser);
    if (newText)
        free(newText);
    return result;
}

void Text_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct Text* text = (struct Text*)sceneNode;
    SDL_Point coordinates = Renderer_convertCoordinates(renderer, text->sceneNode.coordinates);
    text->dstRect.x = coordinates.x;
    text->dstRect.y = coordinates.y;
    SDL_Point size;
    size.x = text->srcRect.w;
    size.y = text->srcRect.h;
    SDL_Point convertedSize = Renderer_convertCoordinates(renderer, size);
    text->dstRect.w = convertedSize.x * sceneNode->scaleX;
    text->dstRect.h = convertedSize.y * sceneNode->scaleY;
}

void Text_render(struct SceneNode* sceneNode, struct Renderer* renderer) {
    if (!sceneNode || !renderer)
        return;
    struct Text* text = (struct Text*)sceneNode;
    SDL_RenderCopyEx(renderer->renderer, text->textureResource->texture, &text->srcRect, &text->dstRect,
                     text->sceneNode.angle, &text->sceneNode.rotatePointCoordinates, text->sceneNode.flip);
}
