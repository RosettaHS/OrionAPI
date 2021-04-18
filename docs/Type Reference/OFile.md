# Type Reference: OFile
## Orion-Native File handling
OFile is the Orion-Native implementation for handling Files, providing easy methods for reading and writing to any File.

## Expansion
```cpp
class OFile : public CLoggable{
	protected:
		OFileType    type;
		OFileAction  action;
		OFileContent contents;
		OFileHash    hash;
		size_t       size;
		char*        name;
		char*        path;
		char*        ext;
		struct{
			void* RAW;
			int   DESC;
		}CFILE;
		struct{
			bool storeMem;
			bool storeLinearly;
			bool storeMisc;
			bool evalContents;
		}flags;

		void init(bool skipEval);
	public:
		OFile(void);
		OFile(const char* filename, OFileAction=OFILE_AUTO);
		OFile(const char* directory, const char* filename, OFileAction=OFILE_AUTO);

		bool open(const char* filename, OFileAction action=OFILE_AUTO);
		bool open(const char* directory, const char* filename, OFileAction action=OFILE_AUTO);
		bool close(bool saveChanges);
		bool storeToMem(void);

		bool save(void);
		bool saveAs(const char* filename);
		bool saveAs(const char* directory, const char* file);
		bool rename(const char* newName);
		bool reset(void);
		bool deleteCurrent(void);

		bool valid(void) const; operator bool(void) const;
		inline void shouldStoreMisc(bool v)     { flags.storeMisc=v; }
		inline void shouldStoreToMem(bool v)    { flags.storeMem=v; }
		inline void shouldStoreLinearly(bool v) { flags.storeLinearly=v; }
		inline void shouldEvalContents(bool v)  { flags.evalContents=v; }
		bool hasBeenModified(void) const;
		bool equalTo(OFile&) const; bool operator==(OFile&) const;
		OFileHash recalcHash(void);

		bool setLine(size_t line, const char* newText);

		OFileType getType(void) const;
		const char* getTypeAsString(void) const;
		const char* getPath(void) const; operator const char*(void) const;
		const char* getName(void) const;
		const char* getExtension(void) const;
		size_t getSize(void) const;
		OFileHash getHash(void) const;
		size_t getLineCount(void) const;
		size_t getCharCount(void) const;
		inline OFileContent* getContents(void) { return &contents; }
		OFileLine getLine(size_t line) const;
		char* operator [](size_t line) const;
		void* getCFile(void) const;

		virtual void log(bool verbose=false, bool newLine=true) override;
};
```
```
[Altname  : file_t]
[Library  : OSL]
[Declared : src/include/OSL/OFile.hpp]
[Defined  : src/OSL/OFile.hpp]
```

## Use
OFile is used for reading, writing, or creating Files. It provides many utilities to make File operations incredibly simple.

