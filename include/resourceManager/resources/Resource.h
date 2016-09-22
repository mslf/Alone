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
#ifndef ALONE_RESOURCE_H
#define ALONE_RESOURCE_H

#include <string>

// Struct for storing connections resources between modules
struct ConnectionResource {
    std::string firstDeviceTileIndex;
    int indexFirstSection;
    std::string secondDeviceTileIndex;
    int indexSecondSection;
};

class Resource {
public:
    virtual bool loadResource(std::string path) =0;
    virtual bool saveResource(std::string path) =0;

private:

};


#endif //ALONE_RESOURCE_H
