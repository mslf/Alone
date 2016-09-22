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
#ifndef ALONE_MICROMODULE_H
#define ALONE_MICROMODULE_H

#include "module/Module.h"
#include "musican/Sound.h"
#include "module/section/MicroSection.h"
#include "resourceManager/ResourceManager.h"
#include "module/NanoModule.h"

struct MicroModule {

};
/*
class MicroModule : Module{
public:
    MicroModule(MicroModuleResource* microModuleResource, ResourceManager* resourceManager);
    ~MicroModule();

    Graphics* getGraphics();
    void updateGraphics();
    int isGraphicsUpdatable();
    void setCoordinate(int x, int y);
    void setControlling(std::list <ControllerEvent*>* controllerEventsList);
    std::list <Sound*>* getSoundEvents();
    //void setSection(int index, MacroSection* macroSection);
    MicroSection* getSection(int index);
    int getSectionsCount();
    void Cycle();
    bool addModule(NanoModule* nanoModule);
    bool removeModule(NanoModule* nanoModule);
    bool addConnection(Connection connection);
    bool removeConnection(Connection connection);
private:
    std::list <MicroSection*> sectionsList;
    std::list <Sound*> soundsList;
    std::list <NanoModule*> nanoModulesList;
    std::list <Connection> connectionsList;
};*/


#endif //ALONE_MICROMODULE_H
