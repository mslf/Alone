//
// Created by mslf on 8/10/16.
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
#ifndef ALONE_GAMEMANAGER_H
#define ALONE_GAMEMANAGER_H

#include "logger/Logger.h"
#include "renderer/Renderer.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "eventManager/EventManager.h"
#include "settings/Settings.h"
#include "scene/Scene.h"

#define INITIAL_NUMBER_ALLOCATED_SCENES 10

struct GameManager {
    struct Logger logger;
    struct EventManager* eventManager;
    struct ResourceManager* resourceManager;
    struct Renderer* renderer;
    struct Musican* musican;
    struct Scene** scenesStack;
    struct Settings* settings;
    size_t allocatedScenesCount;
    size_t scenesCount;
};

struct GameManager* GameManager_construct();
void GameManager_destruct(struct GameManager* gm);

int GameManager_main(struct GameManager* gm);
void GameManager_pushScene(struct GameManager* gm, const char* const resId);
void GameManager_popScene(struct GameManager* gm);
#endif //ALONE_GAMEMANAGER_H
