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
 * @file ScriptResource.h
 * @author mslf
 * @date 26 Sep 2016
 * @brief File containing #ScriptResource and its stuff.
 */
#ifndef ALONE_SCRIPTRESOURCE_H
#define ALONE_SCRIPTRESOURCE_H

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

/**
 * @brief Resource, which is represents runnable lua script.
 */
struct ScriptResource {
    size_t pointersCount;
    /**< Count of pointers to this resource. */
    char* id;
    /**< ID (path) string of this resource. */
    lua_State* luaState;
    /**< Lua vm. */
};

/**
 * @brief Constructs #ScriptResource and inits it.
 * @param path String with path to the lua script file.
 * @return Pointer to a #ScriptResource or NULL, if something failed.
 */
struct ScriptResource* ScriptResource_construct(const char* const path);

/**
 * @brief Destructs #ScriptResource and frees memory, used by it.
 * @param scriptResource Pointer to a #ScriptResource. Can be NULL. Can be not fully initialized.
 */
void ScriptResource_destruct(struct ScriptResource* scriptResource);

/**
 * @brief Decreases ScriptResource#pointersCount.
 * Useful for deffered resource managment by #ResourceManager.
 * @param scriptResource Pointer to a #ScriptResource. Can be NULL.
 * @see #ResourceManager
 */
void ScriptResource_decreasePointersCounter(struct ScriptResource* scriptResource);
//void ScriptResource_registerActiveModuleFunctions(struct ScriptResource* scriptResource);
//void ScriptResource_registerMicroModuleFunctions(struct ScriptResource* scriptResource);
//void ScriptResource_registerNanoModuleFunctions(struct ScriptResource* scriptResource);
//void ScriptResource_registerEventControllerFunctions(struct ScriptResource* scriptResource);
//void ScriptResource_updateActiveModule(struct ScriptResource* scriptResource, struct ActiveModule* activeModule);
//void ScriptResource_updateMicroModule(struct ScriptResource* scriptResource, struct MicroModule* microModule);
//void ScriptResource_updateNanoModule(struct ScriptResource* scriptResource, struct NanoModule* nanoModule);
// ^ This will be in other's (ActiveModule's, MicroModule's, etc) code.
#endif //ALONE_SCRIPTRESOURCE_H
