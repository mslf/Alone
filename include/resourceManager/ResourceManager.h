//
// Created by mslf on 8/10/16.
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
#ifndef ALONE_RESOURCEMANAGER_H
#define ALONE_RESOURCEMANAGER_H

#include "TextureResource.h"
#include "TextResource.h"
#include "SoundResource.h"

struct ResourceManager {
    char** textureResourcesIDs;
    char** textResourcesIDs;
    char** soundResourcesIDs;
    struct TextureResource** textureResources;
    struct TextResource** textResources;
    struct SoundResource** soundResources;
};
/*
class ResourceManager {
public:
    ResourceManager();
    ~ResourceManager();

    bool loadResources(ResourceType type);
    bool saveResource (std::string id);
    Resource* getResource(std::string id);
    bool setResource (std::string id, Resource* resource);
    bool addResource (std::string id, Resource* resource);
    void deleteResource (std::string id);

private:
    std::unordered_map <std::string, Resource*> resourcesList;
};
*/

#endif //ALONE_RESOURCEMANAGER_H
