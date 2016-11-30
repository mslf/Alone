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
 * @file Settings.c
 * @author mslf
 * @date 11 Aug 2016
 * @brief File containing implementation of #Settings.
 */
#include <resourceManager/ResourceManager.h>
#include "settings/Settings.h"
#include "textParser/TextParser.h"

/**
 * @brief Error message strings for #Button.
 */
static const struct Settings_errorMessages {
    const char* const errNoTypeString;
    /**< Will be displayed when #TextParser have no type string. */
    const char* const errType;
    /**< Will be displayed when #TextParser haven't Settings_parserStrings#type string. */
    const char* const errConstructingTextParser;
    /**< Will be displayed when constructing #TextParser for some requiredreason failed. */
    const char* const errDefault;
    /**< Will be displayed when some of #Settings_parserStrings haven't found, so they will be got from #Settings_default. */
    const char* const errAllocMainSceneString;
    /**< Will be displayed when allocating memory for Settings#mainScene failed. */
}Settings_errorMessages = {
    "Settings_construct: TextParser have no type string!",
    "Settings_construct: TextParser have no suitable type string!",
    "Settings_construct: constructing TextParser failed!",
    "Settings_construct: using defaults settings!",
    "Settings_construct: allocating memory for mainScene string failed!"};


/**
 * @brief Checks type string in #TextParser for suitability for initializing #Settings.
 * @param textParser Pointer to a #TextParser with Settings_parserStrings#type string. Can be NULL.
 * @param logger Pointer to a #Logger for logging purpose. Can be NULL.
 * @return #Settings_errors value.
 * @see #Settings
 * @see #Settings_errors
 */
static enum Settings_errors Settings_checkTextResourceType(struct TextParser* textParser,
                                                           struct Logger* logger) {
    if (!textParser)
        return SETTINGS_ERR_NULL_ARGUMENT;
    const char* settingsTypeString = NULL;
    settingsTypeString = TextParser_getString(textParser, TextParser_standartTypeString, 0);
    if (textParser->lastError) {
        Logger_log(logger, Settings_errorMessages.errNoTypeString);
        Logger_log(logger, "%s <ALL>", Settings_errorMessages.errDefault);
        return SETTINGS_ERR_NO_TYPE_STRING;
    }
    if (strcmp(settingsTypeString, Settings_parserStrings.type) != 0) {
        Logger_log(logger, Settings_errorMessages.errType);
        Logger_log(logger, "%s <ALL>", Settings_errorMessages.errDefault);
        return SETTINGS_ERR_TYPE;
    }
    return SETTINGS_NO_ERRORS;
}

/**
 * @brief Initalizes #Settings from #TextParser.
 * @param settings Pointer to a #Settings to be initalized. Can be NULL.
 * @param textParser Pointer to a #TextParser with #Settings_parserStrings.
 * @return #Settings_errors value.
 * @see #Settings
 * @see #Settings_errors
 */
static enum Settings_errors Settings_tryGetSettingsFromTextParser(struct Settings* settings, struct TextParser* textParser) {
    if (!settings || !textParser)
        return SETTINGS_ERR_NULL_ARGUMENT;
    unsigned char result = 0;
    settings->w = (size_t)TextParser_getInt(textParser, Settings_parserStrings.screenSize, 0);
    if (textParser->lastError && ++result)
        settings->w = Settings_default.w;
    settings->h = (size_t)TextParser_getInt(textParser, Settings_parserStrings.screenSize, 1);
    if (textParser->lastError && ++result)
        settings->h = Settings_default.h;
    settings->virtualW = (size_t)TextParser_getInt(textParser, Settings_parserStrings.virtualScreenSize, 0);
    if (textParser->lastError && ++result)
        settings->virtualW = Settings_default.virtualW;
    settings->virtualH = (size_t)TextParser_getInt(textParser, Settings_parserStrings.virtualScreenSize, 1);
    if (textParser->lastError && ++result)
        settings->virtualH = Settings_default.virtualH;
    settings->isFullscreen = TextParser_getFlag(textParser, Settings_parserStrings.fullscreen, 0);
    if (textParser->lastError && ++result)
        settings->isFullscreen = Settings_default.isFullscreen;
    settings->isMusicActive = TextParser_getFlag(textParser, Settings_parserStrings.music, 0);
    if (textParser->lastError && ++result)
        settings->isMusicActive = Settings_default.isMusicActive;
    settings->isSoundActive = TextParser_getFlag(textParser, Settings_parserStrings.sound, 0);
    if (textParser->lastError && ++result)
        settings->isSoundActive = Settings_default.isSoundActive;
    settings->isVsyncActive = TextParser_getFlag(textParser, Settings_parserStrings.vsync, 0);
    if (textParser->lastError && ++result)
        settings->isVsyncActive = Settings_default.isVsyncActive;
    const char* tempString = TextParser_getString(textParser, Settings_parserStrings.mainScene, 0);
    if (textParser->lastError && ++result)
        tempString = Settings_default.mainScene;
    settings->mainScene = (char*)malloc(sizeof(char) * (strlen(tempString) + 1));
    if (!settings->mainScene)
        return SETTINGS_ERR_ALLOC_MAIN_SCENE_STRING;
    strcpy(settings->mainScene, tempString);
    if (result)
        return SETTINGS_WARN_DEFAULTS;
    return SETTINGS_NO_ERRORS;
}

