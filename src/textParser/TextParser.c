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
#include <string.h>
#include <stdio.h>
#include "textParser/TextParser.h"

const char* const TEXT_PARSER_ERR_OPERANDS_ALLOC =
        "TextParser_parseTextResource: allocating memory for operands failed!";
const char* const TEXT_PARSER_ERR_UNEXPEXTED_EOF_SPLIT_EXPRESSION =
        "TextParser_parseTextResource: unexpected end of file while splitting expression!";
const char* const TEXT_PARSER_ERR_SYNTAX_SPLIT_EXPRESSION =
        "TextParser_parseTextResource: syntax error while splitting expression!";
const char* const TEXT_PARSER_ERR_PAIRS_REALLOC =
        "TextParser_reallocatePairsList: reallocating memory for pairsList failed!";
const char* const TEXT_PARSER_ERR_ITEMS_REALLOC =
        "TextParser_reallocateItemsList: reallocating memory for itemsList failed!";
const char* const TEXT_PARSER_ERR_STRING_REALLOC =
        "TextParser_reallocateString: reallocating memory for string failed!";
const char* const TEXT_PARSER_ERR_LEFT_OPERAND_STRING_ALLOC =
        "TextParser_addPair: allocating memory for leftOperand failed!";
const char* const TEXT_PARSER_ERR_SYNTAX_QUOTES =
        "TextParser_addPair: syntax error: odd count of quotes!";
const char* const TEXT_PARSER_ERR_UNEXPEXTED_EOF_ITEMS_ARRAY_STRING =
        "TextParser_addPair: unexpected end of file while parsing items array string!";
const char* const TEXT_PARSER_ERR_SYNTAX_BRACKETS =
        "TextParser_addPair: syntax error: opening and closing square-brackets not found!";
const char* const TEXT_PARSER_ERR_ADD_ITEM =
        "TextParser_addItemToRightOperand: adding item failed!";

unsigned char TextParser_reallocatePairsList(struct Logger* logger, struct TextParser* textParser) {
    struct Pair* newPairsList = NULL;
    size_t newSize = textParser->allocatedPairsCount + INITIAL_NUMBER_ALLOCATED_ITEMS;
    size_t i;
    newPairsList = (struct Pair*)malloc(sizeof(struct Pair) * newSize);
    if (!newPairsList) {
        Logger_log(logger, TEXT_PARSER_ERR_PAIRS_REALLOC);
        return 1;
    }
    for (i = 0; i < textParser->pairsCount; i++)
        newPairsList[i] = textParser->pairsList[i];
    free(textParser->pairsList);
    textParser->pairsList = newPairsList;
    textParser->allocatedPairsCount = newSize;
    return 0;
}

unsigned char TextParser_reallocateItemsList(struct Logger* logger, struct RightOperand* rightOperand) {
    char** newItemsList = NULL;
    size_t newSize = rightOperand->allocatedItemsCount + INITIAL_NUMBER_ALLOCATED_ITEMS;
    size_t i;
    newItemsList = (char**)malloc(sizeof(char*) * newSize);
    if (!newItemsList) {
        Logger_log(logger, TEXT_PARSER_ERR_ITEMS_REALLOC);
        return 1;
    }
    for(i = 0; i < rightOperand->itemsCount; i++)
        newItemsList[i] = rightOperand->rightOperandItemsList[i];
    free(rightOperand->rightOperandItemsList);
    rightOperand->rightOperandItemsList = newItemsList;
    rightOperand->allocatedItemsCount = newSize;
    return  0;
}

unsigned char TextParser_reallocateString(struct Logger* logger, char** string, size_t* oldLength, size_t step) {
    char* newString = NULL;
    size_t i;
    size_t newLength = (*oldLength) + step;
    newString = (char*)malloc(sizeof(char) * newLength);
    if (!newString) {
        Logger_log(logger, TEXT_PARSER_ERR_STRING_REALLOC);
        return 1;
    }
    for (i = 0; i < (*oldLength); i++)
        newString[i] = (*string)[i];
    free((*string));
    (*string) = newString;
    (*oldLength) = newLength;
    return  0;
}

