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
 * @file SceneNode.h
 * @author mslf
 * @date 11 Aug 2016
 * @brief File containing #SceneNode and it's stuff.
 */
#ifndef ALONE_SCENENODE_H
#define ALONE_SCENENODE_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "eventManager/EventManager.h"
#include "renderer/Renderer.h"
#include "musican/Musican.h"

/**
 * @brief Possible types of #SceneNode.
 */
enum SceneNodeType {
    SCENE_NODE_SIMPLE = 0,
    /**< Represents simple #SceneNode. */
    SCENE_NODE_DYNAMIC = 1,
    /**< Represents #DynamicSceneNode. */
    SCENE_NODE_PHYSICAL = 2
    /**< Represents #PhysicalSceneNode. */
};

/**
 * @brief Basic object, which is represent node of #Scene.
 * Use SceneNode_init() to init this #SceneNode.
 * @see #SceneNodeType
 */
struct SceneNode {
    enum SceneNodeType nodeType;
    /**< Determines type of #SceneNode. */
    bool isActive;
    /**< Flag, which is determine activity state of #SceneNode. */
    struct TextResource* sceneNodeTextResource;
    /**< Pointer to a #TextResource with data strings of this #SceneResource inheritor. */
    char* type;
    /**< String with type of #SceneNode inheritor. */
    SDL_Point coordinates;
    /**< Coordinates of #SceneNode in Upixels. */
    void (*control)(struct SceneNode* sceneNode, struct EventManager* eventManager);
    /**< Pointer to a function, that implements controlling of #SceneNode inheritor. */
    void (*update)(struct SceneNode* sceneNode, struct EventManager* eventManager, struct Renderer* renderer);
    /**< Pointer to a function, that implements updating of #SceneNode inheritor. */
    void (*render)(struct SceneNode* sceneNode, struct Renderer* renderer);
    /**< Pointer to a function, that implements rendering of #SceneNode inheritor. */
    void (*sound)(struct SceneNode* sceneNode, struct Musican* musican);
    /**< Pointer to a function, that implements playing sounds of #SceneNode inheritor. */
    void (*destruct)(struct SceneNode* sceneNode);
    /**< Pointer to a function, that implements destruction of #SceneNode inheritor. */
};

/**
 * @brief Dynamic version of #SceneNode.
 * @see #SceneNode
 */
struct DynamicSceneNode {
    struct SceneNode sceneNode;
    /**< Includes #SceneNode for compatibility with simple #SceneNode. */
    double scaleX;
    /**< X scaling value. */
    double scaleY;
    /**< Y scaling value. */
    SDL_RendererFlip flip;
    /**< Flip value. */
    SDL_Point rotatePointCoordinates;
    /**< Coordinates of rotation point. */
    double angle;
    /**< Rotation angle. */
};

/**
 * @brief Physical version of #DynamicSceneNode.
 * @see #DynamicSceneNode
 */
struct PhysicalSceneNode {
    struct DynamicSceneNode dynamicSceneNode;
    /**< Includes #DynamicSceneNode for compatibility with simple #DynamicSceneNode. */
    double velX;
    /**< Velosity of #PhysicalSceneNode by X axis. */
    double velY;
    /**< Velosity of #PhysicalSceneNode by Y axis. */
    double angleVel;
    /**< Angular velocity of #PhysicalSceneNode. */
};

/**
 * @brief Inits #SceneNode and sets SceneNode#nodeType to SceneNodeType#SCENE_NODE_SIMPLE.
 * @param sceneNode Pointer to a #SceneNode to be initialized. Can be NULL.
 */
void SceneNode_init(struct SceneNode* sceneNode);

/**
 * @brief Inits #DynamicSceneNode and sets SceneNode#nodeType to SceneNodeType#SCENE_NODE_DYNAMIC.
 * @param dynamicSceneNode Pointer to a #DynamicSceneNode to be initialized. Can be NULL.
 */
void SceneNode_initDynamic(struct DynamicSceneNode* dynamicSceneNode);

/**
 * @brief Inits #PhysicalSceneNode and sets SceneNode#nodeType to SceneNodeType#SCENE_NODE_PHYSICAL.
 * @param physicalSceneNode Pointer to a #PhysicalSceneNode to be initialized. Can be NULL.
 */
void SceneNode_initPhysical(struct PhysicalSceneNode* physicalSceneNode);

#endif //ALONE_SCENENODE_H