struct Settings* Settings_construct(struct ResourceManager* resourceManager, const char* const settingsResId) {
    if (!resourceManager || !settingsResId)
        return NULL;
    struct Settings *settings = NULL;
    settings = (struct Settings *) calloc(1, sizeof(struct Settings));
    if (!settings)
        return NULL;
    (*settings) = Settings_default;
    settings->settingsResource = ResourceManager_loadTextResource(resourceManager, settingsResId, 0);
    if (!settings->settingsResource) {
        Logger_log(resourceManager->logger, "%s <ALL>", Settings_errorMessages.errDefault);
        return settings;
    }
    struct TextParser *textParser = NULL;
    textParser = TextParser_constructFromTextResource(resourceManager->logger, settings->settingsResource);
    if (!textParser) {
        Logger_log(resourceManager->logger, Settings_errorMessages.errConstructingTextParser);
        Logger_log(resourceManager->logger, "%s <ALL>", Settings_errorMessages.errDefault);
        return settings;
    }
    if (Settings_checkTextResourceType(textParser, resourceManager->logger)) {
        Logger_log(resourceManager->logger, "%s <ALL>", Settings_errorMessages.errDefault);
        return settings;
    }
    enum Settings_errors result = Settings_tryGetSettingsFromTextParser(settings, textParser);
    if (result == SETTINGS_ERR_ALLOC_MAIN_SCENE_STRING)
        Logger_log(resourceManager->logger, Settings_errorMessages.errAllocMainSceneString);
    if (result == SETTINGS_WARN_DEFAULTS)
        Logger_log(resourceManager->logger, "%s <SOME>", Settings_errorMessages.errDefault);
    TextParser_destruct(textParser);
    return settings;
}

enum Settings_errors Settings_updateMainSceneString(struct Settings* settings, const char* const mainScene) {
    if (!settings || !mainScene)
        return SETTINGS_ERR_NULL_ARGUMENT;
    char* newText = NULL;
    newText = (char*)malloc(sizeof(char) * (strlen(mainScene) + 1));
    if (!newText)
        return SETTINGS_ERR_ALLOC_MAIN_SCENE_STRING;
    strcpy(newText, mainScene);
    free(settings->mainScene);
    settings->mainScene = newText;
    return SETTINGS_NO_ERRORS;
}

enum Settings_errors Settings_save(struct Settings* settings, struct ResourceManager* resourceManager,
                            const char* const settingsResId) {
    if (!settings || !resourceManager || !settingsResId)
        return SETTINGS_ERR_NULL_ARGUMENT;
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser)
        return SETTINGS_ERR_CONSTRUCTIG_TEXT_PARSER;
    unsigned char result = 0;
    result += (TextParser_addString(textParser, TextParser_standartTypeString, Settings_parserStrings.type) != 0);
    result += (TextParser_addInt(textParser, Settings_parserStrings.screenSize, settings->w) != 0);
    result += (TextParser_addInt(textParser, Settings_parserStrings.screenSize, settings->h) != 0);
    result += (TextParser_addInt(textParser, Settings_parserStrings.virtualScreenSize, settings->virtualW) != 0);
    result += (TextParser_addInt(textParser, Settings_parserStrings.virtualScreenSize, settings->virtualH) != 0);
    result += (TextParser_addFlag(textParser, Settings_parserStrings.fullscreen, settings->isFullscreen) != 0);
    result += (TextParser_addFlag(textParser, Settings_parserStrings.music, settings->isMusicActive) != 0);
    result += (TextParser_addFlag(textParser, Settings_parserStrings.sound, settings->isSoundActive) != 0);
    result += (TextParser_addFlag(textParser, Settings_parserStrings.vsync, settings->isVsyncActive) != 0);
    result += (TextParser_addString(textParser, Settings_parserStrings.mainScene, settings->mainScene) != 0);
    char* newText = NULL;
    newText = TextParser_convertToText(textParser);
    result += (textParser->lastError != 0);
    /* FIXME in that place (and in other like this) will occur errors, if we want to construct new objects in runtime
     * without assotiated (and existing) file in filesystem.
     * Need to implement function for creating TextResource from TextParser. */
    result += (TextResource_updateContent(settings->settingsResource, newText) != 0);
    result += (ResourceManager_saveTextResource(resourceManager, settings->settingsResource, settingsResId) != 0);
    TextParser_destruct(textParser);
    if (newText)
        free(newText);
    if (result)
        return SETTINGS_ERR_SAVING;
    return SETTINGS_NO_ERRORS;
}

void Settings_destruct(struct Settings* settings) {
    if (!settings)
        return;
    if (settings->mainScene)
        free(settings->mainScene);
    TextResource_decreasePointersCounter(settings->settingsResource);
    free(settings);
}
