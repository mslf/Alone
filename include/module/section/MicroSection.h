//
// Created by mslf on 8/23/16.
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
#ifndef ALONE_MICROSECTION_H
#define ALONE_MICROSECTION_H

#include <stddef.h>
#include "module/section/NanoSection.h"

enum MicroSectionType {
    ElectricityMicroSection,
    MCIMicroSection,
    SCIMicroSection,
    MDIMicroSection,
    SDIMicroSection,
    PipeMicroSection
};

struct MicroSection {
    enum MicroSectionType type;
    char* sectionName;
    char* pipeResourceName;
    double pipeResourceHasCount;
    double pipeResourceWantsCount;
    struct NanoSection** nanoSectionsList;
    size_t nanoSectionsCount;
};

struct MicroSection* MicroSection_construct(enum MicroSectionType microSectionType,const char* const pipeResName,
                                            const char* const name);
void MicroSection_destruct(struct MicroSection* microSection);

void MicroSection_addNanoSection(struct MicroSection* microSection, struct NanoSection* nanoSection);
void MicroSection_removeNanoSection(struct MicroSection* microSection, const char* const name);
void MicroSection_setPipeResource(struct MicroSection* microSection, const char* const pipeResourceName, size_t count);

#endif //ALONE_MICROSECTION_H
