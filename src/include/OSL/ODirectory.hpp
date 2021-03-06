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

	/* An enumeration of possible actions for opening a given Directory. */
	enum ODirectoryAction : char{
		/* Open an existing Directory for reading its contents. */
		ODIR_OPEN,
		/* Creates a Directory if it does not exist, then opens it. */
		ODIR_NEW,
		/* Automatically opens a Directory, or creates a Directory (then opens it) based on the availability of the Directory. */
		ODIR_AUTO
	};

/** Directory Entry **/

	/* An enumeration of possible types for a given DirectoryEntry. */
	enum ODEType : char{
		ODT_UNKNOWN=-1,
		ODT_ERROR,
		ODT_DIR,
		ODT_FILE,
		ODT_SYML,
		ODT_SOCKET,
		ODT_PIPE,
	};

	/* An item of a given Directory. */
	struct ODirectoryEntry{
		/* The type of this Entry. */
		ODEType type;
		/* The name of this Entry. */
		char*   name;
		/* The full, real path to this Entry. */
		char*   path;

		/* Empty constructor. Sets all values to 0. */
		inline ODirectoryEntry(void) : type{ODT_ERROR}, name{0},path{0} {}
	};

/*** Abstractive Directory handling ***/

	/* An Orion-Native implementation for easily accessing Directories. */
	class ODirectory : CLoggable{
		protected:
			/* The action used to open this Directory. */
			ODirectoryAction action;
			/* The absolute path to this Directory. */
			char*            path;
			/* The name of this Directory. */
			char*            name;
			/* The C DIR struct used for this Directory. */
			void*            RAW;
			/* An array of DirectoryEntries for this Directory. */
			ODirectoryEntry* items;
			/* The count of entries in this Directory. */
			size_t           itemCount;

			/* Internal. Initialises information regarding this Directory. Called by open(). */
			void init(void);
		public:
			/* Empty constructor. Sets all values to 0. */
			ODirectory(void);

			/**
			 * @brief Opens the given Directory relative to the OApp's working directory with the given action.
			 * @param directory The name/path (absolute or relative) of the Directory to either open or create.
			 * @param action The action to access the Directory with.
			 * By default, this is ODIR_AUTO, meaning if the Directory exists it will attempt to open it and read its contents.
			 * If it does not exist, it will be created.
			 * See also: ODIR_OPEN, ODIR_NEW
			 * @return True if the Directory could be successfully opened or created, otherwise false.
			 */
			bool open(const char* directory, ODirectoryAction action=ODIR_AUTO); ODirectory(const char* directory, ODirectoryAction action=ODIR_AUTO);
			/**
			 * @brief Opens the Sub-Directory relative to the given Directory with the given action.
			 * @param parentDirectory A path to a Directory (absolute or relative) to attempt to search for the Sub-Directory in.
			 * @param subDirectory The name of the Sub-Directory to either open or create relative to the given Directory.
			 * @param action The action to access the Sub-Directory with.
			 * By default, this is ODIR_AUTO, meaning if the Sub-Directory exists it will attempt to open it and read its contents.
			 * If it does not exist, it will be created.
			 * See also: ODIR_OPEN, ODIR_NEW
			 * @return True if the Directory could be successfully opened or created, otherwise false.
			 */
			bool open(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO); ODirectory(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO);
			/**
			 * @brief Closes the Directory and frees all associated memory.
			 * @return True if the Directory could be successfully closed, otherwise false if either the Directory could not be closed, or it was never open to begin with.
			 */
			bool close(void);

			/**
			 * @brief Has the Directory been opened properly, and is ready for use?
			 * @return True if Directory has been successfully opened, otherwise false.
			 */
			inline bool valid(void) const { return ( (RAW) ? true : false ); } inline operator bool(void) const { return ( (RAW) ? true : false ); }

			/**
			 * @brief Returns the full, real path to this Directory. The result must NOT be freed.
			 * @return The full path to the active Directory. The result must NOT be freed.
			 */
			inline const char* getPath(void) const { return (const char*)path; }
			/**
			 * @brief Returns the name of this Directory. The result must NOT be freed.
			 * @return The name of the active Directory. The result must NOT be freed.
			 */
			inline const char* getName(void) const { return (const char*)name; }

			/**
			 * @brief Returns the amount of Entries in this Directory.
			 * @return The count of Entries (folders/files) in this Directory.
			 */
			inline size_t getEntryCount(void) const { return itemCount; }
			/**
			 * @brief Returns an Entry of this Directory at the given index.
			 * The Entries are not sorted in any way.
			 * @param index The index to attempt to retrieve an Entry from.
			 * @return If successful, returns a pointer to a struct (ODirectoryEntry) containing information regarding the given Entry, such as the type and name.
			 * Otherwise returns NULL.
			 */
			ODirectoryEntry* getEntry(size_t index) const; inline ODirectoryEntry* operator [](size_t index) const { return getEntry(index); }

			/**
			 * @brief Returns an Entry of this Directory with the given name.
			 * @param filename The name of the Entry(File or Folder) to attempt to retrieve.
			 * @return If successful, returns a pointer to a struct (ODirectoryEntry) containing information regarding the given Entry, such as the type and name.
			 * Otherwise returns NULL.
			 */
			ODirectoryEntry* getEntryByName(const char* filename) const;
			/**
			 * @brief Returns just the full, real path to the Entry of this Directory at the given index.
			 * The result must NOT be freed.
			 * The Entries are not sorted in any way.
			 * @param index The index to attempt to retrieve an Entry from.
			 * @return If successful, returns a null-terminated String that contains the full, real path of a given Entry (folder/file) corrisponding to the given index.
			 * The result must NOT be freed.
			 * Otherwise returns NULL on failure.
			 */
			char* getEntryPath(size_t index) const;

			/**
			 * @brief Returns the C DIR struct used by this File internally.
			 * @return A (void) pointer that points to the C (stdio) DIR struct used by this internally.
			 * Make sure to cast this back into a C DIR struct (DIR*).
			 */
			inline void* getCDir(void) const { return RAW; }
			/**
			 * @brief Logs the information of this Directory to the terminal.
			 * @param verbose Log verbose information about this Directory instead of the traditional information. Default is false.
			 * @param newLine Should the output be placed on a newline or append to the current one if applicable? Default is true.
			 */
			virtual void log(bool verbose=false, bool newLine=true) override;
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

#ifndef ORION_NOALTNAMES

	/* An enumeration of possible actions for opening a given Directory. */
	typedef ODirectoryAction diraction_t;
	/* An enumeration of possible types for a given DirectoryEntry. */
	typedef ODEType          direnttype_t;
	/* An item of a given Directory. */
	typedef ODirectoryEntry  direntry_t;
	/* An Orion-Native implementation for easily accessing Directories. */
	typedef ODirectory       dir_t;

/* These might be stupid... going to hold off on altname functions for now. */
	// /**
	 // * @brief Does the given Directory exist relative to the OApp's working directory?
	 // * @param directory The name/path (absolute or relative) of the Directory to check.
	 // * @return True if the Directory exists at the location, otherwise false.
	 // */
	// inline bool directoryExists(const char* directory) { return ODirectoryExists(directory); }
	// /**
	 // * @brief Does the Sub-Directory exist relative to the given Directory?
	 // * @param parentDirectory A path (absolute or relative) to attempt to search for the Sub-Directory in.
	 // * @param subDirectory The name/path of the Sub-Directory to check relative to the given Directory.
	 // * @return True if the Sub-Directory exists at the location, otherwise false.
	 // */
	// inline bool directoryExists(const char* parentDirectory, const char* subDirectory)
		// { return ODirectoryExists(parentDirectory,subDirectory); }

#endif /* !ORION_NOALTNAMES */
}

#endif /* !__ORION_OSL_ODIRECTORY_H__ */