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
#ifndef ALONE_LEVEL_H
#define ALONE_LEVEL_H

#include "scene/SceneNode.h"
#include "resourceManager/ResourceManager.h"
#include "module/macro/BackgroundModule.h"
#include "module/macro/ActiveModule.h"
#include "module/Module.h"

struct  Level {

};
/*
class Level : SceneNode {
public:
    Level(LevelResource* levelResource, ResourceManager* resourceManager);
    ~Level();

    Graphics* getGraphics();
    void updateGraphics();
    int isGraphicsUpdatable();
    void setCoordinate(int x, int y);
    void setControlling(std::list <ControllerEvent*>* controllerEventsList);
    void setEvents(std::list <GameEvent*>* gameEventsList);
    std::list <GameEvent*>* getEvents();
    std::list <Sound*>* getSoundEvents();
    bool addModule(MacroModule* macroModule, bool isActive);
    bool removeModule(MacroModule* macroModule, bool isActive);
    bool addConnection(Connection connection);
    bool removeConnection(Connection connection);
private:
    std::list <BackgroundModule*> backgroundModulesList;
    std::list <ActiveModule*> activeModulesList;
    std::list <Connection> connectionsList;
};*/


#endif //ALONE_LEVEL_H
