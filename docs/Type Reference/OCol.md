# Type Reference: OCol
## Dealing with Colours

## Definition
```cpp
struct OCol : public CLoggable{
	uint8_t  r,g,b;
	uint32_t XCOL;

	inline OCol(void) : r{0},g{0},b{0},XCOL{0} {}

	void setTo(uint8_t r, uint8_t g, uint8_t b); OCol(uint8_t r, uint8_t g, uint8_t b);
	bool setTo(const char* format);

	virtual void log(bool verbose=false, bool newLine=true) override;
};
```
**[Altname  : `col_t`]**
**[Declared : `src/include/OKit/OCol.hpp`]**
**[Defined  : `src/OKit/OCol.cpp`]**

## Use