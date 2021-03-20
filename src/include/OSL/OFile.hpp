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

#include "common.hpp"

namespace Orion{
/*** File Information ***/

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

	/* An enumeration of possible file actions. */
	enum OFileAction : char{
		/* Open an existing File for both reading and writing. */
		OFILE_OPEN,
		/* Open an existing File for reading only. Useful for files where you do not have write privileges. */
		OFILE_OPEN_READONLY,
		/* Creates a File (or overrides an existing one) for both reading and writing. */
		OFILE_NEW,
		/* Automatically opens a File (read/write or read-only) or creates a File given on the availability of the File. */
		OFILE_AUTO
	};

	/* A simple numeric hash of a File's contents. */
	typedef uint64_t OFileHash;

/** File Content **/
	/* An individual Line of a File. */
	struct OFileLine{
		/* The length of this Line. */
		size_t length;
		/* The string stored in this Line. */
		char* str;

		/* Allows for if() checking on this Line. */
		operator bool(void) const;
		/* Allows for getting a character from a given index without having to use the "str" member. */
		char operator [](size_t) const;
		/* Allows for converting to a string. */
		operator char*(void) const;
	};

	/* The internal Content of a File. */
	struct OFileContent{
		/* The total count of lines in this File (starting at 1). */
		size_t lineCount;
		/* The total count of characters in this File. */
		size_t charCount;
		/* An array of Lines for this File. */
		OFileLine* lines;
		/* Have the contents been modified from when changes were last applied, or when the File was last opened? */
		bool modified;

		/* Allows for if() checking on this Content without having to check "lines" member manually. */
		operator bool(void) const;
		/* Allows for getting a Line from a given index without having to use the "lines" member. */
		OFileLine operator [](size_t) const;
	};

/*** Abstractive File handling ***/

	/* TODO: Re-implement OFile. */

/*** Generic File actions ***/

	/* Does the given File exist relative to the OApp's working directory? */
	extern bool OFileExists(const char* file);
	/* Does the File exist relative to the given directory? */
	extern bool OFileExists(const char* directory, const char* file);
	/* Deletes the given File relative to the OApp's working directory. */
	extern bool OFileDelete(const char* file);
	/* Deletes the File relative to the given directory. */
	extern bool OFileDelete(const char* directory, const char* file);
	/* Renames the File (relative to the OApp's working directory) to the given name. Returns true on success. */
	extern bool OFileRename(const char* file, const char* newName);
	/* Renames the File (relative to the given directory) to the given name. Returns true on success. */
	extern bool OFileRename(const char* directory, const char* file, const char* newName);
	/* Returns the FileHash of the given file relative to the OApp's working directory. */
	extern OFileHash OFileGetHash(const char* file);
	/* Returns the FileHash of the file relative to the given directory. */
	extern OFileHash OFileGetHash(const char* directory, const char* file);
}

#endif /* !__ORIONAPI_OSL_OFILE_H__ */