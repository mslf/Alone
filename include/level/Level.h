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
#ifndef ALONE_LEVEL_H
#define ALONE_LEVEL_H

#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "module/macro/BackgroundModule.h"
#include "module/macro/ActiveModule.h"
#include "module/Connections.h"

#define LEVEL_SCENENODE_PARSER_TYPE_STRING "Level"
/*
 * Level is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode* blablaNode;" at the begining of Level struct,
 * if you want code to work with Level like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct  Level {
    struct SceneNode* sceneNode;
    struct BackgroundModule** backgroundModulesList;
    struct ActiveModule** activeModulesList;
    struct MacroConnection* connectionsList;
    size_t backgroundModulesNumber;
    size_t activeModulesNumber;
    size_t connectionsNumber;
};

struct Level* Level_construct(struct ResourceManager* const resourceManager, const char* const levelResId);
void Level_destruct(struct Level* level);

void Level_save(
        const struct Level* const level, struct ResourceManager* const resourceManager,
        const char* const levelResId);
void Level_control(struct SceneNode* sceneNode, struct EventManager* eventManager);
void Level_update(struct SceneNode* sceneNode, struct EventManager* eventManager);
void Level_render(struct SceneNode* sceneNode, struct Renderer* renderer);
void Level_sound(struct SceneNode* sceneNode, struct Musican* musican);

void Level_addBackgroundModule(struct Level* level, const char* const textureResId, SDL_Point coordinates);
void Level_removeBackgroundModule(struct Level* level, SDL_Point coordinates);
void Level_addActiveModule(struct Level* level, const char* const activeModuleResId, SDL_Point coordinates);
void Level_removeActiveModule(struct Level* level, SDL_Point coordinates);
void Level_addConnection(struct Level* level, struct MacroConnection macroConnection);
void Level_removeConnection(struct Level* level, size_t index);

#endif //ALONE_LEVEL_H
