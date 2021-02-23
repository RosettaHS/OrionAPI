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

#include <sys/types.h>
#include <limits.h>
#include "CLoggable.hpp"

#define OSTRING_NOTFOUND ULONG_MAX

namespace Orion{
	/* Minimal wrapper for strings. */
	class OString : public CLoggable {
		private:
			/* Internal. The character array that makes up the string. */
			char* str;
			/* The length of the string. */
			size_t length;
			/* Internal. Was the memory dynamically allocated? */
			bool isMemStatic;
		public:
			/* Frees all memory (if dynamically allocated) and sets all values to 0. */
			~OString(void);
			/* Empty constructor. Sets all values to 0. */
			OString(void);
			/* Creates the string with the given text. */
			OString(const char*);
			/* Sets the string to the given text. */
			void setTo(const char*);
			/* Appends the new text to the current string. */
			void append(const char*);

			/* Gets the length of the current string. */
			size_t getLength(void);
			/* Logs the information of this String to the terminal. Pass true for more verbose information. */
			virtual void log(bool verbose=false) override;

			/* Overload. Converts into a char*. Example: char* test=myOString; */
			operator char*(void) const;
			/* Overload. Same as above, but with a const char* instead. */
			operator const char*(void) const;
			/* Overload. Same as append(const char*). */
			OString& operator+=(const char*);
			/* Overload. Returns a new string that is the first string with the second string appended. Example: OString newString = oldString+"Appended String!"; */
			OString operator+(const char*);
			/* Overload. Same as setTo(const char*). */
			OString& operator=(const char*);
			/* Overload. Same as OStringCompare(const char*, const char*) */
			bool operator==(const char* other);
			/* Overload. Gets the character at the given index. */
			char operator[](size_t);
	};

	/* Returns the length of the passed String. */
	extern size_t OStringLength(const char*);
	/* Returns the starting index of the first occurance of the substring (second argument) in the given string (first argument). Returns OSTRING_NOTFOUND if substring could not be found. */
	extern size_t OStringFindFirst(const char* string, const char* substring);
	/* Compares the two strings and returns true if they are identical. */
	extern bool OStringCompare(const char*, const char*);
}

#endif /* !__ORIONAPI_OSL_OSTRING_H__ */
