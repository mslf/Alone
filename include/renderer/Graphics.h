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
#ifndef ALONE_GRAPHICS_H
#define ALONE_GRAPHICS_H

#include <SDL2/SDL.h>

struct RotationInfoStruct {
        char isRotatable;
        SDL_Point* rotatePoint;
        double angle;
        SDL_RendererFlip flip;
};
struct Graphics {

};
/*
class Graphics {
public:
    Graphics(SDL_Texture* texture);
    ~Graphics();

    void setSrcSDLRect(SDL_Rect* newSrcRect);
    void setDstSDLRect(SDL_Rect* newDstRect);
    SDL_Texture* getSDLTexture();
    SDL_Rect* getSrcSDLRect();
    SDL_Rect* getDstSDLRect();
    RotationInfoStruct* getRotationInfoStruct();
    void setRotationInfoStruct(RotationInfoStruct* rotationInfoStruct);

private:
    SDL_Rect* srcRect;
    SDL_Rect* dstRect;
    SDL_Texture* texture;
    RotationInfoStruct rotationInfoStruct;

};*/


#endif //ALONE_GRAPHICS_H
