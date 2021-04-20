# Type Reference: ODirectory
## Orion-Native Directory handling
**[PLACEHOLDER]**

## Expansion
```cpp
class ODirectory : CLoggable{
	protected:
		/* The action used to open this Directory. */
		ODirectoryAction action;
		/* The absolute path to this Directory. */
		char* path;
		/* The name of this Directory. */
		char* name;
		/* A struct containing internal information used to access and interface with this Directory. */
		struct{
			/* The C DIR struct used for this Directory. */
			void* RAW;
		}CDIR;
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
		 * @brief Returns the full, real path to this Directory. The result must NOT be freed.
		 * @return The full path to the active Directory. The result must NOT be freed.
		 */
		inline const char* getPath(void) { return (const char*)path; }
		/**
		 * @brief Returns the name of this Directory. The result must NOT be freed.
		 * @return The name of the active Directory. The result must NOT be freed.
		 */
		inline const char* getName(void) { return (const char*)name; }

		/**
		 * @breif Returns the amount of Entries in this Directory.
		 * @return The count of Entries (folders/files) in this Directory.
		 */
		inline size_t getEntryCount(void) { return itemCount; }
		/**
		 * @brief Returns an Entry of this Directory at the given index.
		 * The Entries are not sorted in any way.
		 * @param index The index to attempt to retrieve an Entry from.
		 * @return If successful, returns a pointer to a struct (ODirectoryEntry) containing information regarding the given Entry, such as the type and name.
		 * Otherwise returns NULL.
		 */
		ODirectoryEntry* getEntry(size_t index); inline ODirectoryEntry* operator [](size_t index) { return getEntry(index); }
		/**
		 * @brief Returns just the full, real path to the Entry of this Directory at the given index.
		 * The result must NOT be freed.
		 * The Entries are not sorted in any way.
		 * @param index The index to attempt to retrieve an Entry from.
		 * @return If successful, returns a null-terminated String that contains the full, real path of a given Entry (folder/file) corrisponding to the given index.
		 * The result must NOT be freed.
		 * Otherwise returns NULL on failure.
		 */
		char* getEntryPath(size_t index);

		/**
		 * @brief Logs the information of this Directory to the terminal.
		 * @param verbose Log verbose information about this Directory instead of the traditional information. Default is false.
		 * @param newLine Should the output be placed on a newline or append to the current one if applicable? Default is true.
		 */
		virtual void log(bool verbose=false, bool newLine=true) override;
};
```
```
[Altname  : dir_t]
[Library  : OSL]
[Declared : src/include/OSL/ODirectory.hpp]
[Defined  : src/OSL/ODirectory.cpp]
```