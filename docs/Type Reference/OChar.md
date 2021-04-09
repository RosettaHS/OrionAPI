# Type Reference: OChar
## Multi-byte Unicode Handling
OChar is the Orion-Native implementation for handling multi-byte Unicode ([UTF-8](https://en.wikipedia.org/wiki/UTF-8)) characters.

## Expansion
```cpp
struct OChar : CLoggable{
	union{
		char          asSingleByte;
		unsigned char asMultiByte[OCHARBYTES];
	}get;
	OUnicodeType  bd[OCHARBYTES];
	unsigned char byteCount;
	bool          isUnicode;

	inline OChar(void) : get{0}, bd{OUNI_NULL,OUNI_NULL,OUNI_NULL,OUNI_NULL,OUNI_NULL}, byteCount{1}, isUnicode{0} {}
	OChar(const char*); void setTo(const char*); OChar& operator=(const char*);
	OChar(char); void setTo(char); OChar& operator=(char);

	void clear(void);

	inline operator char(void)        { return get.asSingleByte; }
	inline operator const char*(void) { return ( (isUnicode) ? (const char*)get.asMultiByte : 0 ); }

	virtual void log(bool verbose=false, bool newLine=true) override;
};
```
```
[Altname  : unichar_t]
[Library  : OSL]
[Declared : src/include/OSL/OString.hpp]
[Defined  : src/OSL/OString.cpp]
```
