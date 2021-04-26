# Type Reference: ODirectory
## Orion-Native Directory handling
ODirectory is the Orion-Native implementation for handling Directories, providing a minimal abstraction over the native filesystem.

## Expansion
```cpp
class ODirectory : CLoggable{
	protected:
		ODirectoryAction action;
		char*            path;
		char*            name;
		void*            RAW;
		ODirectoryEntry* items;
		size_t           itemCount;

		void init(void);
	public:
		ODirectory(void);
		ODirectory(const char* directory, ODirectoryAction action=ODIR_AUTO);
		ODirectory(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO);

		bool open(const char* directory, ODirectoryAction action=ODIR_AUTO);
		bool open(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO);
		bool close(void);

		inline bool valid(void) const    { return ( (RAW) ? true : false ); }
		inline operator bool(void) const { return ( (RAW) ? true : false ); }

		inline const char* getPath(void) const { return (const char*)path; }
		inline const char* getName(void) const { return (const char*)name; }

		inline size_t getEntryCount(void) const { return itemCount; }
		ODirectoryEntry* getEntry(size_t index) const;
		ODirectoryEntry* getEntryByName(const char* filename) const;
		inline ODirectoryEntry* operator [](size_t index) const { return getEntry(index); }
		char* getEntryPath(size_t index) const;

		inline void* getCDir(void) const { return RAW; }
		virtual void log(bool verbose=false, bool newLine=true) override;
};
```
```
[Altname  : dir_t]
[Library  : OSL]
[Declared : src/include/OSL/ODirectory.hpp]
[Defined  : src/OSL/ODirectory.cpp]
```

## Use
ODirectory is used for iterating throughout the files and folders(Entries) of a given Directory.

