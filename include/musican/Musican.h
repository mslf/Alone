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
#ifndef ALONE_MUSICAN_H
#define ALONE_MUSICAN_H

#include "resourceManager/SoundResource.h"

struct Musican {
    unsigned char isInitialized;
    // Actually, this struct is empty. It will contain some other data later, if I need the music.
};

struct Musican* Musican_construct();
void Musican_destruct(struct Musican* musican);

void Musican_playSound(struct Musican* musican, struct SoundResource* soundResource, int loops);

#endif //ALONE_MUSICAN_H
