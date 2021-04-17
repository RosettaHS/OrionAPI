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
In reverse order, the last parameter denotes the action to open the File with.

This is a list of possible actions to use when opening a File:

`OFILE_AUTO` - Automatically opens a File (read/write or read-only) or creates a File given on the availability of the File.

`OFILE_NEW` - Creates a File (or overrides an existing one) for both reading and writing.

`OFILE_OPEN` - Open an existing File for both reading and writing.

`OFILE_OPEN_READONLY` - Open an existing File for reading only. Useful for files where you do not have write privileges.
