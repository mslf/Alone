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
 * @file TextParser.h
 * @author mslf
 * @date 6 OCt 2016
 * @brief File containing #TextParser and its stuff.
 */
#ifndef ALONE_TEXTPARSER_H
#define ALONE_TEXTPARSER_H

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "logger/Logger.h"
#include "resourceManager/TextResource.h"

static const char* const TextParser_standartTypeString = "type";
/**< Standard type string for all typed resources, which are loaded from #TextParser.
 * <tt>TextParser_getString(#TextParser, #TextParser_standartTypeString, 0)</tt> should return string, which is
 * represent type of #TextResource, which was parsed by #TextParser.
 */

/**
 * @brief Some initial constants for #TextParser.
 */
enum TextParser_constants{
    TP_INITIAL_NUMBER_ALLOCATED_PAIRS = 10,
    /**< Init alocating number and reallocating step for TextParser#pairsList. */
    TP_INITIAL_NUMBER_ALLOCATED_ITEMS = 5,
    /**< Init alocating number and reallocating step for RightValue#itemsList. */
    TP_INITIAL_NUMBER_ALLOCATED_SYMBOLS_FOR_LEFT_VALUE_STRING = 100,
    /**< Init alocating number and reallocating step for Pair#leftValue. */
    TP_INITIAL_NUMBER_ALLOCATED_SYMBOLS_FOR_RIGHT_VALUE_STRING = 100,
    /**< Init alocating number and reallocating step for #RightValue <B>full</B> string, while parsing. */
    TP_INITIAL_NUMBER_ALLOCATED_SYMBOLS_FOR_ITEM_STRING = 50
    /**< Init alocating number and reallocating step for each item in RightValue#itemsList. */
};

/**
 * @brief Represents right value of #Pair.
 * It can contain a lot of items (strings) for array organization.
 */
struct RightValue {
    char** itemsList;
    /**< Array of pointers to a item string. */
    size_t itemsCount;
    /**< Current number of existing items in RightValue#itemsList. */
    size_t allocatedItemsCount;
    /**< Current allocated number of items in RightValue#itemsList. */
};

/**
 * @brief Represents one assigment pair.
 */
struct Pair {
    char* leftValue;
    /**< Left part of pair assigment. Only non-spaced strings allowed. */
    struct RightValue rightValue;
    /**< Right part of pair assigment. Can be an only string or an array of strings.
     * Use quotes for strings with spaces. */
};

/**
 * @brief Error codes for #TextResource.
 */
enum TextParser_errors {
    TEXT_PARSER_NO_ERRORS = 0,
    /**< All right, no errors. */
    TEXT_PARSER_ERR_NULL_ARGUMENT = 1,
    /**< Some of function's arguments are NULL. */
    TEXT_PARSER_ERR_NO_LEFT_VALUE_IN_LIST = 2,
    /**< No such Pair#leftValue found in any #Pair in TextParser#pairsList. */
    TEXT_PARSER_ERR_CONVERTING = 3,
    /**< Converting string to some value failed. */
    TEXT_PARSER_ERR_OUT_OF_RANGE = 4,
    /**< RightValue#itemsList have only RightValue#itemsCount, but you are trying to get more. */
    TEXT_PARSER_ERR_ALLOC_STRING = 5,
    /**< Allocating memory for some string while parsing or adding or converting failed. */
    TEXT_PARSER_ERR_REALLOC_PAIRS_LIST = 6,
    /**< Allocating memory for TextParser#pairsList failed. */
    TEXT_PARSER_ERR_REALLOC_ITEMS_LIST = 7,
    /**< Allocating memory for RightValue#itemsList failed. */
    TEXT_PARSER_ERR_SPLITTING_ASSIGMENT = 8,
    /**< Splitting assigment string failed due to syntax error. */
    TEXT_PARSER_ERR_SPLITTING_ASSIGMENT_EOF = 9,
    /**< Splitting assigment string failed due to unexpected EOF. */
    TEXT_PARSER_ERR_DELETING_NOT_QUOTED_SPACES = 10,
    /**< Deleting space-symbols from right value string failed due to odd count of quotes or unexpected EOF. */
    TEXT_PARSER_ERR_PARSING_ITEMS_ARRAY_EOF = 11,
    /**< Parsing items array string failed due to unexpected EOF. */
    TEXT_PARSER_ERR_PARSING_ITEMS_ARRAY_BRACKETS = 12,
    /**< Parsing items array string failed because opening or closing square brackets haven't found. */
    TEXT_PARSER_ERR_DELETING_COMMENTS = 13
    /**< Syntax error or unexpected EOF happened while deleting comment blocks. */

};

