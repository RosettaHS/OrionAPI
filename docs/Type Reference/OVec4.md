# Type Reference: OVec4
## Storing both Positional and Sizing information.
OVec4 is used for storing both the position or size of a given OrionUI Element,
whereas [OVec](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OVec.md) only stores **EITHER** the position or the size.

## Expansion
```cpp
struct OVec4 : CLoggable {
	int16_t  x, y;
	uint16_t w, h;

	inline OVec4(void) : x{0}, y{0}, w{0}, h{0} {}
	inline void setTo(int16_t _x, int16_t _y, uint16_t _w, uint16_t _h) { x=_x; y=_y; w=_w; h=_h; };
	inline OVec4(int16_t _x, int16_t _y, uint16_t _w, uint16_t _h) : x{_x}, y{_y}, w{_w}, h{_h} {}
	bool setTo(const char* format);

	virtual void log(bool verbose=false, bool newLine=true) override;
};
```
```
[Altname  : vec4_t]
[Library  : OKit]
[Declared : src/include/OKit/OVec.hpp]
[Defined  : src/OKit/OVec.cpp]
```

## Use
OVec4s, much like [OVecs](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OVec.md), are rarely created manually.
They are often the return value of a method (typically `getGeometry()`) of a given Element, used for retrieving both the position and size with a single operation.

To create an OVec4 for use, use the following syntax:
```cpp
OVec4 myVec4(760,365,400,350);
```
The constructor takes in 4 values, each corresponding to the X and Y positional and the W and H sizing values of the given Element.

The positional values and the sizing values are stored separately, and with different [signedness](https://en.wikipedia.org/wiki/Signedness), preventing mixing potentially negative positional values with
the strictly positive sizing values required by OrionUI Elements.

A valid size value for an Element must be greater than 0. Negative size implies an inverted Element, which is invalid and can cause a crash on attempted assignment.

Unlike [OCol](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OCol.md), it is safe to directly access and modify
the internal `x`, `y`, `w` and `h` values stored within the Vector4.

The following three methods of setting a given Vector4 are essentially identical and considered valid:
```cpp
myVec4.x=760;
myVec4.y=365;
myVec4.w=400;
myVec4.h=350;

myVec4={760,365,400,350};

myVec4.setTo(760,365,400,350);
```
All of these are valid methods to set the values of a given Vector4, and are used interchangeably within OrionAPI.

A Vector4 can also be initialised using a properly formatted String.

A String such as `"(760,365,400,350)"` can be inputted into the `setTo()` method, and if the String is properly formatted, will set the Vector4's values to
the values stored within the String.

The formatted section can be at any point of the String, but it must be present and have 4 distinct numbers (maximum 10 digits each) that correspond to the `x`, `y`, `w`, and `h` values of the Vector.
```cpp
const char* myString="This is a Vector4 (760,365,400,350) formatted within a String!";
myVec4.setTo(myString);
```
This will set the Vector4's values to the values stored within the String.

## Breakdown
```cpp
int16_t  x, y;
```
The X and Y positions of the Element.
```cpp
uint16_t w, h;
```
The Width and Height of the Element.
```cpp
inline OVec4(void) : x{0}, y{0}, w{0}, h{0} {}
```
Empty constructor. Sets all values to 0.
```cpp
inline void setTo(int16_t _x, int16_t _y, uint16_t _w, uint16_t _h) { x=_x; y=_y; w=_w; h=_h; };
inline OVec4(int16_t _x, int16_t _y, uint16_t _w, uint16_t _h) : x{_x}, y{_y}, w{_w}, h{_h} {}
```
Sets the positional and sizing values to the given parameters.

Parameters:

`int16_t _x  - The X (horizontal) positional value to set on this Vec4.`

`int16_t _y  - The Y (vertical) positional value to set on this Vec4.`

`uint16_t _w - The W (horizontal) sizing value to set on this Vec4.`

`uint16_t _h - The H (vertical) sizing value to set on this Vec4.`
```cpp
bool setTo(const char* format);
```
Attempts to set and initialise this Vector4 from the given formatted String. 

The String MUST be in a very specific format. Example: "(760,365,400,350)"

The formatted part can be at any position of the String, but it MUST contain four values corresponding to each of the X,Y, W, and H values.

Parameters:

`const char* format - The format String to attempt to retrieve the values from.`

Returns: `True on success, otherwise false if format String is not valid.`
```cpp
virtual void log(bool verbose=false, bool newLine=true) override;
```
Logs the information of this Vec4 to the terminal.

Parameters:

`bool verbose - Log verbose information about this Vec4 instead of the traditional information. Default is false.`

`bool newLine - Should the output be placed on a newline or append to the current one if applicable? Default is true.`

## Other Information
Attempting to initialise an OVec4's positional values from an [OVec](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OVec.md) is potentially dangerous,
as the `x` and `y` values of an OVec has a different type `int32_t` from OVec4's `x` and `y` values `int16_t`.
This is so OVec can store signed positional values as well as unsigned size values `uint16_t`.

In most cases it won't be an issue initialising an OVec4 from an OVec if the OVec is retrieved from a function or method from OrionAPI.
If, however, the OVec is initialised manually, it poses a potential risk if whatever initialised it did so with a value using the full available size of `int32_t`.