
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
		union{
			struct{
				/* The total count of Lines in this File (starting at 1). */
				size_t lineCount;
				/* The total count of Characters in this File. */
				size_t charCount;
				/* An array of Lines for this File. */
				OFileLine* list;
			}asLines;
			struct{
				/* The size of the array. */
				size_t   byteCount;
				/* The array of bytes of this File. */
				uint8_t* bytes;
			}asLinear;
		};
		/* Have the contents been modified from when changes were last applied, or when the File was last opened? */
		bool modified;
		/*
		 * If this is true, this means the File's contents are stored linearly in memory, as if it were one big Line,
		 * and indexxing operations MUST be done using "data.asLinear.bytes" instead of "data.asLines.list"
		 * If this is false, this means the File's contents are stored as separate lines in memory, as it would be displayed in an editor,
		 * and indexxing operations MUST be done using "data.asLines.list" instead of "data.asLinear.bytes"
		 */
		bool isLinear;
	};

/*** Abstractive File handling ***/

	/* An Orion-Native implementation for easily reading and writing to Files. */
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
				/* Should this File's contents be loaded into memory linearly? Default is false. */
				bool storeLinearly;
				/* Should this File's information (such as the type and name) be gathered and stored? Default is true. */
				bool storeMisc;
				/* Should this File's contents be evaluated? Default is true. (Required for storing to memory). */
				bool evalContents;
			}flags;

			/**
			 * @brief Internal. Initialises internal information of this File.
			 * @param skipEval Determines whether OFile should attempt to evaluate the File's contents, by scanning through the File.
			 * This is intensive, but required for storing the File to memory.
			 */
			void init(bool skipEval);
		public:
			/* Empty constructor. Sets all values to 0. */
			OFile(void);

			/**
			 * @brief Opens the given File relative to the OApp's working directory with the given action.
			 * @param filename The name/path (absolute or relative) of the File to either open or create.
			 * @param action The action to access the File with.
			 * By default, this is OFILE_AUTO, meaning if the File exists it will attempt to open it with either read/write or read-only permissions.
			 * If it does not exist, it will be created.
			 * See also: OFILE_OPEN, OFILE_OPEN_READONLY, OFILE_NEW
			 * @return True if the File could be successfully opened or created, false otherwise.
			 */
			bool open(const char* filename, OFileAction action=OFILE_AUTO); OFile(const char* filename, OFileAction=OFILE_AUTO);
			/**
			 * @brief Opens the File relative to the given directory with the given action.
			 * @param directory A path (absolute or relative) to attempt to search for the File in.
			 * @param filename The name of the File to either open or create relative to the given directory.
			 * @param action The action to access the File with.
			 * By default, this is OFILE_AUTO, meaning if the File exists it will attempt to open it with either read/write or read-only permissions.
			 * If it does not exist, it will be created.
			 * See also: OFILE_OPEN, OFILE_OPEN_READONLY, OFILE_NEW
			 * @return True if the File could be successfully opened or created, false otherwise.
			 */
			bool open(const char* directory, const char* filename, OFileAction action=OFILE_AUTO); OFile(const char* directory, const char* filename, OFileAction=OFILE_AUTO);
			/**
			 * @brief Closes the File, and can applies any modifications and save it before closing.
			 * @param saveChanges If this is true, any modifications done to the File will be written before closing.
			 * @return True if File was successfully closed, otherwise false if File either could not be closed or File wasn't open to begin with.
			 */
			bool close(bool saveChanges);
			/**
			 * @brief Stores the File's contents to memory.
			 * @return True if contents could be stored to memory, otherwise false if contents could either not be stored, or contents were already stored.
			 */
			bool storeToMem(void);

			/**
			 * @brief Applies the current modifications.
			 * @return True if there were any modifications to apply, and if the process was a success.
			 */
			bool save(void);
			/** 
			 * @brief Saves a copy of the current File to the given filename relative to the OApp's working directory.
			 * @param filename The name/path (absolute or relative) of the File to be saved as.
			 * @return True if there were any modifications to apply, and if the process was a success.
			 */
			bool saveAs(const char* filename);
			/**
			 * @brief Saves a copy of the current File to the filename relative to the given directory.
			 * @param directory A path (absolute or relative) to attempt to save the File in.
			 * @param filename The name to save the File as, relative to the given directory.
			 * @return True if there were any modifications to apply, and if the process was a success.
			 */
			bool saveAs(const char* directory, const char* file);
			/**
			 * @brief Renames the current File to the given name, or moves it if the new name is located in a different directory.
			 * @param newName The new name to save the File as. Note that this is relative to the OApp's working directory, not relative to the File's location.
			 * @return True if the File could be renamed, otherwise false if either File could not be renamed, or File wasn't open to begin with.
			 */
			bool rename(const char* newName);
			/** 
			 * @brief Discards any modifications and reloads the File.
			 * @return True if there were modifications to discard.
			 */
			bool reset(void);
			/** 
			 * @brief Deletes the current File.
			 * @return True if File could be successfully deleted.
			 */
			bool deleteCurrent(void);

			/**
			 * @brief Has the File been opened properly, and is ready for use?
			 * @return True if File has been successfully opened, otherwise false.
			 */
			bool valid(void) const; operator bool(void) const;
			/*
			 * Should this File care about allocating and initialising miscellaneous information?
			 * These operations are intensive so if you need to open dozens of files every second, set this to false. Default is true.
			 * Changes do not apply until File is closed and reopened.
			 */
			inline void shouldStoreMisc(bool v)     { flags.storeMisc=v; }
			/*
			 * If the File is allowed to evaluate Contents, should this File store all of its Contents to memory?
			 * This is true by default, and is required to be on if you want to access or modify a File's Contents.
			 * But it is very intensive storing all Contents to memory, so if you just need to hold generic File information,
			 * then turn this off. Indexing Contents will require this to be turned on.
			 * If you need to turn this off, turn this off BEFORE you open a File.
			 * Changes do not apply until File is closed and reopened.
			 */
			inline void shouldStoreToMem(bool v)    { flags.storeMem=v; }
			/*
			 * If this File is allowed to store its Contents to memory, should the stored Contents be linear (a giant array), or separated in different arrays based on each Line?
			 * This is false by default, as all indexxing and modification operations that OFile provides REQUIRE the Contents to be stored in different Line arrays.
			 * However, for Files such as images, binaries, videos, or any File that isn't meant to be read by the user, this should be set to true.
			 * To do indexxing and modification on the File's contents if they are stored linearly, you must first retrieve the Contents of the file by using getContents(),
			 * and access/modify the bytes directly. See documentation for OFileContent for more information.
			 * Changes do not apply until File is closed and reopened.
			 */
			inline void shouldStoreLinearly(bool v) { flags.storeLinearly=v; }
			/*
			 * Should this File evaluate Contents?
			 * This is true by default, and is required to be on if you want to store a File's contents to memory.
			 * If this is true, OFile will iterate through each byte of a newly-opened File and calculate the number of Lines, number of characters, generates a hash from the Contents,
			 * and allows storage of the File's Contents to memory.
			 * Changes do not apply until File is closed and reopened.
			 */
			inline void shouldEvalContents(bool v)  { flags.evalContents=v; }
			/**
			 * @brief Has the File been modified since when changes were last applied, or when the File was last opened?
			 * @return True if File has been modified since last save.
			 */
			bool hasBeenModified(void) const;
			/**
			 * @brief Do the two Files share the same content?
			 * @return True if Files share identical contents.
			 */
			bool equalTo(OFile&) const; bool operator==(OFile&) const;
			/**
			 * @brief Recalculates the File's hash. Do this after setLine() operations if you need to compare the modified File
			 * against another File, as this is NOT called after the File has been modified!
			 * @return A numeric hash of the File's contents.
			 */
			OFileHash recalcHash(void);

			/**
			 * @brief Sets the given line of the File (starting at 0) to the new text.
			 * This will fail if you attempt to call this on an OFile that has the "storeLinearly" flag set to true.
			 * @param line The Line number to modify. Note that this starts at 0 instead of 1 as a File would display, as internally this is an array of Strings for easy iteration.
			 * If the given Line is out of bounds, new, empty Lines will be created in the empty space.
			 * @param newText The new text to set the given line to. This will resize the given Line if the new text is larger than the size of the Line.
			 * @return True if Line could be set, otherwise false if either the File has not been stored to memory, the File is in read-only mode, or the File is not open to begin with.
			 */
			bool setLine(size_t line, const char* newText);

			/**
			 * @brief Returns the generic type of the File if it could be determined.
			 * @return A value corrisponding to an enumeration of generic FileTypes.
			 * See: OFileType
			 */
			OFileType getType(void) const;
			/** 
			 * @brief Returns the type of the File as a String if it could be determined.
			 * @return A String version of the given OFileType value corrisponding to this File.
			 * For example: if the File's Type is OFT_IMAGE, this will return "OFT_IMAGE" as a String. Good for easily readable debug logs.
			 */
			const char* getTypeAsString(void) const;
			/**
			 * @brief Returns the full path pointing to this File.
			 * @return A String that contains the full, real path of this File.
			 */
			const char* getPath(void) const; operator const char*(void) const;
			/**
			 * @brief Returns the name of this File.
			 * @return If the File has been allowed to store misc information (see shouldStoreMisc), this will return the actual name of the File, excluding the path but including the extension.
			 */
			const char* getName(void) const;
			/**
			 * @brief Returns the extension of this File (if it has one).
			 * @return If the File has been allowed to store misc information (see shouldStoreMisc), this will return the raw extension of the File, excluding the fullstop.
			 * For example: if the File's name is "myFile.txt", this will return "txt".
			 */
			const char* getExtension(void) const;
			/**
			 * @brief Returns the size of the File in bytes. 
			 * @return The raw size of the File in bytes.
			 */
			size_t getSize(void) const;
			/**
			 * @brief Returns the simplistic numerical hash of this File.
			 * @return A numeric hash of this File's contents corrisponding to the last time recalcHash() was ran.
			 * If you have modified any content of this File, run recalcHash(). since modification operations do not automatically recalculate the hash.
			 */
			OFileHash getHash(void) const;
			/**
			 * @brief Returns the count of Lines of this File.
			 * This will fail if you attempt to call this on an OFile that has the "storeLinearly" flag set to true.
			 * @return The Line count of this File. This is always AT LEAST 1 if the File is valid, but otherwise 0 if the File has not been opened.
			 */
			size_t getLineCount(void) const;
			/**
			 * @brief Returns the count of characters in this File.
			 * This will fail if you attempt to call this on an OFile that has the "storeLinearly" flag set to true.
			 * @return The character count (NOT BYTE COUNT!) of this File.
			 */
			size_t getCharCount(void) const;
			/**
			 * @brief Returns pointer to a a struct containing the content of this File. See OFileContent. 
			 * @return A pointer to a struct (OFileContent) containing information regarding the contents of this File.
			 */
			inline OFileContent* getContents(void) { return &contents; }
			/** 
			 * @brief Returns a specific line (starting at 0) of this File as an OFileLine.
			 * Do NOT use this to retrieve a Line to use as a normal character array, instead use the array notation! ( file[line] ).
			 * This will fail if you attempt to call this on an OFile that has the "storeLinearly" flag set to true.
			 * @param line The Line to attempt to return. Note that this starts at 0 instead of 1 as a File would display, as internally this is an array of Strings for easy iteration.
			 * @return A struct (OFileLine) containing information regarding the given Line.
			 */
			OFileLine getLine(size_t line) const;
			/**
			 * @brief Returns a specific Line (starting at 0) of this File as a String. 
			 * Do NOT modify this! Use setLine() instead!
			 * This will fail if you attempt to call this on an OFile that has the "storeLinearly" flag set to true.
			 * @param line The Line to attempt to return. Note that this starts at 0 instead of 1 as a File would display, as internally this is an array of Strings for easy iteration.
			 * @return A character array corrisponding to the given Line.
			 */
			char* operator [](size_t line) const;
			/**
			 * @brief Returns the C FILE struct used by this File internally.
			 * @return A (void) pointer that points to the C (stdio) FILE struct used by this internally.
			 * Make sure to cast this back into a C FILE struct (FILE*).
			 */
			void* getCFile(void) const;

			/**
			 * @brief Logs the content of this File out to the terminal. 
			 * @param verbose Log verbose information (such as File information like size and type) instead of the content itself. Default is false.
			 * @param newLine Should the output be placed on a newline or append to the current one if applicable? Default is true.
			 */
			virtual void log(bool verbose=false, bool newLine=true) override;
	};

