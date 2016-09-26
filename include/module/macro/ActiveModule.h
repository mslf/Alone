//
// Created by mslf on 8/21/16.
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
#ifndef ALONE_ACTIVEMODULE_H
#define ALONE_ACTIVEMODULE_H

#include <SDL2/SDL.h>
#include "lua.h"
#include "resourceManager/ResourceManager.h"
#include "resourceManager/TextureResource.h"
#include "resourceManager/SoundResource.h"
#include "module/section/MacroSection.h"
#include "module/section/MicroSection.h"
#include "module/section/NanoSection.h"
#include "module/MicroModule.h"

struct ActiveModule {
    SDL_Point coordinates;
    double scaleX;
    double scaleY;
    struct TextureResource* textureResource;
    struct MacroSection** macroSectionsList;
    struct SoundResource** soundResourcesList;
    struct MicroModule** microModulesList;
    
};
/*
class ActiveModule : MacroModule {
public:
    ActiveModule(MacroModuleResource* macroModuleResource, ResourceManager* resourceManager);
    ~ActiveModule();

    Graphics* getGraphics();
    void updateGraphics();
    int isGraphicsUpdatable();
    void setCoordinate(int x, int y);
    void setControlling(std::list <ControllerEvent*>* controllerEventsList);
    std::list <Sound*>* getSoundEvents();
    //void setSection(int index, MacroSection* macroSection);
    MacroSection* getSection(int index);
    int getSectionsCount();
    void Cycle();
    bool addModule(MicroModule* microModule);
    bool removeModule(MicroModule* microModule);
    bool addConnection(Connection connection);
    bool removeConnection(Connection connection);
private:
    std::list <MacroSection*> sectionsList;
    std::list <Sound*> soundsList;
    bool isCollisionsActive;
    std::list <MicroModule*> microModulesList;
    std::list <Connection> connectionsList;
};*/


#endif //ALONE_ACTIVEMODULE_H
