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

struct Settings {
    char isMusicActive;
    char isSoundActive;
    char isFullscreen;
    SDL_DisplayMode displayMode;
};
struct Settings* Settings_construct();
SDL_DisplayMode Settings_getDisplayMode(struct Settings* settings);
char Settings_getMusicState(struct Settings* settings);
char Settings_getSoundState(struct Settings* settings);
char Settings_getFullscreenState(struct Settings* settings);
void Settings_destruct(struct Settings* settings);
/*class Settings {
public:
    Settings(SettingsResource resource);
    ~Settings();

    SDL_DisplayMode getDisplayMode();
    bool getMusicState();
    bool getSoundState();
    bool getFullscreenState();

private:
    bool isMusicActive;
    bool isSoundActive;
    bool isFullscreen;
    SDL_DisplayMode displayMode;
};*/


#endif //ALONE_SETTINGS_H
