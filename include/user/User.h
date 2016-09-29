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
#ifndef ALONE_USER_H
#define ALONE_USER_H

#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "controller/Controller.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "resourceManager/TextureResource.h"
#include "resourceManager/TextResource.h"
#include "resourceManager/ScriptResource.h"
#include "resourceManager/SoundResource.h"
#include "module/section/MacroSection.h"
/*
 * User is an inheritor of the PhysicalSceneNode.
 * You SHOULD include the "struct PhysicalSceneNode* blablaNode;" at the begining of User struct,
 * if you want code to work with User like with a PhysicalSceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL or to your function implementation.
 * Don't forget to add this warning comment to your own new PhysicalSceneNode inheritors.
 */
struct User {
    struct PhysicalSceneNode* physicalSceneNode;
    struct TextureResource* textureResource;
    struct ScriptResource* scriptResource;
    struct SoundResource** soundResources;
    struct MacroSection* macroSection;
    size_t soundResourcesCount;
    size_t currentAnimation;
    size_t currentFrame;
    SDL_Rect* srcRect;
    SDL_Rect* dstRect;
};

struct User* User_construct(struct ResourceManager* const resourceManager, const char* const resId);
void User_destruct(struct User* user);

void User_save(
        const struct User* const user, struct ResourceManager* const resourceManager,
        const char* const resId);
void User_control(struct SceneNode* sceneNode, struct Controller* controller);
void User_update(struct SceneNode* sceneNode, struct EventManager* eventManager);
void User_render(struct SceneNode* sceneNode, struct Renderer* renderer);
void User_sound(struct SceneNode* sceneNode, struct Musican* musican);

#endif //ALONE_USER_H
