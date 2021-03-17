/**********************************************************************************/
/*                                                                                */
/*                              Copyright (c) 2021                                */
/*                           Rosetta H&S International                            */
/*                                                                                */
/*  Permission is hereby granted, free of charge, to any person obtaining a copy  */
/*  of this software and associated documentation files (the "Software"), to deal */
/*  in the Software without restriction, including without limitation the right   */
/*    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell   */
/*       copies of the Software, and to permit persons to whom the Software is    */
/*          furnished to do so, subject to the following conditions:              */
/*                                                                                */
/* The above copyright notice and this permission notice shall be included in all */
/*                copies or substantial portions of the Software.                 */
/*                                                                                */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  */
/* SOFTWARE.                                                                      */
/*                                                                                */
/**********************************************************************************/

#ifndef __ORIONAPI_OSL_OSTRING_H__
#define __ORIONAPI_OSL_OSTRING_H__

#include "common.hpp"

#define OSTRING_NOTFOUND ULONG_MAX

namespace Orion{
	/* Writes the formatted String to the output. */
	extern void OFormat(char* output, const char* format, ...);
	/* Returns the length of the passed String. */
	extern size_t OStringLength(const char*);
	/* Returns the starting index of the first occurance of the substring (second argument) in the given string (first argument). Returns OSTRING_NOTFOUND if substring could not be found. */
	extern size_t OStringFindFirst(const char* string, const char* substring);
	/* Returns the starting index of the last occurance of the substring (second argument) in the given string (first argument). Returns OSTRING_NOTFOUND if substring could not be found. */
	extern size_t OStringFindLast(const char* string, const char* substring);
	/* Compares the two strings and returns true if they share identical bytes. */
	extern bool OStringCompare(const char*, const char*);
}

#endif /* !__ORIONAPI_OSL_OSTRING_H__ */
