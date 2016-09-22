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
#ifndef ALONE_GUIRESOURCE_H
#define ALONE_GUIRESOURCE_H

#include "resourceManager/resources/Resource.h"

struct GuiInfoStruct {};

class GuiResource : Resource{
public:
    virtual GuiInfoStruct* getGuiInfoStruct() =0;
    virtual void setGuiInfoStruct (GuiInfoStruct* guiInfoStruct) =0;

private:

};


#endif //ALONE_GUIRESOURCE_H
