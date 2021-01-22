# Class Reference: OCol
## Dealing with Colours.

### Definition
```cpp
struct OCol : public CLoggable {
	unsigned char r,g,b;
	unsigned long XCOL;

	OCol(void);
	OCol(unsigned char r, unsigned char g, unsigned char b);
	void setTo(unsigned char r, unsigned char g, unsigned char b);
	virtual void log(bool verbose=false) override;
};
```
### Use
OCol is the primary method OKit deals with colouring GUI elements.
Most GUI elements in OKit colour themselves automatically, however you can manually assign them colours. Be aware that this can be overridden by a given user's Orion [theming]((https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OTheme.md)) settings 
Assigning custom colours to elements is detailed in class reference for said element.
OCol extends from [CLoggable](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/Control%20Classes/CLoggable.md), meaning it can be logged both verbosely and minimally.

To create an OCol for use, use the following syntax.
```cpp
OCol myCol(255,255,255);
```
The first 3 values of the constructor are the R,G and B values of the colour respectively.
The OCol can now be passed to functions that take in an OCol. Often times they take in a reference, not a pointer. See the specifications for the given function.
To modify an existing OCol, use the `setTo()` function. Do NOT modify the R,G, and B values individually, as this will not work.

You can print the data of a given OCol to the terminal by calling the `log()` function on it.

### Structure Breakdown
```cpp
unsigned char r,g,b;
```
This is the respective RGB values for a given colour.
```cpp
unsigned long XCOL;
```
The actual colour that is passed to the X Display Server for rendering. Do not interact with or modify this manually.
```cpp
OCol(void);
```
An empty constructor. Will set all values to 0 and will be rejected by any function that takes in an `OCol&` or `OCol*`.
```cpp
OCol(unsigned char r, unsigned char g, unsigned char b);
```
Creates and initalises an OCol with the given parameters.
```cpp
void setTo(unsigned char r, unsigned char g, unsigned char b);
```
Modifies an existing OCol and re-initialises it with the new values.
```cpp
virtual void log(bool verbose=false) override;
```
Prints the RGB values of a given OCol to the terminal. Pass `true` to get more verbose information.

### Other Information
To use the application's [theme data](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OTheme.md), there are several macros that point directly to [application's theme](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OTheme.md#other-information) colours.
They are

`OTHEME_PRIMARY`   Often used as the background for a window or container.

`OTHEME_SECONDARY` Often used as the background for a button or top level GUI element.

`OTHEME_TERTIARY`  Rarely used, but used as a divider between GUI elements.

`OTHEME_ACCENT`    Often used as an accent colour, such as the border around OWindows.

For information on theme data as a whole, see documentation for [OTheme](https://github.com/RosettaHS/OKit/blob/main/docs/Class%20Reference/OTheme.md).
