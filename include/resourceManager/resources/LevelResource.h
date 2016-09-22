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
#ifndef ALONE_LEVELRESOURCE_H
#define ALONE_LEVELRESOURCE_H

#include <list>
#include "Resource.h"

struct MacroTile {
    std::string macroModuleResourceId;
    int x;
    int y;
    bool z; //Background tile (0) or not (1)
};

class LevelResource : Resource{
public:
    LevelResource();
    ~LevelResource();

    bool loadResource(std::string path);
    bool saveResource(std::string path);
    std::list<MacroTile>* getTilesList();
    std::list<ConnectionResource>* getConnectionResourcesList();
    bool addMacroTile(MacroTile macroTile);
    bool removeMacroTile(MacroTile macroTile);
    bool addConnectionResource(ConnectionResource connectionResource);
    bool removeConnectionResource(ConnectionResource connectionResource);
private:
    std::list<MacroTile> tilesList;
    std::list<ConnectionResource> connectionResourcesList;
};


#endif //ALONE_LEVELRESOURCE_H
