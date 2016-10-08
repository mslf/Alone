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

#define SETTINGS_DEFAULT_VSYNC 1
#define SETTINGS_DEFAULT_MUSIC 1
#define SETTINGS_DEFAULT_SOUND 1
#define SETTINGS_DEFAULT_FULLSCREEN 0
#define SETTINGS_DEFAULT_W 800
#define SETTINGS_DEFAULT_H 450
#define SETTINGS_DEFAULT_MAIN_SCENE "data/main.scene"

struct ResourceManager;

struct Settings {
    unsigned char isVsyncActive;
    unsigned char isMusicActive;
    unsigned char isSoundActive;
    unsigned char isFullscreen;
    struct TextResource* settingsResource;
    size_t w;
    size_t h;
    size_t virtualW;
    size_t virtualH;
    char* mainScene;
};

struct Settings* Settings_construct(struct ResourceManager* resourceManager, const char* const settingsResId);
void Settings_destruct(struct Settings* settings);

#endif //ALONE_SETTINGS_H
