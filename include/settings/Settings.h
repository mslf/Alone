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
#include "resourceManager/ResourceManager.h"

struct Settings {
    struct TextResource* settingsResource;
    char isMusicActive;
    char isSoundActive;
    char isFullscreen;
    SDL_DisplayMode displayMode;
};
struct Settings* Settings_construct(struct ResourceManager* resourceManager, const char* const settingsResId);
void Settings_destruct(struct Settings* settings);

#endif //ALONE_SETTINGS_H
