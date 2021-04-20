# Type Reference: OCol
## Dealing with Colours
OCol is a [wrapper](https://en.wikipedia.org/wiki/Wrapper_function) used for assigning colours to OrionUI Elements.
Most OrionUI Elements assign colours to themselves automatically, however (in some cases) this can be manually overridden to assign them custom colours.
Be aware that this itself can be overridden by a given user's Orion Theme Settings, or by the given Element itself.

## Expansion
```cpp
	struct OCol : public CLoggable{
		union{
			uint32_t XCOL;
			struct{
				uint8_t b : 8;
				uint8_t g : 8;
				uint8_t r : 8;
			}raw;
		};

		inline OCol(void) : raw{0,0,0} { }
		inline OCol(uint8_t r, uint8_t g, uint8_t b) : raw{b,g,r} { }

		inline void setTo(uint8_t r, uint8_t g, uint8_t b) { raw={b,g,r}; }

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
To create an OCol for use, use the following syntax:
```cpp
OCol myCol(255,255,255); /* Pure White */
```
The three values in the constructor are the R, G, and B values of the Colour respectively.

After initialisation, the OCol can be passed into functions and methods that take in an OCol, most notably being the `setCol()` method on all OrionUI Elements.
Most of the time these methods take in a [reference](https://en.wikipedia.org/wiki/Reference_(C%2B%2B)) to the OCol, however sometimes they make take a pointer to an OCol.

It's recommended to review the documentation of the given OUI Element you are trying to modify in order to see what `setCol()` does in the given implementation,
since each Element implements `setCol()` differently.

If you wish to change an existing OCol, you can either use the `setTo()` method or directly change the `r,g,b` values.
```cpp
myCol.setTo(127,127,127); /* Grey */
/* Or */
myCol.raw.r=127;
myCol.raw.g=127;
myCol.raw.b=127;
```
An OCol can also be initialised using a properly formatted String.

A String such as `"(255,255,255)"` can be inputted into the `setTo()` method, and if the String is properly formatted, will set the OCol to
the value stored within the String.

The formatted section can be at any point of the String, but it must be present and have 3 distinct numbers (in range of 0-255) that correspond to the R,G, and B values of the Colour.
```cpp
const char* myString="This is a colour (255,255,255) formatted within a String!";
myCol.setTo(myString);
```
This will set the OCol to Pure White, which is the colour value stored within the String.

## Breakdown
```cpp
union{
	/* Internal. Representation of the RGB values that X can use. */
	uint32_t XCOL;
	struct{
		/* The Blue value of this Colour. */
		uint8_t b : 8;
		/* The Green value of this Colour. */
		uint8_t g : 8;
		/* The Red value of this Colour. */
		uint8_t r : 8;
	}raw;
};
```
A small [union](https://www.tutorialspoint.com/cprogramming/c_unions.htm) that allows for easy setting and retrieval of the RGB values.

`uint32_t XCOL` - The raw colour sent to the [X Service.](https://en.wikipedia.org/wiki/X_Window_System) This is not to be accessed manually.

`uint8_t b,g,r` - The Red, Green, and Blue values of the Colour in reverse order. Can be any number between 0-255.
```cpp
inline OCol(void) : b{0},g{0},r{0} { }
```
Empty constructor. Sets all values to 0.
```cpp
inline void setTo(uint8_t r, uint8_t g, uint8_t b) { raw={b,g,r}; }
inline OCol(uint8_t r, uint8_t g, uint8_t b) : raw{b,g,r} { }
```
Initialises and sets the Colour to the given RGB values. 

Parameters:

`uint8_t r - The red value (0 - 255) of this Colour.`

`uint8_t g - The green value (0 - 255) of this Colour.`

`uint8_t b - The blue value (0 - 255) of this Colour.`
```cpp
bool setTo(const char* format);
```
Attempts to set and initialise this Colour from the given formatted String. 

The String MUST be in a very specific format. Example: "(255,255,255)"
The formatted part can be at any position of the String, but it MUST contain three values corresponding to each of the RGB values.

Parameters:

`const char* format - The format String to attempt to retrieve the values from.`

Returns: `True on success, otherwise false if format String is not valid.`

```cpp
virtual void log(bool verbose=false, bool newLine=true) override;
```
Logs the information of this Col to the terminal.

Parameters:

`bool verbose - Log verbose information about this Col instead of the traditional information. Default is false.`

`bool newLine - Should the output be placed on a newline or append to the current one if applicable? Default is true.`
