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
#include <resourceManager/ResourceManager.h>
#include "settings/Settings.h"
#include "textParser/TextParser.h"

const char* const SETTINGS_PARSER_ERR_NO_TYPE_STRING =
        "Settings_construct: TextParser have no type string!";
const char* const SETTINGS_PARSER_ERR_NOT_MY_TYPE_STRING =
        "Settings_construct: TextParser have no suitable type string!";
const char* const SETTINGS_ERR_TEXT_PARSER_CONSTRUCTING = 
        "Settings_construct: constructing TextParser failed!";
const char* const SETTINGS_ERR_DEFAULTS = "Settings: constructor: using defaults settings!";
const char* const SETTINGS_ERR_MAIN_SCENE_STRING_ALLOC = 
        "Settings_construct: allocating memory for mainScene string failed!";

struct Settings* Settings_defaults(struct Settings* settings) {
    if (settings) {
        settings->h = SETTINGS_DEFAULT_H;
        settings->w = SETTINGS_DEFAULT_W;
        settings->virtualH = SETTINGS_DEFAULT_H;
        settings->virtualW = SETTINGS_DEFAULT_W;
        settings->isFullscreen = SETTINGS_DEFAULT_FULLSCREEN;
        settings->isMusicActive = SETTINGS_DEFAULT_MUSIC;
        settings->isSoundActive = SETTINGS_DEFAULT_SOUND;
        settings->isVsyncActive = SETTINGS_DEFAULT_VSYNC;
        char* tempString = SETTINGS_DEFAULT_MAIN_SCENE;
        settings->mainScene = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
        if (!settings->mainScene) {
            Settings_destruct(settings);
            return NULL;
        }
        strcpy(settings->mainScene, tempString);
    }
    return settings;
}

static unsigned char Settings_checkTextResourceType(struct Settings* settings, struct TextParser* textParser,
                                             struct ResourceManager* resourceManager) {
    if (!settings || !textParser || !resourceManager)
        return 1;
    char* settingsTypeString = NULL;
    settingsTypeString = TextParser_getString(textParser, TEXT_PARSER_TYPE_STRING, 0);
    if (textParser->lastError) {
        Logger_log(resourceManager->logger, SETTINGS_PARSER_ERR_NO_TYPE_STRING);
        Logger_log(resourceManager->logger, SETTINGS_ERR_DEFAULTS);
        settings->settingsResource->pointersCount = 0;
        ResourceManager_destructNeedlessTextResources(resourceManager);
        return 2;
    }
    if (strcmp(settingsTypeString, SETTINGS_PARSER_TYPE_STRING) != 0) {
        Logger_log(resourceManager->logger, SETTINGS_PARSER_ERR_NOT_MY_TYPE_STRING);
        Logger_log(resourceManager->logger, SETTINGS_ERR_DEFAULTS);
        settings->settingsResource->pointersCount = 0;
        ResourceManager_destructNeedlessTextResources(resourceManager);
        return 3;
    }
    return 0;
}

static unsigned char Settings_tryGetSettingsFromTextParser(struct Settings* settings, struct TextParser* textParser) {
    if (!settings || !textParser)
        return 1;
    settings->h = (size_t)TextParser_getInt(textParser, SETTINGS_PARSER_SCREEN_HEIGHT, 0);
    if (textParser->lastError)
        settings->h = SETTINGS_DEFAULT_H;
    settings->w = (size_t)TextParser_getInt(textParser, SETTINGS_PARSER_SCREEN_WIDTH, 0);
    if (textParser->lastError)
        settings->w = SETTINGS_DEFAULT_W;
    settings->virtualH = (size_t)TextParser_getInt(textParser, SETTINGS_PARSER_VIRTUAL_SCREEN_HEIGHT, 0);
    if (textParser->lastError)
        settings->virtualH = SETTINGS_DEFAULT_H;
    settings->virtualW = (size_t)TextParser_getInt(textParser, SETTINGS_PARSER_VIRTUAL_SCREEN_WIDTH, 0);
    if (textParser->lastError)
        settings->virtualW = SETTINGS_DEFAULT_W;
    settings->isFullscreen = TextParser_getFlag(textParser, SETTINGS_PARSER_FULLSCREEN, 0);
    if (textParser->lastError)
        settings->isFullscreen = SETTINGS_DEFAULT_FULLSCREEN;
    settings->isMusicActive = TextParser_getFlag(textParser, SETTINGS_PARSER_MUSIC, 0);
    if (textParser->lastError)
        settings->isMusicActive = SETTINGS_DEFAULT_MUSIC;
    settings->isSoundActive = TextParser_getFlag(textParser, SETTINGS_PARSER_SOUND, 0);
    if (textParser->lastError)
        settings->isSoundActive = SETTINGS_DEFAULT_SOUND;
    settings->isVsyncActive = TextParser_getFlag(textParser, SETTINGS_PARSER_VSYNC, 0);
    if (textParser->lastError)
        settings->isVsyncActive = SETTINGS_DEFAULT_VSYNC;
    char* tempString = TextParser_getString(textParser, SETTINGS_PARSER_MAIN_SCENE, 0);
    if (textParser->lastError)
        tempString = SETTINGS_DEFAULT_MAIN_SCENE;
    settings->mainScene = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
    if (!settings->mainScene)
        return 2;
    strcpy(settings->mainScene, tempString);
    return 0;
}

