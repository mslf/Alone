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
#include <settings/Settings.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "renderer/Renderer.h"

struct Renderer* Renderer_construct(struct Logger* logger, struct Settings* settings) {
    struct Renderer* renderer = NULL;
    if (!settings) {
        fprintf(stderr, "NULL pointer to Settings!\n");
        return NULL;
    }
    if (SDL_Init( SDL_INIT_VIDEO ) < 0) {
        fprintf(stderr, "SDL_Init video failed! SDL error: %s\n", SDL_GetError());
        return NULL;
    }
    renderer = (struct Renderer*)malloc(sizeof(struct Renderer));
    if (!renderer) {
        fprintf(stderr, "Renderer memory allocating failed!\n");
        return NULL;
    }
    renderer->window = SDL_CreateWindow("Alone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        (int)settings->w, (int)settings->h,
                                        (Uint32)settings->isFullscreen * SDL_WINDOW_FULLSCREEN);
    if (!renderer->window) {
        fprintf(stderr, "Window creating failed! SDL Error: %s\n", SDL_GetError());
        Renderer_destruct(renderer);
        return NULL;
    }
    renderer->renderer = SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED |
            (Uint32)settings->isVsyncActive * SDL_RENDERER_PRESENTVSYNC);
    if (!renderer->renderer) {
        fprintf(stderr, "SDL Renderer creating failed! SDL error: %s\n", SDL_GetError());
        Renderer_destruct(renderer);
        return NULL;
    }
    SDL_SetRenderDrawColor(renderer->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "SDL_image initializing failed! SDL_image error: %s\n", IMG_GetError());
        Renderer_destruct(renderer);
        return NULL;
    }
    renderer->cameraPosition.x = 0;
    renderer->cameraPosition.y = 0;
    renderer->currentScreenSize.x = (int)settings->w;
    renderer->currentScreenSize.y = (int)settings->h;
    renderer->virtualScreenSize.x = (int)settings->virtualW;
    renderer->virtualScreenSize.y = (int)settings->virtualH;
    renderer->logger = logger;
    return renderer;
}

void Renderer_destruct(struct Renderer* renderer) {
    if (renderer) {
        if (renderer->renderer)
            SDL_DestroyRenderer(renderer->renderer);
        if (renderer->window)
            SDL_DestroyWindow(renderer->window);
        free(renderer);
        IMG_Quit();
        SDL_Quit();
    }
}

SDL_Point Renderer_convertCoordinates(struct Renderer* renderer, SDL_Point point) {
    SDL_Point newPoint;
    newPoint.x = renderer->currentScreenSize.x * point.x / renderer->virtualScreenSize.x;
    newPoint.y = renderer->currentScreenSize.y * point.y / renderer->virtualScreenSize.y;
    return newPoint;
}