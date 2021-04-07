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
OVecs are rarely ever created manually, they are often returned from functions or methods that retrieve either the size or position of a given OrionUI Element.
They can (sometimes) also be fed into methods that modify said positions or sizes.

To create an OVec for use, use the following syntax:
```cpp
OVec myVec(760,350);
```
The constructor takes in 2 values, corresponding to either the X and Y positional values of the Element, or the W and H size values of the Element.
Note that if the Vector is being used to size values, they should **NEVER** contain a negative number.

Negative numbers are **ONLY** valid for positional values, since Elements can be "outside" of their parent, however they can not be inverted, as a negative size would imply.

Unlike [OCol,](https://github.com/RosettaHS/OrionAPI/blob/main/docs/Type%20Reference/OCol.md) it is safe to directly access and modify
the internal `x` and `y` values stored within the Vector.

The following three methods of setting a given Vector are essentially identical and considered valid:
```cpp
myVec.x=760;
myVec.y=350;

myVec={760,350};

myVec.setTo(760,350);
```
All of these are valid methods to set the values of a given Vector, and are used interchangeably within OrionAPI. 

A Vector can also be initialised using a properly formatted String.

A String such as `"(760,350)"` can be inputted into the `setTo()` method, and if the String is properly formatted, will set the Vector's values to
the values stored within the String.

The formatted section can be at any point of the String, but it must be present and have 2 distinct numbers (maximum 10 digits) that correspond to the X and Y (Or W and H) values of the Vector.
```cpp
const char* myString="This is a Vector (760,350) formatted within a String!";
myVec.setTo(myString);
```
This will set the Vector to the values stored within the String.

## Breakdown
```cpp
int32_t x, y;
```
The X and Y positional (or W and H sizing) values stored within this Vector.
```cpp
inline OVec(void) : x{0}, y{0} {}
```
Empty constructor. Sets all values to 0.
```cpp
inline void setTo(int32_t _x, int32_t _y) { x=_x; y=_y; };
inline OVec(int32_t _x, int32_t _y) : x{_x}, y{_y} {}
```
Sets the X and Y positional (or W and H sizing) values to the given parameters.

Parameters:

`int32_t _x - The X (horizontal) positional value to set on this Vec.`

`int32_t _y - The Y (vertical) positional value to set on this Vec.`
```cpp
bool setTo(const char* format);
```
Attempts to set and initialise this Vector from the given formatted String. 
The String MUST be in a very specific format. Example: "(760,365)"
The formatted part can be at any position of the String, but it MUST contain two values corresponding to each of the X and Y values.

Parameters:

`const char* format - The format String to attempt to retrieve the values from.`

Returns: `True on success, otherwise false if format String is not valid.`

```cpp
virtual void log(bool verbose=false, bool newLine=true) override;
```
Logs the information of this Vec to the terminal.

Parameters:

`bool verbose - Log verbose information about this Vec instead of the traditional information. Default is false.`

`bool newLine - Should the output be placed on a newline or append to the current one if applicable? Default is true.`

## Other Information
OVec stores the values as `int32_t`. This is to allow for either positional (`int16_t`) and size (`uint16_t`) values
to be safely stored within the values inside of the OVec. This does NOT mean you should attempt to use the full size of `int32_t`, as this WILL cause errors and potentially a crash!