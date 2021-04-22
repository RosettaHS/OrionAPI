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

Much like with [OFile,](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md#file-opening) examining the method to open a Directory, we find it has [two versions,](https://en.wikipedia.org/wiki/Function_overloading) and both of them have a [default parameter.](https://en.wikipedia.org/wiki/Default_argument)
```cpp
bool open(const char* directory, ODirectoryAction action=ODIR_AUTO);
bool open(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO);
```
First, examining the two-parameter version:
```cpp
bool open(const char* directory, ODirectoryAction action=ODIR_AUTO);
```
The first parameter denotes the path [(relative or absolute)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to the Directory to attempt to open.
Usually this would be used to create or open a Directory in the [current working directory,](https://en.wikipedia.org/wiki/Working_directory) or to create/open a Directory with a pre-determined absolute path.

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
This is primarily to support the [Application Structure's](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md) native [helpers:](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md#utilising-helpers)
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

ODirectory interprets a Directory as an array of [ODirectoryEntries.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectoryEntry.md)
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
Admittedly, this can all be replaced by using the `log()` method in ODirectory, however this is how that method works internally.

#### Opening using [OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)
You can open a given Entry using [OFile](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFile.md)
by either iterating through all of the Entries and [opening them from the Entry itself,](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectoryEntry.md#opening-using-ofile)
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

## Other Information

#### See Also:
[ODirectoryEntry](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/ODirectoryEntry.md)