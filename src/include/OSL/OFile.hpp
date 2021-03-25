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
#include "OLog.hpp"

namespace Orion{
/*** File Information ***/

	/* An enumeration of generic File types. */
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

	/* An enumeration of possible File actions. */
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
		unsigned char* str;

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

	class OFile : public CLoggable{
		protected:
			/* The Type of this File. */
			OFileType    type; /* misc */
			/* The action used to open this File. */
			OFileAction  action;
			/* The contents of File. */
			OFileContent contents;
			/* The hash of File. */
			OFileHash    hash;
			/* The size (in bytes) of File. */
			size_t       size;
			/* The name of File. */
			char*        name; /* misc */
			/* The full path to File. */
			char*        path;
			/* The raw extension File. */
			char*        ext;  /* misc */
			/* A struct containing internal information of this File. */
			struct{
				/* A pointer to a C FILE struct used for operations on this File. */
				void* RAW;
				/* The UNIX file descriptor of this File. */
				int   DESC;
			}CFILE;
			/* A struct containing internal flags for management of this File. */
			struct{
				/* Should this File's contents be loaded into memory? Default is true. */
				bool storeMem;
				/* Should this File's information (such as the type and name) be gathered and stored? Default is true. */
				bool storeMisc;
				/* Should this File's contents be evaluated? Default is true. (Required for storing to memory). */
				bool evalContents;
			}flags;

			/* Internal. Initialises internal information of this File. Pass true to skip evaluating contents. */
			void init(bool skipEval);
		public:
			/* Empty constructor. Sets all values to 0. */
			OFile(void);

			/* Opens the given File relative to the OApp's working directory with the given action. */
			bool open(const char* filename, OFileAction action=OFILE_AUTO); OFile(const char* filename, OFileAction=OFILE_AUTO);
			/* Opens the File relative to the given directory with the given action. */
			bool open(const char* directory, const char* filename, OFileAction action=OFILE_AUTO); OFile(const char* directory, const char* filename, OFileAction=OFILE_AUTO);
			/* Closes the File, and if true is passed, applies any modifications and saves it. */
			bool close(bool saveChanges);
			/* Stores the File's contents to memory. */
			bool storeToMem(void);

			/* Applies the current modifications. Returns true if there were any modifications to apply, and the process was a success. */
			bool save(void);
			/* Saves a copy of the current File to the given filename relative to the OApp's working directory. */
			bool saveAs(const char* file);
			/* Saves a copy of the current File to the filename relative to the given directory. */
			bool saveAs(const char* directory, const char* file);
			/* Renames the current File to the given name, or moves it if the new name is located in a differen directory. */
			bool rename(const char* newName);
			/* Discards any modifications and reloads the File. Returns true if there were modifications to discard. */
			bool reset(void);
			/* Deletes the current File. Returns true on success. */
			bool deleteCurrent(void);

			/* Has the File been opened properly, and is ready for use? */
			bool valid(void) const; operator bool(void) const;
			/*
			 * Should this File care about allocating and initialising miscellaneous information?
			 * These operations are intensive so if you need to open dozens of files every second, set this to false. Default is true.
			 */
			void shouldStoreMisc(bool);
			/*
			 * Should this File store all of its Contents to memory?
			 * This is true by default, and is required to be on if you want to access or modify a File's Contents.
			 * But it is very intensive storing all Contents to memory, so if you just need to hold generic File information,
			 * then turn this off. Indexing Contents will require this to be turned on.
			 * If you need to turn this off, turn this off BEFORE you open a File.
			 */
			void shouldStoreToMem(bool);
			/* Has the File been modified since when changes were last applied, or when the File was last opened? */
			bool hasBeenModified(void) const;
			/* Do the two Files share the same content? */
			bool equalTo(OFile&) const; bool operator==(OFile&) const;
			/*
			 * Recalculates the File's hash. Do this after setLine() operations if you need to compare the modified File
			 * against another File, as this is NOT called after the File has been modified!
			 */
			OFileHash recalcHash(void);

			/* Sets the given line of the File (starting at 0) to the new text. Returns true on success. */
			bool setLine(size_t line, const char* newText);

			/* Returns the type of the File if it could be determined. */
			OFileType getType(void) const;
			/* Returns the type of the File as a String if it could be determined. */
			const char* getTypeAsString(void) const;
			/* Returns the full path pointing to this File. */
			const char* getPath(void) const; operator const char*(void) const;
			/* Returns the name of this File. */
			const char* getName(void) const;
			/* Returns the extension of this File (if it has one). */
			const char* getExtension(void) const;
			/* Returns the size of the File in bytes. */
			size_t getSize(void) const;
			/* Returns the simplistic numerical hash of this File. */
			OFileHash getHash(void) const;
			/* Returns the count of lines of this File. */
			size_t getLineCount(void) const;
			/* Returns the count of characters in this File. */
			size_t getCharCount(void) const;
			/* Returns a struct containing the content of this File. See OFileContent. */
			OFileContent getContent(void) const;
			/* Returns a specific line (starting at 0) of this File as an OFileLine. */
			OFileLine getLine(size_t line) const;
			/* Returns a specific line (starting at 0) of this File as a string. Do NOT modify this! Use setLine() instead! */
			char* operator [](size_t) const;
			/* Returns the C FILE struct used by this File internally. */
			void* getCFile(void) const;

			/*
			 * Logs the content of this File out to the terminal. 
			 * Pass true on the first parameter to get File information rather than content.
			 * Pass true on the second parameter to push the information to a new line if applicable.
			 */
			virtual void log(bool verbose=false, bool newLine=true) override;
	};

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