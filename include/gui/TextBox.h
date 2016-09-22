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
#ifndef ALONE_TEXTBOX_H
#define ALONE_TEXTBOX_H

#include "gui/Gui.h"
#include "eventManager/GameEvent.h"
#include "resourceManager/ResourceManager.h"
#include "musican/Sound.h"

struct TextBox {

};
/*
class TextBox : Gui {
public:
    TextBox();
    ~TextBox();

    void createGuiElementFromResource(TextBoxResource* textBoxResource, ResourceManager* resourceManager);
    Graphics* getGraphics();
    void updateGraphics();
    int isGraphicsUpdatable();
    void setCoordinate(int x, int y);
    void setControlling(std::list <ControllerEvent*>* controllerEventsList);
    //void setEvents(std::list <GameEvent*>* gameEventsList);
    std::list <GameEvent*>* getEvents();
    std::list <Sound*>* getSoundEvents();
    void setText(std::string);

private:
    bool isCreated;
    bool isSounded;
    std::string text;
    Sound* focusedStateSound;
    Sound* pressedStateSound;
    GameEvent* pressedStateEvent;
};*/


#endif //ALONE_TEXTBOX_H