unsigned char TextParser_splitExpression(struct Logger* logger, const char* const string, size_t* startIndex, char** leftOperandString,
                                         size_t* leftCounter, size_t* allocatedCharsForLeftOperand,
                                         char** rightOperandString, size_t* rightCounter,
                                         size_t* allocatedCharsForRightOperand) {
    unsigned char state = 0;
    while ((*startIndex) < strlen(string) && state != 3 && state != 4) {
        char c = string[*startIndex];
        if ((*leftCounter) >= (*allocatedCharsForLeftOperand))
            if (TextParser_reallocateString(logger, leftOperandString, allocatedCharsForLeftOperand,
                                            INITIAL_NUMBER_ALLOCATED_SYMBOLS_FOR_LEFT_OPERAND_STRING))
                return 5;
        if ((*rightCounter) >= (*allocatedCharsForRightOperand))
            if (TextParser_reallocateString(logger, rightOperandString, allocatedCharsForRightOperand,
                                            INITIAL_NUMBER_ALLOCATED_SYMBOLS_FOR_RIGHT_OPERAND_STRING))
                return 6;
        switch (state) {
            case 0: // start
                if (c == ';'){
                    state = 4;
                    break;
                }
                if (c != ' ' && c!= '\t' && c!= '\n') {
                    state = 1;
                    (*leftOperandString)[*leftCounter] = c;
                    (*leftCounter)++;
                }
                break;
            case 1: // onLeftPart
                if (c == '=' && (*leftCounter) > 0) {
                    state = 2;
                    break;
                }
                if (c == '=' && (*leftCounter) == 0) {
                    state = 4;
                    break;
                }
                if (c == '\n' || c == ';') {
                    state = 4;
                    break;
                }
                if (c != ' ' && c != '\t') {
                    (*leftOperandString)[*leftCounter] = c;
                    (*leftCounter)++;
                }
                break;
            case 2: // onRightPart
                if (c == ';' && (*rightCounter) > 0) {
                    state = 3;
                    break;
                }
                if (c == ';' && (*rightCounter) == 0) {
                    state = 4;
                    break;
                }
                (*rightOperandString)[*rightCounter] = c;
                (*rightCounter)++;
                break;
            case 3: // end
                break;
            case 4: // syntax error
                break;
        }
        (*startIndex)++;
    }
    return state;
}

unsigned char TextParser_deleteNonQuotedSpaces(char* rightOperandString, size_t* rightCounter) {
    unsigned char state = 0;
    size_t i = 0;
    size_t j = 0;
    size_t quotesCount = 0;
    while (i < (*rightCounter)) {
        char c = rightOperandString[i];
        switch (state) {
            case 0: // notInQuotes
                if (c == '\"') {
                    state = 1;
                    quotesCount++;
                    break;
                }
                if (c == ' ' || c == '\t' || c == '\n') {
                    for (j = i; j < (*rightCounter) - 1; j++)
                        rightOperandString[j] = rightOperandString[j + 1];
                    (*rightCounter)--;
                    i--;
                }
                break;
            case 1: // inQuotes
                if (c == '\"') {
                    state = 0;
                    quotesCount++;
                    break;
                }
                if (c == '\n') {
                    for (j = i; j < (*rightCounter) - 1; j++)
                        rightOperandString[j] = rightOperandString[j + 1];
                    (*rightCounter)--;
                    i--;
                }
                break;
        }
        i++;
    }
    return (quotesCount % 2);
}

unsigned  char TextParser_addItemToRightOperand(struct Logger* logger, struct Pair* pair, const char* const itemString,
                                                size_t itemStringLength) {
    size_t i = 0;
    if (pair->rightOperand.itemsCount >=
            pair->rightOperand.allocatedItemsCount)
        if (TextParser_reallocateItemsList(logger, &(pair->rightOperand)))
            return 1;
    pair->rightOperand.rightOperandItemsList[pair->rightOperand.itemsCount] =
            (char*)malloc(sizeof(char) * (itemStringLength + 1));
    if (!pair->rightOperand.rightOperandItemsList[pair->rightOperand.itemsCount]) {
        Logger_log(logger, TEXT_PARSER_ERR_ADD_ITEM);
        return 2;
    }
    for (i = 0; i < itemStringLength; i ++)
        pair->rightOperand.rightOperandItemsList[pair->rightOperand.itemsCount][i] = itemString[i];
    pair->rightOperand.rightOperandItemsList[pair->rightOperand.itemsCount][itemStringLength] = 0;
    pair->rightOperand.itemsCount++;
    return 0;
}

