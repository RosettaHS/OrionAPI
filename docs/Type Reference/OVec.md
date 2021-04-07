# Type Reference: OVec
## Storing Positional and Sizing information.
OVec is used for storing either the position or size of a given OrionUI Element,
and is accepted by (some) methods used to modify positions or sizes of OrionUI Elements in place of directly inputting the X and Y (or W and H) values.

## Expansion
```cpp
struct OVec : CLoggable {
	int32_t x, y;

	inline OVec(void) : x{0}, y{0} {}
	inline void setTo(int32_t _x, int32_t _y) { x=_x; y=_y; }; inline OVec(int32_t _x, int32_t _y) : x{_x}, y{_y} {}
	bool setTo(const char* format);

	virtual void log(bool verbose=false, bool newLine=true) override;
};
```
```
[Altname  : vec_t]
[Library  : OKit]
[Declared : src/include/OKit/OVec.hpp]
[Defined  : src/OKit/OVec.cpp]
```

## Use
OVecs are rarely ever 