/*** Generic File actions ***/

	/**
	 * @brief Does the given File exist relative to the OApp's working directory?
	 * @param filename The name/path (absolute or relative) of the File to check.
	 * @return True if the File exists at the location, otherwise false.
	 */
	extern bool OFileExists(const char* filename);
	/**
	 * @brief Does the File exist relative to the given directory? 
	 * @param directory A path (absolute or relative) to attempt to search for the File in.
	 * @param filename The name/path of the File to check relative to the given directory.
	 * @return True if the File exists at the location, otherwise false.
	 */
	extern bool OFileExists(const char* directory, const char* filename);
	/**
	 * @brief Deletes the given File relative to the OApp's working directory.
	 * @param filename The name/path (absolute or relative) of the File to delete.
	 * @return True if the File could be deleted, otherwise false.
	 */
	extern bool OFileDelete(const char* filename);
	/**
	 * @brief Deletes the File relative to the given directory.
	 * @param directory A path (absolute or relative) to attempt to delete the File from.
	 * @param filename The name/path  of the File to delete relative to the given directory.
	 * @return True if the File could be deleted, otherwise false.
	 */
	extern bool OFileDelete(const char* directory, const char* filename);
	/**
	 * @brief Renames the File (relative to the OApp's working directory) to the given name. Returns true on success.
	 * @param filename The name/path (absolute or relative) of the File to rename.
	 * @param newName The new name to rename the File as. Note that this is relative to the OApp's working directory, not relative to the File's location.
	 * @return True if the File could be renamed, otherwise false.
	 */
	extern bool OFileRename(const char* filename, const char* newName);
	/**
	 * @brief Renames the File (relative to the given directory) to the given name. Returns true on success.
	 * @param directory A path (absolute or relative) to attempt to scan for the File in.
	 * @param filename The name/path of the File to rename relative to the given directory.
	 * @param newName The new name to rename the File as. Note that this is relative to the given directory, not relative to the File's location within subfolders of the directory.
	 * @return True if the File could be renamed, otherwise false.
	 */
	extern bool OFileRename(const char* directory, const char* filename, const char* newName);
	/**
	 * @brief Returns the FileHash of the given file relative to the OApp's working directory.
	 * @param filename The name/path (absolute or relative) of the File to calculate the hash of.
	 * @return A numeric hash of the File's contents.
	 */
	extern OFileHash OFileGetHash(const char* filename);
	/**
	 * @brief Returns the FileHash of the file relative to the given directory.
	 * @param directory A path (absolute or relative) to attempt to scan for the File in.
	 * @param filename The name/path of the File to calculate the hash of relative to the given directory.
	 * @return A numeric hash of the File's contents.
	 */
	extern OFileHash OFileGetHash(const char* directory, const char* filename);

#ifndef ORION_NOALTNAMES

	/* An enumeration of generic File types. */
	typedef OFileType    filetype_t;
	/* An enumeration of possible File actions. */
	typedef OFileAction  fileaction_t;
	/* A simple numeric hash of a File's contents. */
	typedef OFileHash    filehash_t;
	/* An individual Line of a File. */
	typedef OFileLine    fileline_t;
	/* The internal Content of a File. */
	typedef OFileContent filecontent_t;
	/* An Orion-Native implementation for easily reading and writing to Files. */
	typedef OFile        file_t;

#endif /* !ORION_NOALTNAMES */
}

#endif /* !__ORIONAPI_OSL_OFILE_H__ */