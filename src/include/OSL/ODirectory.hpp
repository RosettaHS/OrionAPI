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

#ifndef __ORION_OSL_ODIRECTORY_H__
#define __ORION_OSL_ODIRECTORY_H__

#include "common.hpp"
#include "OLog.hpp"

namespace Orion{
/*** Directory Information ***/

	enum ODirectoryAction : char{
		/* Open an existing Directory for reading its contents. */
		ODIR_OPEN,
		/* Creates a Directory if it does not exist, then opens it. */
		ODIR_NEW,
		/* Automatically opens a Directory, or creates a Directory (then opens it) given on the availability of the Directory. */
		ODIR_AUTO
	};

/** Directory Entry **/

	enum ODEType : char{
		ODT_UNKNOWN=-1,
		ODT_ERROR,
		ODT_DIR,
		ODT_FILE,
		ODT_SYML,
		ODT_SOCKET,
		ODT_PIPE,
	};

	struct ODirectoryEntry{
		ODEType type;
		char*   name;

		ODirectoryEntry(void);
	};

/*** Abstractive Directory handling ***/

	class ODirectory{
		protected:
			ODirectoryAction action;
			char* path;
			char* name;
			struct{
				void* RAW;
			}CDIR;
			ODirectoryEntry* items;
			size_t           itemCount;

			void init(void);
		public:
			ODirectory(void);
			bool open(const char* directory, ODirectoryAction action=ODIR_AUTO); ODirectory(const char* directory, ODirectoryAction action=ODIR_AUTO);
			bool open(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO); ODirectory(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO);
			bool close(void);
	};

/*** Generic Directory actions ***/

	/**
	 * @brief Does the given Directory exist relative to the OApp's working directory?
	 * @param directory The name/path (absolute or relative) of the Directory to check.
	 * @return True if the Directory exists at the location, otherwise false.
	 */
	extern bool ODirectoryExists(const char* directory);
	/**
	 * @brief Does the Sub-Directory exist relative to the given Directory? 
	 * @param parentDirectory A path (absolute or relative) to attempt to search for the Sub-Directory in.
	 * @param subDirectory The name/path of the Sub-Directory to check relative to the given Directory.
	 * @return True if the Sub-Directory exists at the location, otherwise false.
	 */
	extern bool ODirectoryExists(const char* parentDirectory, const char* subDirectory);

}

#endif /* !__ORION_OSL_ODIRECTORY_H__ */