# Type Reference: OString
## Orion-Native Unicode String handling

## Expansion
```cpp
class OString : public CLoggable{
	protected:
		char*  raw;
		struct{
			uint32_t apparent;
			uint32_t real;
		}length;
		uint32_t memuse;

		uint32_t apparentToReal(uint32_t index);
	public:
		~OString(void);
		inline OString(void) : raw{0}, length{0,0}, memuse{0} {};
		inline OString(const char* text) : raw{0}, length{0,0}, memuse{0} { setTo(text); };

		bool clear(void);

		bool setMemory(uint32_t newSize);

		bool setTo(const char* text); OString& operator=(const char* text);
		bool append(const char* text); OString& operator+=(const char* text);
		bool setChar(OChar c, uint32_t index);

		char* getText(void) const; operator char*(void) const;
		OChar getChar(uint32_t index); OChar operator[](uint32_t);
		inline char getCharFast(uint32_t index) { return raw[index]; }
		uint32_t getLength(bool realLength=false);
		inline uint32_t getMemory(void) { return memuse; }

		bool equalTo(const char* text) const; bool operator==(const char* text) const;
		bool ready(void) const; operator bool(void) const;
		bool contains(const char* substring);

		OString operator+(const char* text) const;

		virtual void log(bool verbose=false,  bool newLine=true) override;
};
```
```
[Altname  : string_t]
[Library  : OSL]
[Declared : src/include/OSL/OString.hpp]
[Defined  : src/OSL/OString.cpp]
```