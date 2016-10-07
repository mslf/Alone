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

struct Settings* Settings_construct(struct ResourceManager* resourceManager, const char* const settingsResId) {
    struct Settings* settings = NULL;
    settings = (struct Settings*)malloc(sizeof(struct Settings));
    if (!settings)
        return  NULL;
    settings->settingsResource = ResourceManager_loadTextResource(resourceManager, settingsResId, 0);
    if (!settings->settingsResource) {
        Settings_destruct(settings);
        return NULL;
    }
    struct TextParser* textParser = NULL;
    textParser = TextParser_construct(settings->settingsResource);
    if (!textParser) {
        Settings_destruct(settings);
        return NULL;
    }
    settings->h = (size_t)TextParser_getInt(textParser, "screenHeight", 0);
    settings->w = (size_t)TextParser_getInt(textParser, "screenWidth", 0);
    settings->virtualH = (size_t)TextParser_getInt(textParser, "virtualScreenHeight", 0);
    settings->virtualW = (size_t)TextParser_getInt(textParser, "virtualScreenWidth", 0);
    settings->isFullscreen = TextParser_getFlag(textParser, "fullscreen", 0);
    settings->isMusicActive = TextParser_getFlag(textParser, "music", 0);
    settings->isSoundActive = TextParser_getFlag(textParser, "sound", 0);
    settings->isVsyncActive = TextParser_getFlag(textParser, "vsync", 0);
    TextParser_destruct(textParser);
    return settings;
}

void Settings_destruct(struct Settings* settings) {
    if (settings)
        free(settings);
}