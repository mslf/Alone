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
 * @file Settings.h
 * @author mslf
 * @date 11 Aug 2016
 * @brief File containing #Settings and it's stuff.
 */
#ifndef ALONE_SETTINGS_H
#define ALONE_SETTINGS_H

#include <SDL2/SDL.h>
#include <stdbool.h>

struct ResourceManager;

/**
 * @brief Strings which are used for constructing #Settings from #TextParser.
 * @see Settings_construct
 */
static const struct Settings_parserStrings {
    const char* const type;
    /**< Type string, which is used to distinquish #Settings resource from others. */
    const char* const screenSize;
    /**< Two numbers, which are used to set Settings#w and Settings#h. */
    const char* const virtualScreenSize;
    /**< Two numbers, which are used to set Settings#virtualW and Settings#virtualH. */
    const char* const fullscreen;
    /**< Flag, which is used to set Settings#isFullscreen. */
    const char* const music;
    /**< Flag, which is usedto set Settings#isMusicActive. */
    const char* const sound;
    /**< Flag, which is used to set Settings#isSoundActive. */
    const char* const vsync;
    /**< Flagm which is used to set #Settings#isVsyncActive. */
    const char* const maxFps;
    /**< Limits maximum FPS. */
    const char* const mainScene;
    /**< String with ID (path) to set Settings#mainScene. */
}Settings_parserStrings = {
    "Settings",
    "screenSize",
    "virtualScreenSize",
    "fullscreen",
    "music",
    "sound",
    "vsync",
    "maxFps",
    "mainScene"};

/**
 * @brief Error codes for #Settings.
 */
enum Settings_errors {
    SETTINGS_NO_ERRORS = 0,
    /**< All right, no errors. */
    SETTINGS_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    SETTINGS_ERR_NO_TYPE_STRING = 2,
    /**< #TextParser have no type string. */
    SETTINGS_ERR_TYPE = 3,
    /**< #TextParser haven't Settings_parserStrings#type string. */
    SETTINGS_ERR_ALLOC_MAIN_SCENE_STRING = 4,
    /**< Allocating memory for Settings#mainScene failed. */
    SETTINGS_WARN_DEFAULTS = 5,
    /**< Getting some of #Settings fields failed, so they will be default. */
    SETTINGS_ERR_CONSTRUCTIG_TEXT_PARSER = 6,
    /**< Constructing new #TextParser for some required reason failed. */
    SETTINGS_ERR_SAVING = 7
    /**< Saving #Settings failed due to internal reason. */
};

/**
 * @brief Represents initial settings for #GameManager.
 */
struct Settings {
    bool isVsyncActive;
    /**< Flag determines state of virtual syncronization. Will be used by #Renderer. */
    bool isMusicActive;
    /**< Flag, which is used to set Musican#isMusicActive. */
    bool isSoundActive;
    /**< Flag, which is used to set Musican#isSoundActive. */
    bool isFullscreen;
    /**< Fullscreen flag to create Renderer#window in right way. */
    struct TextResource* settingsResource;
    /**< Pointer to a #TextResource, which is represents #Settings. */
    size_t w;
    /**< Wishful screen X size in pixels to create Renderer#window. Should be supported by your display and video card. */
    size_t h;
    /**< Wishful screen Y size in pixels to create Renderer#window. Should be supported by your display and video card. */
    size_t virtualW;
    /**< Virtual screen X size in Upixels. Will be projected to real screen relativy. */
    size_t virtualH;
    /**< Virtual screen Y size in Upixels. Will be projected to real screen relativy. */
    char* mainScene;
    /**< String with ID (path) to the main #Scene resource. */
};

/**
 * @brief Default #Settings.
 */
static const struct Settings Settings_default = {
    .isVsyncActive = true,
    .isMusicActive = true,
    .isSoundActive = true,
    .isFullscreen = false,
    .w = 640,
    .h = 480,
    .virtualW = 640,
    .virtualH = 480,
    .mainScene = "data/main.scene",
    .settingsResource = NULL
};
        
/**
 * @brief Constructs and inits #Settings from file in filesystem.
 * @param resourceManager Pointer to a #ResourceManager, which is needed to load 
 * Settings#settingsResource by ID. Can be NULL.
 * @param settingsResId String with ID (path) to the #Settings resource. Can be NULL.
 * @return Pointer to an initialized from file #Settings or NULL, if something failed.
 * @note If some of settings will be not found in #TextResource, then they will be set from #Settings_default.
 */
struct Settings* Settings_construct(struct ResourceManager* resourceManager, const char* const settingsResId);

/**
 * @brief Destructs #Settings and frees memory, used by it.
 * @param settings Pointer to a #Settings. Can be NULL. Can be not fully initialized.
 */
void Settings_destruct(struct Settings* settings);

/**
 * @brief Changes Settings#mainScene.
 * @param settings Pointer to a #Settings, where Settings#mainScene will be changed. Can be NULL.
 * @param mainScene String with ID (path) of main #Scene resource. Can be NULL.
 * @return #Settings_errors value.
 * @see #Settings_errors
 */
enum Settings_errors Settings_updateMainSceneString(struct Settings* settings, const char* const mainScene);

/**
 * @brief Saves #Settings to the filesystem via #ResourceManager.
 * Before saving, it updates Settings#settingsResource with the latest changes in #Settings.
 * @param settings Pointer to a #Settings which will be saved. Can be NULL.
 * @param resourceManager Pointer to a #ResourceManager which is used to 
 * save Settings#settingsResource. Can be NULL.
 * @param settingsResId Path string, where #ResourceManager will 
 * save Settings#settingsResource. Can be NULL.
 * @return #Settings_errors value.
 * @see #Settings
 * @see #Settings_errors
 * @see #ResourceManager
 * @see #Settings_errors
 */
enum Settings_errors Settings_save(struct Settings* settings,
                                   struct ResourceManager* resourceManager,
                                   const char* const settingsResId);

#endif //ALONE_SETTINGS_H