unsigned char TextParser_parseItemsArrayString(struct Logger* logger, struct Pair* pair, char* itemsString,
                                               size_t itemsStringLength) {
    unsigned char state = 0;
    size_t i = 1; // start from first symbol, because zero-zymbol is '['
    char* tempItemString = NULL;
    tempItemString = (char*)malloc(sizeof(char) * INITIAL_NUMBER_ALLOCATED_SYMBOLS_FOR_ITEM_STRING);
    if (!tempItemString)
        return  1;
    size_t allocatedCharsForTempItemString = INITIAL_NUMBER_ALLOCATED_SYMBOLS_FOR_ITEM_STRING;
    size_t charsCounter = 0;
    while (i < itemsStringLength && state != 3 && state != 4) {
        char c = itemsString[i];
        if (charsCounter >= allocatedCharsForTempItemString)
            if (TextParser_reallocateString(logger, &tempItemString, &allocatedCharsForTempItemString,
                                            INITIAL_NUMBER_ALLOCATED_SYMBOLS_FOR_ITEM_STRING)) {
                free(tempItemString);
                return 5;
            }
        switch (state) {
            case 0: // buildingItemString_NotInQuotes
                if (c == '\"') {
                    state = 1;
                    break;
                }
                if (c == ']') {
                    if (charsCounter > 0) {
                        TextParser_addItemToRightOperand(logger, pair, tempItemString, charsCounter);
                        charsCounter = 0;
                        state = 3;
                    } else
                        state = 4;
                    break;
                }
                if (c == ',') {
                    TextParser_addItemToRightOperand(logger, pair, tempItemString, charsCounter);
                    charsCounter = 0;
                    break;
                }
                tempItemString[charsCounter] = c;
                charsCounter++;
                break;
            case 1: // buildingItemString_InQuotes
                if (c == '\"') {
                    if (charsCounter > 0) {
                        TextParser_addItemToRightOperand(logger, pair, tempItemString, charsCounter);
                        charsCounter = 0;
                        state = 2;
                    } else
                        state = 4;
                    break;
                }
                tempItemString[charsCounter] = c;
                charsCounter++;
                break;
            case 2: // checkingEnd
                if (c == ',') {
                    state = 0;
                    break;
                }
                if (c == ']') {
                    state = 3;
                    break;
                }
                state = 4;
                break;
            case 3: // end
                break;
            case 4: // syntax error
                break;
        }
        i++;
    }
    free(tempItemString);
    return state;
}

void TextParser_destructTempOperandStrings(char* left, char* right) {
    if (left)
        free(left);
    if (right)
        free(right);
}

unsigned char TextParser_addPair(struct Logger* logger, struct TextParser* textParser, char* leftOperandString,
                                 size_t leftCounter, char* rightOperandString, size_t rightCounter) {
    size_t j;
    // adding leftOperandString string as a leftOperand to the pair
    if (textParser->pairsCount >= textParser->allocatedPairsCount)
        if (TextParser_reallocatePairsList(logger, textParser)) {
            TextParser_destructTempOperandStrings(leftOperandString, rightOperandString);
            return  1;
        }
    textParser->pairsList[textParser->pairsCount].leftOperand = (char*)malloc(sizeof(char) * (leftCounter + 1));
    if (!textParser->pairsList[textParser->pairsCount].leftOperand) {
        Logger_log(logger, TEXT_PARSER_ERR_LEFT_OPERAND_STRING_ALLOC);
        TextParser_destructTempOperandStrings(leftOperandString, rightOperandString);
        return 2;
    }
    for (j = 0; j < leftCounter; j ++)
        textParser->pairsList[textParser->pairsCount].leftOperand[j] =  leftOperandString[j];
    textParser->pairsList[textParser->pairsCount].leftOperand[leftCounter] = 0;
    // Working with rightOperand
    if (TextParser_deleteNonQuotedSpaces(rightOperandString, &rightCounter)) {
        Logger_log(logger, TEXT_PARSER_ERR_SYNTAX_QUOTES);
        TextParser_destructTempOperandStrings(leftOperandString, rightOperandString);
        return 3;
    }
    if (rightOperandString[0] == '[' && rightOperandString[rightCounter - 1] == ']') {
        if (TextParser_parseItemsArrayString(logger, &(textParser->pairsList[textParser->pairsCount]),
                                             rightOperandString, rightCounter) != 3) {
            Logger_log(logger, TEXT_PARSER_ERR_UNEXPEXTED_EOF_ITEMS_ARRAY_STRING);
            TextParser_destructTempOperandStrings(leftOperandString, rightOperandString);
            return 4;
        }
    } else { // adding rightOperandString as a first item to the pair
        if (rightOperandString[0] == '[' || rightOperandString[rightCounter - 1] == ']') {
            Logger_log(logger, TEXT_PARSER_ERR_SYNTAX_BRACKETS);
            TextParser_destructTempOperandStrings(leftOperandString, rightOperandString);
            return 5;
        }
        if (TextParser_addItemToRightOperand(logger, &(textParser->pairsList[textParser->pairsCount]),
                                             rightOperandString, rightCounter)) {
            TextParser_destructTempOperandStrings(leftOperandString, rightOperandString);
            return 6;
        }
    }
    textParser->pairsCount++;
    return 0;
}

