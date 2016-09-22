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
#ifndef ALONE_MODULERESOURCE_H
#define ALONE_MODULERESOURCE_H

#include <SDL2/SDL.h>
#include "resourceManager/resources/Resource.h"

struct ModuleInfoStruct {
    std::string pathToImage;
    int frameWidth;
    int frameHeigh;
    SDL_Texture* texture;
};

class ModuleResource : Resource{
public:
    virtual ModuleInfoStruct* getModuleInfoStruct() =0;
    virtual void setModuleInfoStruct (ModuleInfoStruct* moduleInfoStruct) =0;
    virtual bool loadResource(std::string path) =0;
    virtual bool saveResource(std::string path) =0;

protected:

};


#endif //ALONE_MODULERESOURCE_H
