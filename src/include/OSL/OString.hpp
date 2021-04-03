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

#ifndef ORION_NOHELPERS

	#define CHARBIT_UTF8_MB (0x80)
	#define CHARBIT_UTF8_1X (0x40)
	#define CHARBIT_UTF8_2X (0x20)
	#define CHARBIT_UTF8_3X (0x10)
	#define CHARBIT_UTF8_4X (0x8)
	#define CHARBIT_UTF8_5X (0x4)
	#define CHARBIT_UTF8_6X (0x2) /* Invalid! Only used for calculation. */

#endif /* !ORION_NOHELPERS */

/*** String #defines ***/

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
		/* An array of OUnicodeType identifiers for each byte stored by this Char. */
		OUnicodeType  bd[OCHARBYTES];
		/* The count of bytes used by this Char. */
		unsigned char byteCount;
		/* Is this Char Unicode (multi-byte)? */
		bool          isUnicode;

		/* Empty constructor. Sets all values to 0. */
		inline OChar(void) : get{0}, bd{OUNI_NULL,OUNI_NULL,OUNI_NULL,OUNI_NULL,OUNI_NULL}, byteCount{1}, isUnicode{0} {}
		/* Sets this Char's contents to the given multi-byte character [string]. */
		OChar(const char*); void setTo(const char*); OChar& operator=(const char*);
		/* Sets this Char's contents to the given single-byte character. */
		OChar(char); void setTo(char); OChar& operator=(char);

		/* Clears this Char's contents. */
		void clear(void);

		/* Converts this Char into a standard single-byte character, or the first byte of the multi-byte character if this is Unicode. */
		inline operator char(void)       { return get.asSingleByte; }
		/* Converts this Char into a String, or nothing if this is not Unicode. */
		inline operator const char*(void){ return ( (isUnicode) ? (const char*)get.asMultiByte : 0 ); }

		/**
		 * @brief Logs this Char's contents out to the terminal.
		 * @param verbose Log verbose information (such as byte and bit information) instead of the character itself. Default is false.
		 * @param newLine Should the output be placed on a newline or append to the current one if applicable? Default is true.
		 */
		virtual void log(bool verbose=false, bool newLine=true) override;
	};

	/**
	 * @brief Analyses the UTF8 header of the given character and returns the corrisponding Unicode Header type
	 * @param eval The character to evaluate.
	 * @return One of the many possible enumerations included in OUnicodeType. Review the documentation for OUnicodeType for further details
	 */
	extern OUnicodeType OCharGetUnicodeType(char eval);