unsigned char TextParser_parseTextResource(struct Logger* logger, struct TextParser* textParser,
                                           const struct TextResource* const textResource) {
    char* leftOperandString = NULL;
    char* rightOperandString = NULL;
    leftOperandString = (char*)malloc(sizeof(char) * INITIAL_NUMBER_ALLOCATED_SYMBOLS_FOR_LEFT_OPERAND_STRING);
    rightOperandString = (char*)malloc(sizeof(char) * INITIAL_NUMBER_ALLOCATED_SYMBOLS_FOR_RIGHT_OPERAND_STRING);
    if (!leftOperandString || !rightOperandString) {
        char tempString[600];
        sprintf(tempString, "%s ResourceID: %s", TEXT_PARSER_ERR_OPERANDS_ALLOC, textResource->id);
        Logger_log(logger, tempString);
        TextParser_destructTempOperandStrings(leftOperandString, rightOperandString);
        return 1;
    }
    size_t allocatedCharsForLeftOperand = INITIAL_NUMBER_ALLOCATED_SYMBOLS_FOR_LEFT_OPERAND_STRING;
    size_t allocatedCharsForRightOperand = INITIAL_NUMBER_ALLOCATED_SYMBOLS_FOR_RIGHT_OPERAND_STRING;
    size_t leftCounter = 0;
    size_t rightCounter = 0;
    size_t i = 0;
    size_t j = 0;
    unsigned char state = 0;
    while (i < strlen(textResource->text)) {
        leftCounter = 0;
        rightCounter = 0;
        state = TextParser_splitExpression(logger, textResource->text, &i, &leftOperandString, &leftCounter,
                                           &allocatedCharsForLeftOperand, &rightOperandString, &rightCounter,
                                           &allocatedCharsForRightOperand);

        if (state != 3 && state != 4 && state != 0) {
            char tempString[600];
            sprintf(tempString, "%s ResourceID: %s", TEXT_PARSER_ERR_UNEXPEXTED_EOF_SPLIT_EXPRESSION, textResource->id);
            Logger_log(logger, tempString);
            TextParser_destructTempOperandStrings(leftOperandString, rightOperandString);
            return 2;
        }
        if (state == 4) {
            char tempString[600];
            sprintf(tempString, "%s ResourceID: %s", TEXT_PARSER_ERR_SYNTAX_SPLIT_EXPRESSION, textResource->id);
            Logger_log(logger, tempString);
            TextParser_destructTempOperandStrings(leftOperandString, rightOperandString);
            return 3;
        }
        if (state == 3) {
            unsigned char result = TextParser_addPair(logger, textParser, leftOperandString, leftCounter,
                                                      rightOperandString, rightCounter);
            if (result) {
                char tempString[600];
                sprintf(tempString, "\t in ResourceID: %s", textResource->id);
                Logger_log(logger, tempString);
                return (result + 3);
            }

        }
    }
    TextParser_destructTempOperandStrings(leftOperandString, rightOperandString);
    return 0;
}

struct TextParser* TextParser_constructFromTextResource(struct Logger* logger,
                                                        const struct TextResource* const textResource) {
    struct TextParser* textParser = NULL;
    textParser = TextParser_constructEmpty();
    if (!textParser || !textResource)
        return NULL;
    if (TextParser_parseTextResource(logger, textParser, textResource)) {
        TextParser_destruct(textParser);
        return  NULL;
    }
    return textParser;
}

