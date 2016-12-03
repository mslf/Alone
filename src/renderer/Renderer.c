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
/**
 * @file Renderer.c
 * @author mslf
 * @date 10 Aug 2016
 * @brief File containing implementation of #Renderer.
 */
#include <settings/Settings.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "renderer/Renderer.h"

/**
 * @brief Error message strings for #Renderer.
 */
static const struct Renderer_errorMessages {
    const char* const errSdlInitializingVideo;
    /**< Will be displayed when initializing SDL video subsystem failed. */
    const char* const errAlloc;
    /**< Will be displayed when allocating memory for #Renderer failed. */
    const char* const errCreatingWindow;
    /**< Will be displayed when creating Renderer#window failed. */
    const char* const errSdlRendererCreating;
    /**< Will be displayed when creating Renderer#renderer failed. */
    const char* const errSdlImgInitializing;
    /**< Will be displayed when initializing SDL_image failed. */
    const char* const errSdlTtfInitializing;
    /**< Will be displayed when initializing SDL_ttf failed. */
}Renderer_errorMessages = {
    "Renderer_construct: SDL_Init video failed!",
    "Renderer_construct: allocating memory failed!",
    "Renderer_construct: window creating failed!",
    "Renderer_construct: SDL renderer creating failed!",
    "Renderer_construct: IMG_Init failed!",
    "Renderer_construct: TTF_Init failed!"};

struct Renderer* Renderer_construct(struct Logger* logger, struct Settings* settings) {
    if (!settings)
        return NULL;
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        Logger_log(logger, "%s SDL error: %s", Renderer_errorMessages.errSdlInitializingVideo, SDL_GetError());
        return NULL;
    }
    struct Renderer* renderer = NULL;
    renderer = (struct Renderer*)malloc(sizeof(struct Renderer));
    if (!renderer) {
        Logger_log(logger, Renderer_errorMessages.errAlloc);
        Renderer_destruct(renderer);
        return NULL;
    }
    renderer->window = SDL_CreateWindow("Alone", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                        (int)settings->w, (int)settings->h,
                                        (Uint32)settings->isFullscreen * SDL_WINDOW_FULLSCREEN);
    if (!renderer->window) {
        Logger_log(logger, "%s SDL error: %s", Renderer_errorMessages.errCreatingWindow, SDL_GetError());
        Renderer_destruct(renderer);
        return NULL;
    }
    renderer->renderer = SDL_CreateRenderer(renderer->window, -1, SDL_RENDERER_ACCELERATED |
            (Uint32)settings->isVsyncActive * SDL_RENDERER_PRESENTVSYNC);
    if (!renderer->renderer) {
        Logger_log(logger, "%s SDL error: %s", Renderer_errorMessages.errSdlRendererCreating, SDL_GetError());
        Renderer_destruct(renderer);
        return NULL;
    }
    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        Logger_log(logger, "%s SDL_image error: %s", Renderer_errorMessages.errSdlImgInitializing, IMG_GetError());
        Renderer_destruct(renderer);
        return NULL;
    }
    if(TTF_Init() == -1) {
        Logger_log(logger, "%s SDL_ttf error: %s", Renderer_errorMessages.errSdlTtfInitializing, TTF_GetError());
        Renderer_destruct(renderer);
        return NULL;
    }
    SDL_SetRenderDrawColor(renderer->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
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
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

SDL_Point Renderer_convertCoordinates(struct Renderer* renderer, SDL_Point point, SDL_Point parallax) {
    SDL_Point newPoint = {
        renderer->currentScreenSize.x * (point.x - renderer->cameraPosition.x  * parallax.x / 100) /
                renderer->virtualScreenSize.x,
        renderer->currentScreenSize.y * (point.y - renderer->cameraPosition.y * parallax.y / 100) /
                renderer->virtualScreenSize.y};
    return newPoint;
}

SDL_Point Renderer_convertCoordinatesA(struct Renderer* renderer, SDL_Point point) {
    SDL_Point newPoint = {
        renderer->currentScreenSize.x * point.x / renderer->virtualScreenSize.x,
        renderer->currentScreenSize.y * point.y / renderer->virtualScreenSize.y};
    return newPoint;
}

SDL_Point Renderer_convertCoordinatesBack(struct Renderer* renderer, SDL_Point point) {
    SDL_Point newPoint = {
        renderer->virtualScreenSize.x * point.x / renderer->currentScreenSize.x + renderer->cameraPosition.x,
        renderer->virtualScreenSize.y * point.y / renderer->currentScreenSize.y + renderer->cameraPosition.y};
    return newPoint;
}

SDL_Point Renderer_convertCoordinatesBackA(struct Renderer* renderer, SDL_Point point) {
    SDL_Point newPoint = {
        renderer->virtualScreenSize.x * point.x / renderer->currentScreenSize.x,
        renderer->virtualScreenSize.y * point.y / renderer->currentScreenSize.y};
    return newPoint;
}
