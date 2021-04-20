# Type Reference: ODirectory
## Orion-Native Directory handling
**[PLACEHOLDER]**

## Expansion
```cpp
class ODirectory : CLoggable{
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
		ODirectory(const char* directory, ODirectoryAction action=ODIR_AUTO);
		ODirectory(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO);

		bool open(const char* directory, ODirectoryAction action=ODIR_AUTO);
		bool open(const char* parentDirectory, const char* subDirectory, ODirectoryAction action=ODIR_AUTO);
		bool close(void);

		inline const char* getPath(void) { return (const char*)path; }
		inline const char* getName(void) { return (const char*)name; }

		inline size_t getEntryCount(void) { return itemCount; }
		ODirectoryEntry* getEntry(size_t index); inline ODirectoryEntry* operator [](size_t index) { return getEntry(index); }
		char* getEntryPath(size_t index);

		virtual void log(bool verbose=false, bool newLine=true) override;
};
```
```
[Altname  : dir_t]
[Library  : OSL]
[Declared : src/include/OSL/ODirectory.hpp]
[Defined  : src/OSL/ODirectory.cpp]
```