struct TextParser* TextParser_constructEmpty() {
    struct TextParser* textParser = NULL;
    textParser = (struct TextParser*)calloc(1, sizeof(struct TextParser));
    if (!textParser)
        return NULL;
    textParser->pairsCount = 0;
    textParser->pairsList = (struct Pair*)malloc(sizeof(struct Pair) * INITIAL_NUMBER_ALLOCATED_PAIRS);
    if (!textParser->pairsList) {
        TextParser_destruct(textParser);
        return NULL;
    }
    textParser->allocatedPairsCount = INITIAL_NUMBER_ALLOCATED_PAIRS;
    size_t i;
    for (i = 0; i < textParser->allocatedPairsCount; i++){
        textParser->pairsList[i].rightOperand.itemsCount = 0;
        if (!(textParser->pairsList[i].rightOperand.rightOperandItemsList =
                    (char**)malloc(sizeof(char*) * INITIAL_NUMBER_ALLOCATED_ITEMS))) {
            TextParser_destruct(textParser);
            return NULL;
        }
        textParser->pairsList[i].rightOperand.allocatedItemsCount = INITIAL_NUMBER_ALLOCATED_ITEMS;
    }
    textParser->lastError = NoError;
    return textParser;
}

void TextParser_destruct(struct TextParser* textParser) {
    if (!textParser)
        return;
    if (textParser->pairsList) {
        size_t i;
        size_t j;
        for (i = 0; i < textParser->pairsCount; i++){
            free(textParser->pairsList[i].leftOperand);
            if (textParser->pairsList[i].rightOperand.rightOperandItemsList)
                for (j = 0; j < textParser->pairsList[i].rightOperand.itemsCount; j++)
                    free(textParser->pairsList[i].rightOperand.rightOperandItemsList[j]);
            free(textParser->pairsList[i].rightOperand.rightOperandItemsList);
        }
        free(textParser->pairsList);
    }
    free(textParser);
}

size_t TextParser_getItemsCount(struct TextParser* textParser, const char* const leftOperand) {
    if (!textParser && !leftOperand)
        return 0;
    size_t i = 0;
    for (i = 0; i < textParser->pairsCount; i++)
        if (strcmp(leftOperand, textParser->pairsList[i].leftOperand) == 0) {
            textParser->lastError = NoError;
            return textParser->pairsList[i].rightOperand.itemsCount;
        }
    textParser->lastError = NoLeftOperandError;
    return 0;
}

char* TextParser_getString(struct TextParser* textParser, const char* const leftOperand, size_t index) {
    if (!textParser && !leftOperand)
        return NULL;
    size_t i = 0;
    unsigned char found = 0;
    size_t foundIndex = 0;
    for (i = 0; i < textParser->pairsCount; i++)
        if (strcmp(leftOperand, textParser->pairsList[i].leftOperand) == 0) {
            found = 1;
            foundIndex = i;
            break;
        }
    if (!found) {
        textParser->lastError = NoLeftOperandError;
        return NULL;
    }
    if (index >= textParser->pairsList[foundIndex].rightOperand.itemsCount) {
        textParser->lastError = OutOfRangeError;
        return NULL;
    }
    textParser->lastError = NoError;
    return textParser->pairsList[foundIndex].rightOperand.rightOperandItemsList[index];
}

long int TextParser_getInt(struct TextParser* textParser, const char* const leftOperand, size_t index) {
    char* intString = TextParser_getString(textParser, leftOperand, index);
    if (textParser->lastError)
        return 0;
    return strtol(intString, NULL, 10);
}

double TextParser_getDouble(struct TextParser* textParser, const char* leftOperand, size_t index) {
    char* intString = TextParser_getString(textParser, leftOperand, index);
    if (textParser->lastError)
        return 0.0;
    return strtod(intString, NULL);
}

bool TextParser_getFlag(struct TextParser* textParser, const char* const leftOperand, size_t index){
    char* intString = TextParser_getString(textParser, leftOperand, index);
    if (textParser->lastError)
        return 0;
    if (strtol(intString, NULL, 10))
        return 1;
    else
        return 0;
}

