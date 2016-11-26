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
 * @file SceneNodeTypesRegistrar.h
 * @author mslf
 * @date 5 Nov 2016
 * @brief File containing #SceneNodeTypesRegistrar and it's stuff.
 */
#ifndef ALONE_SCENENODE_TYPES_REGISTRAR_H
#define ALONE_SCENENODE_TYPES_REGISTRAR_H

#include "textParser/TextParser.h"
#include "resourceManager/ResourceManager.h"
#include "renderer/Renderer.h"
#include "scene/SceneNode.h"

/**
 * @brief Some initial constants for #SceneNodeTypesRegistrar.
 */
enum SNTR_constants{
    SNTR_TYPES_REALLOCATION_STEP = 20
    /**< Init alocating number and reallocating step for SceneNodeTypesRegistrar#sceneNodeTypesList. */
};

/**
 * @brief Error codes for #SceneNodeTypesRegistrar.
 */
enum SNTR_errors {
    SNTR_NO_ERRORS = 0,
    /**< All right, no errors. */
    SNTR_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's argument is NULL. */
    SNTR_ERR_ALLOC_LIST = 2,
    /**< Allocating or reallocating memory for SceneNodeTypesRegistrar#sceneNodeTypesList failed. */
    SNTR_ERR_REGISTERED_BEFORE = 3,
    /**< Type, which you trying to register, already there is in SceneNodeTypesRegistrar#sceneNodeTypesList. */
    SNTR_ERR_ALLOC_TYPE_STRING = 4,
    /**< Allocating or reallocating memory for SceneNodeType#type failed. */
    SNTR_ERR_NO_TYPE_STRING = 5,
    /**< Type string haven't found in #TextParser. */
    SNTR_ERR_NO_REGISTERED_TYPE = 6,
    /**< SceneNodeTypesRegistrar#sceneNodeTypesList doesn't contain registered 
     * type of #SceneNode inheritor with type string like that, which was loaded from #TextResource type string. */
    SNTR_ERR_TYPE = 7
    /**< Wishful type string haven't found in #TextParser. */
};

struct SceneNodeTypesRegistrar;

/**
 * @brief Represent #SceneNode inheritor's type.
 */
struct SceneNodeType {
    char* type;
    /**< Type string, which will be found in #TextParser for constructing #SceneNode inheritor. */
    struct SceneNode* (*construct)(struct ResourceManager* const resourceManager, 
                                    struct Renderer* const renderer,
                                    struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar, 
                                    struct TextParser* const textParser);
    /**< Pointer to a function, that constructs #SceneNode inheritor of that type. */
};

/**
 * @brief Contains list of all registered #SceneNode inheritors with their constructors.
 */
struct SceneNodeTypesRegistrar {
    struct SceneNodeType* sceneNodeTypesList;
    /**< Array of #SceneNodeType. */
    size_t allocatedSceneNodeTypes;
    /**< Allocated number of #SceneNodeType in the SceneNodeTypesRegistrar#sceneNodeTypesList. */
    size_t sceneNodeTypesCount;
    /**< Current number of existing #SceneNodeType in the SceneNodeTypesRegistrar#sceneNodeTypesList. */
};

/**
 * @brief Constructs and inits #SceneNodeTypesRegistrar.
 * @return Pointer to a #SceneNodeTypesRegistrar or NULL, if failed.
 */
struct SceneNodeTypesRegistrar* SceneNodeTypesRegistrar_construct();

/**
 * @brief Destructs #SceneNodeTypesRegistrar.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar to be destructed. Can be NULL.
 * Can be not fully initialized.
 */
void SceneNodeTypesRegistrar_destruct(struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar);

/**
 * @brief Registers new #SceneNode inheritor type in #SceneNodeTypesRegistrar.
 * Increases SceneNodeTypesRegistrar#sceneNodeTypesCount and reallocatrs SceneNodeTypesRegistrar#sceneNodeTypesList (if needed).
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar, where new type will be registered. Can be NULL.
 * @param typeString String with #SceneNode inheritor's type. Can be NULL.
 * @param constructor Pointer t a function, which constructs #SceneNode inheritor. Can be NULL.
 * @return #SNTR_errors value.
 * @see #SNTR_errors
 * @see #SceneNode
 */
enum SNTR_errors SceneNodeTypesRegistrar_registerNewSceneNodeType(
                                            struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                            const char* const typeString,
                                            struct SceneNode* (*constructor)(
                                                struct ResourceManager* const resourceManager, 
                                                struct Renderer* const renderer,
                                                struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                struct TextParser* const textParser));

/**
 * @brief Constructs new #SceneNode inheritor by assotiated constructor.
 * @param resourceManager Pointer to a #ResourceManager to loading resources, 
 * which needed for #SceneNode inheritor. Can be NULL.
 * @param renderer Pointer to a #Renderer to construct textures, which needed for #SceneNode inheritor. Can be NULL.
 * @param sceneNodeTypesRegistrar Pointer to a #SceneNodeTypesRegistrar with registered types. Can be NULL.
 * @param resId String with ID (path) to the #SceneNode inheritor resource. Can be NULL.
 * @param requiredTypeString String with wishful #SceneNode inheritor type. Can be NULL.
 * If it is NULL, then any registered #SceneNode type will be constructed (if its resource contains apropriate type string).
 * @return Pointer to a #SceneNode inheritor type object casted to a #SceneNode, or NULL if failed.
 */
struct SceneNode* SceneNodeTypesRegistrar_constructSceneNode(struct ResourceManager* resourceManager,
                                                             struct Renderer* renderer,
                                                             struct SceneNodeTypesRegistrar* sceneNodeTypesRegistrar,
                                                             const char* const resId,
                                                             const char* const requiredTypeString);
#endif //ALONE_SCENENODE_TYPES_REGISTRAR_H
