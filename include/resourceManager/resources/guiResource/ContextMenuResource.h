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
#ifndef ALONE_CONTEXTMENURESOURCE_H
#define ALONE_CONTEXTMENURESOURCE_H

#include <list>
#include <SDL2/SDL.h>
#include "GuiResource.h"

struct ContextMenuInfoStruct : GuiInfoStruct {
    std::string pathToImage;
    int frameWidth;
    int frameHeigh;
    SDL_Texture* texture;
    std::list <std::string> pressedStateEventsList;
    std::list <std::string> labelsList;
    bool isSounded;
    std::string focusedStateSoundResourceId;
    std::string pressedStateSoundResourceId;
};

class ContextMenuResource : GuiResource {
public:
    ContextMenuResource();
    ~ContextMenuResource();

    bool loadResource (std::string path);
    bool saveResource (std::string path);
    ContextMenuInfoStruct* getGuiInfoStruct();
    void setGuiInfoStruct (ContextMenuInfoStruct* contextMenuInfoStruct);

private:
    ContextMenuInfoStruct contextMenuInfoStruct;
};


#endif //ALONE_CONTEXTMENURESOURCE_H
