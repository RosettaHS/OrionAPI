# Type Reference: OCol
## Dealing with Colours
OCol is used for giving colours to OrionUI (OKit) Elements.
Most Elements in OKit colour themselves automatically, however you can manually assign them colours. 
Be aware that this can be overridden by a given user's Orion Theme Settings, or by the given Element itself.

## Expansion
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
```
[Altname  : col_t]
[Library  : OKit]
[Declared : src/include/OKit/OCol.hpp]
[Defined  : src/OKit/OCol.cpp]
```

## Use
 