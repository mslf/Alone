//
// Created by mslf on 9/27/16.
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
#include "resourceManager/ScriptResource.h"
#include <stdlib.h>
#include "string.h"

struct ScriptResource* ScriptResource_construct(const char* const path) {
    if (!path)
        return  NULL;
    struct ScriptResource* scriptResource = NULL;
    scriptResource = (struct ScriptResource*)calloc(1, sizeof(struct ScriptResource));
    if (!scriptResource)
        return NULL;
    scriptResource->id = (char*)malloc(sizeof(char) * (strlen(path) + 1));
    if (!scriptResource->id) {
        ScriptResource_destruct(scriptResource);
        return NULL;
    }
    strcpy(scriptResource->id, path);
    scriptResource->luaState = luaL_newstate();
    if (!scriptResource->luaState) {
        ScriptResource_destruct(scriptResource);
        return NULL;
    }
    luaL_openlibs(scriptResource->luaState);
    if (luaL_loadfile(scriptResource->luaState, path)) {
        ScriptResource_destruct(scriptResource);
        return NULL;
    }
    scriptResource->pointersCount = 1;
    return scriptResource;
}

void ScriptResource_destruct(struct ScriptResource* scriptResource) {
    if (scriptResource) {
        if (scriptResource->id)
            free(scriptResource->id);
        if (scriptResource->luaState)
            lua_close(scriptResource->luaState);
        free(scriptResource);
    }
}
