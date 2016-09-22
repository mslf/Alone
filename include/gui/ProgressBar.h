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
#ifndef ALONE_PROGRESSBAR_H
#define ALONE_PROGRESSBAR_H

#include "gui/Gui.h"
#include "eventManager/GameEvent.h"
#include "resourceManager/ResourceManager.h"
#include "musican/Sound.h"

struct  ProgressBar {

};
/*
class ProgressBar : Gui {
public:
    ProgressBar();
    ~ProgressBar();

    void createGuiElementFromResource(ProgressBarResource* progressBarResource, ResourceManager* resourceManager);
    Graphics* getGraphics();
    void updateGraphics();
    int isGraphicsUpdatable();
    void setCoordinate(int x, int y);
    //void setEvents(std::list <GameEvent*>* gameEventsList);
    void setValue(int value);

private:
    bool isCreated;
    int value;

};*/


#endif //ALONE_PROGRESSBAR_H
