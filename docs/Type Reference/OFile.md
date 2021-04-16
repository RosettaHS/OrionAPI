# Type Reference: OFile
## Orion-Native File handling

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
