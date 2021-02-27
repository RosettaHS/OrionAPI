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

#ifndef __ORIONAPI_OSL_OFILE_H__
#define __ORIONAPI_OSL_OFILE_H__

#include <stdint.h>
#include "../application.hpp"
#include "CLoggable.hpp"

namespace Orion{
	/* An enumeration of generic file types. */
	enum OFileType : char{
		OFT_UNKNOWN=-1,
		OFT_ERROR,
		OFT_TEXT,
		OFT_CONFIG,
		OFT_IMAGE,
		OFT_VIDEO,
		OFT_FONT,
		OFT_BINARY
	};

	/* Possible file actions. */
	enum OFileAction : char{
		/* Open an existing file for both reading and writing. */
		OFILE_OPEN,
		/* Open an existing file for reading only. Useful for files where you do not have write privileges. */
		OFILE_OPEN_READONLY,
		/* Creates a file (or overrides an existing one) for both reading and writing. */
		OFILE_NEW,
		/* Creates a file (or overrides an existing one) for writing only. */
		OFILE_NEW_WRITEONLY
	};

	/* An individual Line of a File. */
	struct OFileLine{
		/* The length of this Line. */
		size_t length;
		/* The string stored in this Line. */
		int* str;

		/* Allows for if() checking on this Line. */
		operator bool(void) const;
		/* Allows for getting a character from a given index without having to use the "str" member. */
		int operator [](size_t) const;
		/* Allows for converting to a string. */
		operator char*(void) const;
	};

	/* The internal Content of a FIle. */
	struct OFileContent{
		/* The total count of lines in this File (starting at 1). */
		size_t lineCount;
		/* The total count of characters in this File. */
		size_t charCount;
		/* The actual lines of this File. */
		OFileLine* lines;
		/* Have the contents been modified from when changes were last applied, or when the File was last opened? */
		bool modified;

		/* Allows for if() checking on this Content without having to check "lines" member manually. */
		operator bool(void) const;
		/* Allows for getting a Line from a given index without having to use the "lines" member. */
		OFileLine operator [](size_t) const;
	};

	/* A simple numeric hash of a File's contents. */
	typedef uint64_t OFileHash;

	/* An Orion-Native implementation for easily reading and writing to files. */
	class OFile : public CLoggable {
		protected:
			/* The type of the File. */
			OFileType type;
			/* The action which was used for this File. */
			OFileAction action;
			/* Important, internal information regarding the File. */
			struct{
				/* The path to the File itself. */
				char* PATH;
				/* A pointer to the C FILE struct. */
				void* RAW;
				/* The descriptor of this File that the operating system uses to locate it. */
				int DESC;
				/* The size of the File in bytes. */
				size_t SIZE;
				/* A simple hash of the File. Used for internal comparisons between Files. */
				OFileHash HASH;
			}FILEINF;
			/* Miscellaneous information regarding the File. */
			struct{
				/* The name of the File (plus extension if it has one). */
				char* name;
				/* The raw extension of the File (if it has one) */
				char* ext;
				/* Should this File care about allocating and initialising miscellaneous information such as the name, extension, and type? */
				bool careAboutMisc;
			}misc;
			/* The internal contents of this File. */
			OFileContent contents;

			/* Initialises most of the internal variables. */
			void init(void);
			/* Stores the File's contents to memory. Handled during init(). */
			void storeToMem(void);
		public:
			/* Destructor. Frees all allocated memory and closes the file. */
			~OFile(void);
			/* Empty constructor. Sets all values to 0. */
			OFile(void);

			/* Opens the given File relative to the OApp's working directory with the given action. */
			bool open(const char* file, OFileAction=OFILE_OPEN); OFile(const char* file, OFileAction=OFILE_OPEN);
			/* Opens the File relative to the given directory with the given action. */
			bool open(const char* directory, const char* file, OFileAction=OFILE_OPEN); OFile(const char* directory, const char* file, OFileAction=OFILE_OPEN);
			/* Applies the current modifications. Returns true if there were any modifications to apply, and the process was a success. */
			bool apply(void);
			/* Closes the File and if true is passed, applies any pending modifications. */
			bool close(bool applyChanges);

			/* Has the File been opened properly, and is ready for use? */
			bool valid(void) const; operator bool(void) const;
			/*
			 * Should this File care about allocating and initialising miscellaneous information?
			 * These operations are intensive so if you need to open dozens of files every second, set this to false. Default is true.
			 */
			void shouldInitMisc(bool);
			/* Has the File been modified since when changes were last applied, or when the File was last opened? */
			bool hasBeenModified(void) const;
			/* Do the two Files share the same content? */
			bool equalTo(OFile&) const; bool operator==(OFile&) const;

			/* Returns the type of the File if it could be determined. */
			OFileType getType(void) const;
			/* Returns the full path pointing to this File. */
			const char* getFullPath(void) const;
			/* Returns the name of this File. */
			const char* getName(void) const;
			/* Returns the extension of this File (if it has one). */
			const char* getExtension(void) const;
			/* Returns the size of the File in bytes. */
			size_t getSize(void) const;
			/* Returns the simplistic numerical hash of this File. */
			OFileHash getHash(void) const;
			/* Returns the count of lines of this File starting at 1. */
			size_t getLineCount(void) const;
			/* Returns the count of characters in this File. */
			size_t getCharCount(void) const;
			/* Returns a struct containing the content of this File. See OFileContent. */
			OFileContent getContent(void) const;
			/* Returns a specific line of this File. */
			OFileLine getLine(size_t line) const;
			/* Returns the C FILE struct used by this File internally. */
			void* getCFile(void) const;
	};

	/* Does the given File exist relative to the OApp's working directory? */
	extern bool OFileExists(const char* file);
	/* Does the File exist relative to the given directory? */
	extern bool OFileExists(const char* directory, const char* file);
	/* Returns the FileHash of the given file relative to the OApp's working directory. */
	extern OFileHash OFileGetHash(const char* file);
	/* Returns the FileHash of the file relative to the given directory. */
	extern OFileHash OFileGetHash(const char* directory, const char* file);
}

#endif /* !__ORIONAPI_OSL_OFILE_H__ */