/**
 * @brief Represents structure for containing #Pair, parsed from #TextResource.
 * Some info about parsing format:
 * @note #TextParser parses TextResource#text for assigments like this:
 * <tt>lvalue = rvalue;</tt>, and this:
 * <tt>lvalue = [item1, item2, item3, ..., itemN];</tt>
 * Also, you can quote your rvalue in quotes '"' for protecting spaces:
 * <tt>lvalue1 = "rvalue 1";
 *     lvalue2 = ["item 1", "item 2"];</tt>
 * You can add a multiline comments like in C in any place of TextResource#text (remove'_' symbol).
 * <tt>/_* comment *_/</tt>
 * @see #Pair
 */
struct TextParser {
    struct Pair* pairsList;
    /**< List of parsed or added assigments. */
    size_t pairsCount;
    /**< Current number of existing #Pair in TextParser#pairsList. */
    size_t allocatedPairsCount;
    /**< Current number os allocated #Pair in TextParser#pairsList. */
    enum TextParser_errors lastError;
    /**< Place for storing errors (if function return other useful value),
     * which are happened while doing some things with existing #TextParser. */
    const char* file;
    /**< String with ID (path) of file, which is parsed by #TextParser. Used for error messages. */
    //size_t lineNumber;
    /**< Indicates current line number. Used for error messages. */
};

/**
 * @brief Constructs and inits #TextParser from #TextResource.
 * @param logger Pointer to a #Logger for logging purpose. Can be NULL.
 * @param textResource Pointer to a #TextResource, where TextResource#text will be used for 
 * filling #TextParser with data. Can be NULL.
 * @return Pointer to an initialzed #TextParser or NULL, if something failed.
 */
struct TextParser* TextParser_constructFromTextResource(struct Logger* logger,
                                                        const struct TextResource* const textResource);

/**
 * @brief Constructs empty #TextParser without data.
 * Useful for saving some data back to text later with TextParser_convertToText().
 * @return Pointer to constructed #TextParser or NULL, if constructing failed.
 */
struct TextParser* TextParser_constructEmpty();

/**
 * @brief Destructs #TextParser and frees memory, used by it.
 * @param textParser Pointer to a #TextParser. Can be NULL. Can be not fully initialized.
 */
void TextParser_destruct(struct TextParser* textParser);

/**
 * @brief Gets RightValue#itemsCount of #Pair with given Pair#leftValue from #TextParser.
 * @param textParser Pointer to a #TextParser with needed #Pair. Can be NULL.
 * @param leftValue String which will be found in Pair#leftValue in TextParser#pairsList. Can be NULL.
 * @return RightValue#itemsCount value of found #Pair in TextParser#pairsList with given Pair#leftValue or 0 if failed.
 * @note This functions sets TextParser#lastError.
 * @see #TextParser_errors
 * @see #Pair
 * @see #RightValue
 * @see #TextParser
 */
size_t TextParser_getItemsCount(struct TextParser* textParser, const char* const leftValue);

/**
 * @brief Gets RightValue#itemsList with given index of #Pair with given Pair#leftValue from #TextParser.
 * @param textParser Pointer to a #TextParser with needed #Pair. Can be NULL.
 * @param leftValue String which will be found in Pair#leftValue in TextParser#pairsList. Can be NULL.
 * @param index Index of item in RightValue#itemsList.
 * @return RightValue#itemsList string with given index of found #Pair in TextParser#pairsList with 
 * Pair#leftValue or NULL if failed.
 * @note This functions sets TextParser#lastError.
 * @see #TextParser_errors
 * @see #Pair
 * @see #RightValue
 * @see #TextParser
 * @warning You <B>should NOT</B> destruct memory for the returned string.
 */
const char* TextParser_getString(struct TextParser* textParser, const char* const leftValue, size_t index);

/**
 * @brief Gets RightValue#itemsList with given index of #Pair with given Pair#leftValue from #TextParser
 * and then converts it to long int.
 * @param textParser Pointer to a #TextParser with needed #Pair. Can be NULL.
 * @param leftValue String which will be found in Pair#leftValue in TextParser#pairsList. Can be NULL.
 * @param index Index of item in RightValue#itemsList.
 * @return Converted to long int RightValue#itemsList string with given index of found #Pair in TextParser#pairsList with 
 * Pair#leftValue or 0 if failed.
 * @note 0 can be returned even if there is no errors.
 * @note This functions sets TextParser#lastError.
 * @see #TextParser_errors
 * @see #Pair
 * @see #RightValue
 * @see #TextParser
 */
long int TextParser_getInt(struct TextParser* textParser, const char* const leftValue, size_t index);

/**
 * @brief Gets RightValue#itemsList with given index of #Pair with given Pair#leftValue from #TextParser
 * and then converts it to double.
 * @param textParser Pointer to a #TextParser with needed #Pair. Can be NULL.
 * @param leftValue String which will be found in Pair#leftValue in TextParser#pairsList. Can be NULL.
 * @param index Index of item in RightValue#itemsList.
 * @return Converted to double RightValue#itemsList string with given index of found #Pair in TextParser#pairsList with 
 * Pair#leftValue or 0.0 if failed.
 * @note 0.0 can be returned even if there is no errors.
 * @note This functions sets TextParser#lastError.
 * @see #TextParser_errors
 * @see #Pair
 * @see #RightValue
 * @see #TextParser
 */
