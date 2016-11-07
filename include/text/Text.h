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
#ifndef ALONE_TEXT_H
#define ALONE_TEXT_H

#include <SDL2/SDL.h>
#include "scene/SceneNode.h"
#include "renderer/Renderer.h"
#include "eventManager/EventManager.h"
#include "musican/Musican.h"
#include "resourceManager/ResourceManager.h"
#include "scene/SceneNodeTypesRegistrar.h"

#define TEXT_SCENENODE_PARSER_TYPE_STRING "Text"
#define TEXT_SCENENODE_PARSER_FONT_PATH "fontPath"
#define TEXT_SCENENODE_PARSER_TEXT "text"
#define TEXT_SCENENODE_PARSER_SIZE "size"
#define TEXT_SCENENODE_PARSER_COLOR "color"
#define TEXT_SCENENODE_DEFAULT_FONT_PATH "data/default.ttf"
#define TEXT_SCENENODE_DEFAULT_TEXT "Sample Text."

enum {
    TEXT_SCENENODE_DEFAULT_SIZE = 12,
    TEXT_SCENENODE_DEFAULT_COLOR_R = 0,
    TEXT_SCENENODE_DEFAULT_COLOR_G = 0,
    TEXT_SCENENODE_DEFAULT_COLOR_B = 0,
    TEXT_SCENENODE_DEFAULT_COLOR_A = 0
};
/*
 * Text is an inheritor of the SceneNode.
 * You SHOULD include the "struct SceneNode blablaNode;" at the begining of Text struct,
 * if you want code to work with Text like with a SceneNode.
 * More, you SHOULD initialize function pointers in 'blablaNode' to NULL (by calling SceneNode_init)
 * or to your function implementation.
 * Don't forget to add this warning comment to your own new SceneNode inheritors.
 */
struct Text {
    struct SceneNode sceneNode;
    struct TextureResource* textureResource;
    char* fontPath;
    char* text;
    int size;
    SDL_Color color;
    SDL_Rect srcRect;
    SDL_Rect dstRect;
};

struct SceneNode* Text_construct(struct ResourceManager* const resourceManager,
                                                 struct Renderer* const renderer,
                                                 struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                 struct TextParser* const textParser);
void Text_destruct(struct SceneNode* text);

unsigned char Text_regenerateTexture(struct Text* text, struct ResourceManager* resourceManager, struct Renderer* renderer,
                                     const char* const textString, const char* const fontPath, int size, SDL_Color color);
unsigned char Text_save(
        const struct  Text* const text, struct ResourceManager* const resourceManager,
        const char* const textResId);
void Text_update(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);
void Text_render(struct SceneNode* sceneNode, struct Renderer* renderer);

#endif //ALONE_TEXT_H