To create an ODirectory for use, use the following syntax:
```cpp
ODirectory myDirectory("testDirectory");
```
This create a Directory called `testDirectory` inside of the [current working directory](https://en.wikipedia.org/wiki/Working_directory) if it does not exist yet.
If it does exist, it will instead open and read the contents of the Directory.

### Directory Opening
The previous example, while functional, leaves out crucial details on how Directory opening works with ODirectory.

Much like with [OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md#file-opening), examining the method to open a Directory, we find it has [two versions,](https://en.wikipedia.org/wiki/Function_overloading) and both of them have a [default parameter](https://en.wikipedia.org/wiki/Default_argument).
```cpp
bool open(const char* directory, ODirectoryAction action=ODIR_AUTO);
bool open(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO);
```
First, examining the two-parameter version:
```cpp
bool open(const char* directory, ODirectoryAction action=ODIR_AUTO);
```
The first parameter denotes the path [(relative or absolute)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to the Directory to attempt to open.
Usually this would be used to create or open a Directory in the [current working directory](https://en.wikipedia.org/wiki/Working_directory), or to create/open a Directory with a pre-determined absolute path.

The last parameter denotes the action to open the Directory with.

This is a list of possible actions to use when opening a Directory:

`ODIR_OPEN` - Open an existing Directory for reading its contents.

`ODIR_NEW` - Creates a Directory if it does not exist, then opens it.

`ODIR_AUTO` - Automatically opens a Directory, or creates a Directory (then opens it) based on the availability of the Directory.

By default, it will use the `ODIR_AUTO` option.

The second version of the method, while only a small change, contains far more functionality:
```cpp
bool open(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO);
```
The last parameter is the same as the previous version's last parameter, but the first and second parameters are different.

The first parameter is a path [(relative or absolute)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to a Directory to scan for the sub-Directory (second parameter) within.

The second parameter is the (sub-)Directory (relative to the scan Directory) to access.

As an example, using the first version of the method, this opens the system's [include directory:](https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard#Directory_structure)
```cpp
ODirectory myDirectory("/usr/include");
```
Note how it uses the full path to the Directory itself. Now this version does the exact same thing, just in a different way:
```cpp
ODirectory myDirectory("/usr","include");
```
Instead of utilising the full path to the Directory, it instead searches for the given sub-Directory within a parent Directory.
This is primarily to support the [Application Structure](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md)'s native [helpers](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md#utilising-helpers):
```cpp
ODirectory myDirectory(OAPP_HOME,"testDirectory");
```
This will either create or open a Directory called `"testDirectory"` in the [home folder.](https://en.wikipedia.org/wiki/Home_directory)

Multi-level sub-Directories can be accessed from parent Directories as well:
```cpp
ODirectory myDirectory("/","usr/include");
```

### Directory Reading
Once a Directory has been successfully opened, the contents can be read via iteration.

ODirectory interprets a Directory as an array of [ODirectoryEntries](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectoryEntry.md).
It is important to review the documentation for them to fully understand how to utilise ODirectory.

The following code opens a Directory, and lists off all of the Entries of the Directory (in no particular order):
```cpp
ODirectory       myDirectory("."); /* Opens the current Directory. */
ODirectoryEntry* myEntry;

for(size_t i=0; i<myDirectory.getEntryCount(); i++){
	myEntry=myDirectory[i];
	if(myEntry){
		/* Do whatever you want with the Entries here... */
		OLog("Entry: %s\n",myEntry->name);
	}
}

myDirectory.close();
```
Admittedly, this can all be replaced by using the `log()` method in ODirectory, however this is similar to how that method works internally.

It's also very important to note that the list of Entries is not sorted in any way.

#### Opening using [OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)
You can open a given Entry using [OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)
by either iterating through all of the Entries and [opening them from the Entry itself](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectoryEntry.md#opening-using-ofile),
or opening it directly from the Directory via the name of the Entry.

The following code attempts to access a specific Entry in a given Directory by using the Entry's filename:
```cpp
ODirectory       myDirectory("/usr/include");
ODirectoryEntry* myEntry;
OFile            myFile;

myEntry=myDirectory.getEntryByName("stdio.h");

if(myEntry){ /* If the Entry was found */
	myFile.open(myEntry->path);
	/* Do File operations here... */
}
myDirectory.close();
```

### Directory Closing
Once no longer needed, an ODirectory can be closed by using the `close()` method.
However, keep in mind that the array of [ODirectoryEntries](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectoryEntry.md) is freed as well,
meanning any residual Entry pointers are invalid. Do NOT attempt to access Entries after having closed their originating ODirectory.

[OFiles](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md) created from said Entries, however, are still safe to use.

## Breakdown
### The following members are protected, and cannot be accessed directly.
```cpp
ODirectoryAction action;
```
The action used to open this Directory.
```cpp
char* path;
```
The absolute path to this Directory.
```cpp
char* name;
```
The name of this Directory.
```cpp
void* RAW;
```
The C DIR struct used for this Directory.
```cpp
ODirectoryEntry* items;
```
An array of DirectoryEntries for this Directory.
```cpp
size_t itemCount;
```
The count of entries in this Directory.
```cpp
void init(void);
```
Internal. Initialises information regarding this Directory. Called by open().
### The following members are public, and can be accessed directly.
```cpp
ODirectory(void);
```
Empty constructor. Sets all values to 0.
```cpp
bool open(const char* directory, ODirectoryAction action=ODIR_AUTO); 
ODirectory(const char* directory, ODirectoryAction action=ODIR_AUTO);
```
Opens the given Directory relative to the OApp's working directory with the given action.

Parameters:

`const char* directory` - The name/path (absolute or relative) of the Directory to either open or create.

`ODirectoryAction action` - The action to access the Directory with.
By default, this is ODIR_AUTO, meaning if the Directory exists it will attempt to open it and read its contents.
If it does not exist, it will be created.

Returns: `True if the Directory could be successfully opened or created, otherwise false.`

```cpp
bool open(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO);
ODirectory(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO);
```
Opens the Sub-Directory relative to the given Directory with the given action.

Parameters:

`const char* parentDirectory` - A path to a Directory (absolute or relative) to attempt to search for the Sub-Directory in.

`const char* subDirectory` - The name of the Sub-Directory to either open or create relative to the given Directory.

`ODirectoryAction action` - The action to access the Directory with.
By default, this is ODIR_AUTO, meaning if the Directory exists it will attempt to open it and read its contents.
If it does not exist, it will be created.

Returns: `True if the Directory could be successfully opened or created, otherwise false.`
```cpp
bool close(void);
```
Closes the Directory and frees all associated memory.

Returns: `True if the Directory could be successfully closed, otherwise false if either the Directory could not be closed, or it was never open to begin with.`

```cpp
inline bool valid(void) const    { return ( (RAW) ? true : false ); }
inline operator bool(void) const { return ( (RAW) ? true : false ); }
```
Has the Directory been opened properly, and is ready for use?

Returns: `True if Directory has been successfully opened, otherwise false.`
```cpp
inline const char* getPath(void) const { return (const char*)path; }
```
Returns: `The full, real path to this Directory. The result must NOT be freed.`

```cpp
inline const char* getName(void) const { return (const char*)name; }
```
Returns: `The name of this Directory. The result must NOT be freed.`

```cpp
inline size_t getEntryCount(void) const { return itemCount; }
```
Returns: `The count of Entries (folders/files) in this Directory.`

```cpp
ODirectoryEntry* getEntry(size_t index) const;
inline ODirectoryEntry* operator [](size_t index) const { return getEntry(index); }
```
Returns an Entry of this Directory at the given index.

**The Entries are not sorted in any way.**

Parameters:

`size_t index` - The index to attempt to retrieve an Entry from.

Returns: `If successful, returns a pointer to a struct (ODirectoryEntry) containing information regarding the given Entry, such as the type and name.
Otherwise returns NULL.`

```cpp
ODirectoryEntry* getEntryByName(const char* filename) const;
```
Returns an Entry of this Directory with the given name.

Parameters:

`const char* filename` - The name of the Entry(File or Folder) to attempt to retrieve.

Returns: `If successful, returns a pointer to a struct (ODirectoryEntry) containing information regarding the given Entry, such as the type and name.
Otherwise returns NULL.`

```cpp
char* getEntryPath(size_t index) const;
```
Returns just the full, real path to the Entry of this Directory at the given index. The result must **NOT** be freed.

**The Entries are not sorted in any way.**

Parameters:

`size_t index` - The index to attempt to retrieve an Entry from.

Returns: `If successful, returns a null-terminated String that contains the full, real path of a given Entry (folder/file) corrisponding to the given index.
The result must NOT be freed. Otherwise returns NULL on failure.`

```cpp
inline void* getCDir(void) const { return RAW; }
```
Returns: `A (void) pointer that points to the C (stdio) DIR struct used by this internally.
Make sure to cast this back into a C DIR struct (DIR*).`

```cpp
virtual void log(bool verbose=false, bool newLine=true) override;
```
Logs the information of this Directory to the terminal.

Parameters:

`bool verbose` - Log verbose information about this Directory instead of the traditional information. Default is false.

`bool newline` - Should the output be placed on a newline or append to the current one if applicable? Default is true.

## Other Information
ODirectory is a powerful tool for creating and reading Directories, however it is fairly useless without [OFile.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)
Review the documentation for [OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md) and the [Orion-Native Application Structure](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md) to learn how to fully utilise OrionAPI's data management utilities.

#### See Also:
[ODirectoryEntry](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectoryEntry.md)