double TextParser_getDouble(struct TextParser* textParser, const char* leftValue, size_t index);

/**
 * @brief Gets RightValue#itemsList with given index of #Pair with given Pair#leftValue from #TextParser
 * and then converts it to bool.
 * @param textParser Pointer to a #TextParser with needed #Pair. Can be NULL.
 * @param leftValue String which will be found in Pair#leftValue in TextParser#pairsList. Can be NULL.
 * @param index Index of item in RightValue#itemsList.
 * @return Converted to bool RightValue#itemsList string with given index of found #Pair in TextParser#pairsList with 
 * Pair#leftValue or false if failed.
 * @note false can be returned even if there is no errors.
 * @note This functions sets TextParser#lastError.
 * @see #TextParser_errors
 * @see #Pair
 * @see #RightValue
 * @see #TextParser
 */
bool TextParser_getFlag(struct TextParser* textParser, const char* const leftValue, size_t index);

/**
 * @brief Adds new #RightValue item string to an existing #Pair (or creates new #Pair, if not exists) with given
 * leftValue in TextParser#pairsList .
 * @param textParser Pointer to a #TextParser where to add new #RightValue item or #Pair. Can be NULL.
 * @param leftValue String which will be found in Pair#leftValue in TextParser#pairsList. If not found,
 * then new #Pair will be created. Can be NULL.
 * @param item #RightValue item string to add. Can be NULL.
 * @return #TextParser_errors value.
 * @note This functions sets TextParser#lastError.
 * @see #TextParser_errors
 * @see #Pair
 * @see #RightValue
 * @see #TextParser
 */
enum TextParser_errors TextParser_addString(struct TextParser* textParser,
                                            const char* const leftValue,
                                            const char* const item);

/**
 * @brief Adds new #RightValue item string (converted from long int) to an existing #Pair 
 * (or creates new #Pair, if not exists) with given
 * leftValue in TextParser#pairsList .
 * @param textParser Pointer to a #TextParser where to add new #RightValue item or #Pair. Can be NULL.
 * @param leftValue String which will be found in Pair#leftValue in TextParser#pairsList. If not found,
 * then new #Pair will be created. Can be NULL.
 * @param item #RightValue item string (converted to long int) to add. Can be NULL.
 * @return #TextParser_errors value.
 * @note This functions sets TextParser#lastError.
 * @see #TextParser_errors
 * @see #Pair
 * @see #RightValue
 * @see #TextParser
 */
enum TextParser_errors TextParser_addInt(struct TextParser* textParser,
                                         const char* const leftValue,
                                         long int item);

/**
 * @brief Adds new #RightValue item string (converted from double) to an existing #Pair 
 * (or creates new #Pair, if not exists) with given
 * leftValue in TextParser#pairsList .
 * @param textParser Pointer to a #TextParser where to add new #RightValue item or #Pair. Can be NULL.
 * @param leftValue String which will be found in Pair#leftValue in TextParser#pairsList. If not found,
 * then new #Pair will be created. Can be NULL.
 * @param item #RightValue item string (converted to double) to add. Can be NULL.
 * @return #TextParser_errors value.
 * @note This functions sets TextParser#lastError.
 * @see #TextParser_errors
 * @see #Pair
 * @see #RightValue
 * @see #TextParser
 */
enum TextParser_errors TextParser_addDouble(struct TextParser* textParser,
                                            const char* leftValue,
                                            double item);

/**
 * @brief Adds new #RightValue item string (converted from bool) to an existing #Pair 
 * (or creates new #Pair, if not exists) with given
 * leftValue in TextParser#pairsList .
 * @param textParser Pointer to a #TextParser where to add new #RightValue item or #Pair. Can be NULL.
 * @param leftValue String which will be found in Pair#leftValue in TextParser#pairsList. If not found,
 * then new #Pair will be created. Can be NULL.
 * @param item #RightValue item string (converted to bool) to add. Can be NULL.
 * @return #TextParser_errors value.
 * @note This functions sets TextParser#lastError.
 * @see #TextParser_errors
 * @see #Pair
 * @see #RightValue
 * @see #TextParser
 */
enum TextParser_errors TextParser_addFlag(struct TextParser* textParser,
                                          const char* const leftValue,
                                          bool item);

/**
 * @brief Converts all #Pair in TextParser#pairsList to the plain text.
 * @param textParser Pointer to a #TextParser to be converted. Can be NULL.
 * @return String with plain text, converted from #TextParser or NULL, if something failed.
 * @warning You <B>should</B> destruct memory for the returned string.
 */
char* TextParser_convertToText(struct TextParser* textParser);

#endif //ALONE_TEXTPARSER_H
