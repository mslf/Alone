//
// Created by mslf on 8/16/16.
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
#ifndef ALONE_LISTBOXRESOURCE_H
#define ALONE_LISTBOXRESOURCE_H

#include <list>
#include <SDL2/SDL.h>
#include "GuiResource.h"

struct ListBoxInfoStruct : GuiInfoStruct {
    std::string pathToImage;
    int frameWidth;
    int frameHeigh;
    SDL_Texture* texture;
    std::list <std::string> pressedStateEventsList;
    std::list <std::string> labelsList;
    int activeElementIndex;
    bool isSounded;
    std::string focusedStateSoundResourceId;
    std::string pressedStateSoundResourceId;
};

class ListBoxResource : GuiResource {
public:
    ListBoxResource();
    ~ListBoxResource();

    bool loadResource (std::string path);
    bool saveResource (std::string path);
    ListBoxInfoStruct* getGuiInfoStruct();
    void setGuiInfoStruct (ListBoxInfoStruct* listBoxInfoStruct);
private:
    ListBoxInfoStruct listBoxInfoStruct;
};


#endif //ALONE_LISTBOXRESOURCE_H