A simple way to create an OFile for use can be done using the following syntax:
```cpp
OFile myFile("testFile.txt");
```
This will create a File called `testFile.txt` in the [current working directory](https://en.wikipedia.org/wiki/Working_directory) if it does not exist.
If it does exist, it will open the File and store its contents to memory.

### File Opening
The previous example, while functional, leaves out crucial details on how File opening works with OFile.

Examining the method to open a File, we find it has [two versions,](https://en.wikipedia.org/wiki/Function_overloading) and both of them have a [default parameter.](https://en.wikipedia.org/wiki/Default_argument)
```cpp
bool open(const char* filename, OFileAction action=OFILE_AUTO);
bool open(const char* directory, const char* filename, OFileAction action=OFILE_AUTO);
```

First, examining the two-parameter version:
```cpp
bool open(const char* filename, OFileAction action=OFILE_AUTO);
```
The first parameter denotes the path [(relative or absolute)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to the File to attempt to open.
Usually this would be used to create or open a File in the [current working directory,](https://en.wikipedia.org/wiki/Working_directory) or to create/open a File with a pre-determined absolute path.

The last parameter denotes the action to open the File with.

This is a list of possible actions to use when opening a File:

`OFILE_AUTO` - Automatically opens a File (read/write or read-only) or creates a File given on the availability of the File.

`OFILE_NEW` - Creates a File (or overrides an existing one) for both reading and writing.

`OFILE_OPEN` - Open an existing File for both reading and writing.

`OFILE_OPEN_READONLY` - Open an existing File for reading only. Useful for files where you do not have write privileges.

By default, it will use the `OFILE_AUTO` option.

The second version of the method, while only a small change, contains far more functionality:
```cpp
bool open(const char* directory, const char* filename, OFileAction action=OFILE_AUTO);
```
The last parameter is the same as the previous version's last parameter, but the first and second parameters are different.

The first parameter is a path [(relative or absolute)](https://www.lifewire.com/absolute-and-relative-paths-3466467) to a Directory to scan for the File (second parameter) within.

The second parameter is the File (relative to the scan directory) to access.

As an example, using the first version of the method, this opens the [LibC](https://en.wikipedia.org/wiki/C_standard_library) `stdio.h` header file:
```cpp
OFile myFile("/usr/include/stdio.h");
```
Note how it uses the full path to the File itself.
Now this version does the exact same thing, just in a different way:
```cpp
OFile myFile("/usr/include","stdio.h");
```
Instead of utilising the full path to the File, it instead searches for the a given File within a Directory.
This is primarily to support the [Application Structure7's](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md) native [helpers:](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Application%20Structure.md#utilising-helpers)
```cpp
OFile myFile(OAPP_HOME,"myFile.txt");
```
This will either create or open a File called `"myFile.txt"` in the [home folder.](https://en.wikipedia.org/wiki/Home_directory)

Files can also be accessed from within subdirectories as well:
```cpp
OFile myFile("/usr","include/stdio.h"); 
```

### Files in Memory
Before any reading and writing can be done, the File **must first** be stored to memory.
This is done by default whenever a File is opened, however it is a very intensive process.

Disabling this procedure isn't recommended as it essentially makes OFile useless, however there might be certain cases where it might be done.
If this is required, use the method `shouldStoreToMem()` and pass `false` to disable this **BEFORE** loading the File.

If a File has been loaded with this off, call it with `true` and then call `storeToMem()` to load the Contents into memory.

Files are stored to memory in the form of an [OFileContent.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFileContent.md)
Review the documentation for this type for more information.

A File could also be stored [linearly](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFileContent.md#reading-linearly)
as opposed to the default method of storing a File with each of its Lines separated.

Storing the Lines separate allows for the easy reading and writing in the [next section,](file-readingwriting) but has [some drawbacks.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFileContent.md#benefits-to-both)
Primarily, [binary files](https://en.wikipedia.org/wiki/Binary_file) may have their structure altered, and are made far more difficult to read when the Lines are separated.

By default, binary files are usually opened with `shouldStoreLinearly()` set to `true`, although more traditional Files (such as text documents) are still opened and stored as separated Lines.

### File Reading/Writing
Once a File has been opened (and it is not read-only), it can be read and modified.

OFile interprets Files as a list of Lines, as opposed to a huge array of characters (which they actually are).
This makes both reading and writing incredibly easy.

Take this File for example, `myFile.txt`:
```
This is a File
With multiple lines!
With OFile, you can jump to a specific line
Or even change lines at will!
```
The following is a list of read actions you can do with OFile on this File:
```cpp
OFile   myFile("myFile.txt");
OString myString; /* Use this to read the Lines. */

/* Log the whole File to the terminal. */
myFile.log();

/* Log each Line out directly. */
for(size_t i=0; i<OFile.getLineCount(); i++){
	OLog("%s\n",myFile[i]);
}

/* Store (and log) a specific Line. */
myString=myFile[2]; /* Will capture "With OFile, you can jump to a specific line" */
myString.log(); /* Could also do OLog("%s\n",myFile[2]); */
```
Attempting to read an out-of-bounds Line will return `NULL`, meaning the Line list is null-terminated.

It is important to note that array notation (`myFile[]`) for retriving a Line *ALWAYS* returns a `char*`, whereas the `getLine()` method *ALWAYS* returns an [OFileLine.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFileLine.md)

Going back to the example File, we can change a Line of the File very easily:
```cpp
OFile myFile("myFile.txt");
myFile.log(); /* To show the File in its original state. */

myFile.setLine(3,"This line has been modified!\n");

myFile.log(); /* To show the modified File. */
```
Lines will be resized to fit the new text, and the File can be resized to fit new Lines as well:
```cpp
OFile myFile("myFile.txt");
myFile.log(); /* To show the File in its original state. */

/* Starting position is 4 because that is the end of the example File. */
for(size_t i=4;i<10;i++){
	myFile.setLine(i,"This is a new Line!");
}

myFile.log(); /* To show the modified File. */
```
Note that any read/write operations **CANNOT** be done this way if the File's Contents are [being stored linearly.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFileContent.md#reading-linearly)

### File Saving/Closing
Modifications to Files do not apply automatically. Instead, they can be batched up and applied at a later time.
To check if a given File has been modified, use the `hasBeenModified()` method.

If a File has been modified (and the File is not read-only), the File can be have all changes applied with the `save()` method.

When closing a File using the `close()` method, a boolean must be passed to decide whether it should apply any pending modifiations, or discard them and don't save the File before closing.
If `true` is passed, any pending modifications will be applied, and the File will be closed.
If `false` is passed, pending modifications are discarded and the changes are not saved when the File is closed.

Note that `close()` *ALWAYS* frees any associated memory from the File, including its [Content.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFileContent.md)
Do NOT close a File that is in use, especially one being used by an OrionUI Element that is displaying an Image File.

### Comparing Files/OFileHash
OFileHash is an [alias](https://en.wikipedia.org/wiki/Typedef) for the type `uint64_t`.
It is a number that contains the (simple) [numerical hash](https://en.wikipedia.org/wiki/Hash_function) of a File's content.

(Note that OFileHash's [altname](https://github.com/RosettaHS/OrionAPI/blob/main/docs/API%20Configuration.md#orion_noaltnames) is `filehash_t`)

Retrieving a File's hash can be done using the `getHash()` method. This number is unique to the Contents and only the Contents. 
The File itself can be different, but if the Contents are identical then the hash will be the same.

Note that an empty File and a non-existent File both have the identical hash `0`.

Comparing two Files can be done using the `==` operator:
```cpp
if( myFile==myOtherFile ){
	...
}
```
If both of these Files share the identical Content, the condition will be `true` and any code within the block will be executed.

The `equalTo()` method could also be used. Both are functionally identical.

Note that if a File is modified (such as with `setLine()`) the hash will NOT be recalculated automatically. Use `recalcHash()` to force the hash to be recalculated.

## Breakdown
### The following members are protected, and cannot be accessed directly.
```cpp
OFileType type; /* misc */
```
The Type of this File.
```cpp
OFileAction action;
```
The action used to open this File.
```cpp
OFileContent contents;
```
The contents of File.
```cpp
OFileHash hash;
```
The hash of File.
```cpp
size_t size;
```
The size (in bytes) of File.
```cpp
char* name; /* misc */
```
The name of File.
```cpp
char* path;
```
The full path to File.
```cpp
char* ext;  /* misc */
```
The raw extension File.
```cpp
struct{
	void* RAW;
	int   DESC;
}CFILE;
```
A struct containing internal information of this File.

`void* RAW;` - A pointer to a C FILE struct used for operations on this File.

`int DESC;` - The UNIX [file descriptor](https://en.wikipedia.org/wiki/File_descriptor) of this File.
```cpp
struct{
	bool storeMem;
	bool storeLinearly;
	bool storeMisc;
	bool evalContents;
}flags;
```
A struct containing internal flags for management of this File.

`bool storeMem;` - Should this File's contents be loaded into memory? Default is true.

`bool storeLinearly;` - Should this File's contents be loaded into memory linearly? Default is false.

`bool storeMisc` - Should this File's information (such as the type and name) be gathered and stored? Default is true.

`bool evalContents` - Should this File's contents be evaluated? Default is true. (Required for storing to memory).
```cpp
void init(bool skipEval);
```
Internal. Initialises internal information of this File.

Parameters:

`bool skipEval - Determines whether OFile should attempt to evaluate the File's contents, by scanning through the File. This is intensive, but required for storing the File to memory.`
### The following members are public, and can be accessed directly.
```cpp
OFile(void);
```
Empty constructor. Sets all values to 0.

```cpp
bool open(const char* filename, OFileAction action=OFILE_AUTO);
/* Or */
OFile(const char* filename, OFileAction=OFILE_AUTO);
```
Opens the given File relative to the OApp's working directory with the given action.

Parameters:

`const char* filename - The name/path (absolute or relative) of the File to either open or create.`

`OFileAction action - The action to access the File with.
By default, this is OFILE_AUTO, meaning if the File exists it will attempt to open it with either read/write or read-only permissions.
If it does not exist, it will be created.`

Returns: `True if the File could be successfully opened or created, false otherwise.`
```cpp
bool open(const char* directory, const char* filename, OFileAction action=OFILE_AUTO);
/* Or */
OFile(const char* directory, const char* filename, OFileAction=OFILE_AUTO);
```
Opens the File relative to the given directory with the given action.

Parameters:

`const char* directory - A path (absolute or relative) to attempt to search for the File in.`

`const char* filename - The name of the File to either open or create relative to the given directory.`

`OFileAction action - The action to access the File with.
By default, this is OFILE_AUTO, meaning if the File exists it will attempt to open it with either read/write or read-only permissions.
If it does not exist, it will be created.`

Returns: `True if the File could be successfully opened or created, false otherwise.`
```cpp
bool close(bool saveChanges);
```
Closes the File, and can applies any modifications and save it before closing.

Parameters:

`bool saveChanges - If this is true, any modifications done to the File will be written before closing.`

Returns: `True if File was successfully closed, otherwise false if File either could not be closed or File wasn't open to begin with.`
```cpp
bool storeToMem(void);
```
Stores the File's contents to memory.

Returns: `True if contents could be stored to memory, otherwise false if contents could either not be stored, or contents were already stored.`
```cpp
bool save(void);
```
Applies the current modifications.

Returns: `True if there were any modifications to apply, and if the process was a success.`
```cpp
bool saveAs(const char* filename);
```
Saves a copy of the current File to the given filename relative to the OApp's working directory.

`const char* filename - The name/path (absolute or relative) of the File to be saved as.`

Returns: `True if there were any modifications to apply, and if the process was a success.`
```cpp
bool saveAs(const char* directory, const char* file);
```
Saves a copy of the current File to the filename relative to the given directory.

Parameters:

`const char* directory - A path (absolute or relative) to attempt to save the File in.`

`const char* filename - The name to save the File as, relative to the given directory.`

Returns: `True if there were any modifications to apply, and if the process was a success.`
```cpp
bool rename(const char* newName);
```
Renames the current File to the given name, or moves it if the new name is located in a different directory.

Parameters:

`const char* newName - The new name to save the File as. Note that this is relative to the OApp's working directory, not relative to the File's location.`

Returns: `True if the File could be renamed, otherwise false if either File could not be renamed, or File wasn't open to begin with.`
```cpp
bool reset(void);
```
Discards any modifications and reloads the File.

Returns: `True if there were modifications to discard.`
```cpp
bool deleteCurrent(void);
```
Deletes the current File.

Returns: `True if File could be successfully deleted.`
```cpp
bool valid(void) const;
/* Or */
operator bool(void) const;
```
Has the File been opened properly, and is ready for use?

Returns: `True if File has been successfully opened, otherwise false.`
```cpp
inline void shouldStoreMisc(bool v)     { flags.storeMisc=v; }
```
Should this File care about allocating and initialising miscellaneous information?
These operations are intensive so if you need to open dozens of files every second, set this to false. Default is true.

Changes do not apply until File is closed and reopened.
```cpp
inline void shouldStoreToMem(bool v)    { flags.storeMem=v; }
```
If the File is allowed to evaluate Contents, should this File store all of its Contents to memory?
This is true by default, and is required to be on if you want to access or modify a File's Contents.
But it is very intensive storing all Contents to memory, so if you just need to hold generic File information,
then turn this off. Indexing Contents will require this to be turned on.
If you need to turn this off, turn this off BEFORE you open a File.

Changes do not apply until File is closed and reopened.
```cpp
inline void shouldStoreLinearly(bool v) { flags.storeLinearly=v; }
```
If this File is allowed to store its Contents to memory, should the stored Contents be linear (a giant array), or separated in different arrays based on each Line?
This is false by default, as all indexxing and modification operations that OFile provides REQUIRE the Contents to be stored in different Line arrays.
However, for Files such as images, binaries, videos, or any File that isn't meant to be read by the user, this should be set to true.
To do indexxing and modification on the File's contents if they are stored linearly, you must first retrieve the Contents of the file by using getContents(),
and access/modify the bytes directly. See documentation for OFileContent for more information.

Changes do not apply until File is closed and reopened.
```cpp
inline void shouldEvalContents(bool v)  { flags.evalContents=v; }
```
Should this File evaluate Contents?
This is true by default, and is required to be on if you want to store a File's contents to memory.
If this is true, OFile will iterate through each byte of a newly-opened File and calculate the number of Lines, number of characters, generates a hash from the Contents,
and allows storage of the File's Contents to memory.

Changes do not apply until File is closed and reopened.
```cpp
bool hasBeenModified(void) const;
```
Has the File been modified since when changes were last applied, or when the File was last opened?

Returns: `True if File has been modified since last save.`
bool equalTo(OFile&) const; bool operator==(OFile&) const;
```cpp
```
Do the two Files share the same content?

Returns: `True if Files share identical contents.`
```cpp
OFileHash recalcHash(void);
```
Recalculates and returns the File's hash. Do this after setLine() operations if you need to compare the modified File
against another File, as this is NOT called after the File has been modified!
```cpp
bool setLine(size_t line, const char* newText);
```
Sets the given line of the File (starting at 0) to the new text.
This will fail if you attempt to call this on an OFile that has the "storeLinearly" flag set to true.

Parameters:

`size_t line - The Line number to modify. Note that this starts at 0 instead of 1 as a File would display, as internally this is an array of Strings for easy iteration.
If the given Line is out of bounds, new, empty Lines will be created in the empty space.`

`const char* newText - The new text to set the given line to. This will resize the given Line if the new text is larger than the size of the Line.`

Returns: `True if Line could be set, otherwise false if either the File has not been stored to memory, the File is in read-only mode, or the File is not open to begin with.`
```cpp
OFileType getType(void) const;
```
Returns the generic type of the File if it could be determined.
Can be any one of these:
```
OFT_UNKNOWN
OFT_ERROR
OFT_TEXT
OFT_CONFIG
OFT_IMAGE
OFT_VIDEO
OFT_FONT
OFT_BINARY
```
```cpp
const char* getTypeAsString(void) const;
```
Returns the type of the File as a String if it could be determined.
For example: if the File's Type is OFT_IMAGE, this will return "OFT_IMAGE" as a String. Good for easily readable debug logs.
```cpp
const char* getPath(void) const;
/* Or */
operator const char*(void) const;
```
Returns the full, real path pointing to this File.
```cpp
const char* getName(void) const;
```
If the File has been allowed to store misc information (see shouldStoreMisc), this will return the actual name of the File, excluding the path but including the extension.
```cpp
const char* getExtension(void) const;
```
If the File has been allowed to store misc information (see shouldStoreMisc), this will return the raw extension of the File, excluding the fullstop.
For example: if the File's name is "myFile.txt", this will return "txt".
```cpp
size_t getSize(void) const;
```
Returns the raw size of the File in bytes. 
```cpp
OFileHash getHash(void) const;
```
Returns a numeric hash of this File's contents corrisponding to the last time recalcHash() was ran.
If you have modified any content of this File, run recalcHash(). since modification operations do not automatically recalculate the hash.
```cppp
size_t getLineCount(void) const;
```
Returns the count of Lines of this File, which is always AT LEAST 1 if the File is valid, but otherwise 0 if the File has not been opened.
This will fail if you attempt to call this on an OFile that has the "storeLinearly" flag set to true.
```cpp
size_t getCharCount(void) const;
```
Returns the count of characters (NOT BYTE COUNT!) in this File.
This will fail if you attempt to call this on an OFile that has the "storeLinearly" flag set to true.
```cpp
inline OFileContent* getContents(void) { return &contents; }
```
Returns pointer to a a struct containing information regarding the content of this File.

See [OFileContent.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFileContent.md)
```cpp
OFileLine getLine(size_t line) const;
```
Returns a specific line (starting at 0) of this File as an [OFileLine.](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OFileLine.md)
Do NOT use this to retrieve a Line to use as a normal character array, instead use the array notation! ( file[line] ).
This will fail if you attempt to call this on an OFile that has the "storeLinearly" flag set to true.

Parameters:

`size_t line - The Line to attempt to return. Note that this starts at 0 instead of 1 as a File would display, as internally this is an array of Strings for easy iteration.`
```cpp
char* operator [](size_t line) const;
```
Returns a specific Line (starting at 0) of this File as a character array(String). 
Do NOT modify this! Use setLine() instead!

This will fail if you attempt to call this on an OFile that has the "storeLinearly" flag set to true.

Parameters:

`size_t line - The Line to attempt to return. Note that this starts at 0 instead of 1 as a File would display, as internally this is an array of Strings for easy iteration.`
```cpp
void* getCFile(void) const;
```
Returns the C [FILE](https://www.tutorialspoint.com/cprogramming/c_file_io.htm) struct used by this File internally.
Make sure to cast this back into a C FILE struct `(FILE*)` **[REQUIRES INCLUSION OF `<stdio.h>`]**
```cpp
virtual void log(bool verbose=false, bool newLine=true) override;
```
Logs the content of this File out to the terminal. 

Parameters:

`bool verbose - Log verbose information (such as File information like size and type) instead of the content itself. Default is false.`
`bool newLine - Should the output be placed on a newline or append to the current one if applicable? Default is true.`
