//
// Created by mslf on 10/6/16.
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
#ifndef ALONE_TEXTPARSER_H
#define ALONE_TEXTPARSER_H

#include <stdlib.h>
#include "resourceManager/TextResource.h"

enum {
    INITIAL_NUMBER_ALLOCATED_PAIRS = 10,
    INITIAL_NUMBER_ALLOCATED_ITEMS = 1
};
#define TEXT_PARSER_TYPE_STRING "type"

struct RightOperand {
    char** rightOperandItemsList;
    size_t itemsCount;
    size_t allocatedItemsCount;
};

struct Pair {
    char* leftOperand;
    struct RightOperand rightOperand;
};

enum TextParserError {
    NoError,
    NoLeftOperandError,
    ConvertingError,
    OutOfRangeError,
    MemoryAllocationError
};

struct TextParser {
    struct Pair* pairsList;
    size_t pairsCount;
    size_t allocatedPairsCount;
    enum TextParserError lastError;
};

struct TextParser* TextParser_constructFromTextResource(const struct TextResource* const textResource);
struct TextParser* TextParser_constructEmpty();
void TextParser_destruct(struct TextParser* textParser);

size_t TextParser_getItemsCount(struct TextParser* textParser, const char* const leftOperand);
char* TextParser_getString(struct TextParser* textParser, const char* const leftOperand, size_t index);
long int TextParser_getInt(struct TextParser* textParser, const char* const leftOperand, size_t index);
double TextParser_getDouble(struct TextParser* textParser, const char* leftOperand, size_t index);
unsigned char TextParser_getFlag(struct TextParser* textParser, const char* const leftOperand, size_t index);
unsigned char TextParser_addString(struct TextParser* textParser, const char* const leftOperand, const char* const item);
unsigned char TextParser_addInt(struct TextParser* textParser, const char* const leftOperand, long int item);
unsigned char TextParser_addDouble(struct TextParser* textParser, const char* leftOperand, double item);
unsigned char TextParser_addFlag(struct TextParser* textParser, const char* const leftOperand, unsigned char item);
char* TextParser_convertToText(struct TextParser* textParser);

#endif //ALONE_TEXTPARSER_H