struct Settings* Settings_construct(struct ResourceManager* resourceManager, const char* const settingsResId) {
    if (!resourceManager || !settingsResId)
        return NULL;
    struct Settings *settings = NULL;
    settings = (struct Settings *) calloc(1, sizeof(struct Settings));
    if (!settings)
        return NULL;
    settings->settingsResource = ResourceManager_loadTextResource(resourceManager, settingsResId, 0);
    if (!settings->settingsResource) {
        Logger_log(resourceManager->logger, SETTINGS_ERR_DEFAULTS);
        return Settings_defaults(settings);
    }
    struct TextParser *textParser = NULL;
    textParser = TextParser_constructFromTextResource(resourceManager->logger, settings->settingsResource);
    if (!textParser) {
        Logger_log(resourceManager->logger, SETTINGS_ERR_TEXT_PARSER_CONSTRUCTING);
        Logger_log(resourceManager->logger, SETTINGS_ERR_DEFAULTS);
        settings->settingsResource->pointersCount = 0;
        ResourceManager_destructNeedlessTextResources(resourceManager);
        return Settings_defaults(settings);
    }
    if (Settings_checkTextResourceType(settings, textParser, resourceManager)) {
        Logger_log(resourceManager->logger, SETTINGS_ERR_DEFAULTS);
        return Settings_defaults(settings);
    }
    if (Settings_tryGetSettingsFromTextParser(settings, textParser)) {
        Logger_log(resourceManager->logger, SETTINGS_ERR_MAIN_SCENE_STRING_ALLOC);
        Logger_log(resourceManager->logger, SETTINGS_ERR_DEFAULTS);
        return Settings_defaults(settings);
    }
    TextParser_destruct(textParser);
    return settings;
}

unsigned char Settings_updateMainSceneString(struct Settings* settings, const char* const mainScene) {
    if (!settings || !mainScene)
        return 1;
    char* newText = NULL;
    newText = (char*)malloc(sizeof(char) * (strlen(mainScene) + 1));
    if (!newText)
        return 2;
    strcpy(newText, mainScene);
    free(settings->mainScene);
    settings->mainScene = newText;
    return 0;
}

unsigned char Settings_save(struct Settings* settings, struct ResourceManager* resourceManager,
                            const char* const settingsResId) {
    if (!settings || !resourceManager || !settingsResId)
        return 1;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return 2;
    unsigned char result = 0;
    result += TextParser_addString(textParser, TEXT_PARSER_TYPE_STRING, SETTINGS_PARSER_TYPE_STRING);
    result += TextParser_addInt(textParser, SETTINGS_PARSER_SCREEN_HEIGHT, settings->h);
    result += TextParser_addInt(textParser, SETTINGS_PARSER_SCREEN_WIDTH, settings->w);
    result += TextParser_addInt(textParser, SETTINGS_PARSER_VIRTUAL_SCREEN_HEIGHT, settings->virtualH);
    result += TextParser_addInt(textParser, SETTINGS_PARSER_VIRTUAL_SCREEN_WIDTH, settings->virtualW);
    result += TextParser_addFlag(textParser, SETTINGS_PARSER_FULLSCREEN, settings->isFullscreen);
    result += TextParser_addFlag(textParser, SETTINGS_PARSER_MUSIC, settings->isMusicActive);
    result += TextParser_addFlag(textParser, SETTINGS_PARSER_SOUND, settings->isSoundActive);
    result += TextParser_addFlag(textParser, SETTINGS_PARSER_VSYNC, settings->isVsyncActive);
    result += TextParser_addString(textParser, SETTINGS_PARSER_MAIN_SCENE, settings->mainScene);
    char* newText = NULL;
    newText = TextParser_convertToText(textParser);
    result += textParser->lastError;
    result += TextResource_updateContent(settings->settingsResource, newText);
    result += ResourceManager_saveTextResource(resourceManager, settings->settingsResource, settingsResId);
    TextParser_destruct(textParser);
    if (newText)
        free(newText);
    return result;
}


void Settings_destruct(struct Settings* settings) {
    if (!settings)
        return;
    if (settings->mainScene)
        free(settings->mainScene);
    if (settings->settingsResource)
        settings->settingsResource->pointersCount--;
    free(settings);
}
