//
// Created by mslf on 9/25/16.
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
#ifndef ALONE_CONNECTIONS_H
#define ALONE_CONNECTIONS_H

struct NanoConnection {
    size_t firstNanoModuleIndex;
    size_t firstNanoSectionIndex;
    size_t secondNanoModuleIndex;
    size_t secondNanoSectionIndex;
};

struct MicroConnection {
    size_t firstMicroModuleIndex;
    size_t firstMicroSectionIndex;
    size_t secondMicroModuleIndex;
    size_t secondMicroSectionIndex;
    struct NanoConnection nanoConnection;
};

struct  MacroConnection {
    size_t firstMacroModuleIndex;
    size_t firstMacroSectionIndex;
    size_t secondMacroModuleIndex;
    size_t secondMacroSectionIndex;
    struct MicroConnection microConnection;
};

#endif //ALONE_CONNECTIONS_H
