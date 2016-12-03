//
// Created by mslf on 9/28/16.
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
#ifndef ALONE_STANDARD_USER_H
#define ALONE_STANDARD_USER_H

#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "resourceManager/TextureResource.h"
#include "resourceManager/TextResource.h"
#include "resourceManager/ScriptResource.h"
#include "resourceManager/SoundResource.h"
#include "module/section/MacroSection.h"

#define USER_SCENENODE_PARSER_TYPE_STRING "StandardUser"
/*
 * User is an inheritor of the PhysicalSceneNode.
 * You SHOULD include the "struct PhysicalSceneNode* blablaNode;" at the begining of User struct,
 * if you want code to work with User like with a PhysicalSceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL or to your function implementation.
 * Don't forget to add this warning comment to your own new PhysicalSceneNode inheritors.
 */
struct InventoryItem {
    char* name;   //? const maybe
    size_t count;
};

struct StandardUser {
    struct PhysicalSceneNode* physicalSceneNode;
    struct TextResource* spriteResource;
    struct TextureResource* textureResource;
    struct ScriptResource* scriptResource;
    struct SoundResource** soundResources;
    struct MacroSection* macroSection;
    struct InventoryItem* inventoryItemsList;
    size_t soundResourcesCount;
    size_t inventoryItemsCount;
    size_t currentAnimation;
    size_t currentFrame;
    SDL_Rect* srcRect;
    SDL_Rect* dstRect;
};

struct StandardUser* StandartUser_construct(struct ResourceManager* const resourceManager, const char* const userResId);
void StandartUser_destruct(struct StandardUser* standardUser);

void StandartUser_save(
        const struct StandardUser* const standardUser, struct ResourceManager* const resourceManager,
        const char* const userResId);
void StandartUser_control(struct SceneNode* sceneNode, struct EventManager* eventManager);
void StandartUser_update(struct SceneNode* sceneNode, struct EventManager* eventManager);
void StandartUser_render(struct SceneNode* sceneNode, struct Renderer* renderer);
void StandartUser_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_STANDARD_USER_H
