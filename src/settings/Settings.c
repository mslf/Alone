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

const char* const SETTINGS_PARSER_TYPE_STRING = "Settings";
const char* const SETTINGS_PARSER_SCREEN_HEIGHT = "screenHeight";
const char* const SETTINGS_PARSER_SCREEN_WIDTH = "screenWidth";
const char* const SETTINGS_PARSER_VIRTUAL_SCREEN_HEIGHT = "virtualScreenHeight";
const char* const SETTINGS_PARSER_VIRTUAL_SCREEN_WIDTH = "virtualScreenWidth";
const char* const SETTINGS_PARSER_FULLSCREEN = "fullscreen";
const char* const SETTINGS_PARSER_MUSIC = "music";
const char* const SETTINGS_PARSER_SOUND = "sound";
const char* const SETTINGS_PARSER_VSYNC = "vsync";
const char* const SETTINGS_PARSER_MAIN_SCENE = "mainScene";
const char* const SETTINGS_PARSER_ERR_NO_TYPE_STRING =
        "Settings: constructor: TextParser have no suitable type string!";
const char* const SETTINGS_ERR_TEXT_PARSER_CONSTRUCTING = "Settings: constructor: constructing TextParser failed!";
const char* const SETTINGS_ERR_DEFAULTS = "Settings: constructor: using defaults settings!";

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
        settings->mainScene = SETTINGS_DEFAULT_MAIN_SCENE;
    }
    return settings;
}

unsigned char Settings_checkTextResourceType(struct Settings* settings, struct TextParser* textParser,
                                             struct ResourceManager* resourceManager) {
    char* settingsTypeString = NULL;
    settingsTypeString = TextParser_getString(textParser, TEXT_PARSER_TYPE_STRING, 0);
    if (textParser->lastError) {
        Logger_log(resourceManager->logger, SETTINGS_PARSER_ERR_NO_TYPE_STRING);
        Logger_log(resourceManager->logger, SETTINGS_ERR_DEFAULTS);
        settings->settingsResource->pointersCount = 0;
        ResourceManager_destructNeedlessTextResources(resourceManager);
        return 1;
    }
    if (strcmp(settingsTypeString, SETTINGS_PARSER_TYPE_STRING) != 0) {
        Logger_log(resourceManager->logger, SETTINGS_PARSER_ERR_NO_TYPE_STRING);
        Logger_log(resourceManager->logger, SETTINGS_ERR_DEFAULTS);
        settings->settingsResource->pointersCount = 0;
        ResourceManager_destructNeedlessTextResources(resourceManager);
        return 2;
    }
    return 0;
}

void Settings_tryGetSettingsFromTextParser(struct Settings* settings, struct TextParser* textParser) {
    settings->h = (size_t) TextParser_getInt(textParser, SETTINGS_PARSER_SCREEN_HEIGHT, 0);
    if (textParser->lastError)
        settings->h = SETTINGS_DEFAULT_H;
    settings->w = (size_t) TextParser_getInt(textParser, SETTINGS_PARSER_SCREEN_WIDTH, 0);
    if (textParser->lastError)
        settings->w = SETTINGS_DEFAULT_W;
    settings->virtualH = (size_t) TextParser_getInt(textParser, SETTINGS_PARSER_VIRTUAL_SCREEN_HEIGHT, 0);
    if (textParser->lastError)
        settings->virtualH = SETTINGS_DEFAULT_H;
    settings->virtualW = (size_t) TextParser_getInt(textParser, SETTINGS_PARSER_VIRTUAL_SCREEN_WIDTH, 0);
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
    settings->mainScene = TextParser_getString(textParser, SETTINGS_PARSER_MAIN_SCENE, 0);
    if (textParser->lastError)
        settings->mainScene = SETTINGS_DEFAULT_MAIN_SCENE;
}

struct Settings* Settings_construct(struct ResourceManager* resourceManager, const char* const settingsResId) {
    struct Settings *settings = NULL;
    settings = (struct Settings *) malloc(sizeof(struct Settings));
    if (!settings)
        return NULL;
    settings->settingsResource = ResourceManager_loadTextResource(resourceManager, settingsResId, 0);
    if (!settings->settingsResource) {
        Logger_log(resourceManager->logger, SETTINGS_ERR_DEFAULTS);
        return Settings_defaults(settings);
    }
    struct TextParser *textParser = NULL;
    textParser = TextParser_constructFromTextResource(settings->settingsResource);
    if (!textParser) {
        Logger_log(resourceManager->logger, SETTINGS_ERR_TEXT_PARSER_CONSTRUCTING);
        Logger_log(resourceManager->logger, SETTINGS_ERR_DEFAULTS);
        settings->settingsResource->pointersCount = 0;
        ResourceManager_destructNeedlessTextResources(resourceManager);
        return Settings_defaults(settings);
    }
    if (Settings_checkTextResourceType(settings, textParser, resourceManager))
        return Settings_defaults(settings);
    Settings_tryGetSettingsFromTextParser(settings, textParser);
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
    struct TextParser* textParser= NULL;
    size_t i;
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
    if (result) {
        TextParser_destruct(textParser);
        return 3;
    }
    char* newText = NULL;
    newText = TextParser_convertToText(textParser);
    if (textParser->lastError) {
        TextParser_destruct(textParser);
        return 4;
    }
    if (TextResource_updateContent(settings->settingsResource, newText)) {
        TextParser_destruct(textParser);
        return 5;
    }
    ResourceManager_saveTextResource(resourceManager, settings->settingsResource, settingsResId);
    return 0;
}


void Settings_destruct(struct Settings* settings) {
    if (!settings)
        return;
    if (settings->mainScene && settings->mainScene != SETTINGS_DEFAULT_MAIN_SCENE)
        free(settings->mainScene);
    free(settings);
}