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
#ifndef ALONE_SETTINGS_H
#define ALONE_SETTINGS_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define SETTINGS_PARSER_TYPE_STRING "Settings"
#define SETTINGS_PARSER_SCREEN_HEIGHT "screenHeight"
#define SETTINGS_PARSER_SCREEN_WIDTH "screenWidth"
#define SETTINGS_PARSER_VIRTUAL_SCREEN_HEIGHT "virtualScreenHeight"
#define SETTINGS_PARSER_VIRTUAL_SCREEN_WIDTH "virtualScreenWidth"
#define SETTINGS_PARSER_FULLSCREEN "fullscreen"
#define SETTINGS_PARSER_MUSIC "music"
#define SETTINGS_PARSER_SOUND "sound"
#define SETTINGS_PARSER_VSYNC "vsync"
#define SETTINGS_PARSER_MAIN_SCENE "mainScene"
#define SETTINGS_DEFAULT_MAIN_SCENE "data/main.scene"

//? Now that's how you should NOT do things :D 
//? How about creating a  const structure with default values?
enum {
    SETTINGS_DEFAULT_VSYNC = true,
    SETTINGS_DEFAULT_MUSIC = true,
    SETTINGS_DEFAULT_SOUND = true,
    SETTINGS_DEFAULT_FULLSCREEN = false,
    SETTINGS_DEFAULT_W = 640,
    SETTINGS_DEFAULT_H = 480
};

struct ResourceManager;

struct Settings {
    bool isVsyncActive;
    bool isMusicActive;
    bool isSoundActive;
    bool isFullscreen;
    struct TextResource* settingsResource;
    size_t w;
    size_t h;
    size_t virtualW;
    size_t virtualH;
    char* mainScene;
};

struct Settings* Settings_construct(struct ResourceManager* resourceManager, const char* const settingsResId);
void Settings_destruct(struct Settings* settings);

unsigned char Settings_updateMainSceneString(struct Settings* settings, const char* const mainScene);
unsigned char Settings_save(struct Settings* settings, struct ResourceManager* resourceManager,
                            const char* const settingsResId);

#endif //ALONE_SETTINGS_H
