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
#include "OLog.hpp"

/*** UTF8 char #defines ***/

#define OCHARBYTES 5

#define CHARBIT_UTF8_MB (0x80)
#define CHARBIT_UTF8_1X (0x40)
#define CHARBIT_UTF8_2X (0x20)
#define CHARBIT_UTF8_3X (0x10)
#define CHARBIT_UTF8_4X (0x8)
#define CHARBIT_UTF8_5X (0x4)
#define CHARBIT_UTF8_6X (0x2) /* Invalid! Only used for calculation. */

/*** String #defines *///

/* If any String lookup function returns this, it means the lookup failed. */
#define OSTRING_NOTFOUND ULONG_MAX

namespace Orion{
/*** Characters ***/
	/* An enumeration of UTF8 states for a given byte. */
	enum OUnicodeType : char{
		/* The byte is comprised entirely of zeros; a NULL terminator. */
		OUNI_NULL=-1,
		/* The byte is ASCII, and should not be treated as unicode. */
		OUNI_ASCII,
		/* The byte is a UTF8 continuation byte. */
		OUNI_CONTINUE,
		/* The byte is a UTF8 header byte that calls for one extra continuation byte. */
		OUNI_HEADER_1X,
		/* The byte is a UTF8 header byte that calls for two extra continuation bytes. */
		OUNI_HEADER_2X,
		/* The byte is a UTF8 header byte that calls for three extra continuation bytes. */
		OUNI_HEADER_3X,
		/* The byte is a UTF8 header byte that calls for four extra continuation bytes. */
		OUNI_HEADER_4X,
		/* The byte is a UTF8 header byte that calls for five extra continuation bytes. */
		OUNI_HEADER_5X,
		/* The byte is of an unknown type. */
		OUNI_UNKNOWN
	};
	/* An Orion-Native wrapper for individual Unicode characters. */
	struct OChar : CLoggable{
		/* Choose which way you wish to interpret the character data. */
		union{
			/* Gets the character data as single-byte (first index), like an ordinary character. Only for non-Unicode Chars. */
			char          asSingleByte;
			/* Gets the character data as a multi-byte, like a string. Only for Unicode Chars. */
			unsigned char asMultiByte[OCHARBYTES];
		}get;
		OUnicodeType  bd[OCHARBYTES];
		unsigned char byteCount;
		bool          isUnicode;

		/* Empty constructor. Sets all values to 0. */
		inline OChar(void) : get{0}, bd{OUNI_NULL,OUNI_NULL,OUNI_NULL,OUNI_NULL,OUNI_NULL}, byteCount{0}, isUnicode{0} {}
		/* Sets this Char's contents to the given multi-byte character [string]. */
		OChar(const char*); void setTo(const char*); OChar& operator=(const char*);
		/* Sets this Char's contents to the given single-byte character. */
		OChar(char); void setTo(char); OChar& operator=(char);

		/* Clears this Char's contents. */
		inline void clear(void){ byteCount=0; isUnicode=0; for(unsigned char i=0;i<OCHARBYTES;i++){ get.asMultiByte[i]=0; bd[i]=OUNI_NULL; } }

		/* Converts this Char into a standard single-byte character, or the first byte of the multi-byte character if this is Unicode. */
		inline operator char(void)       { return get.asSingleByte; }
		/* Converts this Char into a String, or nothing if this is not Unicode. */
		inline operator const char*(void){ return ( (isUnicode) ? (const char*)get.asMultiByte : 0 ); }

		/* Logs this Char's contents out to the terminal. Pass true to get byte and bit information. */
		virtual void log(bool verbose=false) override;
	};

/*** Strings ***/
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
