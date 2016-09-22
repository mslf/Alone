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
#ifndef ALONE_SCENENODE_H
#define ALONE_SCENENODE_H


#include "renderer/Graphics.h"
#include "controller/ControllerEvent.h"
#include "eventManager/GameEvent.h"
#include "musican/Sound.h"
struct SceneNode {

};
/*
class SceneNode {
public:
    virtual Graphics* getGraphics() =0;
    virtual void updateGraphics() =0;
    virtual int isGraphicsUpdatable() =0;
    virtual void setCoordinate(int x, int y) =0;
    virtual void setControlling(std::list <ControllerEvent*>* controllerEventsList) =0;
    virtual void setEvents(std::list <GameEvent*>* gameEventsList) =0;
    virtual std::list <GameEvent*>* getEvents() =0;
    virtual std::list <Sound*>* getSoundEvents() =0;

protected:
    Graphics* graphics;
    int x;
    int y;

};*/


#endif //ALONE_SCENENODE_H
