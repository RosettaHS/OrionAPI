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

#include "../application.hpp"
#include "CLoggable.hpp"

namespace Orion{
	/* An enumeration of generic file types. */
	enum OFileType : char{
		OFT_UNKNOWN=-1,
		OFT_ERROR,
		OFT_TEXT,
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

	/* An Orion-Native implementation for easily reading and writing to files. */
	class OFile : public CLoggable {
		protected:
			/* The action which was used for this File. */
			OFileAction action;
			/* The path to the File itself. */
			char* path;
			/* The name of the File (plus extension if it has one). */
			char* name;
			/* The raw extension of the File (if it has one) */
			char* ext;
			/* A pointer to the C FILE struct. */
			void* FILERAW;
			/* The descriptor of this File that the operating system uses to locate it. */
			int FILEDESC;
		public:
			/* The type of the File. */
			OFileType type;

			/* Destructor. Frees all allocated memory and closes the file. */
			~OFile(void);
			/* Empty constructor. Sets all values to 0. */
			OFile(void);

			/* Opens the given File relative to the OApp's working directory with the given action. */
			bool open(const char* file, OFileAction); OFile(const char* file, OFileAction);
			/* Opens the File relative to the given directory with the given action. */
			bool open(const char* directory, const char* file, OFileAction); OFile(const char* directory, const char* file, OFileAction);
			/* Closes the file and applies any pending modifications. */
			bool close(void);

			/* Has the File been opened properly, and is ready for use? */
			bool valid(void) const; operator bool(void) const;

			/* Returns the extension of this File (if it has one). */
			const char* getExtension(void) const;
			/* Returns the name of this File. */
			const char* getName(void) const;
			/* Returns the full path pointing to this File. */
			const char* getFullPath(void) const;
			/* Returns the C FILE struct used by this File internally. */
			void* getCFile(void) const;
	};

	/* Does the given File exist relative to the OApp's working directory? */
	extern bool OFileExists(const char* file);
	/* Does the given File exist relative to the given directory? */
	extern bool OFileExists(const char* directory, const char* file);
}

#endif /* !__ORIONAPI_OSL_OFILE_H__ */