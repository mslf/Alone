//
// Created by mslf on 9/26/16.
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
#ifndef ALONE_SCRIPTRESOURCE_H
#define ALONE_SCRIPTRESOURCE_H

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
//#include "module/macro/ActiveModule.h"

struct ScriptResource {
    lua_State* luaState;
};

struct ScriptResource* ScriptResource_construct(const char* const path);
void ScriptResource_destruct(struct ScriptResource* scriptResource);

void ScriptResource_registerActiveModuleFunctions(struct ScriptResource* scriptResource);
void ScriptResource_registerMicroModuleFunctions(struct ScriptResource* scriptResource);
void ScriptResource_registerNanoModuleFunctions(struct ScriptResource* scriptResource);
void ScriptResource_registerEventControllerFunctions(struct ScriptResource* scriptResource);
//void ScriptResource_updateActiveModule(struct ScriptResource* scriptResource, struct ActiveModule* activeModule);
//void ScriptResource_updateMicroModule(struct ScriptResource* scriptResource, struct MicroModule* microModule);
//void ScriptResource_updateNanoModule(struct ScriptResource* scriptResource, struct NanoModule* nanoModule);

#endif //ALONE_SCRIPTRESOURCE_H
