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
#ifndef ALONE_MACROSECTION_H
#define ALONE_MACROSECTION_H

#include <stddef.h>
#include "module/section/MicroSection.h"

enum MacroSectionType {
    ElectricityMacroSection,
    PipeMacroSection,
    InterfaceMacroSection
};

struct MacroSection {
    enum MacroSectionType type;
    char* sectionName;
    struct MicroSection** microSectionsList;
    size_t microSectionsCount;
};

struct MacroSection* MacroSection_construct(enum MacroSectionType macroSectionType, const char* const name);
void MacroSection_destruct(struct MacroSection* macroSection);

void MacroSection_addMicroSection(struct MacroSection* macroSection, struct MicroSection* microSection);
void MacroSection_removeMicroSection(struct MacroSection* macroSection, const char* const name);

#endif //ALONE_MACROSECTION_H