unsigned char TextParser_addString(struct TextParser* textParser, const char* const leftOperand, const char* const item) {
    TextParser_getItemsCount(textParser, leftOperand); // later, we will use textParser->lastError. Don't remove this.
    size_t found = 0;
    size_t index = 0;
    size_t i = 0;
    if (textParser->lastError == NoLeftOperandError) {
        if (textParser->pairsCount >= textParser->allocatedPairsCount)
            if (TextParser_reallocatePairsList(NULL, textParser)) {
                textParser->lastError = MemoryAllocationError;
                return 1;
            }
        char* tempLeftOperandString = NULL;
        tempLeftOperandString = (char*)malloc(sizeof(char) * (strlen(leftOperand) + 1));
        if (!tempLeftOperandString) {
            textParser->lastError = MemoryAllocationError;
            return 2;
        }
        strcpy(tempLeftOperandString, leftOperand);
        textParser->pairsList[textParser->pairsCount].leftOperand = tempLeftOperandString;
        index = textParser->pairsCount;
        textParser->pairsCount++;
    } else if (textParser->lastError == NoError)
        for (i = 0; i < textParser->pairsCount; i++)
            if (strcmp(leftOperand, textParser->pairsList[i].leftOperand) == 0) {
                found = 1;
                index = i;
                break;
            }
    if (TextParser_addItemToRightOperand(NULL, &(textParser->pairsList[index]), item, strlen(item))) {
        textParser->lastError = MemoryAllocationError;
        if (!found) {
            free (textParser->pairsList[index].leftOperand);
            textParser->pairsCount--;
        }
        return 3;
    }
    textParser->lastError = NoError;
    return 0;
}

unsigned char TextParser_addInt(struct TextParser* textParser, const char* const leftOperand, long int item) {
    char tempString[100];
    sprintf(tempString, "%ld", item);
    return TextParser_addString(textParser, leftOperand, tempString);
}

unsigned char TextParser_addDouble(struct TextParser* textParser, const char* leftOperand, double item) {
    char tempString[100];
    sprintf(tempString, "%f", item);
    return TextParser_addString(textParser, leftOperand, tempString);
}

unsigned char TextParser_addFlag(struct TextParser* textParser, const char* const leftOperand, bool item) {
    char tempString[2];
    if (item)
        sprintf(tempString, "1");
    else
        sprintf(tempString, "0");
    return TextParser_addString(textParser, leftOperand, tempString);
}

unsigned char TextParser_checkWroteCounter(struct TextParser* textParser, size_t wrote, size_t* counter,
                                           size_t* allocatedLength, char** string) {
    if (wrote > 0)
        (*counter) += wrote;
    else {
        textParser->lastError = ConvertingError;
        free((*string));
        return 1;
    }
    if ((*counter) >= (*allocatedLength) / 2)
        if (TextParser_reallocateString(NULL, string, allocatedLength, (*allocatedLength))) {
            textParser->lastError = MemoryAllocationError;
            free((*string));
            return 2;
        }
    return 0;
}

char* TextParser_convertToText(struct TextParser* textParser) {
    char* tempString = NULL;
    size_t counter = 0;
    size_t allocatedLength = 5000;
    size_t i = 0;
    size_t j = 0;
    size_t wroteBuf = 0;
    tempString = (char*)malloc(sizeof(char) * allocatedLength);
    if (!tempString) {
        textParser->lastError = MemoryAllocationError;
        return NULL;
    }
    for (i = 0; i < textParser->pairsCount; i++) {
        wroteBuf = sprintf(&(tempString[counter]), "%s = ", textParser->pairsList[i].leftOperand);
        if (TextParser_checkWroteCounter(textParser, wroteBuf, &counter, &allocatedLength, &tempString))
            return  NULL;
        wroteBuf = sprintf(&(tempString[counter]), "[");
        if (TextParser_checkWroteCounter(textParser, wroteBuf, &counter, &allocatedLength, &tempString))
            return  NULL;
        for (j = 0; j < textParser->pairsList[i].rightOperand.itemsCount; j++) {
            if (j == 0)
                wroteBuf = sprintf(&(tempString[counter]), "\"%s\"",
                                textParser->pairsList[i].rightOperand.rightOperandItemsList[j]);
            else
                wroteBuf = sprintf(&(tempString[counter]), ", \"%s\"",
                                textParser->pairsList[i].rightOperand.rightOperandItemsList[j]);
            if (TextParser_checkWroteCounter(textParser, wroteBuf, &counter, &allocatedLength, &tempString))
                return  NULL;
        }
        wroteBuf = sprintf(&(tempString[counter]), "];\n");
        if (TextParser_checkWroteCounter(textParser, wroteBuf, &counter, &allocatedLength, &tempString))
            return  NULL;
    }
    return tempString;
}
