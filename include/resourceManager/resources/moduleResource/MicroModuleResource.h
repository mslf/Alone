//
// Created by mslf on 8/24/16.
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
#ifndef ALONE_MICROMODULERESOURCE_H
#define ALONE_MICROMODULERESOURCE_H

#include <list>
#include "lua.h"
#include "resourceManager/resources/moduleResource/ModuleResource.h"

struct NanoTile {
    std::string nanoModuleResourceId;
    int x;
    int y;
};

struct MicroModuleInfoStruct : ModuleInfoStruct {
    std::string pathToScript;
    lua_State* luaState;
    std::list <std::string> sectionsList;
    bool isNanoLevelExsist;
    std::list<NanoTile> tilesList;
    std::list<ConnectionResource> connectionResourcesList;
    std::list <std::string> soundsResourcesList;
};

class MicroModuleResource : ModuleResource {
public:
    MicroModuleResource();
    ~MicroModuleResource();

    MicroModuleInfoStruct* getModuleInfoStruct();
    void setModuleInfoStruct (MicroModuleInfoStruct* microModuleInfoStruct);
    bool loadResource(std::string path);
    bool saveResource(std::string path);
private:
    MicroModuleInfoStruct* microModuleInfoStruct;
};


#endif //ALONE_MICROMODULERESOURCE_H
