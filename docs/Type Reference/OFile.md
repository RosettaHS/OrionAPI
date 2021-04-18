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

## Breakdown