/*** Strings ***/

	/* An Orion-Native wrapper for Unicode Strings. */
	class OString : public CLoggable{
		protected:
			/* The actual String stored in memory. */
			char*  raw;
			/* A struct containing two different interpretations of this String's length. */
			struct{
				/* The apparent length of this String (without Unicode continuation bytes.) */
				uint32_t apparent;
				/* The real length of this String (with all non-null bytes.) */
				uint32_t real;
			}length;
			/* The actual memory being used by this String (including null terminator.) */
			uint32_t memuse;
			/**
			 * @brief Converts an apparent index (as is inputted in setChar and getChar) to a real index used for computation.
			 * @param index The given apparent index to convert.
			 * @return A real index that can be used directly on the internal String pointer for operations.
			 */
			uint32_t apparentToReal(uint32_t index);
		public:
			/* Destructor. Frees all memory. */
			~OString(void);
			/* Empty constructor. Sets all values to 0. */
			inline OString(void) : raw{0}, length{0,0}, memuse{0} {};
			/**
			 * @brief Initialises this String with the given text.
			 * @param text The text for this String to copy and use.
			 */
			inline OString(const char* text) : raw{0}, length{0,0}, memuse{0} { setTo(text); };
			/**
			 * @brief Frees all memory stored by this String.
			 * @return True if there was memory to free, false if could not free memory, or memory was non-existent. 
			 */
			bool clear(void);

			/**
			 * @brief Sets the exact memory (including terminator) to the given value. Truncates String if the new memory is smaller than the length.
			 * @param newSize The exact size (in bytes) that this String will use, including the terminator.
			 * @return True if memory could be changed, false if either new memory size is identical or otherwise simply could not be resized.
			 */
			bool setMemory(uint32_t newSize);
			
			/** 
			 * @brief Sets this String to the given text.
			 * @param text The text for this String to copy and use.
			 * @return True if String could be set, false if memory for the new String coould not be allocated.
			 */
			bool setTo(const char* text); OString& operator=(const char* text);
			/**
			 * @brief Appends the given text to the end of this String.
			 * @param text The text to append to this String.
			 * @return True if text could be appended, false if memory for the new String coould not be allocated.
			 */
			bool append(const char* text); OString& operator+=(const char* text);
			/**
			 * @brief Sets the character at the given index.
			 * @param c The single/multi-byte character to set.
			 * @param index The index at which to set the new character to.
			 * @return True if new character could be set, false if either the String has not been initialised, or the index is out of bounds.
			 */
			bool setChar(OChar c, uint32_t index);
			/** 
			 * @brief Same as setChar() but directly sets the character in memory. Quicker, but loses easy Unicode helpers.
			 * @param c The single-byte character to set.
			 * @param index he index at which to set the new character to.
			 */
			inline void setCharFast(char c, uint32_t index) { raw[index]=c; }

			/**
			 * @brief Returns the pointer to the actual String stored in memory.
			 * @return A pointer to the character array used by this String.
			 */
			char* getText(void) const; operator char*(void) const;
			/**
			 * @brief Gets and returns the Unicode character at the given index.
			 * Indexing operations are done based on apparent length, not actual bytes.
			 * @param index The index to locate the character at.
			 * @return An OChar, abstraction for single or multi-byte characters. See the documentation for OChar.
			 */
			OChar getChar(uint32_t index); OChar operator[](uint32_t);
			/** 
			 * @brief Same as getChar() but directly indexes the String. Quicker, but loses easy Unicode helpers.
			 * @param index The index to locate the character at.
			 * @return The single-byte character found at the index.
			 */
			inline char getCharFast(uint32_t index) { return raw[index]; }
			/**
			 * @brief Returns the length of this String.
			 * @param realLength Should this function return the actual length of this String, including the Unicode continuation bytes? Default is false.
			 * @returns The length of this String dependent on the parameter used on this method.
			 */
			uint32_t getLength(bool realLength=false);
			/**
			 * @brief Returns the memory usage of this String.
			 * @return The raw memory usage of this String, including any unused bytes.
			 * Unused bytes are kept in case any concatenation is needed.
			 */
			inline uint32_t getMemory(void) { return memuse; }

			/**
			 * @brief Is this String equal to the given String?
			 * @param text The separate String to compare.
			 * @return True if both Strings are identical, false if there are any variations.
			 */
			bool equalTo(const char* text) const; bool operator==(const char* text) const;
			/**
			 * @brief Is this String ready for use / does it point to valid memory?
			 * @return If the String's internal character array pointer is set, this will return true. Otherwise it will return false, meaning the String ahs not beben initialised.
			 */
			bool ready(void) const; operator bool(void) const;
			/**
			 * @brief Does this String contain the given substring?
			 * @param substring The substring to attempt to find within this String.
			 * @return True if substring could be found, false if it could not.
			 */
			bool contains(const char* substring);
			// /* Replaces all occurances of the given substring with the replacement string. Returns true if replacements were made. */
			// bool replace(const char* substring, const char* replacement);

			/**
			 * @brief Creates a new String that is the combination of this String and the given text.
			 * @param text The String to append to the new String
			 * @return A new, separate String with the given text appended to the end.
			 */
			OString operator+(const char* text) const;

	 		/**
			 * @brief Logs this String out to the terminal.
			 * @param verbose Log verbose information (such as memory usage and alongside the String itself. Default is false.
			 * @param newLine Should the output be placed on a newline or append to the current one if applicable? Default is true.
			 */
 			virtual void log(bool verbose=false,  bool newLine=true) override;
	};

	/**
	 * @brief Writes the formatted String to the output.
	 * @param output A pointer to a character array for the formatted String to be placed in. This MUST be able to fit the formatted String, otherwise severe errors could occur.
	 * @param format The String to be formatted into the output. The rest of the arguments can be of any type as long as they're capable of being formatted into the String.
	 * @param ... Variadic arguments. Can be of any type as long as long as they're capable of being formatted into the String.
	 */
	extern void OFormat(char* output, const char* format, ...);
	/**
	 * @brief Returns the length of the passed String. 
	 * @param string The String to get the length of.
	 * @param getApparentLength Should this function return the raw length (in bytes) of the String, or the String's apparent length (excluding UTF-8 continuation bytes)? Default is false.
	 * @return The length of the String, dependent on the mode specificed by the second parameter.
	 */
	extern size_t OStringLength(const char* string, bool getApparentLength=false);
	/**
	 * @brief Returns the starting index of the first occurance of the substring in the given String.
	 * @param string The String to attempt to search the substring in.
	 * @param substring The substring to search for in the given String.
	 * @return If the substring could be found, the index of the starting position will be returned. Otherwise, OSTRING_NOTFOUND will be returned, and an evaluation must be done.
	 */
	extern size_t OStringFindFirst(const char* string, const char* substring);
	/**
	 * @brief Returns the starting index of the last occurance of the substring in the given String.
	 * @param string The String to attempt to search the substring in.
	 * @param substring The substring to search for in the given String.
	 * @return If the substring could be found, the index of the starting position will be returned. Otherwise, OSTRING_NOTFOUND will be returned, and an evaluation must be done.
	 */
	extern size_t OStringFindLast(const char* string, const char* substring);
	/**
	 * @brief Returns the count of times the substring is found in the given string.
	 * @param string The String to attempt to search the substring in.
	 * @param substring The substring to search for in the given String.
	 * @return The number of times the substring was found in the given String.
	 */
	extern size_t OStringFindOccurances(const char* string, const char* substring);
	/**
	 * @brief Compares the two strings and returns true if they share identical bytes.
	 * @return True if both Strings are identical, false if there are any variations.
	 */
	extern bool OStringCompare(const char*, const char*);

#ifndef ORION_NOALTNAMES

	/* An enumeration of UTF8 states for a given byte. */
	typedef OUnicodeType unitype_t;
	/* An Orion-Native wrapper for individual Unicode characters. */
	typedef OChar        unichar_t;
	/* An Orion-Native wrapper for Unicode Strings. */
	typedef OString      string_t;

#endif /* !ORION_NOALTNAMES */
}
#endif /* !__ORIONAPI_OSL_OSTRING_H__ */