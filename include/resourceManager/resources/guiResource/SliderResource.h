//
// Created by mslf on 8/13/16.
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
#ifndef ALONE_SLIDERRESOURCE_H
#define ALONE_SLIDERRESOURCE_H

#include <SDL2/SDL.h>
#include "GuiResource.h"

struct SliderInfoStruct : GuiInfoStruct {
    std::string pathToImage;
    int frameWidth;
    int frameHeigh;
    SDL_Texture* texture;
    std::string pressedStateEvent;
    bool isSounded;
    std::string focusedStateSoundResourceId;
    std::string pressedStateSoundResourceId;
};

class SliderResource : GuiResource {
public:
    SliderResource();
    ~SliderResource();

    bool loadResource (std::string path);
    bool saveResource (std::string path);
    SliderInfoStruct* getGuiInfoStruct();
    void setGuiInfoStruct (SliderInfoStruct* sliderInfoStruct);

private:
    SliderInfoStruct sliderInfoStruct;
};


#endif //ALONE_SLIDERRESOURCE_H
