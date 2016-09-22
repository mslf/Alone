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
#ifndef ALONE_SPRITE_H
#define ALONE_SPRITE_H

#include "scene/SceneNode.h"

struct Sprite {

};
/*
class Sprite : SceneNode {
public:
    Sprite(SpriteResource* spriteResource);
    ~Sprite();

    Graphics* getGraphics();
    void updateGraphics() =0;
    int isGraphicsUpdatable() =0;
    void setCoordinate(int x, int y);
    int getAnimationsCount();
    void setAnimation(int number);
    int getFramesCount();
    void resetAnimation();

private:
    bool isAnimated;
    int animationsCount;
    int* framesCount; // Array of framesCount for each animation
};*/


#endif //ALONE_SPRITE_H
