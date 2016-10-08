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

struct Settings* Settings_construct(struct ResourceManager* resourceManager, const char* const settingsResId) {
    struct Settings* settings = NULL;
    settings = (struct Settings*)malloc(sizeof(struct Settings));
    if (!settings)
        return  NULL;
    settings->settingsResource = ResourceManager_loadTextResource(resourceManager, settingsResId, 0);
    if (!settings->settingsResource) {
        // Failed to load SettingsResource, so using defaults
        return Settings_defaults(settings);
    }
    struct TextParser* textParser = NULL;
    textParser = TextParser_construct(settings->settingsResource);
    if (!textParser) {
        // Failed to create TextParser, so using defaults
        return Settings_defaults(settings);
    }
    settings->h = (size_t)TextParser_getInt(textParser, "screenHeight", 0);
    if (textParser->lastError)
        settings->h = SETTINGS_DEFAULT_H;
    settings->w = (size_t)TextParser_getInt(textParser, "screenWidth", 0);
    if (textParser->lastError)
        settings->w = SETTINGS_DEFAULT_W;
    settings->virtualH = (size_t)TextParser_getInt(textParser, "virtualScreenHeight", 0);
    if (textParser->lastError)
        settings->virtualH = SETTINGS_DEFAULT_H;
    settings->virtualW = (size_t)TextParser_getInt(textParser, "virtualScreenWidth", 0);
    if (textParser->lastError)
        settings->virtualW = SETTINGS_DEFAULT_W;
    settings->isFullscreen = TextParser_getFlag(textParser, "fullscreen", 0);
    if (textParser->lastError)
        settings->isFullscreen = SETTINGS_DEFAULT_FULLSCREEN;
    settings->isMusicActive = TextParser_getFlag(textParser, "music", 0);
    if (textParser->lastError)
        settings->isMusicActive = SETTINGS_DEFAULT_MUSIC;
    settings->isSoundActive = TextParser_getFlag(textParser, "sound", 0);
    if (textParser->lastError)
        settings->isSoundActive = SETTINGS_DEFAULT_SOUND;
    settings->isVsyncActive = TextParser_getFlag(textParser, "vsync", 0);
    if (textParser->lastError)
        settings->isVsyncActive = SETTINGS_DEFAULT_VSYNC;
    char* tempString = NULL;
    tempString = TextParser_getString(textParser, "mainScene", 0);
    if (textParser->lastError)
        settings->mainScene = SETTINGS_DEFAULT_MAIN_SCENE;
    else {
        settings->mainScene = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
        if (!settings->mainScene) {
            // Failed to allocate memory for mainScene string, so using defaults
            return Settings_defaults(settings);
        }
        strcpy(settings->mainScene, tempString);
    }
    TextParser_destruct(textParser);
    return settings;
}

void Settings_destruct(struct Settings* settings) {
    if (settings) {
        if (settings->mainScene && settings->mainScene != SETTINGS_DEFAULT_MAIN_SCENE)
            free(settings->mainScene);
        free(settings);
    }
}